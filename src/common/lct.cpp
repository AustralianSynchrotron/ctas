/******************************************************************************
 *   Copyright (C) 2007 by Anton Maksimenko                                   *
 *   antonmx@post.kek.jp                                                      *
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
/// @date   Thu Jul 17 13:22:00 2008
///
/// @brief %Reconstruction implementation.
///
/// All functions which implement the actual reconstruction are defined here.
///

#define _USE_MATH_DEFINES // for M_PI

#include "ct.h"
#include "common.h"
#include "poptmx.h"
#include <algorithm>
#include <numeric> /* partial_sum */
#include <pthread.h>
#include <gsl/gsl_sf_bessel.h> // for Bessel functions
#include <unistd.h>

using namespace std;

const string Filter::modname="filter";



Filter::Ftype Filter::fromName(const string &_name) {
  string name = upper(_name);
  if ( name == "NONE" ) return NONE;
  else if ( name.empty() || name == "RAMP" ) return RAMP;
  else if ( name == "BARLETT") return RAMP;
  else if ( name == "WELCH") return WELCH;
  else if ( name == "PARZEN") return PARZEN;
  else if ( name == "HANN") return HANN;
  else if ( name == "HAMMING") return HAMMING;
  else if ( name == "BLACKMAN") return BLACKMAN;
  else if ( name == "LANCKZOS") return LANCKZOS;
  else if ( name == "KAISER") return KAISER;
  else if ( name == "GAUSS") return GAUSS;
  else throw_error(modname, "The string \""+ _name +"\""
                     " does not describe any known filter.");
  return NONE; // only to get rid of the non-return warning
}


// \brief Name of the filter type.
///
/// @return Name of the filter.
///
string
Filter::name() const {
  switch (filttp) {
  case NONE    : return "NONE";
  case RAMP    : return "RAMP";
  case BARLETT : return "BARLETT";
  case WELCH   : return "WELCH";
  case PARZEN  : return "PARZEN";
  case HANN    : return "HANN";
  case HAMMING : return "HAMMING";
  case BLACKMAN: return "BLACKMAN";
  case LANCKZOS: return "LANCKZOS";
  case KAISER  : return "KAISER";
  case GAUSS   : return "GAUSS";
  default      : throw_bug(__FUNCTION__); return "NONE"; // return only to silence warning
  }
}


/// \brief Prepares the window function.
///
/// Fills the line with the filter function. If pixels is 0 then reads the
/// the width of the window from the line length.Otherwise resizes the line.
///
/// @param filt Line to store the window.
/// @param pixels Desired size of the window.
///
/// @return Filled window.
///
Line &
Filter::fill(Line &filt, int pixels) const {

  if ( ! pixels )
    pixels = filt.size();
  else
    filt.resize(pixels);
  if ( ! pixels )
    throw_error(modname, "Zero-size filter requested.");

  filt = 0.0;
  filt(0) = 0.25;
  for ( int pix = 1 ; pix < pixels/2 ; pix += 2)
    filt(pix) = -1.0/(pix*pix*M_PI*M_PI);

  Line _filt(safe(filt));
  fftwf_plan planZ = safe_fftwf_plan_r2r_1d (pixels, _filt.data(), FFTW_HC2R);
  fftwf_execute(planZ);
  safe_fftw_destroy_plan(planZ);
  if (filt.data() != _filt.data())
    filt = _filt;

  /*
  for ( int pix = 0 ; pix < pixels ; pix++)
    filt(pix) = ( pix < pixels/2)  ?  pix  :  pixels - pix;
  filt /= pixels;
  */

  for ( int pix = 0 ; pix < pixels ; pix++) {

    float fp = (pix < pixels/2) ? pix : pixels - pix;
    fp *= 2.0/pixels;

    switch (filttp) {

    case NONE:
    case RAMP:
      fp = 1.0;
      break;

    case BARLETT:
      fp = (1-fp);
      break;

    case WELCH:
      fp = (1-fp*fp);
      break;

    case PARZEN:
      fp = (fp <= 0.5) ?
           1-6*fp*fp + 3*fp*fp*fp : 2*(1+fp*fp*fp);
      break;

    case HANN:
      fp = 0.5 * ( 1 + cos(M_PI*fp) );
      break;

    case HAMMING:
      fp = 0.54+0.46*cos(M_PI*fp);
      break;

    case BLACKMAN:
      fp = ( 0.42 + 0.5*cos(M_PI*fp)+0.08*cos(2*M_PI*fp));
      break;

    case LANCKZOS:
      fp = (fp==0.0) ? 0.0 : abs(sin(M_PI*fp))/fp;
      break;

    case KAISER:
      fp = gsl_sf_bessel_I0(alsig*sqrt(1-fp*fp)) /
           gsl_sf_bessel_I0(alsig) ;
      break;

    case GAUSS:
      fp = pow( (float) 2.0, (float) -fp*fp/(alsig*alsig)) ;
      break;

    }

    // Here 2.0 appears because of the R2HC-HC2R FFT pair.
    filt(pix) *= fp/2.0;

  }

  return filt;

}


const string FilterOptionDesc=
  "Must be a string consisting of two parts (the second one is optional):"
  " the filter name and float parameter of the filter. The filter name"
  " can be one of the following (case insensitive):\n"
  "    RAMP\n"
  "    BARLETT\n"
  "    WELCH\n"
  "    PARZEN\n"
  "    HANN\n"
  "    HAMMING\n"
  "    BLACKMAN\n"
  "    LANCKZOS\n"
  "    KAISER\n"
  "    GAUSS\n"
  "Optional float value is required only if the filter is KAISER or GAUSS"
  " and represents the \\d2b parameter of the KAISER filter or \\sigma"
  " parameter of the GAUSS filter. Description of the filtering functions"
  " and their graphs can be found in the html documentation.";

string
type_desc (Filter *) {
  return "STRING[:FLOAT]";
}

bool
_conversion (Filter *_val, const string &in) {
  float alsig;
  std::string::size_type idx=in.find(':');
  if (idx == std::string::npos) {
    *_val = Filter(in);
  } else {
    if ( ! poptmx::_conversion(&alsig, in.substr(idx+1)) )
      return false;
    *_val = Filter(in.substr(0,idx), alsig);
  }
  return true;
}











/// \brief The int to float conversion coefficient.
///
/// The calculations using float point numbers are much more expensive
/// comparing to that of the integer type. In order to save time in the
/// sensitive code it is possible to convert limited float numbers into
/// integer ones, perform all linear calculations in the integer space and
/// convert the result back to the float space. #TR_conf gives the
/// multiplication coefficient used to convert the float number limited
/// to \f$\pm1.0\f$ (it is the cosine and sine of some value) to the integer.
///
static const int TR_conf = 1 << 16;


/// \brief Applies FFT filter to one line.
///
/// @param ln Line to be filtered.
/// @param f_win Filter window.
/// @param planF Forward FFT plan.
/// @param planB Backward FFT plan.
///
static inline void
filter_line(Line &ln, const Line &f_win,
            const fftwf_plan *planF, const fftwf_plan *planB) {
  Line _ln(safe(ln));
  float *lnp = _ln.data();
  fftwf_execute_r2r( *planF, lnp, lnp);
  _ln *= f_win;
  fftwf_execute_r2r( *planB, lnp, lnp);
  _ln /= _ln.size(); // normalization of the data.
  if ( !areSame(ln, _ln) )
    ln = _ln;
}



void RingFilter::apply(Map & sinogram) {
  if ( !box || ! average.size() )
    return;
  const Shape sh(sinogram.shape());
  if (sh(1) != average.size())
    throw_error("RingFilter", "Wrong sinogram width "+toString(sh(1))+
                              " where "+toString(average.size())+" is expected.");
  if (!sh(0))
    throw_error("RingFilter", "Empty sinogram.");

  average=0;
  for(int idx = 0; idx < sh(0); idx++)
    average += sinogram(idx, all);
  average /= sh(0);

  temp=0;
  for(int ii = -box; ii <= (int) box; ii++)
    temp(dstR(sh(1),ii)) += average(srcR(sh(1),ii));
  temp( blitz::Range(box, sh(1)-1-box) ) /= 2*box+1;
  for(int ii = 0; ii < box; ii++) {
    temp(ii) /= box+ii+1;
    temp(sh(1)-1-ii) /= box+ii+1;
  }
  temp -= average;
  for(int idx = 0; idx < sh(0); idx++)
    sinogram(idx, all) += temp;

}






bool CTrec::ForCLdev::checkReady() {
  if (clSlice)
    return true;
  if ( ! CL_isReady() || ( program && ! clSlice ) )
    return false;

  try {

    static const std::string oclsrc = {
      #include "ct.cl.includeme"
    };
    program = initProgram( oclsrc, program, modname, cl.cont);
    if (!program)
      throw_error(modname, "Failed to compile CT CL program.");

    clSlice(clAllocArray<float>(area(parent.osh), CL_MEM_WRITE_ONLY, cl.cont));
    clSino(clAllocArray<float>(area(parent.ish), CL_MEM_READ_ONLY, cl.cont));
    clAngles(blitz2cl(parent.cossins, CL_MEM_READ_ONLY, cl.que));

    kernelSino(program, "fbp");
    kernelSino.setArg(0, clSino());
    kernelSino.setArg(1, clSlice());
    kernelSino.setArg(2, (cl_int) parent.ish(1));
    kernelSino.setArg(3, (cl_int) parent.ish(0));
    kernelSino.setArg(4, clAngles());
    kernelSino.setArg(6, recCof);

  }  catch (...)  {
    kernelSino.free();
    clAngles.free();
    clSlice.free();
    clSino.free();
    return false;
  }
  return true;
}


int CTrec::ForCLdev::reconstruct(Map & fsin, Map & slice, float center) { // already filtered sinogram
  if (fsin.shape() != parent.ish)
    throw_error(modname, "Unexpected array size on OCL reconstruct.");
  if ( ! CL_isReady() || ( program && ! clSlice ) )
    return -1;
  if ( pthread_mutex_trylock(&locker) )
    return 0;
  bool toRet = 0;

  try{
    if (!checkReady())
      throw 0;
    blitz2cl(fsin, clSino(), cl.que);
    kernelSino.setArg(5, (cl_float) center);
    kernelSino.exec(parent.osh, cl.que);
    slice.resize(parent.osh);
    cl2blitz(clSlice(), slice, cl.que);
    toRet=1;
  } catch (...) {}

  pthread_mutex_unlock(&locker);
  return toRet;
} ;

void CTrec::ForCLdev::setRecCof(float _recCof) {
  recCof = _recCof;
  if (kernelSino)
    kernelSino.setArg(6, recCof);
}

bool
CTrec::ForCLdev::sino(Map &sinogram) {
  if ( sinogram.shape() != parent.ish )
    throw_error ( modname, "Shape of input sinogram (" + toString(sinogram.shape()) + ")"
                           " does not match initial (" + toString(parent.ish) + ").");
  if ( ! CL_isReady() || ( program && ! clSino )  )
    return false;
  bool toRet = false;
  pthread_mutex_lock(&locker);
  try { blitz2cl(sinogram, clSino(), cl.que); toRet = true;}
  catch (...) { warn(modname, "Failed to upload sinogram to OCL device."); }
  pthread_mutex_unlock(&locker);
  return toRet;
}

bool
CTrec::ForCLdev::repeat(Map & slice, float center) {
  if ( abs(center) >= parent.ish(1)/2 )
    throw_error(modname, "Rotation center "+toString(center)+
                         " is outside the image of width " + toString(parent.ish(1)) + ".");
  if ( ! CL_isReady() || ( program && ! clSino ) || pthread_mutex_trylock(&locker) )
    return false;
  bool toRet = false;
  try {
    if (!checkReady())
      throw 0;
    kernelSino.setArg(5, (cl_float) center);
    kernelSino.exec(parent.osh, cl.que);
    slice.resize(parent.osh);
    cl2blitz(clSlice(), slice, cl.que);
    toRet = false;
  }
  catch (...) { warn(modname, "Failed to perform reconstruction on OCL device."); }
  pthread_mutex_unlock(&locker);
  return toRet;
}



const string CTrec::modname = "reconstruction";

CTrec::CTrec(const Shape &sinoshape, Contrast cn, float arc, const Filter & ft)
  : ish(sinoshape)
  , osh(ish(1),ish(1))
  , zidth(pow(2, ceil(log2(2*ish(1)-1))))
  , contrast(cn)
  , filter(ft)
  , filt_window(zidth)
  , zsinoline(zidth)
  , recCof(1)
  , cossins(ish(0))
  , envs(_envs)
  , gpuReleasedCondition(new pthread_cond_t(PTHREAD_COND_INITIALIZER))
  , gpuReleasedMutex(new pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER))
  , gpuWasReleased(_gpuWasReleased)
{

  if (ish(1) < 2)
    throw_error (modname, "Number of pixels in the CT reconstruction less than 2.");
  if (!ish(0))
    throw_error (modname, "Zero projections in sinogram.");

  filter.fill(filt_window);
  filt_window *= zidth / (float) ish(1);
  if ( contrast == Contrast::REF )
    filt_window(0)=0.0;
  planF = safe_fftwf_plan_r2r_1d (zidth, 0, FFTW_R2HC);
  planB = safe_fftwf_plan_r2r_1d (zidth, 0, FFTW_HC2R);

  if (arc < 1.0)
    arc *= ish(0);
  arc *= M_PI / 180;
  for (size_t i = 0; i < ish(0); i++) {
    float th = i * arc / ish(0);
    cossins(i).s[0] = sinf(th);
    cossins(i).s[1] = cosf(th);
  }

  if (!CL_isReady())
    warn(modname, "OpenCL is not functional.");
  for (CLenv & env : clenvs)
    try{ envs.emplace_back(env, *this); } catch(...) {}
  if (envs.empty())
    useCPU = true;

}


CTrec::CTrec(CTrec & other)
  : ish(other.ish)
  , osh(other.osh)
  , zidth(other.zidth)
  , contrast(other.contrast)
  , filter(other.filter)
  , filt_window(other.filt_window)
  , zsinoline(zidth)
  , recCof(other.recCof)
  , cossins(other.cossins)
  , envs(other.envs)
  , gpuReleasedCondition(other.gpuReleasedCondition)
  , gpuReleasedMutex(other.gpuReleasedMutex)
  , gpuWasReleased(other.gpuWasReleased)
  , useCPU(other.useCPU)
{
  planF = safe_fftwf_plan_r2r_1d (zidth, 0, FFTW_R2HC);
  planB = safe_fftwf_plan_r2r_1d (zidth, 0, FFTW_HC2R);
}


/// \brief Destructor
CTrec::~CTrec(){
  if(addressof(envs) == std::addressof(envs)) { // parent instance
    pthread_mutex_destroy(gpuReleasedMutex);
    delete gpuReleasedMutex;
    pthread_cond_destroy(gpuReleasedCondition);
    delete gpuReleasedCondition;
  }
  safe_fftw_destroy_plan(planF);
  safe_fftw_destroy_plan(planB);
}


void CTrec::setPhysics(float pixelSize, float lambda) {
  recCof = 1.0;
  if (lambda > 0.0)
    recCof *= lambda / (2*M_PI);
  if (pixelSize > 0.0)
    recCof *= M_PI / pixelSize;
  for (ForCLdev & env : envs)
    env.setRecCof(recCof);
}


void CTrec::prepare_sino(Map & sinogram) {

  if (sinogram.shape() != ish )
    throw_error(modname, "Wrong sinogram shape ("+toString(sinogram.shape())+")"
                         " where ("+toString(ish)+") is expected.");

  if (contrast == Contrast::ABS)
    deAbs(sinogram);

  if ( contrast != Contrast::FLT ) {
    const int zShift = ( zidth - ish(1) ) / 2;
    const blitz::Range zRange(zShift, zShift+ish(1));
    for (int iTheta = 0 ; iTheta < ish(0) ; iTheta++) {
      Line sinoline(sinogram(iTheta, all));
      zsinoline = 0.0;
      zsinoline(zRange) = sinoline;
      filter_line(zsinoline, filt_window, &planF, &planB);
      sinoline = zsinoline(zRange);
    }
  }

  if ( contrast == Contrast::REF ) {
    for (int iTheta = 0 ; iTheta < ish(0) ; iTheta++) {
      Line sinoline(sinogram(iTheta, all));
      partial_sum( sinoline.begin(), sinoline.end(), sinoline.begin() );
    }
  }

}


void
CTrec::sino(Map &sinogram) {
  if ( sinogram.shape() != ish )
    throw_error ( modname, "Shape of input sinogram (" + toString(sinogram.shape()) + ")"
                           " does not match initial (" + toString(ish) + ").");
  mysino.reference(sinogram);
  prepare_sino(mysino);
  for (ForCLdev & env : envs)
    env.sino(mysino);
}


void
CTrec::repeat(Map & slice, float center) {
  if ( abs(center) >= ish(1)/2 )
    throw_error(modname, "Rotation center "+toString(center)+
                         " is outside the sinogram width " + toString(ish(1)) + ".");
  slice.resize(osh);

  if (useCPU) {

    for (ForCLdev & env : envs)
      if (env.repeat(slice, center))
        return;
    reconstructOnCPU(slice, center);

  } else {

    while (true) {
      for (ForCLdev & env : envs) {
        const int reced = env.repeat(slice, center);
        if (reced) {
          pthread_mutex_lock(gpuReleasedMutex);
          gpuWasReleased = true;
          pthread_cond_signal(gpuReleasedCondition);
          pthread_mutex_unlock(gpuReleasedMutex);
        }
        if (reced < 0) { // GPU failed
          warn(modname, "Reconstruction on GPU has failed. Trying CPU.");
          reconstructOnCPU(slice, center);
          return;
        }
        if (reced)
          return;
      }
      pthread_mutex_lock(gpuReleasedMutex);
      if (!gpuWasReleased)
        pthread_cond_wait(gpuReleasedCondition, gpuReleasedMutex);
      gpuWasReleased = false;
      pthread_mutex_unlock(gpuReleasedMutex);
    }


  }

}


void
CTrec::reconstruct(Map &sinogram, Map & slice, float center) {
  if ( abs(center) >= ish(1)/2 )
    throw_error(modname, "Rotation center "+toString(center)+
                         " is outside the sinogram width " + toString(ish(1)) + ".");
  slice.resize(osh);
  prepare_sino(sinogram);

  if (useCPU) {

    for (ForCLdev & env : envs)
      if (env.reconstruct(sinogram, slice, center) > 0)
        return;
    mysino.reference(sinogram);
    reconstructOnCPU(slice, center);

  } else {

    while (true) {
      for (ForCLdev & env : envs) {
        const int reced = env.reconstruct(sinogram, slice, center);
        if (reced) {
          pthread_mutex_lock(gpuReleasedMutex);
          gpuWasReleased = true;
          pthread_cond_signal(gpuReleasedCondition);
          pthread_mutex_unlock(gpuReleasedMutex);
        }
        if (reced < 0) { // GPU failed
          warn(modname, "Reconstruction on GPU has failed. Trying CPU.");
          mysino.reference(sinogram);
          reconstructOnCPU(slice, center);
        }
        if (reced)
          return;
      }
      pthread_mutex_lock(gpuReleasedMutex);
      if (!gpuWasReleased)
        pthread_cond_wait(gpuReleasedCondition, gpuReleasedMutex);
      gpuWasReleased = false;
      pthread_mutex_unlock(gpuReleasedMutex);
    }

  }

}


void
CTrec::reconstructOnCPU(Map & slice, float center) {
  switchToGPU=false;
  const int hp = ish(1) / 2 ;
  const float acent = fabs(center);
  for (ArrIndex i = 0 ; i < osh(1) ; i++) {
    int ip2 = i-hp;
    ip2 *= ip2;
    if ( i <= acent  ||  i >= ish(1)-acent-1 )
       slice(all,i)=0;
    else {
      for (ArrIndex j = 0 ; j < osh(0) ; j++) {
        float ijp2 = j-hp-center;
        ijp2 *= ijp2;
        ijp2 += ip2;
        if (    ( j <= hp+center  &&  ijp2 >= (hp+center) * (hp+center) )
             || ( j >= hp+center  &&  ijp2 >= (hp-center) * (hp-center) ) )
        {
          slice(j,i)=0;
        } else {
          float total = 0.0f;
          for (ArrIndex proj = 0; proj < ish(0); proj++)  {
            const cl_float2 & cossin = cossins(proj);
            ArrIndex iS=   center + (1-cossin.x-cossin.y) * hp
                           + cossin.x * (j-center) + cossin.y * i;
            total += mysino(proj, iS);
          }
          if (recCof != 0.0)
            total *= recCof;
          //total *= 2 / (pixelSize * 100);
          slice(j,i) = total / ish(0);
        }
        if (switchToGPU) {
          switchToGPU=false;
          for (ForCLdev & env : envs)
            if (env.reconstruct(mysino, slice, center))
              return;
        }
      }
    }
  }
}









void
ts_add( Map &projection, Map &result, const Filter & filter,
    const float center, const Contrast contrast,
    const float angle, const int plane){

  if ( contrast == Contrast::ABS )
        projection = -log(projection);

  int pixels = projection.columns();
  int thetas = projection.rows();
  if ( result.columns() != pixels ||
       result.rows()    != thetas)
    throw_error ( "tomosynthesis", "The size of the result array"
                  " does not match the requested reconstruction." );

  Line f_win;
  filter.fill(f_win, pixels);

  fftwf_plan
    planF = safe_fftwf_plan_r2r_1d (pixels, 0, FFTW_R2HC),
    planB = safe_fftwf_plan_r2r_1d (pixels, 0, FFTW_HC2R);

  float cur_sin = sin(angle);
  float cur_cos = cos(angle);
  float plane_cos = (plane-center)*cur_sin;
  float Rcenter = pixels*0.5 + center;

  for (ArrIndex ycur = 0 ; ycur < thetas ; ycur++) {

        Line ln = projection(ycur, all);

  // filtering
  filter_line(ln, f_win, &planF, &planB);
  if ( contrast == Contrast::REF )
    partial_sum( ln.begin(), ln.end(), ln.begin() );

  // projecting
  for (ArrIndex xcur = 0 ; xcur < pixels ; xcur++) {
    float di = ( xcur - Rcenter )*cur_cos - plane_cos + Rcenter;
    di = (di < 0)  ?  0  :  (di >=pixels ) ? pixels - 1 : di;
    result(ycur, xcur) += projection( ycur, (ArrIndex) di );
  }

  }

  safe_fftw_destroy_plan(planF);
  safe_fftw_destroy_plan(planB);

}



float horizontalShift( Map & pxaProj_0, Map & pxaProj_180) {
  const string modname_ax = "Axis shift";
  if (pxaProj_0.shape() != pxaProj_180.shape())
    throw_error(modname_ax, "Different shapes of input projections: "
       "("+toString(pxaProj_0.shape())+") and ("+toString(pxaProj_180.shape())+") .");
  if (!pxaProj_0.size())
    throw_error(modname_ax, "Empty input projections.");
  const Shape ish = pxaProj_0.shape();
  float sum=0;
  blitz::Array<complex<float>,1> mid(ish(1)), mids(ish(1));
  fftwf_complex * midd = (fftwf_complex*)(void*) mid.data(); // Bad trick!
  fftwf_plan fft_f = fftwf_plan_dft_1d(ish(1), midd, midd, FFTW_FORWARD,  FFTW_ESTIMATE);
  fftwf_plan fft_b = fftwf_plan_dft_1d(ish(1), midd, midd, FFTW_BACKWARD, FFTW_ESTIMATE);
  for (ArrIndex sl=0 ; sl<ish(0) ; sl++) {
    mid = blitz::cast< complex<float> >(pxaProj_0(sl, all));
    fftwf_execute(fft_f);
    mids=mid;
    mid = blitz::cast< complex<float> >(pxaProj_180(sl,all));
    fftwf_execute(fft_f);
    mid *= mids;
    //mid(0)=0;
    fftwf_execute(fft_b);
    sum += minIndex(real(mid))(0);
  }
  fftwf_destroy_plan(fft_f);
  fftwf_destroy_plan(fft_b);
  return sum/ish(0);
}


















