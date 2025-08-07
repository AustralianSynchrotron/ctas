/******************************************************************************
 *   Copyright (C) 2023 by Anton Maksimenko                                   *
 *   antonmx@gmail.com                                                      *
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program; if not, write to the                            *
 *   Free Software Foundation, Inc.,                                          *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
 ******************************************************************************/


#include <poptmx.h>
#include "../common/ctas.h"
#include "../common/ct.h"

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  deque<ImagePath> sinograms;         ///< Name of the sinogram file.
  ImagePath outmask;           ///< Name of the file to save the result to.
  ImagePath mask;
  uint ringBox;
  bool beverbose;				///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : beverbose(false)
  , ringBox(0)
{

  poptmx::OptionTable table
  ("CT reconstruction of one sinogram.",
   "The program reads pre-formed sinogram and reconstructs it.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &sinograms, "sinogram",
       "Input image containing the sinogram.", "")

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &outmask, 'o', "output",
       "Output file name or name template for multiple images.", "", "<input>")
  .add(poptmx::OPTION, &ringBox, 'R', "ring",
           "Half size of the ring filter.", "", toString(ringBox))
  .add(poptmx::OPTION, &mask, 'm', "mask",
           "Mask of the areas to filter.", "Must be an image of size [<input width>, <number of inputs>].", toString(ringBox))
  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }
  command = table.name();

  if ( ! table.count(&sinograms) )
    exit_on_error(command, string () +
                  "Missing required argument: "+table.desc(&sinograms)+".");

}


class RingInThread : public InThread {

  const clargs & args;
  ReadVolumeBySlice ivolRd;
  Map mask;
  const Shape<2> sh;
  SaveVolumeBySlice * ovolSv;
  unordered_map<pthread_t, RingFilter*> filters;
  unordered_map<pthread_t, Map*> iomaps;


  bool inThread(long int idx) {

    if (idx >= ivolRd.slices())
      return false;

    const pthread_t me = pthread_self();
    if ( ! filters.count(me) ) {
      RingFilter * ering = new RingFilter(sh(1), args.ringBox);
      Map * eimap = new Map(sh);
      lock();
      filters.emplace(me, ering);
      iomaps.emplace(me, eimap);
      unlock();
    }
    lock();
    RingFilter & myRing = *filters.at(me);
    Map & myMap = *iomaps.at(me);
    unlock();
    const Line & myMask = mask.size() ? mask(idx,all) : defaultLine;

    ivolRd.readTo(idx, myMap);
    myRing.apply(myMap, myMask);
    if (ovolSv)
      ovolSv->save(idx, myMap);
    else
      ivolRd.write(idx, myMap);

    bar.update();
    return true;

  }

public:

  RingInThread(const clargs & _args)
    : InThread(_args.beverbose, "Filtering")
    , args(_args)
    , ivolRd(args.sinograms, args.outmask.empty())
    , sh(ivolRd.face())
    , ovolSv(0)
  {
    if (!args.outmask.empty())
      ovolSv = new SaveVolumeBySlice(args.outmask, ivolRd.shape());
    if (!args.mask.empty()) {
      ReadImage(args.mask, mask, Shape<2>(ivolRd.slices(), sh(1)));
      const float vmin = min(mask);
      const float delta = max(mask) - vmin;
      if (delta==0.0)
        mask = 1.0;
      else
        mask = (mask-vmin)/delta;
    }
    bar.setSteps(ivolRd.slices());
  }

  ~RingInThread() {
    auto dlnl = [&](auto pntr) { if (pntr) { delete pntr; pntr=0; } } ;
    dlnl(ovolSv);
    for (auto celem : filters) dlnl(celem.second);
    for (auto celem : iomaps) dlnl(celem.second);
  }

};



/// \MAIN{ct}
int main(int argc, char *argv[]) { {
  const clargs args(argc, argv) ;
  RingInThread factory(args);
  factory.execute();
} exit(0); }
