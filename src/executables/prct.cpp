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
#include "../common/ipc.h"
#include "../common/kernel.h"
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

struct PhaseRules {
  float d2b;                    ///< \f$\d2b\f$ parameter of the MBA.
  float lambda;                 ///< Wavelength.
  float dist;                   ///< Object-to-detector distance.
  PhaseRules()
    : d2b(-1)
    , lambda(1)
    , dist(0)
  {}
};

struct CTrules {
  float center;                   ///< Rotation center.
  Contrast contrast;            ///< Type of the contrast.
  Filter filter_type;           ///< Type of the filtering function.
  float arc;
  CTrules()
    : center(0)
    , arc(180)
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
  ImagePath out_name;          ///< Name of the output image.
  ImagePath out_proj;          ///< Name of the output projections.
  float dd;                     ///< Pixel size.
  float mincon;         ///< Black intensity.
  float maxcon;
  StitchRules st;
  PhaseRules  phs;
  CTrules ctrl;
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
  , dd(1)
  , mincon(0)
  , maxcon(0)
  , beverbose(false)
{

  deque<ImagePath> iimages;

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &iimages, "images", "Input 2D or 3D images.",
         "All images must be of the same rank (2D or 3D). HDF5 format:\n"
         "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc)

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &out_name, 'o', "output", "Output reconstructed image mask.", "", out_name)
    .add(poptmx::OPTION, &out_proj, 'O', "out-projections", "Output projection image mask.", "", out_proj)
    .add(poptmx::OPTION, &mincon, 0, "min", "Pixel value corresponding to black.",
         " All values below this will turn black.", "<minimum>")
    .add(poptmx::OPTION, &maxcon, 0, "max", "Pixel value corresponding to white.",
         " All values above this will turn white.", "<maximum>")

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

    .add(poptmx::OPTION, &phs.dist, 'z', "dist", "Object to detector distance (mm).",
         "Needed only for the inline phase contrast processing." )
    .add(poptmx::OPTION, &dd, 'p', "pixel", "Pixel size (mum).",
         NeedForQuant, toString(dd) )
    .add(poptmx::OPTION, &phs.d2b, 0, "d2b", "delta/beta ratio.",
         "Needed only for the inline phase contrast processing negative for no IPC.", toString(phs.d2b))
    .add(poptmx::OPTION, &phs.lambda, 'w', "wavelength", "Wavelength of the X-Ray (Angstrom)",
           "Only needed together with " + table.desc(&phs.d2b) + ".", toString(phs.lambda))

    .add(poptmx::OPTION, &ctrl.contrast, 'k', "contrast", "Input component.",
         "Type of the contrast presented in the sinogram. " + Contrast::Desc)
    .add(poptmx::OPTION, &ctrl.center, 't', "center", "Rotation center.",
         CenterOptionDesc, toString(ctrl.center))
    .add(poptmx::OPTION, &ctrl.arc, 'a', "arcan", "CT scan range (if > 1 deg), or step size (if < 1deg).",
         "If value is greater than 1deg then it represents the arc of the CT scan."
         " Otherwise the value is the step size.", toString(ctrl.arc))
    .add(poptmx::OPTION, &ctrl.filter_type, 0, "filter", "Filtering window used in the CT.",
         FilterOptionDesc, ctrl.filter_type.name())

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
  if ( table.count(&st.originF) ) {
    if ( tiledImages % 2 )
      exit_on_error(command, string () +
        "Use of the " + table.desc(&st.originF) + " option requires even number of input images"
        " (while " + toString(tiledImages) + " counted).");
    tiledImages /= 2;
  }
  if ( table.count(&st.origin2) > table.count(&st.origin1) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2) + " require also " + table.desc(&st.origin1) + " option.");
  if ( table.count(&st.origin2) > table.count(&st.origin2size) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2) + " require also " + table.desc(&st.origin2size) + " option.");
  if ( table.count(&st.origin2size) && ! table.count(&st.origin2) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2size) + " require also " + table.desc(&st.origin2) + " option.");
  if ( table.count(&st.origin2size)  &&  st.origin2size  < 2 )
    exit_on_error(command, string () +
      "Requested second stitch size (" + toString(st.origin2size) + ") is less than 2.");
  if ( table.count(&st.origin2size)  &&  tiledImages % st.origin2size )
    exit_on_error(command, string () +
      "Total number of tiled images (" + toString(tiledImages) + ") is not a multiple of the requested second stitch size"
      " (" + toString(st.origin2size) + ") given by " + table.desc(&st.origin2size) + " option.");

  if (phs.d2b >= 0.0) { // inline phase contrast
    if (phs.dist <= 0.0)
      exit_on_error(command, "Zero or negative distance (given by "+table.desc(&phs.dist)+").");
    phs.dist /= 1.0E3; // convert mm -> m
    if (phs.lambda <= 0.0)
      exit_on_error(command, "Zero or negative wavelength (given by "+table.desc(&phs.lambda)+").");
    phs.lambda /= 1.0E10; // convert A -> m
    if ( table.count(&phs.lambda) && ! table.count(&phs.d2b) )
      warn(command, "The wavelength (given by "+table.desc(&phs.lambda)+") has influence only together"
           " with the alpha parameter (given by "+table.desc(&phs.d2b)+").");
    if ( ! table.count(&phs.lambda) && table.count(&phs.d2b) )
      warn(command, "The wavelength (given by "+table.desc(&phs.lambda)+") needed together with"
           " the alpha parameter (given by "+table.desc(&phs.d2b)+") for the correct results.");
  }

  if (ctrl.arc <= 0.0)
    exit_on_error(command, "CT arc (given by "+table.desc(&ctrl.arc)+") must be strictly positive.");
  if (dd <= 0.0)
    exit_on_error(command, "Zero or negative pixel size (given by "+table.desc(&dd)+").");

  dd /= 1.0E6; // convert micron -> m
  st.angle *= M_PI/180;
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
  static cl_program proj_oCLprog;
  static pthread_mutex_t protectProgramCompilation;
  StitchRules st;
  deque<FlatFieldProc> ffprocs;

  deque<Map> msksI, msks1, msks2; // shared
  Map mskF; // shared
  ImageProc iproc; // own
  Map stitched, final; // own
  deque<Map> allIn, o1Stitch, o2Stitch;

  bool doGapsFill;
  CLkernel gaussCL;
  CLmem iomCL;
  CLmem maskCL_R;
  CLmem & maskCL;

  //const PhaseRules & phsrl;
  IPCprocess ipcproc;
  float ipccoef;


  static void stitch(PointF2D origin,  const deque<Map> & iarr, Map & oarr
                    ,  const deque<Map> & gprr = deque<Map>() ) {

    const int isz = iarr.size();
    if ( isz == 0 )
      throw_error(modname, "Nothing to stitch.");
    if ( isz == 1 ) {
      oarr.reference(iarr[0]);
      return;
    }

    if (gprr.size() == 1) {
      const Shape csz = gprr[0].shape();
      for (int acur = 0 ; acur < isz ; acur++ )
        if (iarr[acur].shape()!=csz)
          throw_error(modname, "Different image sizes in mask and inputs.");
    } else if (gprr.size() == iarr.size()) {
      for (int acur = 0 ; acur < isz ; acur++ )
        if (iarr[acur].shape()!=gprr[acur].shape())
          throw_error(modname, "Non matching image sizes in mask and input arrays.");
    } else if (gprr.size()) {
      throw_error(modname, "Inconsistent sizes of mask and image arrays.");
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
        float sweight=0.0;
        float svals=0.0;
        for (int acur = 0 ; acur < isz ; acur++ ) {
          const Map & curar = iarr[acur];
          const Shape cursh = curar.shape();
          const Shape coo = Shape(miny+ycur-acur*origin.y, minx+xcur-acur*origin.x);
          if ( coo(0) >= 0 && coo(0) < cursh(0) && coo(1) >= 0 && coo(1) < cursh(1) ) {
            const float varcur = curar(coo);
            if ( fisok(varcur) ) {
              float weight = 1 + ( cursh(0) - abs( 2*coo(0) - cursh(0) + 1l ) )
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

        oarr(ycur,xcur) = sweight == 0.0 ? 0.0 : svals / sweight ;

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


  void initCL() {
    if (!doGapsFill)
      return;
    pthread_mutex_lock(&protectProgramCompilation);
    if (!proj_oCLprog) {
      const char proj_oCLsrc[] = {
        #include "projection.cl.includeme"
      };
      proj_oCLprog =
        initProgram(proj_oCLsrc, sizeof(proj_oCLsrc), "Projection in OCL");
    }
    pthread_mutex_unlock(&protectProgramCompilation);
    if (!proj_oCLprog)
      throw_error(modname, "Failed to build OCP program for projection formation");

    iomCL(clAllocArray<float>(mskF.size()));
    if (!maskCL())
      maskCL(blitz2cl(mskF, CL_MEM_READ_ONLY));
    gaussCL(proj_oCLprog, "gauss");
    gaussCL.setArg(0, int(mskF.shape()(1)));
    gaussCL.setArg(1, int(mskF.shape()(0)));
    gaussCL.setArg(2, iomCL());
    gaussCL.setArg(3, maskCL());
    gaussCL.setArg(4, float(st.sigma) );
  }

public:

  ProcProj( const StitchRules & _st, const PhaseRules & phsrules, float dd
          , const deque<Map> & bgas, const deque<Map> & dfas
          , const deque<Map> & dgas, const deque<Map> & msas)
    : st(_st)
    , iproc(st.angle, st.crp, st.bnn, _st.ish)
    , allIn(st.nofIn)
    , o1Stitch(st.nofIn/st.origin1size)
    , o2Stitch(st.flipUsed ? 2 : 1)
    , doGapsFill(false)
    , maskCL_R(0)
    , maskCL(maskCL_R)
    , ipcproc(oshape(st), phsrules.d2b)
    , ipccoef(IPCprocess::d2bNorm(phsrules.d2b, dd, phsrules.dist, phsrules.lambda))
  {

    if ( ! area(st.ish) )
      throw_error(modname, "Zerro area to process.");
    if ( ! st.nofIn )
      throw_error(modname, "Zerro images for input.");

    #define chkAuxImgs(imas, lbl) \
      if (imas.size() && imas.size() != 1 && imas.size() != st.nofIn) \
        throw_error(modname, "Number of " lbl " images is neither 0, 1 nor the number of inputs" \
                             " (" + toString(st.nofIn) + ")."); \
      for (int curI = 0; curI < imas.size() ; curI++) \
        if ( imas.at(curI).size() && imas.at(curI).shape() != st.ish ) \
          throw_error(modname, "Unexpected shape of " lbl " image.");

    chkAuxImgs(bgas, "background");
    chkAuxImgs(dfas, "darkfield");
    chkAuxImgs(dgas, "darkground");
    chkAuxImgs(msas, "mask");
    #undef chkAuxImgs

    const Map zmap;
    if (bgas.size() > 1 || dfas.size() > 1 || dgas.size() > 1 || msas.size() > 1) {
      for (int curI = 0; curI < st.nofIn ; curI++) {
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

    if (!st.fcrp)
      final.reference(stitched);

    const int mssz = msas.size();
    if ( ! mssz )
      return;

    for (int curI = 0; curI < msas.size() ; curI++) {
      if (msas[curI].shape() != st.ish)
        throw_error(modname, "Unexpected mask shape.");
      Map msT;
      iproc.proc(msas[curI], msT);
      prepareMask(msT, true);
      msksI.emplace_back(msT.shape());
      msksI.back() = msT;
    }
    while (msksI.size() != st.nofIn)
      msksI.emplace_back(msksI[0]);

    msks1.resize(o1Stitch.size());
    sub_proc(st.origin1size, st.origin1, msksI, deque<Map>(), msks1);

    msks2.resize(o2Stitch.size());
    sub_proc(st.origin2size, st.origin2, msks1, deque<Map>(), msks2);

    if ( st.flipUsed ) {
      msks2[1].reverseSelf(blitz::secondDim);
      stitch(st.originF, msks2, mskF, deque<Map>());
    } else
      mskF.reference(msks2[0]);

    for (int curM=0 ; curM < msks1.size() ; curM++)
      prepareMask(msks1[curM], false);
    for (int curM=0 ; curM < msks2.size() ; curM++)
      prepareMask(msks2[curM], false);
    prepareMask(mskF, false);
    doGapsFill = st.sigma > 0.0  &&  any(mskF==0.0);
    initCL();

  }


  ProcProj(const ProcProj & other)
    : st(other.st)
    , iproc(other.iproc)
    , ffprocs(other.ffprocs)
    , allIn(st.nofIn)
    , o1Stitch(st.nofIn/st.origin1size)
    , o2Stitch(st.flipUsed ? 2 : 1)
    , msks1(other.msks1)
    , msks2(other.msks2)
    , mskF(other.mskF)
    , doGapsFill(other.doGapsFill)
    , maskCL(other.maskCL)
    , ipcproc(other.ipcproc.sh, other.ipcproc.d2b)
    , ipccoef(other.ipccoef)
  {
    if (doGapsFill)
      initCL();
    if (!st.fcrp)
      final.reference(stitched);
  }

  static Shape oshape(const StitchRules & _st) {
    Shape frsh = binn(crop(rotate(_st.ish, _st.angle), _st.crp), _st.bnn);
    Shape stsh1 = Shape( _st.ish(0) + abs(_st.origin1.y)*_st.origin1size,
                         _st.ish(1) + abs(_st.origin1.x)*_st.origin1size );
    Shape stsh2 = Shape( stsh1(0) + abs(_st.origin2.y)*_st.origin2size,
                         stsh1(1) + abs(_st.origin2.x)*_st.origin2size );
    Shape fsh = _st.flipUsed ? stsh2 : Shape(stsh2(0)+abs(_st.originF.y), stsh2(1)+abs(_st.originF.x));
    Shape rsh = crop(fsh, _st.fcrp);
    return rsh;
  }


  void sub_proc(uint orgsize, PointF2D origin, const deque<Map> & hiar, const deque<Map> & msks, deque<Map> & oar) {

    if ( orgsize == 1 ) {
      for(int curM = 0 ; curM < oar.size() ; curM++)
        oar[curM].reference(hiar[curM]);
      return;
    }

    const int nofin = hiar.size();
    for ( int inidx=0 ; inidx<nofin ; inidx += orgsize ) {
      int cidx=inidx/orgsize;
      deque<Map> supplyIm( hiar.begin() + inidx, hiar.begin() + inidx + orgsize) ;
      deque<Map> supplyMs( msks.size() ? msks.begin() + inidx           : msks.begin(),
                           msks.size() ? msks.begin() + inidx + orgsize : msks.begin() );
      stitch(origin, supplyIm, oar[cidx], supplyMs);
    }
    return;

  }


  bool process(deque<Map> & allInR, Map & res) {

    if (allInR.size() != st.nofIn)
      return false;

    // prepare input images
    int curF=0, cur2=0, cur1=0;
    for ( int curproj = 0 ; curproj < st.nofIn ; curproj++) {
      if (ffprocs.size() == 1)
        ffprocs[0].process(allInR[curproj]);
      else if (ffprocs.size() == st.nofIn)
        ffprocs[curproj].process(allInR[curproj]);
      iproc.proc(allInR[curproj], allIn[curproj]);
    }

    // first stitch
    sub_proc(st.origin1size, st.origin1, allIn, msksI, o1Stitch);
    // second stitch
    sub_proc(st.origin2size, st.origin2, o1Stitch, msks1, o2Stitch);
    // flip stitch
    if ( st.flipUsed ) {
      o2Stitch[1].reverseSelf(blitz::secondDim);
      stitch(st.originF, o2Stitch, stitched, msks2);
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
    if (st.fcrp)
      crop(stitched, final, st.fcrp);

    // IPC processing
    ipcproc.extract(final, IPCprocess::PHS, 1.0/ipccoef);

    if (!res.size())
      res.reference(final);
    else if (!areSame(final, res)) {
      res.resize(final.shape());
      res=final;
    }
    return true;

  }


};
const string ProcProj::modname="ProcProj";
cl_program ProcProj::proj_oCLprog = 0;
pthread_mutex_t ProcProj::protectProgramCompilation = PTHREAD_MUTEX_INITIALIZER;


struct InterimStorage {
  SaveVolumeBySlice * wvol;
  SaveVolumeBySlice * tvol;
  ReadVolumeBySlice * rvol;
  Volume vol;
  const Shape psh;
  const Shape ssh;
  const uint projs;
  const uint sinos;
  static const string modname;

  InterimStorage(Shape3 sh, const ImagePath & saveMe, const Path & tmpDir)
    : wvol(0)
    , tvol(0)
    , rvol(0)
    , psh(faceShape(sh))
    , ssh(Shape(sh(0),sh(2)))
    , projs(sh(0))
    , sinos(sh(1))
  {
    try {
      vol.resize(sh);
    } catch (bad_alloc err) {
      vol.free();
    }
    if (!saveMe.empty()) {
      wvol = new SaveVolumeBySlice(saveMe, psh, projs);
      if (!wvol)
        throw_error(modname, "Could not prepare for saving volume " + saveMe.repr() + ".");
    }
    if (vol.size())
      return;

    const deque<string> sldsc = split(saveMe.desc(), ":");
    if ( wvol && saveMe.isValidHDF() && (sldsc.size()<2 || sldsc[1].empty()) ) {
      // use wvol as storage
      tvol = wvol;
      rvol = new ReadVolumeBySlice(saveMe + (sldsc.size()==2 ? "Y" : ":Y"));
    } else {
      // use dedicated temprorary HDF
      ImagePath tempMe = ( tmpDir.empty() ? "." : tmpDir ) + "/.SAMPLE.hdf:/data";
      tvol = new SaveVolumeBySlice(tempMe, psh, projs);
      rvol = new ReadVolumeBySlice(tempMe+":Y");
      if ( unlink(tempMe.c_str()) < 0 )
        warn(modname, "Could not unlink temporary file \"" + tempMe + "\"."
                          " Don't forget to delete it after the program has finished.");
   }

  }

  ~InterimStorage() {
    if (wvol) delete wvol;
    if (tvol && tvol != wvol) delete tvol;
    if (rvol) delete rvol;
  }

  void putProjection(const Map & proj, uint idx) {
    if (proj.shape() != psh)
      throw_error(modname, "Input map shape "+toString(proj.shape())+
                           " does not match constructed "+toString(psh)+".");
    if (idx >= projs)
      throw_error(modname, "Input index " + toString(idx) + " is beyond constructed size "
                           + toString(projs) + ".");
    if (vol.size())
      vol(idx, all, all) = proj;
    if (wvol)
      wvol->save(idx, proj);
    if (tvol && tvol != wvol)
      tvol->save(idx, proj);
  }

  void getSinogram(Map & sino, uint idx) const {
    if (idx>=sinos)
      throw_error(modname, "Requested sinogram"+toString(idx)+" is beyond constructed size "
                           + toString(sinos) + ".");
    if (vol.size()) {
      sino.resize(ssh);
      sino = vol(all,idx,all);
    } else if (!rvol) {
      throw_error(modname, "No storage in either volume of file.");
    } else {
      rvol->read(idx, sino);
    }
  }

};
const string InterimStorage::modname = "Projections storage";



class ProjInThread : public InThread {

  deque<ReadVolumeBySlice> & allInRd;
  InterimStorage & allProjes;
  const ProcProj & proc;

  unordered_map<pthread_t, ProcProj> procs;
  unordered_map<pthread_t, deque<Map> > allInMaps;
  unordered_map<pthread_t, Map > results;

  bool inThread(long int idx) {

    if (idx >= allProjes.projs)
      return false;

    const pthread_t me = pthread_self();
    lock();
    if ( ! procs.count(me) ) { // first call
      procs.emplace(me, proc);
      allInMaps.emplace(me, deque<Map>(allInRd.size()));
      results[me];
    }
    ProcProj & myProc = procs.at(me);
    deque<Map> & myAllIn = allInMaps.at(me);
    Map & myRes = results.at(me);
    unlock();

    try {
      for (ArrIndex curI = 0  ;  curI<allInRd.size()  ;  curI++ )
        allInRd[curI].read(idx, myAllIn[curI]);
      myProc.process(myAllIn, myRes);
      allProjes.putProjection(myRes, idx);
    } catch (...) {
      warn("form projection", toString("Failed on slice %i.", idx));
    }

    bar.update();
    return true;

  }

public:

  ProjInThread(deque<ReadVolumeBySlice> & _allInRd, InterimStorage & _allProjes
              , const ProcProj & _proc, bool verbose=false)
    : InThread(verbose, "processing projections", _allProjes.projs)
    , proc(_proc)
    , allInRd(_allInRd)
    , allProjes(_allProjes)
  {}


};



class RecInThread : public InThread {

  const CTrules & ctrl;
  const float dd;
  const InterimStorage & allSinos;
  const Shape ish;
  const Shape osh;
  SaveVolumeBySlice ovolSv;

  unordered_map<pthread_t, CTrec> recs;
  unordered_map<pthread_t, Map> imaps;
  unordered_map<pthread_t, Map> omaps;

  bool inThread(long int idx) {
    if (idx >= allSinos.sinos)
      return false;

    const pthread_t me = pthread_self();
    lock();
    if ( ! recs.count(me) ) {
      recs.emplace(piecewise_construct,
                   forward_as_tuple(me),
                   forward_as_tuple(ish, ctrl.contrast, ctrl.arc, ctrl.filter_type));
      imaps.emplace(me, ish);
      omaps.emplace(me, osh);
    }
    CTrec & myRec = recs.at(me);
    Map & myImap = imaps.at(me);
    Map & myOmap = omaps.at(me);
    unlock();

    allSinos.getSinogram(myImap, idx);
    myRec.reconstruct(myImap, myOmap, ctrl.center, dd);
    ovolSv.save(idx, myOmap);

    bar.update();
    return true;

  }

public:

  RecInThread( const InterimStorage & snstor, const CTrules & _ctrl, float _dd
             , const ImagePath & out_mask, float mincon, float maxcon, bool beverbose)
    : InThread(beverbose, "CT'ing", snstor.sinos)
    , ctrl(_ctrl)
    , dd(_dd)
    , allSinos(snstor)
    , ish(snstor.ssh)
    , osh(ish(1),ish(1))
    , ovolSv(out_mask, osh, snstor.sinos, mincon, maxcon)
  {
  }



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
  const Shape projSh = ProcProj::oshape(args.st);

  InterimStorage prstor(Shape3(nofProj, projSh(0), projSh(1)), args.out_proj, args.out_name.dir() );
  // form projections
  { // to deconstruct objects
    ProcProj canonPP(st, args.phs, args.dd, bgas, dfas, dgas, msas);
    ProjInThread(allInRd, prstor, canonPP, args.beverbose)
        .execute();
    allInRd.clear();
  }
  // CT reconstruct
  RecInThread(prstor, args.ctrl, args.dd, args.out_name, args.mincon, args.maxcon, args.beverbose)
      .execute();

  exit(0);

}

