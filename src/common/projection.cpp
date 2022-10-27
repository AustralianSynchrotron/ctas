
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

}

void SaveDenan(const ImagePath & filename, const Map & storage, bool saveint=false) {
  Map outm(storage.shape());
  outm=denan(storage);
  SaveImage(filename, outm, saveint);
}


const string Stitcher::modname = "stitcher";


void Stitcher::prepare(PointF2D _origin, Shape _ish, int _isz, const deque<Map> & msks) {

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


void Stitcher::stitch(const deque<Map> & iarr, Map & oarr) const {

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

Map Stitcher::resMask() const {
  Map ret(osh);
  ret = swght;
  invert(ret);
  return ret;
}



const string ProcProj::modname="ProcProj";
cl_program ProcProj::oclProgram = 0;


void ProcProj::prepareMask(Map & _gaps, bool bepicky) {
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
  ster1.prepare(strl.origin1, iproc.outShape(), strl.origin1size, pmsas);
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




ProcProj::ProcProj(const ProcProj & other)
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



void ProcProj::sub_proc( const Stitcher & ster, const deque<Map> & hiar, deque<Map> & oar
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


bool ProcProj::process(deque<Map> & allInR, deque<Map> & res, const ImagePath & interim_name) {

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



void Denoiser::proc(Map & iom, const Map & mask) {
  if ( !area(sh) || rad == 0)
    return;
  if ( iom.shape() != sh )
    throw_error("denoiser", "Non matching shape of input array.");
  if ( mask.size() &&  mask.shape() != sh )
    throw_error("denoiser", "Non matching shape of the mask.");

  static const uint rad2 = rad*rad;
  tarr = iom;
  for (ArrIndex icur=0; icur<sh(0); icur++) {
    const uint iiF = min(0l, icur-rad), iiT = max(sh(0), icur+rad+1);
    for (ArrIndex jcur=0; jcur<sh(1); jcur++) {
      const uint jjF = min(0l, jcur-rad), jjT = max(sh(1), jcur+rad+1);

      uint cnt = 0;
      float sum = 0.0;
      for (ArrIndex ii = iiF ; ii < iiT ; ii++) {
        uint ii2 = ii-icur;
        ii2 *= ii2;
        for (ArrIndex jj = jjF ; jj < jjT ; jj++) {
          uint jj2 = jj-jcur;
          jj2 *= jj2;
          uint rr2 = ii2 + jj2;
          if ( rr2 <= rad2  &&  rr2  &&  ( ! mask.size() || mask(ii,jj) != 0.0 ) ) {
            sum += tarr(ii, jj);
            cnt++;
          }
        }
      }

      if (cnt) {
        if (thr == 0.0) // averaging
          iom(icur,jcur) = (sum + tarr(icur,jcur)) / (cnt + 1);
        else {
          sum /= cnt;
          const float dv =  sum - tarr(icur,jcur);
          if (    ( thr < 0.0  &&  -thr < abs(dv) )
               || ( thr > 0.0  &&  (sum == 0.0 || thr < abs(dv/sum)) ) )
            iom(icur,jcur) = sum;
        }
      }

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
