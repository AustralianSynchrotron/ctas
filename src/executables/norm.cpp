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
/// @brief %Normalizes an image per using selected stripe.
///


#include "../common/ctas.h"
#include "../common/poptmx.h"
#include <math.h>

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  deque<ImagePath> images;        ///< input image
  ImagePath outmask;              ///< Name of the output image.
  deque<Crop<2>> rois;                  ///< Crop input projection image>
  float loThreshold;         ///< Black intensity.
  float hiThreshold;         ///< White intensity.
  bool beverbose;
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  loThreshold(std::numeric_limits<float>::lowest()),         ///< Black intensity.
  hiThreshold(std::numeric_limits<float>::max()),         ///< White intensity.
  beverbose(false)
{

  poptmx::OptionTable table
  ("Normalizes stack of images.",
   "Normolizes stack of grayscale images over the average intensity of the selected ROI's.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
      .add(poptmx::ARGUMENT, &images,    "images", "Input combination of 2D and 3D images.",
           "Either 2D images understood by the IM or HDF5. HDF5 format as follows:\n"
           "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc )

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &outmask, 'o', "output", "Output result mask or filename.",
       "Output filename if output is a single file. Output mask otherwise. " + MaskDesc, outmask)
  .add(poptmx::OPTION, &rois, 'r', "roi", "Region of interest(s) on the selected image.", CropOptionDesc)
  .add(poptmx::OPTION, &loThreshold, 'm', "min", "Low threshold.", "Pixel below this value are not considered.")
  .add(poptmx::OPTION, &hiThreshold, 'M', "max", "High threshold.", "Pixel above this value are not considered.")
  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }

  command = table.name();
  if ( ! table.count(&images) )
    exit_on_error(command, "No input image given.");

}


class ProcInThread : public InThread {

  ReadVolumeBySlice allIn;
  //SaveVolumeBySlice * allOut;
  const deque<Crop<2>> & rois;
  SumProc refProc;
  unordered_map<pthread_t, SumProc*> procs;
  unordered_map<pthread_t, Map*> iomaps;
  unordered_map<pthread_t, Line*> sumlines;

  bool inThread(long int idx) {
    if (idx >= allIn.slices())
      return false;

    SumProc * myProc;
    Map myImage;
    Line myLine;
    const pthread_t me = pthread_self();
    if ( ! procs.count(me) ) {
      myProc = new SumProc(refProc);
      Map * eimap = new Map(allIn.face());
      myImage.reference(*eimap);
      Line * eiline = new Line( rois.size() ? refProc.size() : 0);
      myLine.reference(*eiline);
      lock();
      procs.emplace(me,myProc);
      iomaps.emplace(me,eimap);
      sumlines.emplace(me,eiline);
    } else {
      lock();
      myProc = procs.at(me);
      myImage.reference(*iomaps.at(me));
      myLine.reference(*sumlines.at(me));
    }
    unlock();

    allIn.readTo(idx, myImage);
    pair<float,int> res;
    if (rois.size()) {
      size_t curPos=0;
      for ( const Crop<2> & crp : rois) {
        Map roi(crp.apply(myImage));
        const Shape<2> & csh = roi.shape();
        for ( int ln = 0 ; ln < csh[0] ; ln++) {
          myLine( blitz::Range(curPos,curPos+csh[1]-1)) = roi(ln, all);
          curPos += csh[1];
        }
      }
      res = (*myProc)(myLine);
    } else
      res = (*myProc)(myImage);
    lock();
    cout << idx << ": " << res.first << " / " << res.second << " = "
         << ( res.second ? res.first/res.second : 0 ) << std::endl;
    unlock();
    bar.update();
    return true;
  }

public:

  ProcInThread(const clargs & args)
    : InThread(args.beverbose, "Calculating average")
    , allIn(args.images)
    , rois(args.rois)
    , refProc( [this]() {
                 if (rois.size()) {
                   ssize_t sumSize=0;
                   for (const Crop<2> & crp : rois)
                     sumSize += size(crp.shape(allIn.face()));
                   return sumSize;
                 } else
                   return size(allIn.face());
               } ()
               , args.loThreshold, args.hiThreshold )
  {
    bar.setSteps(allIn.slices());
  }

  ~ProcInThread() {
    auto dlnl = [&](auto pntr) { if (pntr) { delete pntr; pntr=0; } } ;
    for (auto celem : procs) dlnl(celem.second);
    for (auto celem : iomaps) dlnl(celem.second);
    for (auto celem : sumlines) dlnl(celem.second);
  }

};







/// \MAIN{norm}
int main(int argc, char *argv[]) { {
  const clargs args(argc, argv) ;
  ProcInThread(args).execute();
} exit(0); }
