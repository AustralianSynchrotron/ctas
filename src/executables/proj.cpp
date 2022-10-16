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


#include "../common/flatfield.h"
#include "../common/poptmx.h"
#include <algorithm>
#include <string.h>
#include <unordered_map>
#include <deque>
#include <unistd.h>


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
  float sigma;             ///< sigma used in gaussian gap closure.
  StitchRules()
  : nofIn(0)
  , angle(0)
  , origin1size(1)
  , origin2size(1)
  , flipUsed(false)
  , edge(0)
  , sigma(0)
  {}
};


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  deque< deque<ImagePath> > images;        ///< images to combine
  deque<ImagePath> bgs;        ///< Array of the background images.
  deque<ImagePath> dfs;        ///< Array of the dark field images.
  deque<ImagePath> dgs;        ///< Array of the dark field images for backgrounds.
  deque<ImagePath> mss;        ///< Mask Array.
  ImagePath out_name;              ///< Name of the output image.
  string out_range;
  StitchRules st;
  int testMe;          ///< Prefix to save interim results
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
    .add(poptmx::OPTION, &mss, 'M', "mask", "Mask image",
         "Image where values are weights of corresponding pixels in superimposition operations."
         " F.e. 0 values exclude corresponding or affected pixels from further use.")
    .add(poptmx::OPTION, &st.edge, 'e', "edge", "Thickness in pixels of edge transition.",
           "Smoothly reduces the weight of pixels around the mask edges (0 values in mask)"
           " to produce seamless image stitching." )
    .add(poptmx::OPTION, &st.sigma, 0, "sigma", "Sigma used in gaussian gap closure.",
           "The gaps left by the mask superimpositions can be closed with the gaussian blur with the given sigma."
           "If no mask is provided, the closure will be applied to pixels with values <= 0 or NAN." )
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
  st.nofIn = tiledImages;

  #define chkNofIns(ims, lbl) \
    if (ims.size() && ims.size() != 1 && ims.size() != st.nofIn) \
      exit_on_error(command, "Number of " lbl " images given by " + table.desc(&ims) + " option " \
                             " is neither 0, 1, nor the number of inputs (" + toString(st.nofIn) +  ").");

  chkNofIns(bgs, "background");
  chkNofIns(dfs, "darkfield");
  chkNofIns(dgs, "darkground");
  chkNofIns(mss, "mask");
  #undef chkNofIns

  if (dgs.size() && ! bgs.size())
    exit_on_error(command, "No background images (" + table.desc(&bgs) + ") for provided darkgrounds (" + table.desc(&dgs) + ").");
  if ( ! mss.size()  &&  ! bgs.size()  &&  (st.edge || st.sigma > 0.0) )
    throw_error(command, "No background (" + table.desc(&bgs) + ") or mask (" + table.desc(&mss) + ") images provided while"
                         " mask operation (" + table.desc(&st.edge) + " and/or " + table.desc(&st.sigma) + ") is requested.");

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
  st.flipUsed=table.count(&st.originF);
  st.origin1size = st.nofIn / (st.flipUsed ? 2 : 1) / st.origin2size ;

}


namespace blitz {

static inline float denan(float x){ return isnormal(x) ? x : 0.0 ;}
BZ_DECLARE_FUNCTION(denan);

static inline float invert(float x){ return x==0.0 ? 0.0 : 1.0/x ;}
BZ_DECLARE_FUNCTION(invert);

}

void SaveDenan(const ImagePath & filename, const Map & storage, bool saveint=false) {
  Map outm(storage.shape());
  outm=denan(storage);
  SaveImage(filename, outm, saveint);
}








class Stitcher {

  static const string modname;

  Map wght;
  Map swght;
  int minx;
  int miny;

public:

  PointF2D origin;
  int isz;
  Shape ish;
  Shape osh;

  Stitcher()
    : isz(0), minx(0), miny(0)
  {}

  void prepare(PointF2D _origin, Shape _ish, int _isz, const deque<Map> & msks) {

    origin = _origin;
    ish = _ish;
    isz = _isz;

    if ( isz == 0 )
      throw_error(modname, "Nothing to stitch.");
    if (msks.size() && msks.size() != 1 && msks.size() != isz)
      throw_error(modname, "Inconsistent sizes of mask and image arrays.");
    for (int mcur = 0 ; mcur < msks.size() ; mcur++ )
      if (msks[mcur].shape() != ish)
        throw_error(modname, "Non matching mask shapes in input.");

    int maxx(0), maxy(0);
    for (int acur = 0 ; acur < isz ; acur++ ) {
      const float
        orgx = acur*origin.x,
        orgy = acur*origin.y,
        tilx = orgx + ish(1)-1,
        tily = orgy + ish(0)-1;
      if (orgx < minx) minx = orgx;
      if (tilx > maxx) maxx = tilx;
      if (orgy < miny) miny = orgy;
      if (tily > maxy) maxy = tily;
    }

    wght.resize(ish);
    for (ArrIndex ycur = 0 ; ycur < ish(0) ; ycur++ )
      for (ArrIndex xcur = 0 ; xcur < ish(1) ; xcur++ )
        wght(ycur, xcur) = 1 + ( ish(0) - abs( 2*ycur - ish(0) + 1l ) )
                             * ( ish(1) - abs( 2*xcur - ish(1) + 1l ) );

    osh = Shape(maxy-miny+1, maxx-minx+1);
    swght.resize(osh);
    swght=0.0;
    for (int acur = 0 ; acur < isz ; acur++ ) {
      //const Map & curar = iarr[acur];
      const blitz::Range r0(acur*origin.y-miny, acur*origin.y-miny + ish(0)-1);
      const blitz::Range r1(acur*origin.x-minx, acur*origin.x-minx + ish(1)-1);
      if (msks.size()==1)
        swght(r0,r1) += wght * msks[0];
      else if (msks.size())
        swght(r0,r1) += wght * msks[acur];
      else
        swght(r0,r1) += wght;
    }
    swght = invert(swght);

  }

  void prepare(PointF2D _origin, Shape _ish, int _isz, const Map & msks) {
    deque<Map> msksdq(msks.size() ? 1 : 0, msks);
    prepare(_origin, _ish, _isz, msksdq);
  }

  void stitch(const deque<Map> & iarr, Map & oarr) const {

    if (!isz)
      throw_error(modname, "Was never prepared (zero input size).");

    if ( iarr.size() != isz || iarr[0].shape() != ish )
      throw_error(modname, "Non maching input data.");
    if (isz == 1) {
      if (!oarr.size())
        oarr.reference(iarr[0]);
      else if (!areSame(oarr, iarr[0])) {
        oarr.resize(ish);
        oarr = iarr[0];
      }
      return;
    }
    for (int acur = 0 ; acur < isz ; acur++ )
      if (iarr[acur].shape() != ish)
        throw_error(modname, "Non matching image shapes in input.");

    oarr.resize(osh);
    oarr=0.0;
    for (int acur = 0 ; acur < isz ; acur++ )
      oarr( blitz::Range(acur*origin.y-miny, acur*origin.y-miny + ish(0)-1)
          , blitz::Range(acur*origin.x-minx, acur*origin.x-minx + ish(1)-1) )
        += wght * iarr[acur];
    oarr *= swght;

  }

  Map resMask() const {
    Map ret(osh);
    ret = swght;
    invert(ret);
    return ret;
  }

};
const string Stitcher::modname = "stitcher";





class ProcProj {

  static const string modname;
  static cl_program oclProgram;
  deque<FlatFieldProc> ffprocs;
  StitchRules strl;

  Stitcher   ster1_R, ster2_R, sterF_R;
  Stitcher & ster1,   ster2,   sterF;
  Map mskF; // shared
  ImageProc iproc; // own
  Map stitched, final; // own
  deque<Map> allIn, o1Stitch, o2Stitch;

  bool doGapsFill;
  CLkernel gaussCL;
  CLmem iomCL;
  CLmem maskCL_R;
  CLmem & maskCL;

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
    const float step = 1.0 / (strl.edge +1);
    Map tmp(_gaps.shape());
    tmp = _gaps;

    for ( int stp = 1 ; stp <= strl.edge ; stp++ ) {
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


  void initCL() {

    if (!doGapsFill)
      return;

    static const string oclsrc = {
      #include "proj.cl.includeme"
    };
    oclProgram = initProgram(oclsrc, oclProgram, "Projection in OCL");
    if (!oclProgram)
      throw_error(modname, "Failed to compile CL program.");

    iomCL(clAllocArray<float>(mskF.size()));
    if (!maskCL())
      maskCL(blitz2cl(mskF, CL_MEM_READ_ONLY));
    gaussCL(oclProgram, "gauss");
    gaussCL.setArg(0, int(mskF.shape()(1)));
    gaussCL.setArg(1, int(mskF.shape()(0)));
    gaussCL.setArg(2, iomCL());
    gaussCL.setArg(3, maskCL());
    gaussCL.setArg(4, float(strl.sigma) );

  }


public:

  ProcProj( const StitchRules & _st, const deque<Map> & bgas, const deque<Map> & dfas
          , const deque<Map> & dgas, const deque<Map> & msas, const Path & saveMasks = Path())
    : strl(_st)
    , ster1(ster1_R)
    , ster2(ster2_R)
    , sterF(sterF_R)
    , iproc(strl.angle, strl.crp, strl.bnn, strl.ish)
    , allIn(strl.nofIn)
    , o1Stitch(strl.nofIn/strl.origin1size)
    , o2Stitch(strl.flipUsed ? 2 : 1)
    , doGapsFill(false)
    , maskCL_R(0)
    , maskCL(maskCL_R)
  {

    if ( ! area(strl.ish) )
      throw_error(modname, "Zerro area to process.");
    if ( ! strl.nofIn )
      throw_error(modname, "Zerro images for input.");

    #define chkAuxImgs(imas, lbl) \
      if (imas.size() && imas.size() != 1 && imas.size() != strl.nofIn) \
        throw_error(modname, "Number of " lbl " images is neither 0, 1 nor the number of inputs" \
                             " (" + toString(strl.nofIn) + ")."); \
      for (int curI = 0; curI < imas.size() ; curI++) \
        if ( imas.at(curI).size() && imas.at(curI).shape() != strl.ish ) \
          throw_error(modname, "Unexpected shape of " lbl " image.");

    chkAuxImgs(bgas, "background");
    chkAuxImgs(dfas, "darkfield");
    chkAuxImgs(dgas, "darkground");
    chkAuxImgs(msas, "mask");
    #undef chkAuxImgs

    const Map zmap;
    if (bgas.size() > 1 || dfas.size() > 1 || dgas.size() > 1 || msas.size() > 1) {
      for (int curI = 0; curI < strl.nofIn ; curI++) {
        const Map & bgpl = bgas.size() ?  bgas[ bgas.size() == 1 ? 0 : curI ] : zmap;
        const Map & dfpl = dfas.size() ?  dfas[ dfas.size() == 1 ? 0 : curI ] : zmap;
        const Map & dgpl = dgas.size() ?  dgas[ dgas.size() == 1 ? 0 : curI ] : zmap;
        const Map & mspl = msas.size() ?  msas[ msas.size() == 1 ? 0 : curI ] : zmap;
        ffprocs.emplace_back(bgpl, dfpl, dgpl, mspl);
      }
    } else {
      ffprocs.emplace_back( bgas.size() ? bgas[0] : zmap
                          , dfas.size() ? dfas[0] : zmap
                          , dgas.size() ? dgas[0] : zmap
                          , msas.size() ? msas[0] : zmap );
    }

    #define SaveMask(vol, suf) \
      if (saveMasks.length()) \
        SaveDenan(saveMasks.dtitle() + suf + ".tif", vol);

    const int mssz = msas.size();
    deque<Map> pmsas;
    for (int curI = 0; curI < msas.size() ; curI++) {
      if (msas[curI].shape() != strl.ish)
        throw_error(modname, "Unexpected mask shape.");
      Map msT;
      iproc.proc(msas[curI], msT);
      prepareMask(msT, true);
      pmsas.emplace_back(msT.shape());
      pmsas.back() = msT;
      SaveMask(msT, "_I" + (mssz>1 ? toString(curI) : string()) );
    }
    ster1.prepare(strl.origin1, iproc.osh(), strl.origin1size, pmsas);
    Map msks1;
    if (mssz) {
      msks1.reference(ster1.resMask());
      prepareMask(msks1, false);
      SaveMask(msks1, "_U");
    }

    ster2.prepare(strl.origin2, ster1.osh, strl.origin2size, msks1);
    Map msks2;
    if (mssz) {
      if (strl.origin2size>1) {
        msks2.reference(ster2.resMask());
        prepareMask(msks2, false);
        SaveMask(msks2, "_V");
      } else
        msks2.reference(msks1[0]);
    }

    if ( strl.flipUsed ) {
      deque<Map> msksF;
      if (mssz) {
        msksF.emplace_back(msks2);
        msksF.emplace_back(msks2.reverse(blitz::secondDim));
      }
      sterF.prepare(strl.originF, ster2.osh, 2, msks2);
      if (mssz) {
        mskF.reference(sterF.resMask());
        prepareMask(mskF, false);
        SaveMask(mskF, "_W");
      }
    } else if (mssz)
      mskF.reference(msks2[0]);

    #undef SaveMask

    doGapsFill = strl.sigma > 0.0  &&  any(mskF==0.0);
    initCL();

  }


  ProcProj(const ProcProj & other)
    : strl(other.strl)
    , ster1(other.ster1)
    , ster2(other.ster2)
    , sterF(other.sterF)
    , iproc(other.iproc)
    , ffprocs(other.ffprocs)
    , allIn(other.allIn.size())
    , o1Stitch(other.o1Stitch.size())
    , o2Stitch(other.o2Stitch.size())
    , mskF(other.mskF)
    , doGapsFill(other.doGapsFill)
    , maskCL(other.maskCL)
  {
    if (doGapsFill)
      initCL();
  }



  void sub_proc(  const Stitcher & ster, const deque<Map> & hiar, deque<Map> & oar
                , const ImagePath & interim_name, const string & suffix) {

    if (!ster.isz)
      throw_error(modname, "Bad stitcher.");
    if ( ster.isz == 1 ) {
      for(int curM = 0 ; curM < oar.size() ; curM++)
        oar[curM].reference(hiar[curM]);
      return;
    }

    const int nofin = hiar.size();
    for ( int inidx=0 ; inidx<nofin ; inidx += ster.isz ) {
      int cidx=inidx/ster.isz;
      deque<Map> supplyIm( hiar.begin() + inidx, hiar.begin() + inidx + ster.isz) ;
      ster.stitch(supplyIm, oar[cidx]);
      if ( ! interim_name.empty() ) {
        Map cres;
        if ( ster.origin.x * ster.origin.y == 0.0 ) {
          cres.reference(oar[cidx]);
        } else if ( abs(ster.origin.x) < abs(ster.origin.y)  ) {
          int crppx = abs(ster.origin.x * (supplyIm.size()-1));
          crop(oar[cidx], cres, Crop(0, crppx, 0, crppx));
        } else {
          int crppx = abs(strl.origin1.y * (supplyIm.size()-1));
          crop(oar[cidx], cres, Crop(crppx, 0, crppx, 0));
        }
        string svName = interim_name.dtitle() + suffix;
        if (ster.isz<nofin) // more than one result
          svName += toString(mask2format("@", oar.size()),cidx);
        SaveDenan(svName + ".tif", cres);
      }
    }
    return;

  }


  bool process(deque<Map> & allInR, deque<Map> & res, const ImagePath & interim_name = ImagePath()) {

    if (allInR.size() != strl.nofIn)
      return false;

    // prepare input images
    int curF=0, cur2=0, cur1=0;
    for ( int curproj = 0 ; curproj < strl.nofIn ; curproj++) {
      if (ffprocs.size() == 1)
        ffprocs[0].process(allInR[curproj]);
      else if (ffprocs.size() == strl.nofIn)
        ffprocs[curproj].process(allInR[curproj]);
      iproc.proc(allInR[curproj], allIn[curproj]);
      if ( ! interim_name.empty() ) {
        const string sfI = strl.nofIn > 1 ? toString(mask2format("@", strl.nofIn), curproj) : "";
        const string sfF = strl.flipUsed ? (curF ? "_F" : "_D") : "";
        const string sf2 = strl.origin2size > 1 ? toString(mask2format(".@", strl.origin2size), cur2) : "";
        const string sf1 = strl.origin1size > 1 ? toString(mask2format(".@", strl.origin1size), cur1) : "";
        const string svName = interim_name.dtitle() + "_I" + sfI + sfF + sf2 + sf1 + string(".tif");
        SaveDenan(svName, allIn[curproj]);
        cur1++;
        if (cur1==strl.origin1size) {
          cur1=0;
          cur2++;
          if (cur2==strl.origin2size) {
            cur2=0;
            curF++;
          }
        }
      }
    }

    // first stitch
    sub_proc(ster1, allIn   , o1Stitch, interim_name, "_U");
    // second stitch
    sub_proc(ster2, o1Stitch, o2Stitch, interim_name, "_V");
    // flip stitch
    if ( strl.flipUsed ) {
      o2Stitch[1].reverseSelf(blitz::secondDim);
      sterF.stitch(o2Stitch, stitched);
      if ( ! interim_name.empty() )  {
        SaveDenan(interim_name.dtitle() + "_WD.tif" , o2Stitch[0]);
        SaveDenan(interim_name.dtitle() + "_WF.tif" , o2Stitch[1]);
        SaveDenan(interim_name.dtitle() + "_WW.tif" , stitched);
      }
    } else {
      stitched.reference(o2Stitch[0]);
    }

    // closing gaps left after superimposition
    if (doGapsFill) {
      if (stitched.shape() != mskF.shape()) // should never happen
        throw_bug(modname);
      blitz2cl(stitched, iomCL());
      gaussCL.exec(stitched.shape());
      cl2blitz(iomCL(), stitched);
    }

    // final crop
    if (strl.fcrp) {
      crop(stitched, final, strl.fcrp);
      if ( ! interim_name.empty() )
        SaveDenan( interim_name.dtitle() + "_Y.tif", stitched );
    } else {
      final.reference(stitched);
    }

    // splits
    if ( strl.splits.empty() ) {
      res.resize(1);
      res[0].resize(final.shape());
      res[0]=final;
    } else {
      const string svformat = mask2format("_Z@", strl.splits.size() );
      int fLine=0, lLine=0;
      const int vsplit = strl.splits.at(0) ? 0 : 1;
      const int mLine = final.shape()(vsplit);
      for (int curS = vsplit ;  curS<=strl.splits.size()  ;  curS++) {
        int curI = curS - vsplit;
        lLine = ( curS == strl.splits.size()  ||  mLine < strl.splits.at(curS) )
          ?  mLine :  strl.splits.at(curS) ;
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
cl_program ProcProj::oclProgram = 0;




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
      allInMaps.emplace(me, deque<Map>(allInRd.size()));
      results.emplace(me,deque<Map>());
    }
    ProcProj & myProc = procs.at(me);
    deque<Map> & myAllIn = allInMaps.at(me);
    deque<Map> & myRes = results.at(me);
    unlock();

    try {
      for (ArrIndex curI = 0  ;  curI<allInRd.size()  ;  curI++ )
        allInRd[curI].read(projes[idx], myAllIn[curI]);
      myProc.process(myAllIn, myRes);
      for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ )
        allOutSv[curO].save(projes[idx], myRes[curO]);
    } catch (...) {
      warn("form projection", toString("Failed on index %i, slice %i.", idx, projes[idx]));
    }

    bar.update();
    return true;

  }

public:

  ProjInThread(deque<ReadVolumeBySlice> & _allInRd, deque<SaveVolumeBySlice> & _outSave
              , const ProcProj & _proc, const vector<int> & _projes, bool verbose=false)
    : InThread(verbose, "processing projections", _projes.size())
    , proc(_proc)
    , projes(_projes)
    , allInRd(_allInRd)
    , allOutSv(_outSave)
  {}


};




/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;
  const Shape ish(ImageSizes(args.images.at(0).at(0).repr()));
  StitchRules st = args.st;
  st.ish = ish;

  // Read flat-fielding images
  #define rdAux(pfx) \
  deque<Map> pfx##as(args. pfx##s.size()); \
  for ( int curf = 0 ; curf < args. pfx##s.size() ; curf++) \
    ReadImage(args. pfx##s[curf].repr(), pfx##as[curf], ish);

  rdAux(bg);
  rdAux(df);
  rdAux(dg);
  rdAux(ms);
  #undef rdAux

  // Construct masks from bgs and df/gs if not provided but required by other parameters.
  if ( ! msas.size()  &&  (args.st.edge || args.st.sigma > 0.0) ) {
    deque<Map> & bgdf = dgas.size() ? dgas : dfas;
    const int mssz = max(bgas.size(), bgdf.size());
    msas.resize(mssz);
    for (int curms=0 ; curms<mssz ; curms++) {
      Map & mscur = msas[curms];
      mscur.resize(ish);
      Map bgcur = bgas[bgas.size() > curms ? curms : 0];
      mscur = bgcur;
      if (bgdf.size())
        mscur -= bgdf[bgdf.size() > curms ? curms : 0];
      for (ArrIndex ycur = 0 ; ycur < ish(0) ; ycur++ ) {
        for (ArrIndex xcur = 0 ; xcur < ish(1) ; xcur++ ) {
          float val = mscur(ycur,xcur);
          mscur(ycur,xcur) = ( val <= 0.0 || ! fisok(val) ) ? 0 : 1.0;
        }
      }
    }
  }

  // Prepare read factories
  const int nofIn = args.images.size();
  deque<ReadVolumeBySlice> allInRd(nofIn);
  for ( int curI = 0 ; curI < nofIn ; curI++) {
    allInRd.at(curI).add(args.images.at(curI));
    uint cSls = allInRd.at(curI).slices();
    if (!cSls)
      exit_on_error(args.command, "No images in input "+ toString(curI) +".");
    if (curI && allInRd.at(0).slices() != cSls)
      exit_on_error(args.command, "Not matching slices in input "+ toString(curI) +".");
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
  //Map zmap(ish);
  //zmap=0.0;
  deque<Map> allOut, allIn;
  for ( ArrIndex curI = 0 ; curI < nofIn ; curI++) {
    allIn.emplace_back(ish);
    if (args.testMe >= 0)
      allInRd[curI].read(args.testMe, allIn[curI]);
    else if (nofOuts == 1)
      allInRd[curI].read(projes[0], allIn[curI]);
    else
      allIn.back()=0.0;
  }
  string testFormat;
  if (args.testMe >= 0) {
    testFormat = args.out_name.dtitle();
    if (testFormat.find('@') != string::npos)
      testFormat = toString(mask2format(testFormat, nofProj), args.testMe);
    testFormat += "%s";
  }
  ProcProj canonPP(st, bgas, dfas, dgas, msas, toString(testFormat, "_mask.tif"));
  canonPP.process(allIn, allOut, toString(testFormat, ".tif"));

  // Prepare saving factories
  const size_t nofSplts = allOut.size();
  const string spformat = mask2format("_split@", nofSplts);
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
    for (int curSplt = 0 ; curSplt < nofSplts ; curSplt++)
      allOutSv[curSplt].save(args.testMe, allOut[curSplt]);
  else if ( nofOuts == 1 )
    for (int curSplt = 0 ; curSplt < nofSplts ; curSplt++)
      allOutSv[curSplt].save(projes[0], allOut[curSplt]);
  else // finally process
    ProjInThread(allInRd, allOutSv, canonPP, projes, args.beverbose).execute();

  exit(0);

}

