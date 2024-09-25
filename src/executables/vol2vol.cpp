/******************************************************************************
 *   Copyright (C) 2017 by Anton Maksimenko                                   *
 *   antonmx@gmail.com                                                        *
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
/// @date   Thu Jun 06 10:09:31 2017
///
/// @brief %Forms projection from multiple images.
///

#include <poptmx.h>
#include "../common/ctas.h"
#include "../common/flatfield.h"
#include <algorithm>
#include <string.h>


using namespace std;



/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  deque<ImagePath> images;        ///< input image
  ImagePath outmask;              ///< Name of the output image.
  ImagePath bgs;        ///< Array of the background images.
  ImagePath dfs;        ///< Array of the dark field images.
  ImagePath dgs;        ///< Array of the dark field images for backgrounds.
  //bool sinoFF;          ///< Tells if **s above are the sources of sinolines or actual fields
  float ang;
  Crop<3> crp;                  ///< Crop input projection image
  Binn<3> bnn;                  ///< binning factor
  string slicedesc;       ///< String describing the slices to be sino'ed.
  float mincon;         ///< Black intensity.
  float maxcon;         ///< White intensity.
  int bpp;         ///< Save image as 16-bit integer.
  float reNAN;
  bool beverbose;             ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : outmask ("slice.tif")
  //, sinoFF(false)
  , ang(0.0f)
  , mincon(0)
  , maxcon(0)
  , bpp(0)
  , reNAN(NAN)
  , beverbose(false)
{


  poptmx::OptionTable table
    ("Reslices 3D volume.",
     "Reads 3D volume from input file(s) and, after applying manipulations saves"
     " the result as the HDF volume or set of 2D images as requested by the output format.");

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &images,    "images", "Input combination of 2D and 3D images.",
         "Either 2D images understood by the IM or HDF5. HDF5 format as follows:\n"
         "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc )

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &outmask, 'o', "output", "Output result mask or filename.",
         "Output filename if output is a single file. Output mask otherwise. " + MaskDesc, outmask)
    .add(poptmx::OPTION, &ang,'r', "rotate", "Rotates each slice by this rotation angle (deg).", "")
    .add(poptmx::OPTION, &crp, 'c', "crop", "Crop input volume.", CropOptionDesc)
    .add(poptmx::OPTION, &bnn, 'b', "binn", "Binn factor(s).", BinnOptionDesc)
    .add(poptmx::OPTION, &slicedesc, 's', "slice", "Slices to be processed.", DimSliceOptionDesc, "<all>")
    .add(poptmx::OPTION, &bgs, 'B', "bg", "Background image", "")
    .add(poptmx::OPTION, &dfs, 'D', "df", "Dark field image", "")
    .add(poptmx::OPTION, &dgs, 'F', "dg", "Dark field image for backgrounds", "")
    //.add(poptmx::OPTION, &sinoFF, 'S', "sino",
    //     "If set, images given with BDF option(s) are considered as face slices to get lines in sinogram.", "")
    .add(poptmx::OPTION, &mincon, 'm', "min", "Pixel value corresponding to black.",
         " All values below this will turn black.", "<minimum>")
    .add(poptmx::OPTION, &maxcon, 'M', "max", "Pixel value corresponding to white.",
         " All values above this will turn white.", "<maximum>")
    .add(poptmx::OPTION, &bpp,'i', "int", "Bits per pixel to output image(s) as integer.", IntOptionDesc,
         "0 (for 32-bit float-point) if no " + table.desc(&mincon) + " or " + table.desc(&maxcon)
         + " are used; 8 otherwise.")
    .add(poptmx::OPTION, &reNAN,'N', "nan", "Replace NAN's with this number.", "")
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
  if ( beverbose && outmask == "-" )
    exit_on_error(command, "Verbose option ("+table.desc(&beverbose)+") is incompatible with"
                           " printing to stdout (\'-\' given to "+table.desc(&outmask)+" option).");
  //if (table.count(&sinoFF) && !(table.count(&bgs)+table.count(&dfs)+table.count(&dgs)) )
  //  warn(command, "Option "+table.desc(&sinoFF)+" given without any of "
  //                + table.desc(&bgs)+", "+table.desc(&dfs)+" and "+table.desc(&dgs)
  //                + ": will be ignored.");
  if ( ! table.count(&bgs) && table.count(&dgs) )
    exit_on_error(command, "No background images (" + table.desc(&bgs) + ") for provided darkgrounds (" + table.desc(&dgs) + ").");
  if ( table.count(&mincon) + table.count(&maxcon) && ! table.count(&bpp) )
    bpp = 8;

  ang *= M_PI/180;

}






class SliceInThread : public InThread {

  ReadVolumeBySlice * ivolRd;
  const Shape<2> ish;
  const unsigned isz;
  ssize_t bnz;
  ImageProc canonImgProc;
  const Shape<2> osh;
  const unsigned osz;
  const deque<int> indices;
  SaveVolumeBySlice * ovolSv;

  unordered_map<pthread_t, ImageProc* > rdprocs;
  list<BinnProc::Accumulator> accs;


  bool inThread(long int idx) {

    const long int sodx =  idx/bnz + ( bnz > 0 ? 0 : ovolSv->slices() - 1 ) ;
    if ( sodx < 0 || sodx >= ovolSv->slices() || idx >= ivolRd->slices() )
      return false;
    if ( find(indices.begin(), indices.end(), sodx) == indices.end() )
      return true;

    const pthread_t me = pthread_self();
    if ( ! rdprocs.count(me) ) {
      ImageProc * erdprocs = new ImageProc(canonImgProc);
      lock();
      rdprocs.emplace(me, erdprocs);
      unlock();
    }
    lock();
    ImageProc & myrdproc = *rdprocs.at(me);
    unlock();

    lock(1);
    if (!accs.size())
      accs.emplace_back(osh, abs(bnz));
    auto useacc = accs.begin();
    if (abs(bnz)!=1) {
      useacc = accs.end();
      for ( auto accsp  = accs.begin() ; accsp != accs.end() ; accsp++) {
        if (accsp->index()<0)
          useacc = accsp;
        else if (accsp->index() == sodx) {
          useacc = accsp;
          break;
        }
      }
      if (useacc == accs.end()) {
        accs.emplace_back(osh, abs(bnz));
        useacc = accs.end();
        --useacc;
      }
    }
    BinnProc::Accumulator & myacc = *useacc;
    myacc.index(sodx);
    unlock(1);

    Map myrdmap = myrdproc.read(*ivolRd, idx);
    if ( ! myacc.addme(myrdmap) )
      ovolSv->save(sodx, myrdmap);
    bar.update();
    return true;

  }

public:

  SliceInThread(const clargs & args, const Map & bg , const Map & df, const Map & dg)
    : InThread(args.beverbose, "processing slices")
    , ivolRd(new ReadVolumeBySlice(args.images))
    , ish(ivolRd->face())
    , isz(ivolRd->slices())
    , canonImgProc(ish, bg, df, dg, Map(), args.ang,
                   copyTail<2>(args.crp), copyTail<2>(args.bnn), args.reNAN)
    , bnz( args.bnn(0) ? args.bnn(0) : isz )
    , osh( canonImgProc.shape() )
    , osz( binnOne(isz, bnz) )
    , indices( [](string sindex, unsigned int _osz) {
        if (sindex.size() && string("zZ").find(sindex.at(0)) != string::npos)
          sindex.erase(0,1);
        return slice_str2vec(sindex, _osz);
      }(args.slicedesc, osz) )
    , ovolSv( new SaveVolumeBySlice( args.outmask, Shape<3>(osz,osh(0),osh(1))
                                   , args.bpp, args.mincon, args.maxcon ) )
  {
    if (!isz)
      throw_error(args.command, "Empty volume to read.");
    if ( size(osh) <= 0 )
      throw_error(args.command, "Cropping or binning is larger than the shape of input volume.");
    needMutexes(2);
    bar.setSteps(indices.size()*abs(bnz));
  }

  ~SliceInThread() {
    #define delnul(pntr) { if (pntr) delete pntr; pntr = 0;}
    for (auto celem : rdprocs) delnul(celem.second);
    delnul(ivolRd);
    delnul(ovolSv)
    #undef delnul
  }

  void execute(int nThreads=0) { // To deals with possible last incomplete slice
    InThread::execute(nThreads);
    if (abs(bnz)==1)
      return;
    Map last;
    for ( BinnProc::Accumulator & acc : accs ) {
      const int odx = acc.getme(last);
      if ( odx >= 0 ) {
        ovolSv->save(odx, last);
        bar.done();
        break; // there can be only one incomplete sum
      }
    }
    accs.clear();
  }


};


/// \MAIN{projection}
int main(int argc, char *argv[]) { {

  const clargs args(argc, argv) ;
  const Shape<2> ish = ImageSizes(args.images[0]);

  Map bg, df, dg;
  if (!args.bgs.empty())
    ReadImage(args.bgs, bg, ish);
  if (!args.dfs.empty())
    ReadImage(args.dfs, df, ish);
  if (!args.dgs.empty())
    ReadImage(args.dgs, dg, ish);

  if (  ( ! args.slicedesc.empty()
          &&  string("xXyY").find(args.slicedesc.at(0)) != string::npos )
        || ( args.bpp  &&  args.mincon == args.maxcon ) ) {
    // Requires whole volume in memory to produce cross-sections or calculate min/max

    Volume ivol;
    ReadVolume(args.images, ivol, args.beverbose);
    FlatFieldProc ffProc(bg, df, dg, Map());
    if (ffProc)
      InThread::execute(ivol.shape()(0), [&](ssize_t ii){
        Map pmp = ivol(ii,all,all);
        ffProc.process(pmp);
      } ) ;
    Volume rvol;
    RotateProc rotProc(ish, args.ang);
    if (rotProc) {
      rvol.resize(Shape<3>(ivol.shape()(0), rotProc.shape()(0), rotProc.shape()(1)));
      InThread::execute(ivol.shape()(0), [&](ssize_t ii){
        Map rotMap(rvol(ii,all,all));
        rotMap = rotProc.apply(ivol(ii,all,all), rotMap);
      } ) ;
    } else {
      rvol.reference(ivol);
    }
    Volume cvol(args.crp.apply(rvol));
    Volume bvol(args.bnn.apply(cvol));
    SaveVolume( args.outmask, bvol, args.beverbose, args.slicedesc
              , args.bpp, args.mincon, args.maxcon);

  } else {
    // Can work slice-by-slice.
    SliceInThread factory(args, bg, df, dg);
    factory.execute();
  }

} exit(0); }


