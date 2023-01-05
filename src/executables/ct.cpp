/******************************************************************************
 *   Copyright (C) 2007 by Anton Maksimenko                                   *
 *   antonmx@post.kek.jp                                                      *
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

///
/// @file
/// @author antonmx <antonmx@gmail.com>
/// @date   Mon Jul 21 10:09:31 2008
///
/// @brief %Reconstruction of one sinogram.
///
/// Main file for the program which reads the input sinogram from image,
/// performs CT reconstruction of it and stores the result in other
/// image file
///



#include "../common/common.h"
#include "../common/kernel.h"
#include "../common/poptmx.h"

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Contrast contrast;            ///< Type of the contrast.
  Filter filter_type;           ///< Type of the filtering function.
  float center;                   ///< Rotation center.
  deque<ImagePath> sinograms;         ///< Name of the sinogram file.
  ImagePath outmask;           ///< Name of the file to save the result to.
  float arc;
  float dd;             ///< Pixel size.
  bool beverbose;				///< Be verbose flag
  float mincon;         ///< Black intensity.
  float maxcon;         ///< White intensity.


  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : contrast(Contrast::ABS)
  , center(0)
  , beverbose(false)
  , mincon(0)
  , maxcon(0)
  , outmask("reconstructed-<sinogram>")
  , arc(180)
  , dd(1.0)
{

  poptmx::OptionTable table
  ("CT reconstruction of one sinogram.",
   "The program reads pre-formed sinogram and reconstructs it.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &sinograms, "sinogram",
       "Input image containing the sinogram.", "")
  .add(poptmx::ARGUMENT, &outmask, "result",
       "Output reconstructed image.", "", outmask)

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &contrast, 'k', "contrast",
       "Input component.",
       "Type of the contrast presented in the sinogram. " + Contrast::Desc)
  .add(poptmx::OPTION, &center, 'c', "center",
       "Rotation center.", CenterOptionDesc, toString(center))
  .add(poptmx::OPTION, &arc, 'a', "arcan",
       "CT scan range (if > 1 deg), or step size (if < 1deg).",
       "If value is greater than 1deg then it represents the arc of the CT scan."
       " Otherwise the value is the step size.",
       toString(arc))
  .add(poptmx::OPTION, &filter_type, 'f', "filter",
       "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
  .add(poptmx::OPTION, &dd, 'r', "resolution",
       "Pixel size (micron).", ResolutionOptionDesc, toString(dd))
  //  .add(poptmx::OPTION, &lambda, 'w', "wavelength",
  //  "Wave length (Angstrom).", "Wavelength.")
  .add(poptmx::OPTION, &mincon, 'm', "min", "Pixel value corresponding to black.",
       " All values below this will turn black.", "<minimum>")
  .add(poptmx::OPTION, &maxcon, 'M', "max", "Pixel value corresponding to white.",
       " All values above this will turn white.", "<maximum>")
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
  if ( ! table.count(&outmask) )
    outmask = upgrade(sinograms[0], "reconstructed-");
  if ( table.count(&dd) ) {
    if ( dd <= 0.0 )
      exit_on_error(command, "Negative pixel size (given by "+table.desc(&dd)+").");
    dd /= 1.0E6;
  }
  if (arc <= 0.0)
    exit_on_error(command, "CT arc (given by "+table.desc(&arc)+") must be strictly positive.");


}


class RecInThread : public InThread {

  const clargs & ctrl;
  ReadVolumeBySlice ivolRd;
  const Shape ish;
  const Shape osh;
  SaveVolumeBySlice ovolSv;

  unordered_map<pthread_t, CTrec*> recs;
  unordered_map<pthread_t, Map*> imaps;
  unordered_map<pthread_t, Map*> omaps;

  bool inThread(long int idx) {
    if (idx >= ivolRd.slices())
      return false;

    const pthread_t me = pthread_self();
    if ( ! recs.count(me) ) {
      CTrec * erec = new CTrec(ish, ctrl.contrast, ctrl.arc, ctrl.filter_type);
      Map * eimap = new Map(ish);
      Map * eomap = new Map(osh);
      lock();
      recs.emplace(me, erec);
      imaps.emplace(me, eimap);
      omaps.emplace(me, eomap);
      unlock();
    }
    lock();
    CTrec & myRec = *recs.at(me);
    Map & myImap = *imaps.at(me);
    Map & myOmap = *omaps.at(me);
    unlock();

    ivolRd.read(idx, myImap);
    myRec.reconstruct(myImap, myOmap, ctrl.center, ctrl.dd);
    ovolSv.save(idx, myOmap);
    bar.update();
    return true;

  }

public:

  RecInThread(const clargs & _args)
    : InThread(_args.beverbose, "CT'ing")
    , ctrl(_args)
    , ivolRd(ctrl.sinograms)
    , ish(ivolRd.face())
    , osh(ish(1),ish(1))
    , ovolSv(ctrl.outmask, osh, ivolRd.slices(), ctrl.mincon, ctrl.maxcon)
  {
    bar.setSteps(ivolRd.slices());
  }

  ~RecInThread() {
    #define delnul(pntr) { if (pntr) delete pntr; pntr = 0;}
    for (auto celem : recs) delnul(celem.second);
    for (auto celem : imaps)  delnul(celem.second);
    for (auto celem : omaps)  delnul(celem.second);
    #undef delnul
  }



};



/// \MAIN{ct}
int main(int argc, char *argv[]) {
  const clargs args(argc, argv) ;
  RecInThread factory(args);
  factory.execute();
  exit(0);
}
