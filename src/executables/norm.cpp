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

#include <poptmx.h>
#include <math.h>
#include "../common/ctas.h"

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  deque<ImagePath> images;        ///< input image
  ImagePath outmask;              ///< Name of the output image.
  deque<Crop<2>> rois;                  ///< Crop input projection image>
  float loThreshold;         ///< Black intensity.
  float hiThreshold;         ///< White intensity.
  SumProc::Stat norma;
  bool print;
  bool beverbose;
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  loThreshold(std::numeric_limits<float>::lowest()),         ///< Black intensity.
  hiThreshold(std::numeric_limits<float>::max()),         ///< White intensity.
  norma(0.0f,-1),
  print(false),
  beverbose(false)
{

  ssize_t normaSecond=0;

  poptmx::OptionTable table
  ("Normalizes stack of images.",
   "Normolizes stack of grayscale images over the average intensity of the selected ROI's.");

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &images,    "images", "Input combination of 2D and 3D images.",
         "Either 2D images understood by the IM or HDF5. HDF5 format as follows:\n"
         "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc )
    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &outmask, 'o', "output", "Output normalized results (-v for more).",
         "If empty string is given, then overwrites input; if output is a single file,"
         " then filename; otherwise output mask. " + MaskDesc, outmask)
    .add(poptmx::OPTION, &rois, 'r', "roi", "Region of interest(s) on the selected image.", CropOptionDesc)
    .add(poptmx::OPTION, &loThreshold, 'm', "min", "Low threshold.", "Pixel below this value are not considered.")
    .add(poptmx::OPTION, &hiThreshold, 'M', "max", "High threshold.", "Pixel above this value are not considered.")
    .add(poptmx::OPTION, &norma.first, 'n', "norm", "Normalization multiplicator.",
         "Each image will be multiplied by this factor after normalized. If zero, then total average will be used.")
    .add(poptmx::OPTION, &normaSecond, 'N', "least", "Least number of pixels to normalize.",
         "Image is normalized only if number of pixels considered is at least this.")
    .add(poptmx::OPTION, &print, 'p', "print", "Print statistics per image.",
                                 "Outputs sum, numer of pixels counted and average for each input image.")

  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }

  command = table.name();
  if ( ! images.size() )
    exit_on_error(command, "No input image provided.");
  if ( print && beverbose ) {
    warn(command, "Cannot print results and proggress bar at the same time."
                  " Only results will be printed.");
    beverbose = false;
  }

  if ( table.count(&norma.first) + table.count(&normaSecond)
       &&  ! table.count(&outmask) )
    outmask = images[0]; // signals overwrite)
  if ( table.count(&normaSecond) )
    norma.second = normaSecond;
  else if ( table.count(&norma.first) )
    norma.second = 0;
  if ( outmask.empty() && ! print )
    print = true;

}


class ProcInThread : public InThread {

  ReadVolumeBySlice allIn;
  SaveVolumeBySlice * allOut;
  const deque<Crop<2>> & rois;
  SumProc refProc;
  const SumProc::Stat norma;
  const bool print;
  vector<SumProc::Stat> reses;

  struct PerThread {
    SumProc proc;
    Map iomap;
    Line summe;
    SumProc::Stat total;
    PerThread(const SumProc & proc, const Shape<2> sh, bool needLine )
      : proc(proc)
      , iomap(sh)
      , summe(needLine ? proc.size() : 0)
    {}
  };
  unordered_map<pthread_t, PerThread*> perGoods;

  bool inThread(long int idx) {
    if (idx >= allIn.slices())
      return false;

    PerThread * myGoods=0;
    const pthread_t me = pthread_self();
    if ( ! perGoods.count(me) ) {
      myGoods = new PerThread(refProc, allIn.face(), rois.size());
      lock();
      perGoods.emplace(me,myGoods);
    } else {
      lock();
      myGoods = perGoods.at(me);
    }
    unlock();

    allIn.readTo(idx, myGoods->iomap);
    SumProc::Stat & res = reses.at(idx);
    if (rois.size()) {
      size_t curPos=0;
      for ( const Crop<2> & crp : rois) {
        Map roi(crp.apply(myGoods->iomap));
        const Shape<2> & csh = roi.shape();
        for ( int ln = 0 ; ln < csh[0] ; ln++) {
          myGoods->summe( blitz::Range(curPos,curPos+csh[1]-1)) = roi(ln, all);
          curPos += csh[1];
        }
      }
      res = myGoods->proc(myGoods->summe);
      const float mult = SumProc::normMult(norma, res);
      if ( mult != 0.0 && mult != 1.0f ) // with roi(s) image is never normalized;
        myGoods->iomap *= mult;
    } else {
      res = myGoods->proc(myGoods->iomap);
    }

    if ( norma.first != 0.0 && norma.second >= 0 && res.second >= norma.second) {
      if ( allIn.writable() )
        allIn.write(idx, myGoods->iomap);
      else if (allOut)
        allOut->save(idx, myGoods->iomap);
    }
    myGoods->total.first  += res.first;
    myGoods->total.second += res.second;
    ifPrintRes(res, toString(idx));
    bar.update();
    return true;
  }

public:


  ProcInThread(const clargs & args)
    : InThread(args.beverbose, "normalize")
    , allIn(args.images, args.outmask == args.images[0] )
    , allOut( [this](const string & outmask) {
                if (allIn.writable() || outmask.empty() )
                  return (SaveVolumeBySlice*)0 ;
                else
                  return new SaveVolumeBySlice(outmask, allIn.shape());
              } (args.outmask) )
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
               , rois.size() ? SumProc::Stat(0,-1) :  args.norma
               , args.loThreshold, args.hiThreshold )
    , norma(args.norma)
    , print(args.print)
    , reses(allIn.slices())
  {
    bar.setSteps(allIn.slices());
  }

  ~ProcInThread() {
    auto dlnl = [&](auto pntr) { if (pntr) { delete pntr; pntr=0; } } ;
    for (auto celem : perGoods) dlnl(celem.second);
  }

  SumProc::Stat total() {
    SumProc::Stat toRet(0,0);
    for (auto goods : perGoods) {
      toRet.first  += goods.second->total.first;
      toRet.second += goods.second->total.second;
    }
    return toRet;
  }

  void ifPrintRes(const SumProc::Stat & res, const string & idx) {
    if (!print)
      return;
    lock();
    cout << idx << ": " << res.first << " / " << res.second << " = "
         << ( res.second ? res.first/res.second : 0 ) << std::endl;
    unlock();
  }

  void ifPrintRes() {
    ifPrintRes(total(), "Total");
  }

  void postCalc() {

    if (norma.second < 0)
      return; // nothing to normalize

    const SumProc::Stat tot(total());
    if (tot.second<=0) {
      warn("ProcInThread", "No considered pixels in the whole volume.");
      return;
    }
    if ( norma.first == 0.0 && tot.first == 0.0 ) {
      warn("ProcInThread", "Average of the whole volume is exactly zero. Will not normalize further.");
      return;
    }

    float multN = norma.first == 0.0 ? tot.second/tot.first : 1.0f ;
    auto inthread = [this,&tot,multN](size_t idx) {
      const SumProc::Stat & res = reses.at(idx);
      if ( norma.first != 0.0 && res.first != 0.0 && res.second >= norma.second)
        return true; // already normalized
      if ( norma.first == 0.0 && ( res.first == 0.0 || res.second < norma.second ) )
        return true; // no need to normalize - effect will cancel self;
      const float mult = multN * tot.first / tot.second;
      Map iomap;
      allIn.readTo(idx, iomap);
      iomap *= mult;
      if ( allIn.writable() )
        allIn.write(idx, iomap);
      else if (allOut)
        allOut->save(idx, iomap);
      return true;
    };
    InThread::execute( allIn.slices(), inthread );
  }

};







/// \MAIN{norm}
int main(int argc, char *argv[]) { {
  const clargs args(argc, argv) ;
  ProcInThread proc(args);
  proc.execute();
  proc.ifPrintRes();
  proc.postCalc();
} exit(0); }
