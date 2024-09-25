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

#include <unistd.h>
#include <poptmx.h>
#include "../common/external.world.h"
#include "../common/parallel.world.h"
#include "../common/magic_enum.hpp"

#ifdef OPENCV_FOUND
#include <opencv2/photo.hpp> // for inpainting
#endif // OPENCV_FOUND


using namespace std;



enum Inpaint {
  ZEROES=0,
  #ifdef OPENCV_FOUND
  NS, // Navier-Stokes
  AT, // Alexandru Telea
  #endif // OPENCV_FOUND
  AM  // antonmx
};





struct StitchRules {

  uint nofIn;
  Crop<2> crp;                  ///< Crop input projection image
  Crop<2> fcrp;                  ///< Crop final projection image
  Binn<2> bnn;                  ///< binning factor
  float angle;                ///< Rotation angle.
  PointF<2> origin1;            ///< Origin of the next image in the first stitch
  PointF<2> origin2;            ///< Origin of the next image in the second stitch
  uint origin1size;
  uint origin2size;           ///< Nof images in the second stitch - needed only if it is requested (origin2)
  PointF<2> originF;            ///< Origin of the flipped portion
  bool flip;               ///< indicates if originF was given in options.
  std::deque<uint> splits;          ///< Split pooints to separate samples.
  uint edge;               ///< blur of mask and image edges.
  Inpaint inpaint = Inpaint::ZEROES;

  StitchRules()
  : nofIn(0)
  , crp()
  , angle(0)
  , origin1size(1)
  , origin2size(1)
  , flip(false)
  , edge(0)
  {}

  void slot(int cur, int* cur1, int* cur2, int* curF) const;

};


void StitchRules::slot(int cur, int* cur1, int* cur2, int* curF) const {
  if (cur < 0 || cur >= nofIn)
    throw_error("stitch slot", "Current index " +toString(cur)+ " is beyond the range "
                               "[0.." +toString(nofIn-1)+ "].");
  int _curF = flip && cur >= nofIn/2;
  cur -= _curF * nofIn/2;
  if (curF) *curF = _curF;
  if (cur2) *cur2 = cur / origin1size;
  if (cur1) *cur1 = cur % origin1size;
}



/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  deque< deque<ImagePath> > images;        ///< images to combine
  deque<ImagePath> bgs;        ///< Array of the background images.
  deque<ImagePath> dfs;        ///< Array of the dark field images.
  deque<ImagePath> dgs;        ///< Array of the dark field images for backgrounds.
  deque<ImagePath> mss;        ///< Mask Array.
  uint denoiseRad;
  float denoiseThr;
  ImagePath out_name;              ///< Name of the output image.
  string out_range;
  StitchRules st;
  int zbinn;
  int testMe;          ///< Prefix to save interim results
  bool doTest;
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
  , denoiseRad(0)
  , denoiseThr(0.0)
  , zbinn(1)
  , testMe(-1)
  , doTest(false)
  , beverbose(false)
{

  deque<ImagePath> iimages;
  const string allInpaints = [](){
    string toRet;
    for( auto str : magic_enum::enum_names<Inpaint>() )
      toRet += string(str) + ", ";
    if (toRet.size())
      toRet.erase(toRet.size()-2);
    return toRet;
  }();
  string inpaint_str;

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &iimages, "images", "Input 2D or 3D images.",
         "All images must be of the same rank (2D or 3D). HDF5 format:\n"
         "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc)

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &out_name, 'o', "output", "Output image.", "", out_name)
    .add(poptmx::OPTION, &out_range, 'O', "select", "Slices to process. All if not used.",
         SliceOptionDesc + ". Only makes sense in multiple projections.", "all")
    .add(poptmx::OPTION, &st.crp, 'c', "crop", "Crop input images.", CropOptionDesc)
    .add(poptmx::OPTION, &st.fcrp, 'C', "crop-final", "Crop final image.", CropOptionDesc)
    .add(poptmx::OPTION, &st.bnn, 'b', "binn", "Binn factor(s).", BinnOptionDesc)
    .add(poptmx::OPTION, &zbinn, 'z', "zinn", "Binning over multiple input prrojections.",
         "If zero binn binns all outputs. Ignored for tests.")
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
    .add(poptmx::OPTION, &denoiseRad, 'n', "noise-rad", "Radius for noise reducing algorithm.",
         "Radius of the local region used to calculate average to replace the pixel value." )
    .add(poptmx::OPTION, &denoiseThr, 'N', "noise-thr", "Noise threshold.",
         "If positive, then gives maximum absolute deviation from the average;"
         " if negative, then negated value is maximum relative deviation.")
    .add(poptmx::OPTION, &inpaint_str, 'I', "fill", "Type of gap fill algorithm.",
         "If mask is set and after all stitching some gaps are left, they are filled using one of the following methods: "
         + allInpaints + ".", string(magic_enum::enum_name<Inpaint>(st.inpaint)) )
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
      if (inputline.size() && inputline.at(0) != '#' ) {
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

  auto chkNofIns = [&](const deque<ImagePath> & ims, const string & lbl) {
    if (ims.size() && ims.size() != 1 && ims.size() != st.nofIn)
      exit_on_error(command, "Number of " +lbl+ " images (" + toString(ims.size()) +  ")"
                             " given by " + table.desc(&ims) + " option is neither 0, 1,"
                             " nor the number of inputs (" + toString(st.nofIn) +  ").");
  };
  chkNofIns(bgs, "background");
  chkNofIns(dfs, "darkfield");
  chkNofIns(dgs, "darkground");
  chkNofIns(mss, "mask");

  if (dgs.size() && ! bgs.size())
    exit_on_error(command, "No background images (" + table.desc(&bgs) + ") for provided darkgrounds (" + table.desc(&dgs) + ").");
  if ( ! mss.size()  &&  ! bgs.size()  &&  st.edge )
    throw_error(command, "No background (" + table.desc(&bgs) + ") or mask (" + table.desc(&mss) + ") images provided while"
                         " mask operation (" + table.desc(&st.edge) + ") is requested.");

  if ( ! table.count(&out_name) )
    exit_on_error(command, "No output name provided. Use option " + table.desc(&out_name) + ".");
  doTest = table.count(&testMe);

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
  if ( table.count(&st.origin2size) ) {
    if ( ! table.count(&st.origin2) && ! table.count(&testMe) )
      exit_on_error(command, string () +
        "Options " + table.desc(&st.origin2size) + " must be used with either "
        + table.desc(&st.origin2) + " or " + table.desc(&testMe) + " options.");
    if ( st.origin2size  < 2 )
      exit_on_error(command, string () +
        "Requested second stitch size (" + toString(st.origin2size) + ") is less than 2.");
    if ( tiledImages % st.origin2size )
      exit_on_error(command, string () +
        "Total number of tiled images (" + toString(tiledImages) + ")"
        " is not a multiple of the second stitch size (" + toString(st.origin2size) + ")"
        " given by " + table.desc(&st.origin2size) + " option.");
  }

  if ( table.count(&denoiseThr) && ! table.count(&denoiseRad) )
    warn(command, "Setting noise threshold via " + table.desc(&denoiseThr) + " option "
         " without noise rad (option " + table.desc(&denoiseRad) + ") makes no sense.");

  sort(st.splits.begin(), st.splits.end());
  unique(st.splits.begin(), st.splits.end());
  if ( st.splits.size() == 1 && st.splits.at(0) == 0 )
    exit_on_error(command, "The list of splits contains only 0 (marking vertical splits).");

  st.angle *= M_PI/180;
  st.flip=table.count(&st.originF);
  st.origin1size = st.nofIn / st.origin2size / (st.flip ? 2 : 1) ;
  if (st.origin1size<1)
    exit_on_error(command, string () +
      "Number of input images (" + toString(st.nofIn) + ") is not sufficient for given "
      + table.desc(&st.origin2size) + "(" + toString(st.origin2size) + ")" +
      (st.flip ? " and/or " + table.desc(&st.flip) : "") + "." );

  if ( table.count(&inpaint_str) ) {
    auto inp = magic_enum::enum_cast<Inpaint>(inpaint_str, magic_enum::case_insensitive);
    if ( ! inp.has_value() )
      throw_error(command,   "Unknown string \""+inpaint_str+"\" for option " + table.desc(&inpaint_str)
                           + ". Possible values are: " + allInpaints + ".");
    st.inpaint = inp.value();
  }

}




namespace blitz {

static inline float denan(float x){ return isnormal(x) ? x : 0.0 ;}
BZ_DECLARE_FUNCTION(denan);

static inline float invert(float x){ return x==0.0 ? 0.0 : 1.0/x ;}
BZ_DECLARE_FUNCTION(invert);

}



void SaveDenan(const ImagePath & filename, const Map & storage) {
  Map outm(storage.shape());
  outm=denan(storage);
  SaveImage(filename, outm);
}


Map & prepareMask(Map & mask, bool bepicky, uint edge=0) {

  if (!mask.size())
    return mask;
  const float mm = min(mask);
  const float MM = max(mask);
  if (mm==MM) {
    if (MM!=0.0 && MM!=1.0)
      mask = 1.0;
    return mask;
  }
  if (mm != 0.0 || MM != 1.0)
    mask = (mask-mm)/(MM-mm);

  for (ssize_t i = 0 ; i<mask.shape()(0) ; i++)
    for (ssize_t j = 0 ; j<mask.shape()(1) ; j++)
      if (bepicky && mask(i,j)<1.0 )
        mask(i,j)=0.0 ;
      else if (! bepicky && mask(i,j)>0.0 )
        mask(i,j)=1.0 ;
  if (!edge)
    return mask;

  const Shape<2> ish = mask.shape();
  const float step = 1.0 / (edge +1);
  Map tmp(mask.shape());
  tmp = mask;
  for ( int stp = 1 ; stp <= edge ; stp++ ) {
    const float fill = step*stp;
    //const float usq = stp * step - 1 ;
    //const float fill = sqrt(1.0 - usq*usq);

    for (ssize_t i = 0 ; i<ish(0) ; i++)
      for (ssize_t j = 0 ; j<ish(1) ; j++)

        if ( mask(i,j) != 1.0 )
          for (ssize_t ii = i-1 ; ii <= i+1 ; ii++)
            for (ssize_t jj = j-1 ; jj <= j+1 ; jj++)

              if ( ii >= 0 && ii < ish(0) && jj >= 0 && jj < ish(1)
                   && mask(ii,jj) == 1.0 )
                tmp(ii,jj) = fill;

    mask = tmp;
  }
  return mask;
}




class ProcProj {

  static const std::string modname;

  // shared, all const except in the constructor.
  const StitchRules & strl;
  const Shape<2> ish; // input image
  const Shape<2> psh; // processed image
  const Shape<2> ssh; // stitched image
  const std::vector<Shape<2>> oshs; // split images
  std::deque<Map> wghts;
  Map swght;
  std::deque<PointF<2>> origins;
  Map mskF;
  #ifdef OPENCV_FOUND
  blitz::Array<uchar,2> mskI; // for OpenCV::inpaint
  cv::Mat cv_filled;
  #endif // OPENCV_FOUND
  Inpaint inpaint=ZEROES;
  CLmem maskCL;

  // own
  std::deque<FlatFieldProc> ffprocs;
  ImageProc iproc;
  std::deque<Map> allIn;
  Map stitched, final;
  std::deque<Map> res; // will all reference final
  CLprogram oclProgram;
  CLkernel gaussCL;
  CLmem iomCL;

  void stitchSome( const ImagePath & interim_name = ImagePath()
                 , const std::vector<bool> & addMe = std::vector<bool>());
  void initCL();

public:

  ProcProj( const StitchRules & st, const Shape<2> & ish
          , const std::deque<Map> & bgas, const std::deque<Map> & dfas
          , const std::deque<Map> & dgas, const std::deque<Map> & msas
          , const Path & saveMasks = Path());

  ProcProj(const ProcProj & other);

  std::deque<Map> & process( std::deque<Map> & allInR, const ImagePath & interim_name = ImagePath());

  const std::vector<Shape<2>> & outputShapes() const {return oshs;}
  static std::vector<Shape<2>> outputShapes(const StitchRules & st, const Shape<2> & ish);

};



const string ProcProj::modname="ProcProj";


void ProcProj::initCL() {

  if (inpaint != Inpaint::AM)
    return;

  static const string oclsrc = {
    #include "projection.cl.includeme"
  };
  oclProgram(oclsrc);

  iomCL(clAllocArray<float>(mskF.size()));
  if (!maskCL())
    maskCL(blitz2cl<cl_float>(mskF, CL_MEM_READ_ONLY));
  gaussCL(oclProgram, "gauss");
  gaussCL.setArg(0, int(mskF.shape()(1)));
  gaussCL.setArg(1, int(mskF.shape()(0)));
  gaussCL.setArg(2, iomCL());
  gaussCL.setArg(3, maskCL());

}


ProcProj::ProcProj( const StitchRules & st, const Shape<2> & ish
                  , const deque<Map> & bgas, const deque<Map> & dfas
                  , const deque<Map> & dgas, const deque<Map> & msas
                  , const Path & saveMasks)
  : strl(st)
  , ish(ish)
  , psh(MapProc::shape(strl.angle, strl.crp, strl.bnn, ish))
  , ssh( psh(0) + strl.flip*abs(strl.originF(0))
                + (strl.origin1size-1)*abs(strl.origin1(0))
                + (strl.origin2size-1)*abs(strl.origin2(0))
       , psh(1) + strl.flip*abs(strl.originF(1))
                + (strl.origin1size-1)*abs(strl.origin1(1))
                + (strl.origin2size-1)*abs(strl.origin2(1)) )
  , oshs(outputShapes(strl,ish))
  , iproc(strl.angle, strl.crp, strl.bnn, ish)
  , allIn(strl.nofIn)
  , res(oshs.size())
{

  if ( ! size(ish) )
    throw_error(modname, "Zerro area to process.");
  if ( ! strl.nofIn )
    throw_error(modname, "Zerro images for input.");


  auto chkAuxImgs = [&](const deque<Map> & imas, const string & lbl) {
    if (imas.size() && imas.size() != 1 && imas.size() != strl.nofIn)
      throw_error(modname, "Number of " +lbl+ " images is neither 0, 1 nor the number of inputs"
                           " (" + toString(strl.nofIn) + ").");
    for (int curI = 0; curI < imas.size() ; curI++)
      if ( imas.at(curI).size() && imas.at(curI).shape() != ish )
        throw_error(modname, "Unexpected shape of " +lbl+ " image.");
  };
  chkAuxImgs(bgas, "background");
  chkAuxImgs(dfas, "darkfield");
  chkAuxImgs(dgas, "darkground");
  chkAuxImgs(msas, "mask");

  if (bgas.size() > 1 || dfas.size() > 1 || dgas.size() > 1 || msas.size() > 1) {
    for (int curI = 0; curI < strl.nofIn ; curI++) {
      const Map & bgpl = bgas.size() ?  bgas[ bgas.size() == 1 ? 0 : curI ] : defaultMap;
      const Map & dfpl = dfas.size() ?  dfas[ dfas.size() == 1 ? 0 : curI ] : defaultMap;
      const Map & dgpl = dgas.size() ?  dgas[ dgas.size() == 1 ? 0 : curI ] : defaultMap;
      const Map & mspl = msas.size() ?  msas[ msas.size() == 1 ? 0 : curI ] : defaultMap;
      ffprocs.emplace_back(bgpl, dfpl, dgpl, mspl);
    }
  } else {
    ffprocs.emplace_back( bgas.size() ? bgas[0] : defaultMap
                        , dfas.size() ? dfas[0] : defaultMap
                        , dgas.size() ? dgas[0] : defaultMap
                        , msas.size() ? msas[0] : defaultMap );
  }

  // prepare processed masks
  wghts.resize(msas.size(), psh);
  Map zmask(ish), zpmask(psh);
  for (int curI = 0; curI < msas.size() ; curI++) {
    Map & wghtI = wghts[curI];
    wghtI.resize(iproc.shape());
    wghtI = iproc.apply(msas[curI]);
    zmask = msas[curI];
    prepareMask(zmask, false);
    zpmask = iproc.apply(zmask);
    prepareMask(zpmask, true, strl.edge);
    wghtI *= zpmask;
    if (saveMasks.length())
      SaveDenan(saveMasks.dtitle() + "_I" +
                (msas.size()>1 ? toString(curI) : string()) + ".tif", wghtI);
  }

  if (strl.nofIn == 1) { // no stitching
    if (msas.size())
      mskF.reference(wghts[0]);
  } else {

    // prepare origins for each input image
    int maxx(0), maxy(0), minx(0), miny(0);
    for (int curI = 0; curI < strl.nofIn ; curI++) {
      int cur1, cur2, curF;
      strl.slot(curI, &cur1, &cur2, &curF);
      PointF<2> curP( strl.origin1(0) * cur1 + strl.origin2(0) * cur2
                    , strl.origin1(1) * cur1 + strl.origin2(1) * cur2);
      origins.push_back(curP);
      const float
          orgx = curP(1),
          orgy = curP(0),
          tilx = orgx + psh(1)-1,
          tily = orgy + psh(0)-1;
      if (orgx < minx) minx = orgx;
      if (tilx > maxx) maxx = tilx;
      if (orgy < miny) miny = orgy;
      if (tily > maxy) maxy = tily;
    }
    if (strl.flip) {
      const int shiftD = - minx;
      const int shiftF = maxx+1;
      maxx=0; maxy=0; minx=0; miny=0;
      origins.clear();
      for (int curI = 0; curI < strl.nofIn ; curI++) {
        int cur1, cur2, curF;
        strl.slot(curI, &cur1, &cur2, &curF);
        PointF<2> curP( strl.origin1(0) * cur1 + strl.origin2(0) * cur2
                      , strl.origin1(1) * cur1 + strl.origin2(1) * cur2);
        if (curF) {
          curP(0) += strl.originF(0);
          curP(1) = -curP(1)-psh(1) + shiftF + strl.originF(1);
        } else {
          curP(1) = curP(1) + shiftD;
        }
        origins.push_back(curP);
        const float
            orgx = curP(1),
            orgy = curP(0),
            tilx = orgx + psh(1)-1,
            tily = orgy + psh(0)-1;
        if (orgx < minx) minx = orgx;
        if (tilx > maxx) maxx = tilx;
        if (orgy < miny) miny = orgy;
        if (tily > maxy) maxy = tily;
      }
    }
    for (int curI = 0; curI < strl.nofIn ; curI++)
      origins[curI] -= PointF<2>(miny, minx);
    if ( ssh != Shape<2>(maxy-miny+1, maxx-minx+1) )
      throw_bug(modname + " Mistake in calculating stitched shape: "
                +toString(ssh)+" != "+toString(Shape<2>(maxy-miny+1, maxx-minx+1))+".");

    // prepare weights image
    Map wght(psh);
    for (ssize_t ycur = 0 ; ycur < psh(0) ; ycur++ )
      for (ssize_t xcur = 0 ; xcur < psh(1) ; xcur++ )
        wght(ycur, xcur) =   ( psh(0) - abs( 2*ycur - psh(0) + 1l ) )
                           * ( psh(1) - abs( 2*xcur - psh(1) + 1l ) );

    // prepare sum of weights and of masks images
    swght.resize(ssh);
    swght=0.0;
    if (msas.size()) {
      mskF.resize(ssh);
      mskF=0.0;
    }
    for (int acur = 0 ; acur < strl.nofIn ; acur++ ) {
      const blitz::Range r0(origins[acur](0), origins[acur](0) + psh(0)-1)
                       , r1(origins[acur](1), origins[acur](1) + psh(1)-1);
      if (msas.size()) {
        Map wghtI(wghts[ wghts.size()==1 ? 0 : acur ]);
        Map wghtIF( ! strl.flip || acur < strl.nofIn/2
                    ?  wghtI :  wghtI.reverse(blitz::secondDim) );
        mskF(r0, r1) += wghtIF;
        swght(r0, r1) += wghtIF * wght;
      } else
        swght(r0, r1) += wght;
    }
    swght = invert(swght);

    // prepare weights masks
    if (!msas.size())
      wghts.emplace_back(wght);
    else {
      for (int curI = 0; curI < wghts.size() ; curI++)
        wghts[curI] *= wght;
      if (saveMasks.length())
        SaveDenan(saveMasks.dtitle() + "_X.tif", mskF);
    }

  }

  // save final mask
  if (msas.size()) {
    prepareMask(mskF, false);
    if (saveMasks.length()) {
      Map mskSV(strl.fcrp.apply(mskF));
      if (strl.splits.empty())
        SaveDenan(saveMasks.dtitle() + ".tif", mskSV);
      else {
        if (strl.fcrp)
          SaveDenan(saveMasks.dtitle() + "_Y.tif", mskSV);
        const string svformat = mask2format("_Z@", strl.splits.size() );
        const int vsplit = strl.splits.at(0) ? 0 : 1;
        int fLine=0;
        for (int curS=0 ; curS < oshs.size() ; curS++) {
          const int lLine = fLine + oshs[curS](vsplit);
          const blitz::Range srang(fLine, lLine-1);
          fLine = lLine;
          Map mskSVs( vsplit ? mskSV(all, srang) : mskSV(srang, all) );
          SaveDenan( saveMasks.dtitle() + toString(svformat, curS) + ".tif", mskSVs);
        }
      }
    }
  }

  inpaint = any(mskF==0.0) ? strl.inpaint : ZEROES ;
  if (inpaint == Inpaint::AM) {
    initCL();
  }
  #ifdef OPENCV_FOUND
  else if (inpaint == Inpaint::NS || inpaint == Inpaint::AT) {
    mskI.resize(ssh);
    mskI = where(mskF > 0, 0, 255);
  }
  #endif // OPENCV_FOUND

}




ProcProj::ProcProj(const ProcProj & other)
  : strl(other.strl)
  , psh(other.psh)
  , ssh(other.ssh)
  , oshs(other.oshs)
  , wghts(other.wghts)
  , swght(other.swght)
  , origins(other.origins)
  , mskF(other.mskF)
  , maskCL(other.maskCL)
  , ffprocs(other.ffprocs)
  , iproc(other.iproc)
  , allIn(other.allIn.size())
  , res(other.oshs.size())
  , inpaint(other.inpaint)
  #ifdef OPENCV_FOUND
  , mskI(other.mskI)
  #endif // OPENCV_FOUND
{
  if ( other.inpaint == Inpaint::AM )
    initCL();
}


vector<Shape<2>> ProcProj::outputShapes(const StitchRules & st, const Shape<2> & ish) {
  Shape<2> psh(MapProc::shape(st.angle, st.crp, st.bnn, ish));
  Shape<2> ssh( psh(0) + st.flip*abs(st.originF(0))
                       + (st.origin1size-1)*abs(st.origin1(0))
                       + (st.origin2size-1)*abs(st.origin2(0))
              , psh(1) + st.flip*abs(st.originF(1))
                       + (st.origin1size-1)*abs(st.origin1(1))
                       + (st.origin2size-1)*abs(st.origin2(1)) );
  vector<Shape<2>> oshs;
  const Shape<2> cssh = st.fcrp.shape(ssh);
  if ( st.splits.empty() )
    oshs.push_back(cssh);
  else {
    int fLine=0, lLine=0;
    const int vsplit = st.splits.at(0) ? 0 : 1;
    const int mLine = cssh(vsplit);
    for (int curS = vsplit ;  curS<=st.splits.size()  ;  curS++) {
      lLine = ( curS == st.splits.size()  ||  mLine < st.splits.at(curS) )
          ?  mLine :  st.splits.at(curS) ;
      int sz = lLine-fLine;
      if ( sz > 0 )
        oshs.push_back(vsplit ? Shape<2>(cssh(0),sz) : Shape<2>(sz, cssh(1)));
      fLine=lLine;
    }
  }
  return oshs;
}


void ProcProj::stitchSome(const ImagePath & interim_name, const std::vector<bool> & addMe) {
  if ( addMe.size() && addMe.size() != strl.nofIn )
    throw_error(modname, "Unexpected input array size " +toString(addMe.size())+
                         " for " +toString(strl.nofIn)+ " frames stitch.");
  if ( addMe.size() && std::all_of(addMe.begin(), addMe.end(), [](bool a){return !a;}) ) {
    warn(modname, "Stitching no images.");
    stitched=0.0;
    return;
  }
  if (strl.nofIn == 1) {
    warn(modname, "Single input: nothing to stitch.");
    return;
  }

  blitz::Range yRange(all);
  blitz::Range xRange(all);
  if ( ! addMe.size() || std::all_of(addMe.begin(), addMe.end(), [](bool a){return a;}) ) {
    stitched=0.0;
    for (int acur = 0 ; acur < strl.nofIn ; acur++ ) {
      Map incur;
      if (interim_name.empty())
        incur.reference(allIn[acur]);
      else {
        incur.resize(psh);
        incur=allIn[acur];
      }
      incur *= wghts[wghts.size()==1 ? 0 : acur];
      stitched( blitz::Range(origins[acur](0), origins[acur](0) + psh(0)-1)
              , blitz::Range(origins[acur](1), origins[acur](1) + psh(1)-1))
        +=  ! strl.flip || acur < strl.nofIn/2  // don't flipMe
            ?   incur  :  incur.reverse(blitz::secondDim);
    }
    stitched *= swght;
  } else {
    xRange.setRange(ssh(1),0);
    yRange.setRange(ssh(0),0);
    Map sswght(ssh);
    sswght=0;
    stitched=0.0;
    for (int acur = 0 ; acur < strl.nofIn ; acur++ ) {
      const blitz::Range r0(origins[acur](0), origins[acur](0) + psh(0)-1);
      const blitz::Range r1(origins[acur](1), origins[acur](1) + psh(1)-1);
      if (addMe[acur]) {
        const bool flipMe = strl.flip && acur >= strl.nofIn/2;
        Map wght(wghts[wghts.size()==1 ? 0 : acur]);
        Map incur(allIn[acur] * wght);
        stitched(r0, r1) += !flipMe ? incur : incur.reverse(blitz::secondDim);
        sswght  (r0, r1) += !flipMe ? wght  : wght .reverse(blitz::secondDim);
        xRange.setRange(min(xRange.first(), r1.first()), max(xRange.last(), r1.last()));
        yRange.setRange(min(yRange.first(), r0.first()), max(yRange.last(), r0.last()));
      }
    }
    sswght(yRange, xRange) = invert(sswght(yRange, xRange));
    stitched(yRange, xRange) *= sswght(yRange, xRange);
  }

  if ( ! interim_name.empty() )
    SaveDenan(interim_name, stitched(yRange, xRange));

}



std::deque<Map> & ProcProj::process(deque<Map> & allInR, const ImagePath & interim_name) {

  if (allInR.size() != strl.nofIn)
    return res;

  // prepare input images
  for ( int curproj = 0 ; curproj < strl.nofIn ; curproj++) {
    if (ffprocs.size() == 1)
      ffprocs[0].process(allInR[curproj]);
    else if (ffprocs.size() == strl.nofIn)
      ffprocs[curproj].process(allInR[curproj]);
    Map & curIn = allIn[curproj];
    curIn.resize(psh);
    curIn = iproc.apply(allInR[curproj]);
    if ( ! interim_name.empty() ) {
      int cur1, cur2, curF;
      strl.slot(curproj, &cur1, &cur2, &curF);
      const string sfI = strl.nofIn > 1
                         ? toString(mask2format("@", strl.nofIn), curproj) : "";
      const string sf2 = strl.origin2size > 1
                         ? toString(mask2format(".@", strl.origin2size), cur2) : "";
      const string sf1 = strl.origin1size > 1
                         ? toString(mask2format(".@", strl.origin1size), cur1) : "";
      const string sfF = strl.flip ? (curF ? "_F" : "_D") : "";
      const string svName = interim_name.dtitle()
                            + "_I" + sfI + sfF + sf2 + sf1 + string(".tif");
      SaveDenan(svName, curIn);
    }
  }

  // stitching if needed
  if (strl.nofIn == 1) { // no stitching
    stitched.reference(allIn[0]);
    if (mskF.size())
      stitched *= mskF;
  } else {
    stitched.resize(ssh);
    if ( ! interim_name.empty() ) {
      vector<bool> addMe(strl.nofIn, false);
      int cur1, cur2, curF;
      for (int fl=0 ; fl <= (int) strl.flip; fl++ ) {
        const string sfF = strl.flip ? (fl ? "F" : "D") : "";
        if (strl.origin1size>1)
          for (int st = 0 ; st < strl.origin2size; st++) {
            for ( int curproj = 0 ; curproj < strl.nofIn ; curproj++) {
              strl.slot(curproj, &cur1, &cur2, &curF);
              addMe[curproj]  =  cur2 == st  &&  curF == fl;
            }
            const string sf = toString(mask2format("@", strl.origin2size), st);
            stitchSome(interim_name.dtitle() + "_U"+sfF+sf+".tif", addMe);
          }
        if (strl.origin2size>1)
          for (int st = 0 ; st < strl.origin1size; st++) {
            for ( int curproj = 0 ; curproj < strl.nofIn ; curproj++) {
              strl.slot(curproj, &cur1, &cur2, &curF);
              addMe[curproj]  =  cur1 == st  &&  curF == fl;
            }
            const string sf = toString(mask2format("@", strl.origin1size), st);
            stitchSome(interim_name.dtitle() + "_V"+sfF+sf+".tif", addMe);
          }
        if (strl.flip) {
          for ( int curproj = 0 ; curproj < strl.nofIn ; curproj++) {
            strl.slot(curproj, 0, 0, &curF);
            addMe[curproj]  =  curF == fl;
          }
          stitchSome(interim_name.dtitle() + "_W" + sfF + ".tif", addMe);
        }
      }
      stitchSome(interim_name.dtitle() + "_X.tif");
    } else {
      stitchSome();
    }
  }

  // closing gaps left after superimposition
  if ( Inpaint::AM == inpaint ) {
    blitz2cl<cl_float>(stitched, iomCL());
    gaussCL.exec(stitched.shape());
    cl2blitz(iomCL(), stitched);
  }
  #ifdef OPENCV_FOUND
  else if ( Inpaint::NS == inpaint  ||  Inpaint::AT == inpaint ) {
    Map stitched_cont = safe(stitched);
    cv::Mat cv_stitched(cv::Size(ssh(1),ssh(0)), CV_32FC1, stitched.data());
    cv::Mat cv_mask(cv::Size(ssh(1),ssh(0)), CV_8UC1, mskI.data() );
    cv::inpaint( cv_stitched, cv_mask, cv_filled, 8
               , Inpaint::AT == inpaint ? cv::INPAINT_TELEA : cv::INPAINT_NS);
    stitched = Map((float*)cv_filled.ptr(), ssh, blitz::neverDeleteData);
  }
  #endif // OPENCV_FOUND

  // final crop
  const Shape<2> fssh(strl.fcrp.shape(ssh));
  final.reference(strl.fcrp.apply(stitched));
  if ( fssh != final.shape() )
    throw_error(modname, "Shape of the results ("+toString(final.shape())+")"
                         " does not match expected ("+toString(fssh)+").");

  // splits
  if ( strl.splits.empty() ) {
    res[0].reference(final);
  } else {
    if ( ! interim_name.empty() && (inpaint || (bool) strl.fcrp ) )
      SaveDenan( interim_name.dtitle() + "_Y.tif", final);
    const string svformat = mask2format("_Z@", strl.splits.size() );
    const int vsplit = strl.splits.at(0) ? 0 : 1;
    int fLine=0;
    for (int curS=0 ; curS < oshs.size() ; curS++) {
      const int lLine = fLine + oshs[curS](vsplit);
      const blitz::Range srang(fLine, lLine-1);
      fLine = lLine;
      res[curS].reference( vsplit ? final(all, srang) : final(srang, all) );
    }
  }

  return res;

}








static const Denoiser & curDnz(const deque<Denoiser> & dnzs, int cur) {
  if (dnzs.size()==1)
    return dnzs[0];
  else if (dnzs.size() > cur)
    return dnzs[cur];
  throw_error("select denoiser", "Empty or incomplete list of denoisers.");
  return dnzs[0]; // just to get rid of no-return warning
}


class ProjInThread : public InThread {

  deque<ReadVolumeBySlice> & allInRd;
  deque<SaveVolumeBySlice> & allOutSv;
  const deque<Denoiser> & dnsr;
  const ProcProj & proc;
  const deque<int> & projes;
  const int zbinn;

  unordered_map<pthread_t, deque<Denoiser>* > dnsrs;
  unordered_map<pthread_t, ProcProj*> procs;
  unordered_map<pthread_t, deque<Map>* > allInMaps;
  deque< pair< pthread_mutex_t, list<BinnProc::Accumulator> > > accs;


  bool inThread(long int idx) {

    const long int sodx =  idx/zbinn + ( zbinn > 0 ? 0 : allOutSv[0].slices() - 1 ) ;
    if (sodx < 0 || sodx >= allOutSv[0].slices() || idx >= allInRd[0].slices() )
      return false;
    if ( find(projes.begin(), projes.end(), sodx) == projes.end() )
      return true;

    const pthread_t me = pthread_self();
    if ( ! procs.count(me) ) { // first call
      deque<Denoiser> * ednsr = new deque<Denoiser>(dnsr);
      ProcProj* eproc = new ProcProj(proc);
      deque<Map> * eAllInMaps = new deque<Map>(allInRd.size());
      lock();
      dnsrs.emplace(me, ednsr);
      procs.emplace(me, eproc);
      allInMaps.emplace(me, eAllInMaps);
      unlock();
    }
    lock();
    deque<Denoiser> & myDnsr = *dnsrs.at(me);
    ProcProj & myProc = *procs.at(me);
    deque<Map> & myAllIn = *allInMaps.at(me);
    unlock();

    try {
      for (ssize_t curI = 0  ;  curI<allInRd.size()  ;  curI++ ) {
        allInRd[curI].readTo(idx, myAllIn[curI]);
        curDnz(myDnsr, curI).proc(myAllIn[curI]);
      }
      deque<Map> & myRes = myProc.process(myAllIn);
      if (zbinn == 1)
        for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ )
          allOutSv[curO].save(sodx, myRes[curO]);
      else {
        for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ ) {
          pthread_mutex_lock(&accs[curO].first);
          list<BinnProc::Accumulator> & curAccs = accs[curO].second;
          Map & curMyRes = myRes[curO];
          if (!curAccs.size())
            curAccs.emplace_back(curMyRes.shape(), zbinn);
          auto useacc = curAccs.end();
          for ( auto accsp  = curAccs.begin() ; accsp != curAccs.end() ; accsp++) {
            if (useacc == curAccs.end() && accsp->index()<0 ) { // found free
              useacc = accsp;
              useacc->index(sodx); // reserve this acc, but keep searching
            } else if (accsp->index() == sodx) {  // found existing
              if ( useacc != curAccs.end() ) // if was reserved
                useacc->index(-1); // release reservation
              useacc = accsp;
              break;
            }
          }
          if (useacc == curAccs.end()) { // nothing found: create new
            curAccs.emplace_back(curMyRes.shape(), zbinn);
            useacc = --curAccs.end();
          }
          BinnProc::Accumulator & myacc = *useacc;
          myacc.index(sodx);
          pthread_mutex_unlock(&accs[curO].first);
          if (myacc.addme(curMyRes))
            allOutSv[curO].save(sodx, curMyRes);
        }
      }
    } catch (...) {
      warn("form projection", toString("Failed processing input index %i, for output slice %i.", idx, sodx));
    }

    bar.update();
    return true;

  }

public:

  ProjInThread(deque<ReadVolumeBySlice> & _allInRd, deque<SaveVolumeBySlice> & _outSave
              , const deque<Denoiser> & _dnsr, const ProcProj & _proc
              , const deque<int> & _projes, const int _zbinn, bool verbose)
    : InThread(verbose, "processing projections")
    , dnsr(_dnsr)
    , proc(_proc)
    , projes(_projes)
    , allInRd(_allInRd)
    , allOutSv(_outSave)
    , zbinn(_zbinn ? _zbinn : allInRd.at(0).slices())
  {
    bar.setSteps(zbinn*projes.size());
    if (zbinn != 1)
      for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ )
        accs.emplace_back( pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER), list<BinnProc::Accumulator>() );
  }

  ~ProjInThread() {
    auto dlnl = [](auto pntr) { if (pntr) { delete pntr; pntr=0; } } ;
    for (auto celem : dnsrs)       dlnl(celem.second) ;
    for (auto celem : procs)       dlnl(celem.second) ;
    for (auto celem : allInMaps)   dlnl(celem.second) ;
    for (auto & accPair : accs)
      pthread_mutex_destroy(&accPair.first);
  }

  void execute(int nThreads=0) { // To deals with possible last incomplete slice
    InThread::execute(nThreads);
    if (abs(zbinn)==1)
      return;
    Map res;
    for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ ) {
      list<BinnProc::Accumulator> & curAccs = accs[curO].second;
      for ( auto & acc : curAccs ) {
        const int odx = acc.getme(res);
        if ( odx >= 0 )
          allOutSv[curO].save(odx, res);
      }
    }
    accs.clear();
    bar.done();
  }



};




/// \MAIN{projection}
int main(int argc, char *argv[]) { {

  const clargs args(argc, argv) ;
  const Shape<2> ish(ImageSizes(args.images.at(0).at(0).repr()));
  const int nofIn = args.images.size();

  // Prepare read factories
  deque<ReadVolumeBySlice> allInRd(nofIn);
  for ( int curI = 0 ; curI < nofIn ; curI++) {
    ReadVolumeBySlice & curRd = allInRd.at(curI);
    curRd.add(args.images.at(curI));
    const int cSls = curRd.slices();
    if (!cSls)
      exit_on_error(args.command, "No images in input "+ toString(curI) +".");
    if (curI && allInRd.at(0).slices() != cSls)
      exit_on_error(args.command,
                    "Not matching slices in input "+ toString(curI) + ": "
                    + toString(allInRd.at(0).slices()) +" != "+ toString(cSls) +".");
  }
  const int nofProj = allInRd.at(0).slices();
  const int nofOuts = binnOne(nofProj, args.zbinn);
  if ( args.testMe >= nofOuts )
    exit_on_error(args.command, "Requested test is beyond number of projections.");
  const deque<int> projes = slice_str2vec(args.out_range, nofOuts);
  if (!projes.size())
    exit_on_error(args.command, "Given range \"" + args.out_range + "\""
                                " is beyond output slices "+toString(nofOuts)+".");

  // only print output sizes and exit
  if (args.doTest && args.testMe < 0) {
    vector<Shape<2>> oshs(ProcProj::outputShapes(args.st, ish));
    if (oshs.empty())
      exit_on_error(args.command, "No output. Check your input and stitching parameters.");
    for(auto osh : oshs)
      cout << nofOuts << " " << osh(0) << " " << osh(1) << "\n";
    exit(0);
  }

  // Read auxiliary images
  #define rdAux(pfx) \
    deque<Map> pfx##as(args.pfx##s.size()); \
    for ( int curf = 0 ; curf < args. pfx##s.size() ; curf++) \
      ReadImage(args. pfx##s[curf].repr(), pfx##as[curf], ish);
  rdAux(bg);
  rdAux(df);
  rdAux(dg);
  rdAux(ms);
  #undef rdAux

  // normalize masks
  for (int curm=0 ; curm<msas.size() ; curm++) {
    Map & msk = msas[curm];
    const float vmin = min(msk);
    const float delta = max(msk) - vmin;
    if (delta==0.0)
      msk = 1.0;
    else
      msk = (msk-vmin)/delta;
  }

  // Construct masks from bgs and df/gs if not provided but required by other parameters.
  if ( ! msas.size()  &&  args.st.edge ) {
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
      for (ssize_t ycur = 0 ; ycur < ish(0) ; ycur++ ) {
        for (ssize_t xcur = 0 ; xcur < ish(1) ; xcur++ ) {
          float val = mscur(ycur,xcur);
          mscur(ycur,xcur) = ( val <= 0.0 || ! fisok(val) ) ? 0 : 1.0;
        }
      }
    }
  }

  // create canonical denoiser(s)
  deque<Denoiser> canonDZs;
  if (!msas.size())
    canonDZs.emplace_back(ish, (int) args.denoiseRad, args.denoiseThr);
  else if (msas.size()==1)
    canonDZs.emplace_back(ish, (int) args.denoiseRad, args.denoiseThr, msas[0]);
  else
    for ( int curI = 0 ; curI < nofIn ; curI++)
      canonDZs.emplace_back(ish, (int) args.denoiseRad, args.denoiseThr, msas[curI]);

  // denoise bg and dg
  for (int curg=0 ; curg<bgas.size() ; curg++)
    curDnz(canonDZs, curg).proc(bgas[curg]);
  for (int curg=0 ; curg<dgas.size() ; curg++)
    curDnz(canonDZs, curg).proc(dgas[curg]);

  // Prepare canonical projection processor
  const string testFormat = [&](){
    if (args.testMe < 0)
      return string();
    string toRet = args.out_name.dtitle();
    if (toRet.find('@') != string::npos)
      toRet = toString(mask2format(toRet, nofProj), args.testMe);
    toRet += "%s";
    return toRet;
  }();
  ProcProj canonPP(args.st, ish, bgas, dfas, dgas, msas, toString(testFormat, "_mask.tif"));

  // Prepare saving factories
  const std::vector<Shape<2>> & outShapes = canonPP.outputShapes();
  const size_t nofSplts = outShapes.size();
  const string spformat = mask2format("_split@", nofSplts);
  deque<SaveVolumeBySlice> allOutSv;
  for (int curSplt = 0 ; curSplt < nofSplts ; curSplt++) {
    ImagePath filedescind = args.out_name;
    if (args.testMe >= 0)
      filedescind = args.out_name.dtitle() + ".tif";
    if (nofSplts > 1)
      filedescind =   filedescind.dtitle() + toString(spformat, curSplt)
                    + filedescind.ext() + filedescind.desc();
    allOutSv.emplace_back(filedescind, Shape<3>(nofOuts, outShapes[curSplt](0), outShapes[curSplt](1)) );
  }

  // Test or process and save
  if ( args.testMe >= 0 ) { // test
    deque<Map> allIn;
    for ( ssize_t curI = 0 ; curI < nofIn ; curI++) {
      allIn.emplace_back(ish);
      const int trd = args.zbinn * ( args.testMe >= 0  ?  args.testMe  :  projes[0] );
      allInRd[curI].readTo(trd, allIn[curI]);
      curDnz(canonDZs, curI).proc(allIn.back());
    }
    const deque<Map> & allOut = canonPP.process(allIn, toString(testFormat, ".tif"));
    for (int curSplt = 0 ; curSplt < nofSplts ; curSplt++) {
      const Map & cOut = allOut[curSplt];
      const int svIdx = args.zbinn ? args.testMe / args.zbinn : 0;
      ImagePath svPath = allOutSv[curSplt].save(svIdx, cOut);
      if (args.beverbose)
        cout << nofOuts << " " << cOut.shape()(0) << " " << cOut.shape()(1) << " " << svPath << "\n";
    }
  } else // finally process
    ProjInThread(allInRd, allOutSv, canonDZs, canonPP, projes, args.zbinn, args.beverbose)
        .execute();

} exit(0); }

