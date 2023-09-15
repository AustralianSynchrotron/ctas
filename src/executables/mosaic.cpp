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

#include <optional>
#include <unistd.h>
#include "../common/ctas.h"
#include "../common/poptmx.h"


using namespace std;






/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  uint nofIn;
  deque< float > angles;
  deque< Crop<2> > crops;                  ///< Crop input projection image
  deque< Binn<2> > binns;                  ///< binning factor
  deque< PointI<2> > origins;
  deque< Crop<2> > outCrops;                  ///< Crop final projection image
  int zbinn;
  uint edge;               ///< blur of mask and image edges.
  deque< deque<ImagePath> > images;        ///< images to combine
  deque<ImagePath> bgs;        ///< Array of the background images.
  deque<ImagePath> dfs;        ///< Array of the dark field images.
  deque<ImagePath> dgs;        ///< Array of the dark field images for backgrounds.
  deque<ImagePath> mss;        ///< Mask Array.
  uint denoiseRad;
  float denoiseThr;
  ImagePath out_name;              ///< Name template of the output image(s).
  string out_range;
  int testMe;          ///< Prefix to save interim results
  bool doTest;
  bool beverbose;             ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
  poptmx::OptionTable table;
};


clargs::
clargs(int argc, char *argv[])
  : table("Combines multiple images to form single projection.",
          "Transforms and stitches components of the composite image(s)."
          " Transformations are applied in the following order: rotate, crop, binning." )
  , nofIn(0)
  , edge(0)
  , denoiseRad(0)
  , denoiseThr(0.0)
  , zbinn(1)
  , testMe(-1)
  , doTest(false)
  , beverbose(false)
{

  auto chkNofIns = [&](auto & ims, const string & lbl) {
    if (ims.size() && ims.size() != 1 && ims.size() != nofIn)
      exit_on_error(command, "Number of " +lbl+ " given by " + table.desc(&ims) + " option "
                             " is neither 0, 1, nor the number of inputs (" + toString(nofIn) +  ").");
  };
  const string multiIN = "If only one provided, then it will be used for all images."
                         " Otherwise must be equal to the number of input images.";
  deque<ImagePath> iimages;

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &iimages, "images"
         , "Input 2D or 3D images. If '-', then reads image from the stdin."
         , "All images must be of the same rank (2D or 3D). HDF5 format:\n"
           "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc)

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &bgs, 'B', "bg", "Background image(s)", "")
    .add(poptmx::OPTION, &dfs, 'D', "df", "Dark field image(s)", "")
    .add(poptmx::OPTION, &dgs, 'F', "dg", "Dark field image(s) for backgrounds", "")
    .add(poptmx::OPTION, &mss, 'M', "mask", "Mask image(s)",
         "Image where values are weights of corresponding pixels in superimposition operations."
         " F.e. 0 values exclude corresponding or affected pixels from further use.")
    .add(poptmx::OPTION, &out_name, 'o', "output", "Template fo output image(s)."
         , "If more than one is provided, then equal number of final crops must be given.", "")
    .add(poptmx::OPTION, &out_range, 'O', "select", "Slices to process. All if not used.",
         SliceOptionDesc + ". Only makes sense in multiple projections.", "all")
    .add(poptmx::OPTION, &angles,'r', "rotate", "Rotation angle(s).", multiIN)
    .add(poptmx::OPTION, &binns, 'b', "binn", "Binn factor(s)."
         , multiIN + " " + BinnOptionDesc)
    .add(poptmx::OPTION, &crops, 'c', "crop", "Crop input image(s)."
         , multiIN + " " + CropOptionDesc)
    .add(poptmx::OPTION, &origins, 'g', "origin", "Origin of the image relative to the first one.",
         "Number of origins must be one less than the number of input images.")
    .add(poptmx::OPTION, &outCrops, 'C', "crop-final", "Crop final image(s).", CropOptionDesc)
    .add(poptmx::OPTION, &zbinn, 'z', "zinn", "Binning over multiple input projections.",
         "If zero, binns all outputs down to a single image. Ignored for tests.")
    .add(poptmx::OPTION, &edge, 'e', "edge", "Thickness in pixels of edge transition.",
         "Smoothly reduces the weight of pixels around the mask edges (0 values in mask)"
         " to produce seamless image stitching." )
    .add(poptmx::OPTION, &denoiseRad, 'n', "noise-rad", "Radius for noise reducing algorithm.",
         "Radius of the local region used to calculate average to replace the pixel value." )
    .add(poptmx::OPTION, &denoiseThr, 'N', "noise-thr", "Noise threshold.",
         "If positive, then gives maximum absolute deviation from the average;"
         " if negative, then negated value is maximum relative deviation.")
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

  nofIn = table.count(&iimages);
  if (!nofIn)
    exit_on_error(command, "No input images provided.");
  images.resize(nofIn);
  deque<uint> toRead;
  for ( int curI = 0 ; curI < nofIn ; curI++ )
    if ( iimages.at(curI) != "-" )
      images.at(curI).push_back(iimages.at(curI));
    else
      toRead.push_back(curI);
  if (toRead.size()) {
    if (isatty(fileno(stdin)))
      exit_on_error(command, "Could not read from stdin " + toString(toRead.size()) +
                             " input image(s) as requested by '-' name." );
    uint incounter=0;
    string inputline;
    while ( ! cin.eof() && getline(cin, inputline) ) {
      trim(inputline);
      if (inputline.size() && inputline.at(0) != '#' ) {
        deque<string> inputdeque = split(inputline, " ");
        if (inputdeque.size() != toRead.size())
          exit_on_error(command, "Line "+toString(incounter)+" of stdin input contains "
                                 +toString(inputdeque.size())+" images, while "
                                 +toString(toRead.size())+" are expected.");
        for (int curJ = 0 ; curJ < toRead.size() ; curJ++ )
          images.at(toRead[curJ]).push_back(inputdeque.at(curJ));
      }
    }
  }

  chkNofIns(bgs, "background images");
  chkNofIns(dfs, "darkfield images");
  chkNofIns(dgs, "darkground images");
  chkNofIns(mss, "mask images");
  if (dgs.size() && ! bgs.size())
    exit_on_error(command, "No background images (" + table.desc(&bgs) + ") for"
                           " provided darkgrounds (" + table.desc(&dgs) + ").");
  if ( ! mss.size()  &&  ! bgs.size()  &&  edge )
    throw_error(command, "No background (" + table.desc(&bgs) + ") or mask"
                         " (" + table.desc(&mss) + ") images provided while"
                         " mask operation (" + table.desc(&edge) + ") is requested.");

  if ( ! table.count(&out_name) )
    exit_on_error(command, "No output name provided. Use option " + table.desc(&out_name) + ".");
  doTest = table.count(&testMe);

  for (float & ang : angles)
    ang *= M_PI/180;
  chkNofIns(angles, "rotations");
  chkNofIns(crops, "crops");
  chkNofIns(binns, "binns");
  if ( origins.size() != nofIn - 1 )
    exit_on_error(command, "Number of image origins given by " + table.desc(&origins) + " option "
                           " must be one less than the number of inputs (" + toString(nofIn) + "-1)."
                           " Instead " + toString(origins.size()) + " are provided.");
  origins.push_front(PointF<2>()); // first image is always at 0,0.

  if ( table.count(&denoiseThr) && ! table.count(&denoiseRad) )
    warn(command, "Setting noise threshold via " + table.desc(&denoiseThr) + " option "
         " without noise rad (option " + table.desc(&denoiseRad) + ") makes no sense.");

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
  const uint nofIn;

  struct FamilyValues {

    std::vector< Shape<2> > ishs; // input images
    std::deque<ImageProc> canonImProcs;
    std::vector< Shape<2> > pshs; // processed images
    Shape<2> ssh; // stitched image
    Denoiser * canonDenoiser = 0;
    std::vector< Crop<2> > ocrps;
    std::vector<Map> wghts;
    std::vector< PointI<2> > origins;
    Map swght;
    Map mskF;
    bool doGapsFill;

    class ForCLdev {
      static const string modname;
      const Shape<2> sh;
      const Map & mask;
      CLenv & cl;
      CLprogram oclProgram;
      CLkernel kernelGauss;
      CLmem maskCL;
      CLmem iomCL;
      pthread_mutex_t locker;

    public:

      ForCLdev(CLenv & cl, const Map & mask)
        : cl(cl)
        , sh(mask.shape())
        , mask(mask)
        , locker(PTHREAD_MUTEX_INITIALIZER)
      {
        if (!CL_isReady())
          throw_error(modname, "Dysfunctional OpenCL environment.");
        if (!size(sh))
          throw_error(modname, "Zero input size");
        if (!any(mask==0.0)) {
          warn(modname, "Nothing to close in the mask provided.");
          return;
        }
      }

      ~ForCLdev() {
        pthread_mutex_destroy(&locker);
      } ;

      bool process(Map & in) {
        if (in.shape() != sh)
          throw_error(modname, "Unexpected array size on OCL extract.");
        if ( ! CL_isReady()  ||  pthread_mutex_trylock(&locker) )
          return false;
        bool toRet = false;
        try {
          if (!maskCL) {
            static const string oclsrc = {
              #include "projection.cl.includeme"
            };
            oclProgram(oclsrc, cl.cont);
            kernelGauss(oclProgram, "gauss");
            iomCL(clAllocArray<cl_float>(size(sh), CL_MEM_READ_WRITE, cl.cont));
            maskCL(blitz2cl<cl_float>(mask, CL_MEM_READ_ONLY, cl.que));
            kernelGauss.setArg(0, int(sh(1)));
            kernelGauss.setArg(1, int(sh(0)));
            kernelGauss.setArg(2, iomCL());
            kernelGauss.setArg(3, maskCL());
          }
          blitz2cl<cl_float>(in, iomCL(), cl.que);
          kernelGauss.exec(sh, cl.que);
          cl2blitz(iomCL(), in, cl.que);
          toRet = true;
        } catch (...) {}
        pthread_mutex_unlock(&locker);
        return toRet;
      }


    };
    std::list<ForCLdev*>  envs;
    mutable Relocker locker;

    FamilyValues() {}; // to allow empty _values

    FamilyValues( const clargs & st, const std::deque< Shape<2> > & ishs
                , const std::deque<Map> & bgas, const std::deque<Map> & dfas
                , const std::deque<Map> & dgas, const std::deque<Map> & msas
                , const Path & saveMasks = Path() )
      : ishs(whole(ishs))
    {

      // check inputs
      if (!st.nofIn)
        throw_error(ProcProj::modname, "No input images.");
      if (ishs.size() != st.nofIn)
        throw_error(ProcProj::modname, "Numbers of input shapes (" + toString(ishs.size()) + ") and images"
                                         " ("+ toString(st.nofIn) + ") are not equal.");
      for(int curI=0; curI < st.nofIn ; ++curI )
        if (!size(ishs[curI]))
          throw_error(ProcProj::modname, "Zero size size of input image " + toString(curI) + ".");

      auto chkNofIns = [&ishs](const deque<Map> & ims, const string & lbl) {
        if (ims.size() && ims.size() != 1 && ims.size() != ishs.size())
          throw_error( ProcProj::modname
                     , "Number of " +lbl+ " images (" + toString(ims.size()) + ") is neither"
                       " 0, 1, nor the number of inputs (" + toString(ishs.size()) + ").");
        for(int curI=0; curI < ims.size() ; ++curI ) {
          const Shape<2> & imsh = ims.at(curI).shape();
          if ( imsh != ishs.at(curI) )
            throw_error(ProcProj::modname, "Shape of " + lbl + " image " + toString(curI) +
                        " ("+toString(imsh)+") does not match that of the"
                        " corresponding input image ("+toString(ishs.at(curI))+").");
        }
      };
      chkNofIns(bgas, "background");
      chkNofIns(dfas, "darkfield");
      chkNofIns(dgas, "darkground");
      chkNofIns(msas, "mask");

      auto selectSrc = [](const auto & prm, const auto & dflt, int cur=0) -> const auto & {
        return prm.size() ? prm.at( prm.size() > 1 ? cur : 0 ) : dflt;
      };

      // prepare canonImProcs
      canonImProcs.emplace_back( ishs[0]
                               , selectSrc(bgas, defaultMap)
                               , selectSrc(dfas, defaultMap)
                               , selectSrc(dgas, defaultMap)
                               , selectSrc(msas, defaultMap)
                               , selectSrc(st.angles, 0.0f)
                               , selectSrc(st.crops, Crop<2>())
                               , selectSrc(st.binns, Binn<2>())
                               , 0 );
      if (  bgas.size() > 1 || dfas.size() > 1 || dgas.size() > 1 || msas.size() > 1
         || st.angles.size() > 1 || st.crops.size() > 1 || st.binns.size() > 1
         || adjacent_find( whole(ishs), [](auto &lsh, auto &rsh){return lsh != rsh;} ) != ishs.end() )
      {
        for ( int curI = 1 ; curI < st.nofIn ; curI++ )
          canonImProcs.emplace_back( ishs[curI]
                                   , selectSrc(bgas, defaultMap, curI)
                                   , selectSrc(dfas, defaultMap, curI)
                                   , selectSrc(dgas, defaultMap, curI)
                                   , selectSrc(msas, defaultMap, curI)
                                   , selectSrc(st.angles, 0.0f, curI)
                                   , selectSrc(st.crops, Crop<2>(), curI )
                                   , selectSrc(st.binns, Binn<2>(), curI )
                                   , 0 );
      } else {
        for ( int curI = 1 ; curI < st.nofIn ; curI++ )
          canonImProcs.emplace_back(canonImProcs[0]);
      }

      // prepare pshs, ssh and real origins
      PointI<2> lo, hi;
      for ( int curI = 0 ; curI < st.nofIn ; curI++ ) {
        pshs.push_back(canonImProcs[curI].shape());
        PointI<2> curLH = st.origins[curI];
        if (lo(0)>curLH(0)) lo(0) = curLH(0);
        if (lo(1)>curLH(1)) lo(1) = curLH(1);
        curLH += pshs.back();
        if (hi(0)<curLH(0)) hi(0) = curLH(0);
        if (hi(1)<curLH(1)) hi(1) = curLH(1);
      }
      ssh = hi-lo;
      std::transform( whole(st.origins), std::back_inserter(origins)
                    , [&lo](const PointI<2> & orgI) { return orgI - lo; } );

      // prepare processed masks
      deque<Map> pmasks;
      if (msas.size())
        for (int curI = 0; curI < canonImProcs.size() ; curI++) {
          const ImageProc & procI = canonImProcs[curI];
          const Map & maskI = selectSrc(msas, defaultMap, curI);
          Map zmask(ishs[curI]);
          zmask = maskI;
          prepareMask(zmask, false);
          Map zpmask(pshs[curI]);
          zpmask = procI.apply(zmask);
          prepareMask(zpmask, true, st.edge);
          pmasks.push_back(pshs[curI]);
          pmasks.back() = procI.apply(maskI) * zpmask ;
          if (saveMasks.length())
            SaveDenan(saveMasks.dtitle() + "_I" + toString(curI) + ".tif", pmasks.back());
        }

      if (st.nofIn == 1) { // no stitching
        if (msas.size())
          mskF.reference(pmasks[0]);
      } else { // prepare sum of weights and of masks images
        swght.resize(ssh);
        swght=0.0;
        if (msas.size()) {
          mskF.resize(ssh);
          mskF=0.0;
        }
        for (int curI = 0 ; curI < st.nofIn ; curI++ ) {
          const Shape<2> & psh = pshs[curI];
          for ( int curJ = 0 ; curJ<curI ; ++curJ)
            if (psh == pshs[curJ]) { // use existing array of the same size
              wghts.push_back(wghts[curJ]);
              break;
            }
          if (wghts.size()<curI+1) { // no earlier arrays of the same size found
            wghts.push_back(psh);
            wghts.back() = ( psh(0) - abs( 2*i0 - psh(0) + 1l ) ) * ( psh(1) - abs( 2*i1 - psh(1) + 1l ) );
          }
          Map & wghtI = wghts.back();
          const blitz::Range r0(origins[curI](0), origins[curI](0) + psh(0)-1)
                           , r1(origins[curI](1), origins[curI](1) + psh(1)-1);
          if (pmasks.size()) {
            Map & pmask = pmasks[curI];
            mskF(r0, r1) += pmask;
            wghtI *= pmask;
          }
          swght(r0, r1) += wghtI;
        }
        swght = invert(swght);
        if (saveMasks.length())
          SaveDenan(saveMasks.dtitle() + "_X.tif", mskF);
      }

      // prepare output crops
      for ( const Crop<2> & crp : st.outCrops )
        ocrps.push_back(crp);
      if (ocrps.empty())
        ocrps.push_back(Crop<2>());

      // save final mask
      if (msas.size()) {
        prepareMask(mskF, false);
        if (saveMasks.length()) {
          const int nofOuts = ocrps.size();
          const string svformat  =  nofOuts > 1  ?  mask2format("_Z@", nofOuts)  :  "" ;
          for (int curI = 0 ; curI < nofOuts ; ++curI )
            SaveDenan( saveMasks.dtitle() + toString(svformat, curI) + ".tif", ocrps[curI].apply(mskF) );
        }
      }

      // prepare gaps feeling and denoiser
      canonDenoiser = new Denoiser(ssh, st.denoiseRad, st.denoiseThr, mskF);
      doGapsFill = any(mskF==0.0);
      if (doGapsFill) {
        if (!CL_isReady())
          throw_error(modname, "Dysfunctional OpenCL required to close mask gaps.");
        for (CLenv & env : clenvs)
          try{ envs.push_back(new ForCLdev(env, mskF)); } catch(...) {}
      }
    }


    ~FamilyValues() {
      for (ForCLdev * env : envs)
        if (env)
          delete env;
      if (canonDenoiser)
        delete canonDenoiser;
    };



  };
  const FamilyValues  _values;
  const FamilyValues & values;

  // own
  std::deque<ImageProc> imProcs;
  std::optional<Denoiser> denoiser;
  std::deque<Map> allIn;
  Map stitched, final;
  std::deque<Map> res; // will all reference final

public:

  ProcProj( const clargs & st, const std::deque< Shape<2> > & ishs
          , const std::deque<Map> & bgas, const std::deque<Map> & dfas
          , const std::deque<Map> & dgas, const std::deque<Map> & msas
          , const Path & saveMasks = Path())
    : nofIn(st.nofIn)
    , _values(st, ishs, bgas, dfas, dgas, msas, saveMasks)
    , values(_values)
    , res(values.ocrps.size())
  {}


  ProcProj(const ProcProj & other)
    : nofIn(other.nofIn)
    , values(other.values)
    , res(values.ocrps.size())
  {}


  std::deque<Map> & process( std::deque<ReadVolumeBySlice> & allInR, uint prj) {

    if (allInR.size() != nofIn)
      throw_error(modname, "Incorrect number of read volumes: " + toString(allInR.size())
                           + " supplied, " + toString(nofIn) +  " required.");
    if (imProcs.empty()) { // first use
      for (const ImageProc & imProc : values.canonImProcs)
        imProcs.emplace_back(imProc);
      res.resize(values.ocrps.size());
      if (nofIn != 1) {
        stitched.resize(values.ssh);
        for ( int curI = 0 ; curI < values.ocrps.size() ; ++curI )
          res[curI].reference(values.ocrps[curI].apply(stitched));
      }
    }
    if (nofIn == 1) {
      stitched.reference(imProcs[0].read(allInR[0], prj));
      for ( int curI = 0 ; curI < values.ocrps.size() ; ++curI )
        res[curI].reference(values.ocrps[curI].apply(stitched));
    } else {
      stitched=0.0;
      for ( int curproj = 0 ; curproj < nofIn ; curproj++) {
        const Map incur = imProcs[curproj].read(allInR[curproj], prj);
        const PointI<2> & origin = values.origins[curproj];
        const Shape<2> & psh = values.pshs[curproj];
        stitched( blitz::Range(origin(0), origin(0) + psh(0)-1)
                , blitz::Range(origin(1), origin(1) + psh(1)-1) )
            += incur * values.wghts[curproj];
      }
      stitched *= values.swght;
    }
    if (!values.doGapsFill)
      return res;
    while (true) {
      for ( FamilyValues::ForCLdev * env : values.envs ) {
        const bool reced = env->process(stitched);
        values.locker.unlock();
        if (reced)
          return res;
      }
      values.locker.lock();
    }

  }


  std::vector<Shape<2>> shapes() const {
    std::vector<Shape<2>> toRet;
    std::transform( whole(values.ocrps), std::back_inserter(toRet)
                  , [this](const Crop<2> & crp) { return crp.shape(values.ssh) ; } );
    return toRet;
  }


  static std::vector<Shape<2>> shapes(const clargs & st, const std::deque< Shape<2> > & ishs) {
    std::deque<Map> nothing;
    return ProcProj(st, ishs, nothing, nothing, nothing, nothing).shapes();
  }


};


const string ProcProj::FamilyValues::ForCLdev::modname="CloseGapCL";
const string ProcProj::modname = "ProcProj";












class ProjInThread : public InThread {

  deque<ReadVolumeBySlice> & allInRd;
  deque<SaveVolumeBySlice> & allOutSv;
  const ProcProj & proc;
  const deque<int> & projes;
  const int zbinn;

  unordered_map<pthread_t, ProcProj*> procs;
  deque< pair< pthread_mutex_t, list<BinnProc::Accumulator> > > accs;

  bool inThread(long int idx) {

    const long int sodx =  idx/zbinn + ( zbinn > 0 ? 0 : allOutSv[0].slices() - 1 ) ;
    if (sodx < 0 || sodx >= allOutSv[0].slices() || idx >= allInRd[0].slices() )
      return false;
    if ( find(projes.begin(), projes.end(), sodx) == projes.end() )
      return true;

    const pthread_t me = pthread_self();
    if ( ! procs.count(me) ) { // first call
      ProcProj* eproc = new ProcProj(proc);
      lock();
      procs.emplace(me, eproc);
      unlock();
    }
    lock();
    ProcProj & myProc = *procs.at(me);
    unlock();

    try {
      deque<Map> & myRes = myProc.process(allInRd, idx);
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

  ProjInThread(deque<ReadVolumeBySlice> & allInRd, deque<SaveVolumeBySlice> & outSave
               , const ProcProj & proc, const deque<int> & projes, const int zbinn, bool verbose)
    : InThread(verbose, "processing projections")
    , proc(proc)
    , projes(projes)
    , allInRd(allInRd)
    , allOutSv(outSave)
    , zbinn(zbinn ? zbinn : allInRd.at(0).slices())
  {
    bar.setSteps(zbinn*projes.size());
    if (zbinn != 1)
      for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ )
        accs.emplace_back( pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER), list<BinnProc::Accumulator>() );
  }

  ~ProjInThread() {
    auto dlnl = [](auto pntr) { if (pntr) { delete pntr; pntr=0; } } ;
    for (auto celem : procs) dlnl(celem.second) ;
    for (auto & accPair : accs) pthread_mutex_destroy(&accPair.first);
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
  const int nofIn = args.images.size();
  const deque< Shape<2> > ishs ( [&args](){
    deque< Shape<2> > toRet;
    for (auto & imageCollection : args.images )
      toRet.push_back(ImageSizes(imageCollection.at(0).repr()));
    return toRet;
  }() );

  // Prepare read factories
  deque<ReadVolumeBySlice> allInRd(nofIn);
  for ( int curI = 0 ; curI < nofIn ; curI++) {
    allInRd.at(curI).add(args.images.at(curI));
    int cSls = allInRd.at(curI).slices();
    if (!cSls)
      exit_on_error(args.command, "No images in input "+ toString(curI) +".");
    if (curI && allInRd.at(0).slices() != cSls)
      exit_on_error(args.command, "Not matching slices in input "+ toString(curI) +".");
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
    vector<Shape<2>> oshs(ProcProj::shapes(args, ishs));
    if (oshs.empty())
      exit_on_error(args.command, "No output. Check your input and stitching parameters.");
    for(auto osh : oshs)
      cout << nofOuts << " " << osh(0) << " " << osh(1) << "\n";
    exit(0);
  }

  // Read auxiliary images
  auto rdAux = [&ishs](const deque<ImagePath> & comp) {
    deque<Map> toRet(comp.size());
    for ( int curf = 0 ; curf < comp.size() ; curf++) \
      ReadImage(comp[curf].repr(), toRet[curf], ishs[curf]);
    return toRet;
  };
  deque<Map> bgas(std::move(rdAux(args.bgs)));
  deque<Map> dfas(std::move(rdAux(args.dfs)));
  deque<Map> dgas(std::move(rdAux(args.dgs)));
  deque<Map> msas(std::move(rdAux(args.mss)));
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

  /*
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
  */

  // Prepare canonical projection processor
  const string testFormat = [&args,&nofProj](){
    if (args.testMe < 0)
      return string();
    string toRet = args.out_name.dtitle();
    if (toRet.find('@') != string::npos)
      toRet = toString(mask2format(toRet, nofProj), args.testMe);
    toRet += "%s";
    return toRet;
  }();
  ProcProj canonPP( args, ishs, bgas, dfas, dgas, msas, toString(testFormat, "_mask.tif"));

  // Prepare saving factories
  const std::vector<Shape<2>> outShapes = canonPP.shapes();
  const size_t nofSplts = outShapes.size();
  const string spformat = mask2format("_part@", nofSplts);
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
    const int trd = args.zbinn * ( args.testMe >= 0  ?  args.testMe  :  projes[0] );
    const deque<Map> & allOut = canonPP.process(allInRd, trd);
    for (int curSplt = 0 ; curSplt < nofSplts ; curSplt++) {
      const Map & cOut = allOut[curSplt];
      const int svIdx = args.zbinn ? args.testMe / args.zbinn : 0;
      ImagePath svPath = allOutSv[curSplt].save(svIdx, cOut);
      if (args.beverbose)
        cout << nofOuts << " " << cOut.shape()(0) << " " << cOut.shape()(1) << " " << svPath << "\n";
    }
  } else // finally process
    ProjInThread(allInRd, allOutSv, canonPP, projes, args.zbinn, args.beverbose)
        .execute();

} exit(0); }

