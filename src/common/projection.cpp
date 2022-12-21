
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


Shape StitchRules::outShape() const {
  const Shape psh(ImageProc::outShape(angle, crp, bnn, ish));
  return Shape( psh(0) + flip*abs(originF.y)
                + (origin1size-1)*abs(origin1.y) + (origin2size-1)*abs(origin2.y)
              , psh(1) + flip*abs(originF.x)
                + (origin1size-1)*abs(origin1.x) + (origin2size-1)*abs(origin2.x) );
}





const string ProcProj::modname="ProcProj";
cl_program ProcProj::oclProgram = 0;


void ProcProj::prepareMask(Map & _gaps, bool bepicky, uint edge) {
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
  const float step = 1.0 / (edge +1);
  Map tmp(_gaps.shape());
  tmp = _gaps;

  for ( int stp = 1 ; stp <= edge ; stp++ ) {
    const float fill = step*stp;
    //const float usq = stp * step - 1 ;
    //const float fill = sqrt(1.0 - usq*usq);

    for (ArrIndex i = 0 ; i<ish(0) ; i++)
      for (ArrIndex j = 0 ; j<ish(1) ; j++)

        if ( _gaps(i,j) != 1.0 )
          for (ArrIndex ii = i-1 ; ii <= i+1 ; ii++)
            for (ArrIndex jj = j-1 ; jj <= j+1 ; jj++)

              if ( ii >= 0 && ii < ish(0) && jj >= 0 && jj < ish(1)
                   && _gaps(ii,jj) == 1.0 )
                tmp(ii,jj) = fill;

    _gaps = tmp;
  }

}


void ProcProj::initCL() {

  if (!doGapsFill)
    return;

  static const string oclsrc = {
    #include "projection.cl.includeme"
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


ProcProj::ProcProj( const StitchRules & _st
                  , const deque<Map> & bgas, const deque<Map> & dfas
                  , const deque<Map> & dgas, const deque<Map> & msas
                  , const Path & saveMasks)
  : strl(_st)
  , psh(ImageProc::outShape(strl.angle, strl.crp, strl.bnn, strl.ish))
  , iproc(strl.angle, strl.crp, strl.bnn, strl.ish)
  , allIn(strl.nofIn)
  , doGapsFill(false)
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

  // prepare processed masks
  wghts.resize(msas.size(), psh);
  Map zmask(strl.ish), zpmask(psh);
  for (int curI = 0; curI < msas.size() ; curI++) {
    Map & wghtI = wghts[curI];
    iproc.proc(msas[curI], wghtI);
    zmask = msas[curI];
    prepareMask(zmask, false, 0);
    iproc.proc(zmask, zpmask);
    prepareMask(zpmask, true, strl.edge);
    wghtI *= zpmask;
    if (saveMasks.length())
      SaveDenan(saveMasks.dtitle() + "_I" +
                (msas.size()>1 ? toString(curI) : string()) + ".tif", wghtI);
  }

  if (strl.nofIn == 1) { // no stitching
    ssh = psh;
    if (msas.size())
      mskF.reference(wghts[0]);
  } else {

    // prepare origins for each input image
    int maxx(0), maxy(0), minx(0), miny(0);
    const int maxXshift = abs(strl.origin1.x * (strl.origin1size-1)) + abs(strl.origin2.x * (strl.origin2size-1));
    for (int curI = 0; curI < strl.nofIn ; curI++) {
      int cur1, cur2, curF;
      strl.slot(curI, &cur1, &cur2, &curF);
      PointF2D curP;
      curP.x = strl.origin1.x * cur1 + strl.origin2.x * cur2;
      curP.y = strl.origin1.y * cur1 + strl.origin2.y * cur2;
      if (curF) {
        curP.x = strl.originF.x + maxXshift - curP.x - 1;
        curP.y += strl.originF.y;
      }
      origins.push_back(curP);
      const float
          orgx = curP.x,
          orgy = curP.y,
          tilx = orgx + psh(1)-1,
          tily = orgy + psh(0)-1;
      if (orgx < minx) minx = orgx;
      if (tilx > maxx) maxx = tilx;
      if (orgy < miny) miny = orgy;
      if (tily > maxy) maxy = tily;
    }
    for (int curI = 0; curI < strl.nofIn ; curI++)
      origins[curI] -= PointF2D(minx, miny);
    ssh = Shape(maxy-miny+1, maxx-minx+1);

    // prepare weights image
    Map wght(psh);
    for (ArrIndex ycur = 0 ; ycur < psh(0) ; ycur++ )
      for (ArrIndex xcur = 0 ; xcur < psh(1) ; xcur++ )
        wght(ycur, xcur) =   ( psh(0) - abs( 2*ycur - psh(0) + 1l ) )
                           * ( psh(1) - abs( 2*xcur - psh(1) + 1l ) );

    // process masks
    if (!msas.size())
      wghts.emplace_back(wght);
    else
      for (int curI = 0; curI < msas.size() ; curI++)
        wghts[curI] *= wght;

    // prepare sum of weights image
    swght.resize(ssh);
    swght=0.0;
    for (int acur = 0 ; acur < strl.nofIn ; acur++ ) {
      Map wghtI(wghts[ wghts.size()==1 ? 0 : acur ]);
      swght( blitz::Range(origins[acur].y, origins[acur].y + psh(0)-1)
           , blitz::Range(origins[acur].x, origins[acur].x + psh(1)-1))
        +=  ! strl.flip || acur < strl.nofIn/2
            ?   wghtI  :  wghtI.reverse(blitz::secondDim);
    }
    swght = invert(swght);

    // prepare final mask
    if (msas.size()) {
      mskF.resize(ssh);
      mskF = invert(swght);
    }

  }

  // save final mask
  if (msas.size()) {
    prepareMask(mskF, false,0);
    if (saveMasks.length())
      SaveDenan(saveMasks.dtitle() + ".tif", mskF);
  }

  // prepare final
  if (strl.fcrp)
    final.resize(crop(ssh, strl.fcrp));

  doGapsFill = strl.sigma > 0.0  &&  any(mskF==0.0);
  initCL();

}




ProcProj::ProcProj(const ProcProj & other)
  : strl(other.strl)
  , psh(other.psh)
  , ssh(other.ssh)
  , wghts(other.wghts)
  , swght(other.swght)
  , origins(other.origins)
  , mskF(other.mskF)
  , maskCL(other.maskCL)
  , ffprocs(other.ffprocs)
  , iproc(other.iproc)
  , allIn(other.allIn.size())
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
      const bool flipMe = strl.flip && acur >= strl.nofIn/2;
      Map incur;
      if (interim_name.empty())
        incur.reference(allIn[acur]);
      else {
        incur.resize(psh);
        incur=allIn[acur];
      }
      incur *= wghts[wghts.size()==1 ? 0 : acur];
      stitched( blitz::Range(origins[acur].y, origins[acur].y + psh(0)-1)
              , blitz::Range(origins[acur].x, origins[acur].x + psh(1)-1))
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
      const blitz::Range r0(origins[acur].y, origins[acur].y + psh(0)-1);
      const blitz::Range r1(origins[acur].x, origins[acur].x + psh(1)-1);
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



bool ProcProj::process(deque<Map> & allInR, deque<Map> & res, const ImagePath & interim_name) {

  if (allInR.size() != strl.nofIn)
    return false;

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
    blitz2cl(stitched, iomCL());
    gaussCL.exec(stitched.shape());
    cl2blitz(iomCL(), stitched);
  }

  // final crop
  if (strl.fcrp)
    crop(stitched, final, strl.fcrp);
  else
    final.reference(stitched);

  // splits
  if ( strl.splits.empty() ) {
    res.resize(1);
    res[0].resize(final.shape());
    res[0]=final;
  } else {
    if (! interim_name.empty() && (doGapsFill || strl.fcrp) )
      SaveDenan( interim_name.dtitle() + "_Y.tif", final);
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






Denoiser::Denoiser(const Shape & _sh, int _rad, float _threshold, Map & _mask)
  : sh(_sh)
  , rad(abs(_rad))
  , thr(_threshold)
{
  if ( !area(sh) || !rad )
    return;
  if ( _mask.size() && _mask.shape() != sh)
    throw_error("denoiser", "Mask shape ("+toString(_mask.shape())+") does not match"
                            " that of the denoiser("+toString(sh)+").");

  tarr.resize(sh);
  swghts.resize(sh);
  swghts = 0.0;
  if (_mask.size()) {
    mask.resize(_mask.shape());
    mask = _mask;
  }
  int cntr = 0;
  const uint rad2 = rad*rad;
  for (int ii = -rad ; ii <= rad ; ii++) {
    const blitz::Range srcR0( max(0, -ii), sh(0) - 1 + min(0, -ii) );
    const blitz::Range resR0( max(0,  ii), sh(0) - 1 + min(0,  ii) );
    for (int jj = -rad ; jj <= rad ; jj++) {
      if ( ii*ii + jj*jj <= rad2 ) {
        const blitz::Range srcR1( max(0, -jj), sh(1) - 1 + min(0, -jj) );
        const blitz::Range resR1( max(0,  jj), sh(1) - 1 + min(0,  jj) );
        if (mask.size())
          swghts(resR0, resR1) += mask (srcR0, srcR1);
        else
          swghts(resR0, resR1) += 1.0;
        cntr++;
      }
    }
  }
  if (_mask.size())
    _mask = swghts/cntr;
  swghts = invert(swghts);
}


void Denoiser::proc(Map & iom) const {
  if ( !area(sh) || !rad )
    return;
  if ( iom.shape() != sh )
    throw_error("denoiser", "Non matching shape of input array.");
  if ( mask.size() &&  mask.shape() != sh )
    throw_error("denoiser", "Non matching shape of the mask.");

  static const uint rad2 = rad*rad;
  tarr=0.0;
  if (mask.size())
    iom *= mask;
  for (int ii = -rad ; ii <= rad ; ii++) {
    const blitz::Range srcR0( max(0, -ii), sh(0) - 1 + min(0, -ii) );
    const blitz::Range resR0( max(0,  ii), sh(0) - 1 + min(0,  ii) );
    for (int jj = -rad ; jj <= rad ; jj++) {
      if ( ii*ii + jj*jj <= rad2 ) {
        const blitz::Range srcR1( max(0, -jj), sh(1) - 1 + min(0, -jj) );
        const blitz::Range resR1( max(0,  jj), sh(1) - 1 + min(0,  jj) );
          tarr(resR0, resR1) += iom(srcR0, srcR1);
      }
    }
  }
  tarr *= swghts;

  if (thr == 0.0) {
    iom = tarr;
  } else if (thr<0) {
    for (ArrIndex ycur = 0 ; ycur < sh(0) ; ycur++ )
      for (ArrIndex xcur = 0 ; xcur < sh(1) ; xcur++ ) {
        float & tval = tarr(ycur,xcur);
        float & ival = iom(ycur,xcur);
        if (ival == 0.0 || ( tval != 0.0  &&  abs((ival-tval)/tval) > -thr) )
          ival = tval;
      }
  } else {
    for (ArrIndex ycur = 0 ; ycur < sh(0) ; ycur++ )
      for (ArrIndex xcur = 0 ; xcur < sh(1) ; xcur++ ) {
        float & tval = tarr(ycur,xcur);
        float & ival = iom(ycur,xcur);
        if (ival == 0.0 ||  abs(ival-tval) > thr)
          ival = tval;
      }
  }

}





//! Applies a zingers filter of a specified size (must be odd, by default 9) and threshold (by default 1.2)
//  Processing is carried out for each individual detector row (if bTransposed == false) or column (if bTransposed == true)
//  To filter sinograms, apply exp(-) before calling this function, and -log() after
Map ZingersFilt(size_t nFiltSize, double dblThreshold, Map arr)
{
  const Shape sh(arr.shape());
  const size_t sz(arr.size());
  ArrIndex nAngles, nPixels;
  ArrIndex nCurAngle, nCurPixel;
  long box, i, pixel;

  if(nFiltSize%2 != 0) nFiltSize++; //FiltSize must be odd (so it can be centred on a pixel)
  box = (long) (nFiltSize--)/2;
  nAngles = sh(0);
  nPixels = sh(1);

  // allocate the temp arrays
  Line arrAvg(nPixels + 4);
  Line arrFilter(nPixels + 4);

  // sequentially process individual rows/columns in arrAvg
  for(nCurAngle = 0; nCurAngle < nAngles; nCurAngle++)
  {
    // select one row/column
    for(nCurPixel = 0; nCurPixel < nPixels; nCurPixel++)
      arrAvg(nCurPixel + 2) = arr(nCurAngle,nCurPixel);

    arrAvg(0) = arrAvg(3);
    arrAvg(1) = arrAvg(2);
    arrAvg(nPixels + 2) = arrAvg(nPixels + 1);
    arrAvg(nPixels + 3) = arrAvg(nPixels);

    // smooth the row/column
    for(nCurPixel = 0; nCurPixel < nPixels; nCurPixel++)
    {
      double sum = 0;
      ArrIndex count = 0;
      for(i = -box; i <= box; i++)
      {
        pixel = long(nCurPixel) + i;
        if(pixel >= 0 && pixel < nPixels) { sum += arrAvg(pixel + 2); count++; }
      }
      arrFilter(nCurPixel + 2) = (sum / count);
    }

    // divide original row/column by the smoothed row/column and save in arrFilter
    for(nCurPixel = 2; nCurPixel < nPixels + 2; nCurPixel++)
      arrFilter(nCurPixel) = arrAvg(nCurPixel) / arrFilter(nCurPixel);

    arrFilter(0) = arrFilter(3);
    arrFilter(1) = arrFilter(2);
    arrFilter(nPixels + 2) = arrFilter(nPixels + 1);
    arrFilter(nPixels + 3) = arrFilter(nPixels);

    // threshold the arrFilter values
    for(nCurPixel = 0; nCurPixel < nPixels; nCurPixel++)
    {
      if (arrFilter(nCurPixel + 2) >= dblThreshold)
      {
        double sum = 0;
        ArrIndex count = 0;
        if (arrFilter(nCurPixel) < dblThreshold) { sum += arrAvg(nCurPixel); count++; }
        if (arrFilter(nCurPixel + 1) < dblThreshold) { sum += arrAvg(nCurPixel + 1); count++; }
        if (arrFilter(nCurPixel + 3) < dblThreshold) { sum += arrAvg(nCurPixel + 3); count++; }
        if (arrFilter(nCurPixel + 4) < dblThreshold) { sum += arrAvg(nCurPixel + 4); count++; }
        if (count > 0) arrAvg(nCurPixel + 2) = (sum / count);
      }
    }

    // overwrite the original row/column with the filtered one
    for(nCurPixel = 0; nCurPixel < nPixels; nCurPixel++)
      arr(nCurAngle,nCurPixel) = arrAvg(nCurPixel + 2);
  }
  return arr;
}
