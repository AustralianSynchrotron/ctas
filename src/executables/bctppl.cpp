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


#include "../common/common.h"
#include "../common/poptmx.h"
#include "../common/flatfield.h"
#include <algorithm>
#include <string.h>


using namespace std;



/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path outmask;
  Path gaps;
  vector<Path> dfs;
  vector<Path> bgs0;
  vector<Path> bgs1;
  vector<Path> ims0;
  vector<Path> ims1;
  PointF2D shift;
  float aShift;
  float arc;
  Crop crop;                  ///< Crop input projection image
  Crop fcrp;                  ///< Crop final projection image
  float angle;                ///< Rotation angle.
  float center;               ///< flip centre.
  bool SaveInt;         ///< Save image as 16-bit integer.
  bool beverbose;             ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : outmask("_.tif")
  , shift(0,0)
  , arc(180)
  , angle(0)
  , center(0)
  , SaveInt(false)
  , beverbose(false)
{

  poptmx::OptionTable table
    ("IMBL BCT pipeline.",
     "Proccesses the IMBL BCT experiment input data to produce CT-reconstructed slices.");

  table
      .add(poptmx::NOTE, "ARGUMENTS:")
      .add(poptmx::ARGUMENT, &outmask, "output", "Output result mask or filename.",
           "Output filename if output is a single file. Output mask otherwise. " + MaskDesc, outmask)

      .add(poptmx::NOTE, "OPTIONS:")
      .add(poptmx::OPTION, &ims0, 'f', "sample0", "First input set.", "")
      .add(poptmx::OPTION, &ims1, 'F', "sample1", "Second input set.", "")
      .add(poptmx::OPTION, &gaps, 'g', "gaps", "Map of the gaps",
           "Image file containing the mask of the detector gaps.")
      .add(poptmx::OPTION, &dfs, 'D', "df", "Dark field image(s)", "")
      .add(poptmx::OPTION, &bgs0, 'b', "bg0", "Background image(s) for the first input set.", "")
      .add(poptmx::OPTION, &bgs1, 'B', "bg1", "Background image(s) for the second input set.", "")
      .add(poptmx::OPTION, &shift, 's', "shift", "Spatial shift of the second data set.", "")
      .add(poptmx::OPTION, &aShift, 'S', "start", "Starting angle of the second data set.",
           "The starting angular position relative to the starting position of the first data set.")
      .add(poptmx::OPTION, &arc, 'a', "arcan", "CT scan range (if > 1 deg), or step size (if < 1 deg).",
           "If value is greater than 1deg then it represents the arc of the CT scan."
           " Otherwise the value is the step size.", toString(arc))
      .add(poptmx::OPTION, &crop, 't', "crop", "Crop input images: " + CropOptionDesc, "")
      .add(poptmx::OPTION, &fcrp, 'T', "crop-final", "Crops final image: " + CropOptionDesc, "")
      .add(poptmx::OPTION, &angle,'r', "rotate", "Rotate input images by given angle.", "")
      .add(poptmx::OPTION, &center, 'c', "center", "Rotation center.", CenterOptionDesc)
      .add(poptmx::OPTION, &SaveInt,'i', "int", "Output image(s) as integer.", IntOptionDesc)
      .add_standard_options(&beverbose)
      .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }
  command = table.name();

  #define chck_inSet(set, role) \
    if ( ! table.count(&set) ) \
      exit_on_error(command, "Missing required input for the" + string(role) + "set.");
  chck_inSet(ims0, "first image");
  chck_inSet(ims1, "second image");
  chck_inSet(bgs0, "first background");
  chck_inSet(bgs0, "second background");
  #undef chck_inSet

}




struct FFargs {

  Volume & vol;
  const Map & bgs;
  const Map & dfs;
  const Map & gaps;

  ProgressBar bar;
  pthread_mutex_t proglock;

  FFargs(Volume & v, Map & b, Map & d, Map & g, bool verbose=false)
    : vol(v), bgs(b), dfs(d), gaps(g)
    , bar(verbose , "Performing flat field.", vol.shape()(0))
    , proglock(PTHREAD_MUTEX_INITIALIZER)
  {}

  void update_bar() {
    pthread_mutex_lock(&proglock);
    bar.update();
    pthread_mutex_unlock(&proglock);
  }

};

bool FFinThread (void * _thread_args, long int idx) {
  FFargs * dist = (FFargs*) _thread_args;
  if (!dist)
    throw_error("read thread", "Inappropriate thread function arguments.");
  if ( idx >= dist->vol.shape()(0) )
    return false;
  Map io( dist->vol(idx, blitz::Range::all(), blitz::Range::all()) );
// TODO : take care of gaps
  flatfield(io, dist->bgs, dist->dfs);
  dist->update_bar();
  return true;
}




struct FrameFormArgs{

  Volume & res;
  const Volume & ims0;
  const Volume & ims1;
  const Map & gaps;
  const PointF2D & shift;
  const float ashift;
  const float arc;

  ProgressBar bar;
  pthread_mutex_t proglock;

  FrameFormArgs(Volume & _res, const Volume & _ims0, const Volume & _ims1, const Map & _gaps,
                const PointF2D & _shift, float _ashift, float _arc, bool verbose=false)
    : res(_res)
    , ims0(_ims0)
    , ims1(_ims1)
    , gaps(_gaps)
    , shift(_shift)
    , ashift(_ashift)
    , arc(_arc)
    , bar(verbose , "Performing flat field.", ims0.shape()(0))
    , proglock(PTHREAD_MUTEX_INITIALIZER)
  {}

  void update_bar() {
    pthread_mutex_lock(&proglock);
    bar.update();
    pthread_mutex_unlock(&proglock);
  }

};

bool FrameFormInThread (void * _thread_args, long int idx) {

  FrameFormArgs * dist = (FrameFormArgs*) _thread_args;
  if (!dist)
    throw_error("FormFrame", "Inappropriate thread function arguments.");
  if ( idx >= dist->ims0.shape()(0) )
    return false;

  dist->update_bar();
  return true;

}






/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  const Shape ish = ImageSizes(args.ims0.at(0));
  const Shape sh = Shape( ish(0) - args.crop.top  - args.crop.top,
                          ish(1) - args.crop.left - args.crop.right );

  #define ReadSumSet( set, res ) { \
    if ( ! set.empty() ) { \
      Volume tmp; \
      ReadVolume(set, tmp, args.beverbose); \
      if ( faceShape(tmp.shape()) != ish ) \
        exit_on_error("ReadAux", "Wrong image sizes."); \
      binn(tmp, Binn3(1,1,0)); \
      Map img = tmp(0, blitz::Range::all(), blitz::Range::all()); \
      crop(img, res, args.crop); \
    } \
  }
  Map dfs (sh); ReadSumSet(args.dfs , dfs );
  Map bgs0(sh); ReadSumSet(args.bgs0, bgs0);
  Map bgs1(sh); ReadSumSet(args.bgs1, bgs1);
  #undef ReadSumSet

  Map gaps(sh);
  if ( ! args.gaps.empty() ) {
    Map tmp;
    ReadImage(args.gaps, tmp, ish);
    crop(tmp, gaps, args.crop);
  } else
    gaps = 1;

  #define ReadSet(set, bmap, vol) { \
    ReadVolume(set, vol, args.beverbose); \
    crop(vol, args.crop); \
    if ( faceShape(vol.shape()) != sh ) \
      exit_on_error("ReadSet", "Wrong image sizes."); \
    FFargs ffa(vol, bmap, dfs, gaps, args.beverbose); \
    execute_in_thread(FFinThread, &ffa); \
  }
  Volume ims0; ReadSet(args.ims0, bgs0, ims0);
  Volume ims1; ReadSet(args.ims1, bgs1, ims1);
  if (ims0.shape() != ims1.shape())
    exit_on_error("InputSets", "Volumes are of different size.");
  #undef ReadSet


  Volume frames( ims0.shape()(0), ims0.shape()(1)-args.shift.y, ims0.shape()(2)-args.shift.x );




  /*
  Volume ivol;
  ReadVolume(args.images, ivol, args.beverbose);
  crop(ivol,args.crp);
  binn(ivol,args.bnn);

  const blitz::TinyVector<blitz::MyIndexType, 3> vsh(ivol.shape());
  const Path outmask =  ( string(args.outmask).find('@') == string::npos ) ?
                          args.outmask.dtitle() + "-@" + args.outmask.extension() :
                          string( args.outmask ) ;

  int sliceDim;
  Shape ssh;
  string sindex = args.slicedesc.size()  ?  args.slicedesc  :  "Z";
  switch ( sindex.at(0) ) {
    case 'x':
    case 'X':
      sindex.erase(0,1);
      sliceDim=2;
      ssh = Shape(vsh(0),vsh(1));
      break;
    case 'y':
    case 'Y':
      sindex.erase(0,1);
      sliceDim=1;
      ssh = Shape(vsh(0),vsh(2));
      break;
    case 'z':
    case 'Z':
      sindex.erase(0,1);
    default:
      sliceDim=0;
      ssh = Shape(vsh(1),vsh(2));
  }

  const int sliceSz = vsh(sliceDim);
  const string sliceformat = mask2format(outmask, sliceSz);
  const vector<int>indices = slice_str2vec(sindex, sliceSz);
  const bool toInt = fisok(args.mincon)  ||  fisok(args.maxcon) || args.SaveInt;
  const float
    mincon  =  ( fisok(args.mincon)  ||  ! toInt )  ?  args.mincon  :  min(ivol),
    maxcon  =  ( fisok(args.maxcon)  ||  ! toInt )  ?  args.maxcon  :  max(ivol);

  Map cur(ssh);
  ProgressBar bar(args.beverbose, "Saving slices", indices.size() );
  for (unsigned slice=0 ; slice < indices.size() ; slice++ ) {
    const Path fileName =  indices.size() == 1  ?  args.outmask : Path(toString(sliceformat, slice));
    switch ( sliceDim ) {
        case 2:
          cur = ivol(blitz::Range::all(), blitz::Range::all(), indices.at(slice));
          break;
        case 1:
          cur = ivol(blitz::Range::all(), indices.at(slice), blitz::Range::all());
          break;
        case 0:
          cur = ivol(indices.at(slice), blitz::Range::all(), blitz::Range::all());
          break;
    }
    if (toInt)
      SaveImage(fileName, cur , mincon, maxcon);
    else
      SaveImage(fileName, cur);
    bar.update();
  }

  */

  exit(0);








}




using namespace blitz;
