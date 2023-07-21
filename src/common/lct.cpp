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
#include "poptmx.h"
#include <algorithm>
#include <numeric> /* partial_sum */
#include <pthread.h>
#include <gsl/gsl_sf_bessel.h> // for Bessel functions
#include <unistd.h>
#include<functional>


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
  const Shape<2> sh(sinogram.shape());
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

    clSlice(clAllocArray<float>(size(parent.osh), CL_MEM_WRITE_ONLY, cl.cont));
    clSino(clAllocArray<float>(size(parent.ish), CL_MEM_READ_ONLY, cl.cont));
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
  if ( ! checkReady()  )
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
    toRet = true;
  }
  catch (...) { warn(modname, "Failed to perform reconstruction on OCL device."); }
  pthread_mutex_unlock(&locker);
  return toRet;
}



const string CTrec::modname = "reconstruction";

float arcO(const float anarc, const int points) {
  float toRet = anarc;
  if (toRet < 1.0)
    toRet *= points-1;
  toRet *= M_PI / 180; // deg -> rad
  return toRet;
}

float angO(const float anarc, const int points) {
  float toRet = anarc;
  if (toRet > 1.0 && points > 1)
    toRet /= points-1;
  toRet *= M_PI / 180; // deg -> rad
  return toRet;
}


CTrec::CTrec(const Shape<2> &sinoshape, Contrast cn, const float anarc, const Filter & ft)
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

  const float arc = arcO(anarc, ish(0));
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
  if(addressof(envs) == addressof(_envs)) { // parent instance
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
  for (ssize_t i = 0 ; i < osh(1) ; i++) {
    int ip2 = i-hp;
    ip2 *= ip2;
    if ( i <= acent  ||  i >= ish(1)-acent-1 )
       slice(all,i)=0;
    else {
      for (ssize_t j = 0 ; j < osh(0) ; j++) {
        float ijp2 = j-hp-center;
        ijp2 *= ijp2;
        ijp2 += ip2;
        if (    ( j <= hp+center  &&  ijp2 >= (hp+center) * (hp+center) )
             || ( j >= hp+center  &&  ijp2 >= (hp-center) * (hp-center) ) )
        {
          slice(j,i)=0;
        } else {
          float total = 0.0f;
          for (ssize_t proj = 0; proj < ish(0); proj++)  {
            const cl_float2 & cossin = cossins(proj);
            ssize_t iS=   center + (1-cossin.x-cossin.y) * hp
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

  for (ssize_t ycur = 0 ; ycur < thetas ; ycur++) {

        Line ln = projection(ycur, all);

  // filtering
  filter_line(ln, f_win, &planF, &planB);
  if ( contrast == Contrast::REF )
    partial_sum( ln.begin(), ln.end(), ln.begin() );

  // projecting
  for (ssize_t xcur = 0 ; xcur < pixels ; xcur++) {
    float di = ( xcur - Rcenter )*cur_cos - plane_cos + Rcenter;
    di = (di < 0)  ?  0  :  (di >=pixels ) ? pixels - 1 : di;
    result(ycur, xcur) += projection( ycur, (ssize_t) di );
  }

  }

  safe_fftw_destroy_plan(planF);
  safe_fftw_destroy_plan(planB);

}












const string modname_ax = "Axis shift";



class AverageGradient {
private:
  Map sino;
  const Shape<2> ish, osh;
  CTrec ct;
  blitz::Array<complex<float>,2> flt, mid;
  fftwf_plan fft_f;
  fftwf_plan fft_b;
  Map slice;
  Map derH;
  Map derV;

public:

  AverageGradient(Map & _sino, const float anarc=180)
    : sino(_sino)
    , ish(sino.shape())
    , osh(ish(1),ish(1))
    , ct(ish, Contrast::ABS, anarc)
    , mid(osh)
    , fft_f(fftwf_plan_dft_2d(osh(0), osh(1), (fftwf_complex*) mid.data(), (fftwf_complex*) mid.data()
                              , FFTW_FORWARD,  FFTW_ESTIMATE))
    , fft_b(fftwf_plan_dft_2d(osh(0), osh(1), (fftwf_complex*) mid.data(), (fftwf_complex*) mid.data()
                              , FFTW_BACKWARD, FFTW_ESTIMATE))
    , slice(osh)
    , derH(osh)
    , derV(osh)
  {
    if(!size(ish))
      throw_error("Average gradient", "Empty input sinogram.");
    Map insin(sino.copy());
    ct.sino(insin);
    const float sigma = angO(anarc, ish(0)) * ish(1) / M_PI ;
    if (sigma>1) {
      flt.resize(osh);
      const float sig22 = 2 * sigma * sigma;
      mid = exp(-(i0*i0+i1*i1)/sig22);
      fftwf_execute(fft_f);
      flt = mid;
    }
  }

  AverageGradient(AverageGradient & other)
    : sino(other.sino)
    , ish(other.ish)
    , osh(ish(1),ish(1))
    , ct(other.ct)
    , flt(other.flt)
    , mid(osh)
    , fft_f(fftwf_plan_dft_2d(osh(0), osh(1), (fftwf_complex*) mid.data(), (fftwf_complex*) mid.data()
                              , FFTW_FORWARD,  FFTW_ESTIMATE))
    , fft_b(fftwf_plan_dft_2d(osh(0), osh(1), (fftwf_complex*) mid.data(), (fftwf_complex*) mid.data()
                              , FFTW_BACKWARD, FFTW_ESTIMATE))
    , slice(osh)
    , derH(osh)
    , derV(osh)
  {
    //Map insin(sino.copy());
    //ct.sino(insin);
  }


  ~AverageGradient() {
    fftwf_destroy_plan(fft_f);
    fftwf_destroy_plan(fft_b);
  }


  float apply(const float axisR) {

    const float centre = axisR - 0.5*(ish(1)-1);
    ct.repeat(slice, centre);
    const Crop<2> crp = ct.recCrop(centre);
    const Shape<2> oosh = crp.apply(osh);
    if( osh == flt.shape() ){
      mid = blitz::cast< complex<float> >(slice);
      fftwf_execute(fft_f);
      mid *= flt;
      fftwf_execute(fft_b);
      slice = real(mid) / size(osh) ;
    }
    Map cSlice(crp.apply(slice));

    derH=0;
    Map cDerH (crp.apply(derH));
    cDerH(dstRa(oosh,-1,-1)) += -1 * cSlice(srcRa(oosh,-1,-1));
    cDerH(dstRa(oosh,-1, 0)) += -2 * cSlice(srcRa(oosh,-1, 0));
    cDerH(dstRa(oosh,-1, 1)) += -1 * cSlice(srcRa(oosh,-1, 1));
    cDerH(dstRa(oosh, 1,-1)) +=  1 * cSlice(srcRa(oosh, 1,-1));
    cDerH(dstRa(oosh, 1, 0)) +=  2 * cSlice(srcRa(oosh, 1, 0));
    cDerH(dstRa(oosh, 1, 1)) +=  1 * cSlice(srcRa(oosh, 1, 1));
    derV=0;
    Map cDerV (crp.apply(derV));
    cDerV(dstRa(oosh,-1,-1)) += -1 * cSlice(srcRa(oosh,-1,-1));
    cDerV(dstRa(oosh, 0,-1)) += -2 * cSlice(srcRa(oosh, 0,-1));
    cDerV(dstRa(oosh, 1,-1)) += -1 * cSlice(srcRa(oosh, 1,-1));
    cDerV(dstRa(oosh,-1, 1)) +=  1 * cSlice(srcRa(oosh,-1, 1));
    cDerV(dstRa(oosh, 0, 1)) +=  2 * cSlice(srcRa(oosh, 0, 1));
    cDerV(dstRa(oosh, 1, 1)) +=  1 * cSlice(srcRa(oosh, 1, 1));

    slice = 0.0;
    cSlice = sqrt( derH * derH + derV * derV );
    cSlice(all, 0)=0;
    cSlice(all, oosh(1)-1)=0;
    cSlice(0, all)=0;
    cSlice(oosh(0)-1, all)=0;

    const float sm = sum(cSlice);
    const int nm = sum(where(cSlice>0.0, 1, 0));
    return nm ? sm/nm : 0.0;

  }

};




class AGinThread : public InThread {

  AverageGradient agCanon;
  unordered_map<pthread_t, AverageGradient*> ags;
  list<AverageGradient*> fags;

  const Shape<2> sh;
  const float cent;
  const int stpsz;

  float coffee;
  int hfsteps;
  int cdir;
  bool found;

  bool (AGinThread::*alldone)(long int idx);
  float (AGinThread::*vaxis)(long int idx);
  void (AGinThread::*exexafter)(long int idx);

  bool beforeThread() {
    if ( ! alldone || ! vaxis ) {
      warn("Average gradient in thread", "Not ready for processing.");
      return false;
    }
    const pthread_t me = pthread_self();
    if (!ags.count(me)) {
      AverageGradient * ag = 0;
      lock();
      if (fags.size()) {
        ag = fags.front();
        fags.pop_front();
      }
      unlock();
      if (!ag)
        ag = new AverageGradient(agCanon);
      lock();
      ags.insert({me, ag});
      unlock();
    }
    return true;
  }


  bool inThread(long int idx) {
    //clock_t sTV = clock();
    if ( ! alldone || ! vaxis )
      throw_error("Average gradient in thread", "Not ready for processing.");
    if ((this->*alldone)(idx))
      return false;
    const pthread_t me = pthread_self();
    lock();
    if (!ags.count(me)) {
      unlock();
      throw_bug("Did not get through 'beforeThread'.");
    }
    AverageGradient & myAG = *(ags.at(me));
    unlock();

    float & vax = vaxS(idx);
    vax = (this->*vaxis)(idx);
    errS(idx) = myAG.apply(cent + vax);
    if (exexafter)
      (this->*exexafter)(idx);

    bar.update();
    return true;

  }


  bool alldone1(long int idx) {
    return idx>=errS.size();
  }

  float vaxis1(long int idx) {
    return coffee + stpsz * (idx - hfsteps);
  }

  bool alldone2(long int idx) {
    return idx>=errS.size() || found ;
  }

  float vaxis2(long int idx) {
    return coffee + stpsz * idx * cdir;
  }

  void execPre(int steps, int abxsz, float coff) {
    clean();
    bar.done();
    bar.setSteps(0);
    coffee = coff    ;
    vaxS.resize(steps);
    errS.resize(steps);
    bar.setSteps(steps);
  }


  void exexafter2(long int idx) {
    lock(1);
    if (!found) {
      if (errS(idx) < minval) {
        minval = errS(idx);
        idxM = idx;
      } else {
        found = true;
      }
    }
    unlock(1);
  }



public:

  Line vaxS;
  Line errS;
  float minval;
  int idxM;

  AGinThread(Map & sino, const float anarc, const int _stpsz)
    : InThread(false, "AG processing")
    , agCanon(sino, anarc)
    , sh(sino.shape())
    , cent( 0.5 * ( sh(1) - 1 ) )
    , stpsz(_stpsz)
    , idxM(0)
    , minval(std::numeric_limits<float>::max())
  {
    needMutexes(2);
  }

  ~AGinThread() {
    clean();
    auto dlnl = [&](auto pntr) {delete pntr; pntr=0;} ;
    for (auto celem : fags) dlnl(celem);
    ags.clear();
  }

  void clean() {
    errS.free();
    vaxS.free();
    for (auto celem : ags)
      fags.push_back(celem.second);
    ags.clear();
  }


  void exec1(int steps, int abxsz, float coff) {
    execPre(steps, abxsz, coff);
    hfsteps = (steps-1)/2 ;
    alldone = &AGinThread::alldone1;
    vaxis = &AGinThread::vaxis1;
    exexafter = 0;
    execute();
    idxM = 0;
    for( int i = 1; i < errS.size() - 1; i++ ) {
      const float & cval = errS(i);
      if( cval < errS(i-1) && cval < errS(i+1) && cval < minval ) {
        idxM = i;
        minval = cval;
      }
    }
  }


  void exec2(int steps, int abxsz, float coff, int dir) {
    execPre(steps, abxsz, coff);
    cdir  = dir;
    found = false;
    alldone = &AGinThread::alldone2;
    vaxis = &AGinThread::vaxis2;
    exexafter = &AGinThread::exexafter2;
    execute();
  }

  float exec(float x) {
    float ret = agCanon.apply(cent+x);
    bar.update();
    return ret;
  }


};



float improveMe( const float eps, Line & vaxSi, const Line & errSi, int idx, int cdir
             , std::function<float(float)> getvx ) {
  float aa = vaxSi(idx-cdir);
  float bb = vaxSi(idx+cdir);
  float rf = vaxSi(idx);
  float vl = errSi(idx);
  while( abs(aa-rf) > eps  ||  abs(rf-bb) > eps ) {
    const bool afar = abs(aa-rf) > abs(rf-bb);
    float & lo = afar ? aa : bb;
    float & hi = afar ? bb : aa;
    const float x = (rf + lo) / 2;
    const float vx = getvx(x);
    if( vl <= vx )
      lo = x;
    else {
      vl = vx;
      hi = rf;
      rf = x;
    }
  }
  return rf;
}



float raxis( Map & sino, const float anarc, const float maxDev, int algo)	{

  const Shape<2> sh(sino.shape());
  if(sh(0)<3 || sh(1)<3)
    throw_error(modname_ax, "Shape of input sinogram (" + toString(sh) + ")"
                            " is less than minimal (" + toString(Shape<2>(3,3)) + ").");
  const float arc = arcO(anarc, sh(0));
  if (arc==0.0)
    throw_error(modname_ax, "Zero sinogram arc.");
  if (abs(arc)<M_PI)
    throw_error(modname_ax, "Sinogram arc " +toString(abs(arc)*180/M_PI)+ " is less than 180 degree.");
  const float maxDevLM = 0.25;
  if( maxDev < 0 || maxDev > maxDevLM )
    throw_error(modname_ax, "Invalid maximum relative shift of the rotation axis " +toString(maxDev)+ ".");
  const float cent = 0.5 * ( sh(1) - 1 ) ;

  auto raxisQ = [&]() {
    const int idx180 = (sh(0)-1) * M_PI / abs(arcO(anarc, sh(0)));
    const int nofln( sh(0) - idx180 );
    Map row0(nofln, sh(1)), row180(nofln, sh(1));
    for( int j = 0; j < nofln; j++ ) {
      row0(j,all) = sino(j,all)+1;
      row180(j,all) = sino(j+idx180,all)+1;
    }
    return raxis(row0, row180, min(maxDev, 0.5f));
  };

  float fcr=0;
  if (!algo)
    return raxisQ();
  else if (algo==1)
    fcr = cent;
  else if (algo==2)
    fcr = raxisQ();
  else if (algo==3) {
    Line projLine(sh(1));
    projLine=0;
    for(int j = 0; j < sh(0); j++)
      projLine += sino(j,all);
    const float threshold( 0.1 * max(projLine) + 0.9 * min(projLine) );
    auto thrFnd = [&](bool fromStart) {
      for( int i = fromStart ? 0 : sh(1)-1 ; i != fromStart ? sh(1) : -1; i += fromStart ? 1 : -1 )
        if( projLine(i) > threshold )
          return float(i);
      return 0.0f; // just to avoid warning
    };
    fcr = ( thrFnd(true) + thrFnd(false) )/2.0f;
  } else
    throw_error(modname_ax, "Unknown algorithm for rotation axis "+toString(algo)+".");

  if( abs(fcr - cent) / sh(1) > maxDev )
    throw_error(modname_ax, "Rotation axis relative shift " +toString(abs(fcr - cent) / sh(1))+
                         " is larger than maximum shift "+toString(maxDev)+".");
  const float rbxsz = 1.0 - 2.0 * (maxDev + abs(fcr - cent) / sh(1));
  if( rbxsz < 0.5 )
    throw_error(modname_ax, "Relative box size " +toString(rbxsz)+
                            " is less than miminum window of "+toString(0.5)+".");

  const int stpsz = 8;
  const int steps = 1 + 2 * (maxDev == 0.0  ?  1  :  maxDev * sh(1) / stpsz);
  AGinThread agproc(sino, anarc, stpsz);
  const int & idxM = agproc.idxM;
  agproc.exec1(steps, rbxsz * sh(1), fcr - cent);
  int cdir=1;
  if(!idxM) { // no local minimum within the range
    const int idx = (agproc.errS(0) < agproc.errS(steps - 1) ? 0 : steps - 1);
    cdir = (idx == 0 ? -1 : 1);
    const int coffee = agproc.vaxS(idx - cdir * 2);
    const int maxSteps = sh(1) * maxDevLM - cdir * coffee / stpsz;
    if (maxSteps <= 0)
      throw_error(modname_ax, "Impossible number of steps.");
    agproc.exec2(maxSteps, 0.5*sh(1), coffee, cdir);
    if (idxM == maxSteps - 1 || idxM == 0)
      return fcr;
  }
  float rf = improveMe(0.25, agproc.vaxS, agproc.errS, idxM, cdir,
                       [&](float x){return agproc.exec(x);});
  return cent + rf;

};




float raxis(Map & proj0, Map & proj180, const float maxDev ) {

  if( maxDev < 0 || maxDev > 0.5 )
    throw_error(modname_ax, "Invalid maximum relative shift of the rotation axis " +toString(maxDev)+ "."
                            " Must be within [0, 0.5] range.");
  if (proj0.shape() != proj180.shape())
    throw_error(modname_ax, "Different shapes of input projections: "
       "("+toString(proj0.shape())+") and ("+toString(proj180.shape())+") .");
  if (!proj0.size())
    throw_error(modname_ax, "Empty input projections.");
  const Shape<2> sh = proj0.shape();
  const float cent( 0.5 * (sh(1) - 1) );

  Line proj(sh(1));
  proj = 0;
  for( int j = 0; j < sh(0); j++)
    proj += fabs(proj0(j, all)-1.0) + fabs(proj180(j, all)-1.0);
  const float fcr = [&]() { // Approximate position of the rotation axis
    const float sum = blitz::sum(proj);
    if (sum == 0.0)
      throw_error(modname_ax, "Impossible input projection averaging to zero.");
    return blitz::sum(i0*proj(i0)) / sum;
  }();
  const int stpsz = 16;
  const int steps = 1 + 2 * (maxDev == 0.0  ?  1  :  maxDev * sh(1) / stpsz);
  const Map proj180R(proj180.reverse(blitz::secondDim));
  const float aver = ( blitz::sum(proj180R(all,0)) + blitz::sum(proj180R(all,sh(1)-1)) ) / (2*sh(0));

  auto calcErr = [&](float x){
    Map dlt(sh);
    dlt = aver;
    dlt(all, dstR(sh(1), x)) = proj180R(all, srcR(sh(1), x));
    dlt -= proj0;
    return sqrt(blitz::sum(dlt * dlt));
  };
  Line vaxS(steps);
  Line errS(steps);
  InThread::execute( steps, [&](long int ii){
    vaxS(ii) = 2 * (fcr - cent) + stpsz * (ii-(steps-1)/2);
    errS(ii) = calcErr(vaxS(ii));
  } );

  int idx;
  float minval = std::numeric_limits<float>::max();
  for( int i = 1; i < steps - 1; i++)
    if( errS(i) < errS(i-1) && errS(i) < errS(i+1) && errS(i) < minval )
      minval = errS(idx=i);
  if( minval != std::numeric_limits<float>::max() ) // local minimum found
    return  cent + 0.5 * improveMe(1.0, vaxS, errS, idx, 1, calcErr) ;

  idx = (errS(0) < errS(steps - 1) ? 0 : steps - 1);
  const int cdir = (idx == 0 ? -1 : 1);
  minval = errS(idx);
  const int maxSteps = (sh(1) / 2 - cdir * vaxS(idx)) / stpsz;
  if( maxSteps <= 0 )
    throw_error(modname_ax, "Failed to calculate rotation centre.");

  Line vaxS2(maxSteps + 2);
  vaxS2(0) = vaxS(idx - cdir);
  vaxS2(1) = vaxS(idx);
  Line errS2(maxSteps + 2);
  errS2(0) = errS(idx - cdir);
  errS2(1) = errS(idx);
  int idx2 = 1;
  for (int i = 0; i < maxSteps; i++) {
    vaxS2(i+2) = vaxS(idx) + cdir * (i + 1) * stpsz;
    errS2(i+2) = calcErr( vaxS2(i+2) ) ;
    if( errS2(i+2) < minval ) {
      minval = errS2(i+2);
      idx2 = i + 2;
    } else
      break;
  }
  if( idx2 == maxSteps + 1 )
    throw_error(modname_ax, "Failed to calculate rotation centre.");
  return cent + 0.5 * improveMe(1.0, vaxS2, errS2, idx2, cdir, calcErr);

}





float raxis(Map & proj0, Map & proj180) {
  if (proj0.shape() != proj180.shape())
    throw_error(modname_ax, "Different shapes of input projections: "
       "("+toString(proj0.shape())+") and ("+toString(proj180.shape())+") .");
  if (!proj0.size())
    throw_error(modname_ax, "Empty input projections.");
  const Shape<2> ish = proj0.shape();
  float sum=0;
  blitz::Array<complex<float>,1> mid(ish(1)), mids(ish(1));
  fftwf_complex * midd = (fftwf_complex*)(void*) mid.data(); // Bad trick!
  fftwf_plan fft_f = fftwf_plan_dft_1d(ish(1), midd, midd, FFTW_FORWARD,  FFTW_ESTIMATE);
  fftwf_plan fft_b = fftwf_plan_dft_1d(ish(1), midd, midd, FFTW_BACKWARD, FFTW_ESTIMATE);
  for (ssize_t sl=0 ; sl<ish(0) ; sl++) {
    mid = blitz::cast< complex<float> >(proj0(sl, all));
    fftwf_execute(fft_f);
    mids=mid;
    mid = blitz::cast< complex<float> >(proj180(sl,all));
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


















