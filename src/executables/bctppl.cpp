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

#define _USE_MATH_DEFINES // for M_PI

#include "../common/common.h"
#include "../common/poptmx.h"
#include "../common/flatfield.h"
#include "../common/kernel.h"
#include <algorithm>
#include <string.h>
#include <math.h>
#include <unordered_map>


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
  PointF2D shift;  // for frame formation
  float ashift;  // for frame formation
  Filter filter_type;  // for CT
  float dd;  // for Phase and CT
  float arc; // for frame formation and CT
  Crop crop; // for frame formation
  Crop fcrp; // for frame formation
  float angle;
  float center; // for frame formation and CT
  bool SaveInt;
  bool beverbose;
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : outmask("_.tif")
  , shift(0,0)
  , dd(1.0)
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
      .add(poptmx::OPTION, &ashift, 'S', "start", "Starting angle of the second data set.",
           "The starting angular position relative to the starting position of the first data set.")
      .add(poptmx::OPTION, &arc, 'a', "arcan", "CT scan range (if > 1 deg), or step size (if < 1 deg).",
           "If value is greater than 1deg then it represents the arc of the CT scan."
           " Otherwise the value is the step size.", toString(arc))
      .add(poptmx::OPTION, &crop, 't', "crop", "Crop input images: " + CropOptionDesc, "")
      .add(poptmx::OPTION, &fcrp, 'T', "crop-final", "Crops final image: " + CropOptionDesc, "")
      .add(poptmx::OPTION, &angle,'r', "rotate", "Rotate input images by given angle.", "")
      .add(poptmx::OPTION, &center, 'c', "center", "Rotation center.", CenterOptionDesc)
      .add(poptmx::OPTION, &filter_type, 0, "filter",
           "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
      .add(poptmx::OPTION, &dd, 'r', "resolution",
           "Pixel size (micron).", ResolutionOptionDesc, toString(dd))

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

  if ( arc <= 0.0 )
    exit_on_error(command, "CT arc (given by "+table.desc(&arc)+") must be strictly positive.");
  if ( ashift < 0.0 )
    exit_on_error(command, "Angular shift between two sets (given by "+table.desc(&shift)+")"
                           " must be strictly positive.");
  if ( ashift >= 360.0 ) {
    float dummy;
    ashift -= 360 * modf( ashift/360.0, &dummy);
  }




}




class FlatInThread : public InThread {

  Volume & vol;
  const Map & bgs;
  const Map & dfs;
  const Map & gaps;

public:

  FlatInThread(Volume & v, Map & b, Map & d, Map & g, bool verbose=false)
    : vol(v), bgs(b), dfs(d), gaps(g)
    , InThread(verbose , "Performing flat field.", v.shape()(0))
  {}

  bool inThread (long int idx) {
    if ( idx >= vol.shape()(0) )
      return false;
    Map io( vol(idx, whole, whole) );

  // TODO : take care of gaps

    flatfield(io, bgs, dfs);
    update_bar();
    return true;
  }

};




class FrameFormInThread : public InThread {

  Volume & res;
  const Volume & ims0;
  const Volume & ims1;
  const Shape ish;
  const Map & gaps;
  const float arc;
  const float step;
  const float oz;
  const PointF2D & shift;
  const float ashift;
  const int nshift;

public:

  FrameFormInThread(Volume & _res, const Volume & _ims0, const Volume & _ims1, const Map & _gaps,
                float _arc, const PointF2D & _shift, float _ashift, bool verbose=false)
    : res(_res)
    , ims0(_ims0)
    , ims1(_ims1)
    , ish(faceShape(ims0.shape()))
    , gaps(_gaps)
    , arc(_arc)
    , step( arc / (ims0.shape()(0)-1) )
    , oz((int)(180/step))
    , shift(_shift)
    , ashift(_ashift)
    , nshift(ashift/step)
    , InThread(verbose , "Performing flat field.", _ims0.shape()(0))
  {
    res.resize( oz , ims0.shape()(1)-shift.y, ims0.shape()(2)-shift.x );
  }


  bool inThread(long int idx) {

    if ( idx >= oz )
      return false;

    Map im0(ish);
    im0 = ims0(idx, whole, whole);
    Map im1(ish);
    const int idx1 = idx + oz - nshift  -
                     ( idx + oz >= nshift  ?  0  :  oz ) ;
    im1 = ims1(idx1, whole, whole);


    // TODO actual formation


    update_bar();
    return true;
  }


};





class CTinThread : public InThread {

private:

  unordered_map<pthread_t,CTrec> recs;
  const Volume & frames;
  Volume & result;
  const Contrast contrast;
  const Filter filter;
  const float pixelSize;
  const Shape ssh;

  bool inThread(long int idx) {

    if ( idx >= frames.size() )
      return false;

    const pthread_t me = pthread_self();
    if ( ! recs.count(me) ) // first call
      recs.insert({me, CTrec(ssh, contrast, 180, filter)}); // arc is 180 after frames formation
    CTrec & rec = recs.at(me);
    Map sino(frames(whole, idx, whole));
    result(idx, whole, whole)
        = rec.reconstruct(sino , 0, pixelSize); // centre is 0 after frames formation
    update_bar();
    return true;

  }

public:

  CTinThread(const Volume & fms, Volume & res, Contrast cn, Filter ft, float pp, bool verbose=false)
    : frames(fms)
    , result(res)
    , contrast(cn)
    , filter(ft)
    , pixelSize(pp)
    , ssh(fms.shape()(2), fms.shape()(0))
    , InThread(verbose , "Performing flat field.", fms.shape()(0))
  {
    result.resize(fms.shape()(1), ssh(0), ssh(0));
  }

};



/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  const Shape ish = ImageSizes(args.ims0.at(0));
  const Shape sh = Shape( ish(0) - args.crop.top  - args.crop.top,
                          ish(1) - args.crop.left - args.crop.right );

  // Read DFs, BGs, GapMask

  #define ReadSumSet( set, res ) { \
    if ( ! set.empty() ) { \
      Volume tmp; \
      ReadVolume(set, tmp, args.beverbose); \
      if ( faceShape(tmp.shape()) != ish ) \
        exit_on_error("ReadAux", "Wrong image sizes."); \
      binn(tmp, Binn3(1,1,0)); \
      Map img = tmp(0, whole, whole); \
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



  // Read sample sets

  #define ReadSet(set, bmap, vol) { \
    ReadVolume(set, vol, args.beverbose); \
    crop(vol, args.crop); \
    if ( faceShape(vol.shape()) != sh ) \
      exit_on_error("ReadSet", "Wrong image sizes."); \
    FlatInThread(vol, bmap, dfs, gaps, args.beverbose).execute(); \
  }

  Volume ims0; ReadSet(args.ims0, bgs0, ims0);
  Volume ims1; ReadSet(args.ims1, bgs1, ims1);

  #undef ReadSet

  if (ims0.shape() != ims1.shape())
    exit_on_error("InputSets", "Volumes are of different size.");

  dfs.free();
  bgs0.free();
  bgs1.free();



  // Form final frames

  const float arc =  args.arc > 1.0  ?  args.arc  :  args.arc * ims0.shape()(0);
  Volume frames;
  FrameFormInThread(frames, ims0, ims1, gaps, arc, args.shift, args.ashift, args.beverbose).execute();
  const int oz = frames.shape()(0);
  const Shape fsh = faceShape(frames.shape());

  ims0.free();
  ims1.free();



  // TODO Rings

  // TODO Zinger

  // TODO Phase





  // CT

  Volume recs;
  CTinThread(frames, recs, Contrast::PHS, args.filter_type, args.dd).execute();

  // frames.free(); // not yet


  // Output

  const float mincon = args.SaveInt ? min(frames) : 0;
  const float maxcon = args.SaveInt ? max(frames) : 0;
  const string sliceformat = mask2format(args.outmask, oz);
  Map cur(faceShape(frames.shape()));
  ProgressBar bar(args.beverbose, "Saving slices", oz );

  for (unsigned slice=0 ; slice < oz ; slice++ ) {
    const Path fileName =  oz == 1  ?  args.outmask : Path(toString(sliceformat, slice));
    cur = frames(slice, whole, whole);
    if (args.SaveInt)
      SaveImage(fileName, cur, mincon, maxcon);
    else
      SaveImage(fileName, cur);
    bar.update();
  }

  exit(0);


}
