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
#include "../common/flatfield.h"
#include <algorithm>
#include <string.h>
#include <unordered_map>
#include <deque>
#include <unistd.h>






#include "../common/poptmx.h"


using namespace std;

struct StitchRules {
  uint nofIn;
  Shape ish;
  Crop crp;                  ///< Crop input projection image
  Crop fcrp;                  ///< Crop final projection image
  Binn bnn;                  ///< binning factor
  float angle;                ///< Rotation angle.
  PointF2D origin1;            ///< Origin of the next image in the first stitch
  PointF2D origin2;            ///< Origin of the next image in the second stitch
  uint origin1size;
  uint origin2size;           ///< Nof images in the second stitch - needed only if it is requested (origin2)
  PointF2D originF;            ///< Origin of the flipped portion
  bool flipUsed;               ///< indicates if originF was given in options.
  deque<uint> splits;          ///< Split pooints to separate samples.
  uint edge;               ///< blur of mask and image edges.
  StitchRules()
  : nofIn(0)
  , angle(0)
  , origin1size(1)
  , origin2size(1)
  , flipUsed(false)
  , edge(0)
  {}
};


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  deque< deque<ImagePath> > images;        ///< images to combine
  deque<ImagePath> bgs;        ///< Array of the background images.
  deque<ImagePath> dfs;        ///< Array of the dark field images.
  deque<ImagePath> dgs;        ///< Array of the dark field images for backgrounds.
  deque<ImagePath> mks;        ///< Mask Array.
  ImagePath out_name;              ///< Name of the output image.
  string out_range;
  StitchRules st;
  int testMe;          ///< Prefix to save interim results
  //string sliceMatch;           ///< text file with list of matching slices
  bool beverbose;             ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
  poptmx::OptionTable table;
};


clargs::
clargs(int argc, char *argv[])
  : table("Combines multiple images to form the projection.",
          "Transforms and stitches portions of the projection from the complex CT experiment"
          " which may include 2D tiling and 180-deg flip."
          " Transformations are applied in the following order: rotate, crop, binning." )
  , testMe(-1)
  , beverbose(false)
{

  deque<ImagePath> iimages;

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &iimages, "images", "Input 2D or 3D images.",
         "All images must be of the same rank (2D or 3D). HDF5 format:\n"
         "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc)

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &out_name, 'o', "output", "Output image.", "", out_name)
    .add(poptmx::OPTION, &out_range, 'O', "select", "Slices to process.",
         SliceOptionDesc + ". Only makes sense in multiple projections.", "all")
    .add(poptmx::OPTION, &st.crp, 'c', "crop", "Crop input images: " + CropOptionDesc, "")
    .add(poptmx::OPTION, &st.fcrp, 'C', "crop-final", "Crops final image: " + CropOptionDesc, "")
    .add(poptmx::OPTION, &st.bnn, 'b', "binn", BinnOptionDesc, "")
    .add(poptmx::OPTION, &st.angle,'r', "rotate", "Rotation angle.", "")
    .add(poptmx::OPTION, &st.origin1, 'g', "origin", "Origin of the image in the first stitch.",
      "Position of the next image origin (top left corner) on the current image.")
    .add(poptmx::OPTION, &st.origin2, 'G', "second-origin", "Origin of the image in the second stitch.",
      "Position of the next image origin (top left corner) on the current image in the second order stitch.")
    .add(poptmx::OPTION, &st.origin2size, 'S', "second-size", "Number of imasges in the second stitch.",
      "Required if and only if the second stitch is requested.")
    .add(poptmx::OPTION, &st.originF, 'f', "flip-origin", "Origin of the flipped portion of the image.",
      "If used, makes second half of the input images to be assigned to the flipped portion."
      " Requires even number of input images.")
    .add(poptmx::OPTION, &st.splits, 's', "split", "Split point(s)",
         "Final image can be split into sub-images to put different portions of it apart as independent"
         " files, for example to separate samples. By default splitting happens horizontally,"
         " but if the vertical split is needed, just add a 0 split point.")
    .add(poptmx::OPTION, &bgs, 'B', "bg", "Background image(s)", "")
    .add(poptmx::OPTION, &dfs, 'D', "df", "Dark field image(s)", "")
    .add(poptmx::OPTION, &dgs, 'F', "dg", "Dark field image(s) for backgrounds", "")
    .add(poptmx::OPTION, &mks, 'M', "mask", "Mask image",
         "Image where values are weights of corresponding pixels in superimposition operations."
         " F.e. 0 values exclude corresponding or affected pixels from further use.")
    .add(poptmx::OPTION, &st.edge, 'e', "edge", "Thickness in pixels of bluring mask edges.",
           "Smoothly reduces the weight of pixels around the mask edges (0 values in mask)"
           " to produce seamless image stitching." )
    //.add(poptmx::OPTION, &sliceMatch, 0, "match", "File with slice match list",
    //     "In the case of 3D i/o this file must contain lines with the integers,"
    //     " each representing the slice in the corresponding input volume to be used for the projection.")
    .add(poptmx::OPTION, &testMe, 't', "test", "Produces interim images.",
         "Uses output name with suffixes to store results. In case of multiple projections provides"
         " slice index to test; ignored for single image mode.")
    .add_standard_options(&beverbose)
    .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);


  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }
  command = table.name();

  int tiledImages = table.count(&iimages);
  if (tiledImages) {
    images.resize(tiledImages);
    for ( int curI = 0 ; curI < tiledImages ; curI++ )
      images.at(curI).push_back(iimages.at(curI));
  }
  if (!isatty(fileno(stdin))) { // read from pipe
    string inputline;
    while ( ! cin.eof() && getline(cin, inputline) ) {
      trim(inputline);
      if (inputline.size()) {
        deque<string> inputdeque = split(inputline, " ");
        if (!tiledImages) {
          tiledImages = inputdeque.size();
          images.resize(tiledImages);
        } else if (inputdeque.size() != tiledImages) {
          exit_on_error(command, "Inconsistent number of input images given in stdin.");
        }
        for ( int curI = 0 ; curI < tiledImages ; curI++ )
          images.at(curI).push_back(inputdeque.at(curI));
      }
    }
  }
  if ( ! tiledImages )
    exit_on_error(command, "No input images given.");

  if ( ! table.count(&out_name) )
    exit_on_error(command, "No output name provided. Use option " + table.desc(&out_name) + ".");
  if (table.count(&testMe) && testMe<0)
    exit_on_error(command, "Negative test projection given with " + table.desc(&testMe) + " option.");

  if ( table.count(&st.originF) ) {
    if ( tiledImages % 2 )
      exit_on_error(command, string () +
        "Use of the " + table.desc(&st.originF) + " option requires even number of input images"
        " (while " + toString(tiledImages) + " counted).");
    tiledImages /= 2;
  }
  if ( table.count(&st.origin2) > table.count(&st.origin1) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2) + " requires also " + table.desc(&st.origin1) + " option.");
  if ( table.count(&st.origin2) > table.count(&st.origin2size) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2) + " requires also " + table.desc(&st.origin2size) + " option.");
  if ( table.count(&st.origin2size) && ! table.count(&st.origin2) && ! table.count(&testMe) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2size) + " must be used with either "
      + table.desc(&st.origin2) + " or " + table.desc(&testMe) + " options.");
  if ( table.count(&st.origin2size)  &&  st.origin2size  < 2 )
    exit_on_error(command, string () +
      "Requested second stitch size (" + toString(st.origin2size) + ") is less than 2.");
  if ( table.count(&st.origin2size)  &&  tiledImages % st.origin2size )
    exit_on_error(command, string () +
      "Total number of tiled images (" + toString(tiledImages) + ") is not a multiple of the requested second stitch size"
      " (" + toString(st.origin2size) + ") given by " + table.desc(&st.origin2size) + " option.");

  sort(st.splits.begin(), st.splits.end());
  unique(st.splits.begin(), st.splits.end());
  if ( st.splits.size() == 1 && st.splits.at(0) == 0 )
    exit_on_error(command, "The list of splits contains only 0 (marking vertical splits).");

  st.angle *= M_PI/180;
  st.nofIn = tiledImages;
  st.flipUsed=table.count(&st.originF);
  st.origin1size = st.nofIn / (st.flipUsed ? 2 : 1) / st.origin2size ;

}


namespace blitz {
static inline float denan(float x){ return isnormal(x) ? x : 0.0 ;}
BZ_DECLARE_FUNCTION(denan);
}

void SaveDenan(const ImagePath & filename, const Map & storage, bool saveint=false) {
  Map outm(storage.shape());
  outm=denan(storage);
  SaveImage(filename, outm, saveint);
}


class ProcProj {

  static const string modname;
  StitchRules st;
  FlatFieldProc canon;

  Map msk1, msk2, mskf, mskF; // shared
  Map iar, rar, car, final; // own
  deque<Map> allIn, o1Stitch, o2Stitch;

  void procInImg(const Map & im, Map & om, bool doFF=true){
    if (im.shape() != st.ish)
      throw_error(modname, "Unexpected shape of image to process.");
    iar.resize(st.ish);
    iar=im;
    if ( doFF /*&& ( bgar.size() || dfar.size() )*/ )
      canon.process(iar);
    rotate(iar, rar, st.angle);
    crop(rar, car, st.crp);
    binn(car, final, st.bnn);
    om.resize(final.shape());
    om = final;
  }


  static void stitch(PointF2D origin, Map & oarr,  const deque<Map> & iarr
                    ,  const deque<Map> & gprr = deque<Map>() ) {

    const int isz = iarr.size();

    if ( isz == 0 )
      throw_error(modname, "Nothing to stitch.");
    if ( isz == 1 ) {
      oarr.reference(iarr[0]);
      return;
    }
    if (!gprr.size()) {
    } else if (gprr.size() == 1) {
      const Shape csz = gprr[0].shape();
      for (int acur = 0 ; acur < isz ; acur++ )
        if (iarr[acur].shape()!=csz)
          throw_error(modname, "Different image sizes in mask and inputs.");
    } else if (gprr.size() != iarr.size()) {
      throw_error(modname, "Inconsistent sizes of mask and arrays.");
    } else {
      for (int acur = 0 ; acur < isz ; acur++ )
        if (iarr[acur].shape()!=gprr[acur].shape())
          throw_error(modname, "Non matching image sizes in mask and input arrays.");
    }

    int minx=0, maxx=0, miny=0, maxy=0;
    for (int acur = 0 ; acur < isz ; acur++ ) {
      const float
        orgx = acur*origin.x,
        orgy = acur*origin.y,
        tilx = orgx + iarr[acur].shape()(1)-1,
        tily = orgy + iarr[acur].shape()(0)-1;
      if (orgx < minx) minx = orgx;
      if (tilx > maxx) maxx = tilx;
      if (orgy < miny) miny = orgy;
      if (tily > maxy) maxy = tily;
    }

    const Shape osh(maxy-miny+1, maxx-minx+1);
    oarr.resize(osh);

    for (ArrIndex ycur = 0 ; ycur < osh(0) ; ycur++ ) {
      for (ArrIndex xcur = 0 ; xcur < osh(1) ; xcur++ ) {

        int sweight=0;
        float svals=0.0;

        for (int acur = 0 ; acur < isz ; acur++ ) {
          const Map & curar = iarr[acur];
          const Shape cursh = curar.shape();
          const Shape coo = Shape(miny+ycur-acur*origin.y, minx+xcur-acur*origin.x);
          if ( coo(0) >= 0 && coo(0) < cursh(0) && coo(1) >= 0 && coo(1) < cursh(1) ) {
            const float varcur = curar(coo);
            if ( fisok(varcur) ) {
              int weight = 1 + ( cursh(0) - abs( 2*coo(0) - cursh(0) + 1l ) )
                           * ( cursh(1) - abs( 2*coo(1) - cursh(1) + 1l ) );
              if ( gprr.size() == 1 )
                weight *= gprr[0](coo);
              else if ( gprr.size() == iarr.size() )
                weight *= gprr[acur](coo);
              sweight += weight;
              svals += varcur * weight;
            }
          }
        }

        oarr(ycur,xcur) = sweight ? svals / sweight : NAN ;

      }
    }

  }


  void prepareMask(Map & _gaps, bool bepicky) {
    const float mm = min(_gaps);
    const float MM = max(_gaps);
    if (MM <= 0)
      throw_error("GapsMask", "Mask covers whole image.");
    if (mm==MM) // no _gaps
      return;
    _gaps = mm + _gaps / (MM-mm);
    for (ArrIndex i = 0 ; i<_gaps.shape()(0) ; i++)
      for (ArrIndex j = 0 ; j<_gaps.shape()(1) ; j++)
        if (bepicky && _gaps(i,j)<1.0 )
          _gaps(i,j)=0.0 ;
        else if (! bepicky && _gaps(i,j)>0.0 )
          _gaps(i,j)=1.0 ;

    const Shape ish = _gaps.shape();
    const float step = 1.0 / (st.edge +1);
    Map tmp(_gaps.shape());
    tmp = _gaps;

    for ( int stp = 1 ; stp <= st.edge ; stp++ ) {
      const float fill = step*stp;

      for (ArrIndex i = 0 ; i<ish(0) ; i++)
        for (ArrIndex j = 0 ; j<ish(1) ; j++)

          if ( _gaps(i,j) != 1.0 )
            for (ArrIndex ii = i-1 ; ii <= i+1 ; ii++)
              for (ArrIndex jj = j-1 ; jj <= j+1 ; jj++)

                if (     ii >= 0 && ii < ish(0) && jj >= 0 && jj < ish(1)
                  &&  _gaps(ii,jj) == 1.0 )
                  tmp(ii,jj) = fill;
      _gaps = tmp;
    }

  }


public:

  ProcProj( const StitchRules & _st, const Map & bgar, const Map & dfar
          , const Map & dgar, const Map & gpar, const Path & saveMasks = Path())
    : st(_st)
    , canon(bgar, dfar, gpar)
    , allIn(st.nofIn)
    , o1Stitch(st.nofIn/st.origin1size)
    , o2Stitch(st.flipUsed ? 2 : 1)
  {
    if ( ! area(st.ish) )
      throw_error(modname, "Zerro area to process.");
    if ( ! st.nofIn )
      throw_error(modname, "Zerro images for input.");
    if (bgar.size() && bgar.shape() != st.ish)
      throw_error(modname, "Unexpected BG shape.");
    if (dfar.size() && dfar.shape() != st.ish)
      throw_error(modname, "Unexpected DF shape.");
    if (dgar.size() && dgar.shape() != st.ish)
      throw_error(modname, "Unexpected DG shape.");
    if (gpar.size()) {
      if (gpar.shape() != st.ish)
        throw_error(modname, "Unexpected mask shape.");

    #define SaveMask(vol, suf) \
       if (saveMasks.length()) \
         SaveDenan(saveMasks.dtitle() + suf + ".tif", vol);

      procInImg(gpar, msk1, false);
      prepareMask(msk1, true);
      SaveMask(msk1, "1");
      if (st.origin1size==1)
        msk2.reference(msk1);
      else {
        deque<Map> supply(st.origin1size, msk1);
        stitch(st.origin1, msk2, supply);
        prepareMask(msk2, false);
        SaveMask(msk2, "2");
      }
      if ( st.flipUsed ) {
        if ( st.origin2size == 1 )
          mskf.reference(msk2);
        else {
          deque<Map> supply(st.origin2size, msk2);
          stitch(st.origin2, mskf, supply);
          prepareMask(mskf, false);
        }
        SaveMask(mskf, "D");
        mskF.resize(mskf.shape());
        mskF = mskf.reverse(blitz::secondDim);
        SaveMask(mskF, "F");
        if (saveMasks.length()) {
          Map maskA;
          stitch(st.originF, maskA, deque<Map>({mskf, mskF}));
          SaveMask(maskA, "O");
        }
      }
    #undef SaveMask
    }

  }


  ProcProj(const ProcProj & other)
    : st(other.st)
    , canon(other.canon)
    , allIn(st.nofIn)
    , o1Stitch(st.nofIn/st.origin1size)
    , o2Stitch(st.flipUsed ? 2 : 1)
    , msk1(other.msk1)
    , msk2(other.msk2)
    , mskf(other.mskf)
    , mskF(other.mskF)
  {}


  string sub_proc(uint orgsize, PointF2D origin, const deque<Map> & iar, deque<Map> & oar
                 , uint nofin, const Map & msk, const string & format, const ImagePath & interim_name) {

    if ( orgsize == 1 ) {
      for(int curM = 0 ; curM < oar.size() ; curM++)
        oar[curM].reference(iar[curM]);
      return ImagePath();
    }

    ImagePath lastSaved;
    for ( int inidx=0 ; inidx<nofin ; inidx += orgsize ) {
      int cidx=inidx/orgsize;
      deque<Map> supply( iar.begin() + inidx, iar.begin() + inidx + orgsize) ;
      deque<Map> masks( msk.size() ? 1 : 0 , msk);
      stitch(origin, oar[cidx], supply, masks);
      if ( ! interim_name.empty() ) {
        Map cres;
        if ( ! origin.x * origin.y ) {
          cres.reference(oar[cidx]);
        } else if ( abs(origin.x) < abs(origin.y)  ) {
          int crppx = abs(origin.x * (supply.size()-1));
          crop(oar[cidx], cres, Crop(0, crppx, 0, crppx));
        } else {
          int crppx = abs(st.origin1.y * (supply.size()-1));
          crop(o1Stitch[cidx], cres, Crop(crppx, 0, crppx, 0));
        }
        string svformat = mask2format(format+"@", oar.size() );
        lastSaved = interim_name.dtitle() + toString(svformat, cidx) + ".tif";
        SaveDenan(lastSaved, cres);
      }
    }
    return lastSaved;

  }


  bool process(const deque<Map> & allInR, deque<Map> & res, const ImagePath & interim_name = ImagePath()) {

    if (allInR.size() != st.nofIn)
      return false;
    ImagePath lastSaved;

    // prepare input images
    int curF=0, cur2=0, cur1=0;
    for ( int curproj = 0 ; curproj < st.nofIn ; curproj++) {
      procInImg(allInR[curproj], allIn[curproj]);
      if ( ! interim_name.empty() ) {
        const string sfI = toString(mask2format("_I@", st.nofIn), curproj);
        const string sfF = st.flipUsed ? (curF ? "_F" : "_D") : "";
        const string sf2 = st.origin2size > 1 ? toString(mask2format(".@", st.origin2size), cur2) : "";
        const string sf1 = st.origin1size > 1 ? toString(mask2format(".@", st.origin1size), cur1) : "";
        lastSaved = interim_name.dtitle() + sfI + sfF + sf2 + sf1 + string(".tif");
        SaveDenan(lastSaved, allIn[curproj]);
        cur1++;
        if (cur1==st.origin1size) {
          cur1=0;
          cur2++;
          if (cur2==st.origin2size) {
            cur2=0;
            curF++;
          }
        }
      }
    }

    // first stitch
    sub_proc(st.origin1size, st.origin1, allIn, o1Stitch, st.nofIn, msk1, "_U", interim_name);

    // second stitch
    sub_proc(st.origin2size, st.origin2, o1Stitch, o2Stitch, o1Stitch.size(), msk2, "_V", interim_name);

    // flip stitch
    if ( st.flipUsed ) {
      //if ( o2Stitch.size() != 2 ) // May it ever happen ?
      //  throw_error(args.command, "Number of images requested to flip-stitch is not equal to two.");
      o2Stitch[1].reverseSelf(blitz::secondDim);
      deque<Map> masks = mskf.size() ? deque<Map>({mskf, mskF}) : deque<Map>();
      stitch(st.originF, final, o2Stitch, masks);
      if ( ! interim_name.empty() )  {
        Map tmp;
        ReadImage(lastSaved, tmp);
        tmp.reverseSelf(blitz::secondDim);
        SaveImage(lastSaved, tmp);
        SaveDenan(interim_name.dtitle() + "_W.tif" , final);
      }
    } else {
      final.reference(o2Stitch[0]);
    }

    // final crop
    if ( st.fcrp != Crop() ) {
      crop(final, st.fcrp);
      if ( ! interim_name.empty() )
      SaveDenan( interim_name.dtitle() + "_X.tif", final );
    }

    // splits
    if ( st.splits.empty() ) {
      res.resize(1);
      res[0].resize(final.shape());
      res[0]=final;
    } else {
      const string svformat = mask2format("_Z@", st.splits.size() );
      int fLine=0, lLine=0;
      const int vsplit = st.splits.at(0) ? 0 : 1;
      const int mLine = final.shape()(vsplit);
      for (int curS = vsplit ;  curS<=st.splits.size()  ;  curS++) {
        int curI = curS - vsplit;
        lLine = ( curS == st.splits.size()  ||  mLine < st.splits.at(curS) )
          ?  mLine :  st.splits.at(curS) ;
        lLine--;
        if ( lLine > fLine ) {
          Map resp = vsplit
            ? final( all, blitz::Range(fLine, lLine) )
            : final( blitz::Range(fLine, lLine), all ) ;
          if (res.size() < curI+1)
            res.emplace_back();
          res[curI].resize(resp.shape());
          res[curI]=resp;
          if ( ! interim_name.empty() )
            SaveDenan( interim_name.dtitle() + toString(svformat, curI) + ".tif", res[curI] );
        }
        fLine=lLine+1;
      }
    }

    return true;

  }


};

const string ProcProj::modname="ProcProj";



class ProjInThread : public InThread {

  deque<ReadVolumeBySlice> & allInRd;
  deque<SaveVolumeBySlice> & allOutSv;
  const ProcProj & proc;
  const vector<int> & projes;

  unordered_map<pthread_t, ProcProj> procs;
  unordered_map<pthread_t, deque<Map> > allInMaps;
  unordered_map<pthread_t, deque<Map> > results;

  bool inThread(long int idx) {

    if (idx >= projes.size() || projes[idx] >= allOutSv[0].slices())
      return false;

    const pthread_t me = pthread_self();
    lock();
    if ( ! procs.count(me) ) { // first call
      procs.emplace(me, proc);
      allInMaps.emplace(me, allInRd.size());
      results.emplace(me,0);
    }
    ProcProj & myProc = procs.at(me);
    deque<Map> & myAllIn = allInMaps.at(me);
    deque<Map> & myRes = results.at(me);
    unlock();

    for (ArrIndex curI = 0  ;  curI<allInRd.size()  ;  curI++ )
      allInRd[curI].read(projes[idx], myAllIn[curI]);
    myProc.process(myAllIn, myRes);
    for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ )
      allOutSv[curO].save(projes[idx], myRes[curO]);

    bar.update();
    return true;

  }

public:

  ProjInThread(deque<ReadVolumeBySlice> & _allInRd, deque<SaveVolumeBySlice> & _outSave
              , const ProcProj & _proc, const vector<int> & _projes, bool verbose=false)
    : InThread(verbose, "processing projections", _outSave[0].slices() )
    , proc(_proc)
    , projes(_projes)
    , allInRd(_allInRd)
    , allOutSv(_outSave)
  {}


};


void average_stack(Map & oar, const deque<ImagePath> & stack, const Shape & ish) {
  if (stack.empty())
    return;
  oar.resize(ish);
  oar=0.0;
  Map iar;
  for ( int curf = 0 ; curf < stack.size() ; curf++) {
    ReadImage(stack[curf].repr(), iar, ish);
    oar+=iar;
  }
  oar /= stack.size();
}



/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;
  const Shape ish(ImageSizes(args.images.at(0).at(0).repr()));
  StitchRules st = args.st;
  st.ish = ish;

  // Read auxilary images
  Map bgar;
  average_stack(bgar, args.bgs, ish);
  Map dfar;
  average_stack(dfar, args.dfs, ish);
  Map dgar;
  average_stack(dgar, args.dgs, ish);
  Map gpar;
  average_stack(gpar, args.mks, ish);

  // Prepare read factories
  const int nofIn = args.images.size();
  deque<ReadVolumeBySlice> allInRd(nofIn);
  for ( int curI = 0 ; curI < nofIn ; curI++) {
    allInRd.at(curI).add(args.images.at(curI));
    uint cSls = allInRd.at(curI).slices();
    if (!cSls)
      exit_on_error(args.command, "No images in input "+ toString(curI) +".");
    if (curI && allInRd.at(0).slices() != cSls)
      exit_on_error(args.command, "Not matching of slices in input "+ toString(curI) +".");
  }
  const int nofProj = allInRd.at(0).slices();
  const vector<int> projes = slice_str2vec(args.out_range, nofProj);
  const int nofOuts = projes.size();
  if (!nofOuts)
    exit_on_error(args.command, "Given range \"" + args.out_range + "\""
                                " is beyond input slices"+toString(nofProj)+".");
  if ( args.testMe >= nofProj )
    exit_on_error(args.command, "Requested test is beyond number of projections.");

  // Process one slice
  Map zmap(ish);
  zmap=0.0;
  deque<Map> allOut, allIn;
  for ( ArrIndex curI = 0 ; curI < allInRd.size() ; curI++) {
    allIn.emplace_back(ish);
    if (args.testMe >= 0)
      allInRd[curI].read(args.testMe, allIn[curI]);
    else if (nofOuts == 1)
      allInRd[curI].read(projes[0], allIn[curI]);
    else
      allIn.back().reference(zmap);
  }
  const string testFormat =  args.testMe < 0 ? string()
               : toString(mask2format(args.out_name.dtitle(), nofProj), args.testMe) + "%s";
  ProcProj canonPP(st, bgar, dfar, dgar, gpar, toString(testFormat, "_mask.tif"));
  canonPP.process(allIn, allOut, toString(testFormat, ".tif"));

  // Prepare saving factories
  const size_t nofSplts = allOut.size();
  const string spformat = mask2format("_split@", allOut.size());
  deque<SaveVolumeBySlice> allOutSv;
  for (int curSplt = 0 ; curSplt < nofSplts ; curSplt++) {
    ImagePath filedescind = args.out_name;
    if (args.testMe >= 0)
      filedescind = args.out_name.dtitle() + ".tif";
    if (nofSplts > 1)
      filedescind =   filedescind.dtitle() + toString(spformat, curSplt)
                    + filedescind.ext() + filedescind.desc();
    allOutSv.emplace_back(filedescind, allOut[curSplt].shape(), nofProj);
  }

  // save or process
  if ( args.testMe >= 0 )
    for (int curSplt = 0 ; curSplt < allOut.size() ; curSplt++)
      allOutSv[curSplt].save(args.testMe, allOut[curSplt]);
  else if ( nofOuts == 1 )
    for (int curSplt = 0 ; curSplt < allOut.size() ; curSplt++)
      allOutSv[curSplt].save(projes[0], allOut[curSplt]);
  else // finally process
    ProjInThread(allInRd, allOutSv, canonPP, projes, args.beverbose).execute();

  exit(0);

}

