
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


ProcProj::ProcProj( const StitchRules & _st, const Shape<2> & _ish
                  , const deque<Map> & bgas, const deque<Map> & dfas
                  , const deque<Map> & dgas, const deque<Map> & msas
                  , const Path & saveMasks)
  : strl(_st)
  , ish(_ish)
  , psh(ImageProc::outShape(strl.angle, strl.crp, strl.bnn, ish))
  , ssh( psh(0) + strl.flip*abs(strl.originF(0))
         + (strl.origin1size-1)*abs(strl.origin1(0)) + (strl.origin2size-1)*abs(strl.origin2(0))
       , psh(1) + strl.flip*abs(strl.originF(1))
         + (strl.origin1size-1)*abs(strl.origin1(1)) + (strl.origin2size-1)*abs(strl.origin2(1)) )
  , oshs( [&](){
      vector<Shape<2>> toRet;
      const Shape<2> cssh = strl.fcrp.apply(ssh);
      if ( strl.splits.empty() )
        toRet.push_back(cssh);
      else {
        int fLine=0, lLine=0;
        const int vsplit = strl.splits.at(0) ? 0 : 1;
        const int mLine = cssh(vsplit);
        for (int curS = vsplit ;  curS<=strl.splits.size()  ;  curS++) {
          lLine = ( curS == strl.splits.size()  ||  mLine < strl.splits.at(curS) )
                  ?  mLine :  strl.splits.at(curS) ;
          int sz = lLine-fLine;
          if ( sz > 0 )
            toRet.push_back(vsplit ? Shape<2>(cssh(0),sz) : Shape<2>(sz, cssh(1)));
          fLine=lLine;
        }
      }
      return toRet;
    }()   )
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
    iproc.proc(msas[curI], wghtI);
    zmask = msas[curI];
    prepareMask(zmask, false);
    iproc.proc(zmask, zpmask);
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
    iproc.proc(allInR[curproj], allIn[curproj]);
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
      SaveDenan(svName, allIn[curproj]);
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
  const Shape<2> fssh(strl.fcrp.apply(ssh));
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



/*


Trans::Trans(const Shape & _ish,
             float _angle,
             const Crop & _crop,
             const PointF<2> & _binn,
             const Map & _mask)
  : ish(_ish)
  , angle( [](float a, const Shape & s){
             a = remainder(a, 2*M_PIf);
             if (!area(s))
               return a;
             const float a90 = abs(remainder(a, M_PI_2f));
             if ( max(s(0),s(1)) * min(sin(a90),cos(a90)) >= 1.0 ) // far from 90-deg step;
               return a;
             const int nof90 = round(a*M_2_PIf);
             if ( ! (nof90 % 4)  ) // 0deg
               return 0.0f;
             else if ( ! (nof90 % 2) ) //180deg
               return M_PIf;
             else if (  ( nof90 > 0 && (nof90 % 3) ) || ( nof90 < 0 && ! (nof90 % 3) ) )  // 270deg
               return -M_PI_2f;
             else
               return M_PI_2f;
           } (_angle, _ish) )
  , crop(_crop)
  , binn(_binn)
  , mask(_mask.copy())
{
  if (!area(ish))
    return;
  if (mask.size() && mask.shape() != ish)
    throw_error(modname, "Shape missmatch of process ("+toString(ish)+") and mask ("+toString(mask.shape())+").");
  prepareMask(mask, true);
  rotate(mask, afterRotMask);
  //if (afterRotMask.dataFirst() != mask.dataFirst())
  //  prepareMask(afterRotMask, true);
  return;
}



void Trans::scale(const Map & in, Map & out) {
  if (!in.size() || !area(ish))
    return;
  if (binn == PointF<2>(1,1)) {
    out.reference(in);
    return;
  }
  Map rin(in);
  PointF<2> rbinn(abs(binn.x),abs(binn.y));
  if (binn.x<0)
    rin.reverseSelf(blitz::secondDim);
  if (binn.y<0)
    rin.reverseSelf(blitz::firstDim);
}




void Trans::rotate(const Map & in, Map & out) {
  if (!in.size() || !area(ish))
    return;
  if (angle == 0.0f)
    out.reference(in);
  else if (angle == M_PI_2f) // 90 deg
    out.reference(in.transpose(blitz::firstDim, blitz::secondDim).reverse(blitz::firstDim));
  else if (angle == -M_PI_2f) // -90 (270) deg
    out.reference(in.transpose(blitz::firstDim, blitz::secondDim).reverse(blitz::secondDim));
  else if (angle == M_PIf) // 180 deg
    out.reference(in.reverse(blitz::firstDim).reverse(blitz::secondDim));
  else {
    const float cosa = cos(-angle),
                sina = sin(-angle);
    const Shape rsh( floor(abs(ish(1)*sina)+abs(ish(0)*cosa)),
                     floor(abs(ish(1)*cosa)+abs(ish(0)*sina)));
    out.resize(rsh);
    const float constinx = ( ish(1) + rsh(0)*sina - rsh(1)*cosa ) / 2.0,
                constiny = ( ish(0) - rsh(1)*sina - rsh(0)*cosa ) / 2.0;
    for ( ArrIndex y=0 ; y < rsh(0) ; y++) {
      for ( ArrIndex x=0 ; x < rsh(1) ; x++) {
        const float fx = x*cosa - y*sina + constinx;
        const float fy = x*sina + y*cosa + constiny;
        const ArrIndex flx = floor(fx);
        const ArrIndex fly = floor(fy);
        float sum=0.0;
        float wgt=0.0;
        for (ArrIndex  fxx : {flx,flx+1}) {
          const float dx = fxx - fx;
          for (ArrIndex  fyy : {fly,fly+1}) {
            if ( fxx >= 0 && fxx < ish(1) && fyy >= 0 && fyy < ish(0)
                 && ( ! mask.size() || mask(fyy,fxx) > 0.0 )   ) {
              const float dy = fyy - fy;
              const float mywgt = 1 - sqrt(dx*dx+dy*dy);
              if (mywgt>0) {
                wgt += mywgt;
                sum += mywgt*in(fyy,fxx);
              }
            }
          }
        }
        out(y, x) = wgt > 0.0 ? sum/wgt  : 0.0;
        //if ( flx < 0 || flx >= ish(1)-1 || fly < 0  || fly >= ish(0)-1 ) {
        //  out(y, x)=0;
        //} else {
        //  const float v0 = in(fly  ,flx) + ( in(fly  ,flx+1) - in(fly  ,flx) ) * (fx-flx);
        //  const float v1 = in(fly+1,flx) + ( in(fly+1,flx+1) - in(fly+1,flx) ) * (fy-fly);
        //  out(y, x) = v0 + (v1-v0) * (fy-fly);
        //}
      }
    }
  }
}




*/





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




