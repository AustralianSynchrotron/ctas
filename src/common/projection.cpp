
#include "projection.h"
//#include "../common/poptmx.h"
//#include <algorithm>
//#include <string.h>
//#include <unordered_map>
//#include <deque>
//#include <unistd.h>


using namespace std;


namespace blitz {

static inline float denan(float x){ return isnormal(x) ? x : 0.0 ;}
BZ_DECLARE_FUNCTION(denan);

static inline float invert(float x){ return x==0.0 ? 0.0 : 1.0/x ;}
BZ_DECLARE_FUNCTION(invert);

static inline float subzero(float x){ return x < 0.0 ? 0.0 : x ;}
BZ_DECLARE_FUNCTION(subzero);


static inline float thrshld(float v, float av){ return v-av; }
BZ_DECLARE_FUNCTION2(thrshld);

}

void SaveDenan(const ImagePath & filename, const Map & storage, bool saveint=false) {
  Map outm(storage.shape());
  outm=denan(storage);
  SaveImage(filename, outm, saveint);
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


class N_ProcProj::FamilyValues::ForCLdev {
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
      blitz2cl<cl_float>(in, iomCL());
      kernelGauss.exec(sh);
      cl2blitz(iomCL(), in);
      toRet = true;
    } catch (...) {}
    pthread_mutex_unlock(&locker);
    return toRet;
  }


};
const string N_ProcProj::FamilyValues::ForCLdev::modname="CloseGapCL";


N_ProcProj::FamilyValues::FamilyValues( const N_StitchRules & st, const deque< Shape<2> > & ishs
                                      , const deque<Map> & bgas, const deque<Map> & dfas
                                      , const deque<Map> & dgas, const deque<Map> & msas
                                      , const Path & saveMasks )

  : ishs(whole(ishs))
{

  // check inputs
  if (!st.nofIn)
    throw_error(N_ProcProj::modname, "No input images.");
  if (ishs.size() != st.nofIn)
    throw_error(N_ProcProj::modname, "Numbers of input shapes (" + toString(ishs.size()) + ") and images"
                                     " ("+ toString(st.nofIn) + ") are not equal.");
  for(int curI=0; curI < st.nofIn ; ++curI )
    if (!size(ishs[curI]))
      throw_error(N_ProcProj::modname, "Zero size size of input image " + toString(curI) + ".");

  auto chkNofIns = [&ishs](const deque<Map> & ims, const string & lbl) {
    if (ims.size() && ims.size() != 1 && ims.size() != ishs.size())
      throw_error( N_ProcProj::modname
                 , "Number of " +lbl+ " images (" + toString(ims.size()) + ") is neither"
                   " 0, 1, nor the number of inputs (" + toString(ishs.size()) + ").");
    for(int curI=0; curI < ims.size() ; ++curI ) {
      const Shape<2> & imsh = ims.at(curI).shape();
      if ( imsh != ishs.at(curI) )
        throw_error(N_ProcProj::modname, "Shape of " + lbl + " image " + toString(curI) +
                    " ("+toString(imsh)+") does not match that of the"
                    " corresponding input image ("+toString(ishs.at(curI))+").");
    }
  };
  chkNofIns(bgas, "background");
  chkNofIns(dfas, "darkfield");
  chkNofIns(dgas, "darkground");
  chkNofIns(msas, "mask");

  // prepare canonImProcs
  auto selectSrc = [](const auto & prm, const auto & dflt, int cur=0) -> const auto & {
    return prm.size() ? prm.at( prm.size() > 1 ? cur : 0 ) : dflt;
  };
  canonImProcs.emplace_back( selectSrc(bgas, defaultMap)
                           , selectSrc(dfas, defaultMap)
                           , selectSrc(dgas, defaultMap)
                           , selectSrc(msas, defaultMap)
                           , selectSrc(st.angles, 0.0f)
                           , selectSrc(st.crops, Crop<2>())
                           , selectSrc(st.binns, Binn<2>())
                           , ishs[0], 0 );
  if (  bgas.size() > 1 || dfas.size() > 1 || dgas.size() > 1 || msas.size() > 1
     || st.angles.size() > 1 || st.crops.size() > 1 || st.binns.size() > 1
     || adjacent_find( whole(ishs), [](auto &lsh, auto &rsh){return lsh != rsh;} ) != ishs.end() )
  {
    for ( int curI = 1 ; curI < st.nofIn ; curI++ )
      canonImProcs.emplace_back( selectSrc(bgas, defaultMap, curI)
                               , selectSrc(dfas, defaultMap, curI)
                               , selectSrc(dgas, defaultMap, curI)
                               , selectSrc(msas, defaultMap, curI)
                               , selectSrc(st.angles, 0.0f, curI)
                               , selectSrc(st.crops, Crop<2>(), curI )
                               , selectSrc(st.binns, Binn<2>(), curI )
                               , ishs[curI], 0 );
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
    if (lo(1)<curLH(1)) hi(1) = curLH(1);
  }
  ssh = hi-lo;
  std::transform( whole(st.origins), std::back_inserter(origins)
                , [&lo](const PointI<2> & orgI) { return orgI - lo; } );

  // prepare processed masks
  deque<Map> pmasks;
  for (int curI = 0; curI < msas.size() ; curI++) {
    const ImageProc & procI = canonImProcs[curI];
    const Map & maskI = msas[curI];
    Map zmask(ishs[curI]);
    zmask = maskI;
    prepareMask(zmask, false);
    Map zpmask(pshs[curI]);
    zpmask = procI.apply(zmask);
    prepareMask(zpmask, true, st.edge);
    pmasks.push_back(pshs[curI]);
    pmasks.back() = procI.apply(maskI) * zpmask ;
    if (saveMasks.length())
      SaveDenan(saveMasks.dtitle() + "_I" +
                (msas.size()>1 ? toString(curI) : string()) + ".tif", pmasks.back());
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
      if (msas.size()) {
        Map & pmask = pmasks[ msas.size()==1 ? 0 : curI ];
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

  // prepare gaps feeling
  doGapsFill = any(mskF==0.0);
  if (doGapsFill) {
    if (!CL_isReady())
      throw_error(modname, "Dysfunctional OpenCL required to close mask gaps.");
    for (CLenv & env : clenvs)
      try{ envs.push_back(new ForCLdev(env, mskF)); } catch(...) {}
  }

}


N_ProcProj::FamilyValues::~FamilyValues() {
  for (ForCLdev * env : envs)
    if (env)
      delete env;
};



const string N_ProcProj::modname = "ProcProj";

N_ProcProj::N_ProcProj( const N_StitchRules & st, const deque< Shape<2> > & ishs
                      , const deque<Map> & bgas, const deque<Map> & dfas
                      , const deque<Map> & dgas, const deque<Map> & msas
                      , const Path & saveMasks)
  : st(st)
  , _values(st, ishs, bgas, dfas, dgas, msas)
  , values(_values)
  , res(values.ocrps.size())
{}


N_ProcProj::N_ProcProj(const N_ProcProj & other)
  : st(other.st)
  , values(other.values)
  , res(values.ocrps.size())
{}

vector<Shape<2>> N_ProcProj::shapes() const {
  std::vector<Shape<2>> toRet;
  std::transform( whole(values.ocrps), std::back_inserter(toRet)
                , [this](const Crop<2> & crp) { return crp.shape(values.ssh) ; } );
  return toRet;
}


deque<Map> & N_ProcProj::process(std::deque<ReadVolumeBySlice> & allInR, uint prj) {
  if (allInR.size() != st.nofIn)
    throw_error(modname, "Incorrect number of read volumes: " + toString(allInR.size())
                         + " supplied, " + toString(st.nofIn) +  " required.");
  if (imProcs.empty()) { // first use
    for (const ImageProc & imProc : values.canonImProcs)
      imProcs.emplace_back(imProc);
    res.resize(values.ocrps.size());
    if (st.nofIn != 1) {
      stitched.resize(values.ssh);
      for ( int curI = 0 ; curI < values.ocrps.size() ; ++curI )
        res[curI].reference(values.ocrps[curI].apply(stitched));
    }
  }
  if (st.nofIn == 1) {
    stitched.reference(imProcs[0].read(allInR[0], prj));
    for ( int curI = 0 ; curI < values.ocrps.size() ; ++curI )
      res[curI].reference(values.ocrps[curI].apply(stitched));
  } else {
    stitched=0.0;
    for ( int curproj = 0 ; curproj < st.nofIn ; curproj++) {
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



const string ProcProj::modname="ProcProj";


void ProcProj::initCL() {

  if (!doGapsFill)
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
  , doGapsFill(false)
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
    const int maxXshift = abs(strl.origin1(1) * (strl.origin1size-1)) + abs(strl.origin2(1) * (strl.origin2size-1));
    for (int curI = 0; curI < strl.nofIn ; curI++) {
      int cur1, cur2, curF;
      strl.slot(curI, &cur1, &cur2, &curF);
      PointF<2> curP( strl.origin1(0) * cur1 + strl.origin2(0) * cur2
                    , strl.origin1(1) * cur1 + strl.origin2(1) * cur2);
      if (curF) {
        curP(1) = strl.originF(1) + maxXshift - curP(1);
        curP(0) += strl.originF(0);
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
    for (int curI = 0; curI < strl.nofIn ; curI++)
      origins[curI] -= PointF<2>(miny, minx);
    if ( ssh != Shape<2>(maxy-miny+1, maxx-minx+1) )
      throw_bug(modname + " Mistake in calculating stitched shape: "
                +toString(ssh)+" != "+toString(ssh)+".");

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
        SaveDenan(saveMasks.dtitle() + ".tif", mskF);
      else {
        if (doGapsFill || strl.fcrp)
          SaveDenan(saveMasks.dtitle() + "_Y.tif", mskF);
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

  doGapsFill = any(mskF==0.0);
  initCL();

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
  , doGapsFill(other.doGapsFill)
{
  if (doGapsFill)
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
  if (doGapsFill) {
    blitz2cl<cl_float>(stitched, iomCL());
    gaussCL.exec(stitched.shape());
    cl2blitz(iomCL(), stitched);
  }

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
    if (! interim_name.empty() && (doGapsFill || strl.fcrp) )
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






Denoiser::Denoiser(const Shape<2> & _sh, int _rad, float _threshold, const Map & _mask)
  : sh(_sh)
  , rad(abs(_rad))
  , thr(_threshold)
  , mask(_mask)
{
  if ( !size(sh) || !rad )
    return;
  const auto mssz=_mask.size();
  if ( mssz && _mask.shape() != sh)
    throw_error("denoiser", "Mask shape ("+toString(_mask.shape())+") does not match"
                            " that of the denoiser("+toString(sh)+").");

  tarr.resize(sh);
  swghts.resize(sh);
  swghts = 0.0;
  int cntr = 0;
  const uint rad2 = rad*rad;
  for (int ii = -rad ; ii <= rad ; ii++)
    for (int jj = -rad ; jj <= rad ; jj++)
      if ( ii*ii + jj*jj <= rad2 ) {
        if (mssz)
          swghts(dstRa(sh, ii, jj)) += mask (srcRa(sh, ii, jj));
        else
          swghts(dstRa(sh, ii, jj)) += 1.0;
        cntr++;
      }
  //if ( mssz  &&  _mask.shape() == pmask.shape() )
  //  pmask = swghts/cntr;
  swghts = invert(swghts);
}


void Denoiser::proc(Map & iom) const {
  if ( !size(sh) || !rad )
    return;
  if ( iom.shape() != sh )
    throw_error("denoiser", "Non matching shape of input array.");
  if ( mask.size() &&  mask.shape() != sh )
    throw_error("denoiser", "Non matching shape of the mask.");

  static const uint rad2 = rad*rad;
  tarr=0.0;
  if (mask.size())
    iom *= mask;
  for (int ii = -rad ; ii <= rad ; ii++)
    for (int jj = -rad ; jj <= rad ; jj++)
      if ( ii*ii + jj*jj <= rad2 )
        tarr(dstRa(sh, ii, jj)) += iom (srcRa(sh, ii, jj));
  tarr *= swghts;

  if (thr == 0.0) {
    iom = tarr;
  } else if (thr<0) {
    for (ssize_t ycur = 0 ; ycur < sh(0) ; ycur++ )
      for (ssize_t xcur = 0 ; xcur < sh(1) ; xcur++ ) {
        float & tval = tarr(ycur,xcur);
        float & ival = iom(ycur,xcur);
        if (ival == 0.0 || ( tval != 0.0  &&  abs((ival-tval)/tval) > -thr) )
          ival = tval;
      }
  } else {
    for (ssize_t ycur = 0 ; ycur < sh(0) ; ycur++ )
      for (ssize_t xcur = 0 ; xcur < sh(1) ; xcur++ ) {
        float & tval = tarr(ycur,xcur);
        float & ival = iom(ycur,xcur);
        if (ival == 0.0 ||  abs(ival-tval) > thr)
          ival = tval;
      }
  }

}




