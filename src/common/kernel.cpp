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

#include "kernel.h"
#include "common.h"
#include "poptmx.h"
#include <algorithm>
#include <numeric> /* partial_sum */
#include <time.h>
#include <pthread.h>
#include <gsl/gsl_sf_bessel.h> // for Bessel functions
#include <unistd.h>


using namespace std;

const string Filter::modname="filter";

/// \brief Constructor.
///
/// @param _tp Filter type.
/// @param _as Additional parameter.
///
Filter::Filter(Ftype _tp, float _as)
  : filttp(_tp), alsig(_as) {
}


/// \brief Constructor
///
/// Similar to the previous one, but reads the filter type from the string.
///
/// @param _name Filter name.
/// @param _as Additional parameter.
///
Filter::Filter(const string &_name, float _as)
  : alsig(_as) {
  string name = upper(_name);
  if ( name.empty() || name == "NONE" ) filttp = NONE;
  else if ( name == "RAMP" || name.empty() ) filttp = RAMP;
  else if ( name == "BARLETT") filttp = RAMP;
  else if ( name == "WELCH") filttp = WELCH;
  else if ( name == "PARZEN") filttp = PARZEN;
  else if ( name == "HANN") filttp = HANN;
  else if ( name == "HAMMING") filttp = HAMMING;
  else if ( name == "BLACKMAN") filttp = BLACKMAN;
  else if ( name == "LANCKZOS") filttp = LANCKZOS;
  else if ( name == "KAISER") filttp = KAISER;
  else if ( name == "GAUSS") filttp = GAUSS;
  else throw_error(modname, "The string \""+ _name +"\""
                     " does not describe any known filter.");
  //check_alsig();
}

/// \brief Name of the filter type.
///
/// @return Name of the filter.
///
string
Filter::name() const {
  switch (filttp) {
  case NONE:
    return "NONE";
  case RAMP:
    return "RAMP";
  case BARLETT:
    return "BARLETT";
  case WELCH:
    return "WELCH";
  case PARZEN:
    return "PARZEN";
  case HANN:
    return "HANN";
  case HAMMING:
    return "HAMMING";
  case BLACKMAN:
    return "BLACKMAN";
  case LANCKZOS:
    return "LANCKZOS";
  case KAISER:
    return "KAISER";
  case GAUSS:
    return "GAUSS";
  default :
    throw_bug(__FUNCTION__);
    return "";
  }
}

/// \brief Filter type
///
/// @return Type of the filter.
///
Filter::Ftype
Filter::filter() const {
  return filttp;
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

  if ( ! pixels ) pixels = filt.size();
  else filt.resize(pixels);
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

bool
operator==(const Filter &a, const Filter &b) {
  return a.filter() == b.filter();
}

bool
operator!=(const Filter &a, const Filter &b) {
  return a.filter() != b.filter();
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
  if ( ln.data() != _ln.data() )
    ln = _ln;
}



const string CTrec::modname = "reconstruction";
cl_program CTrec::program = 0;


CTrec::CTrec(const Shape &sinoshape, Contrast cn, float arc, const Filter & ft)
  : ish(sinoshape)
  , osh(ish(1),ish(1))
  , zidth(pow(2, ceil(log2(2*ish(1)-1))))
  , contrast(cn)
  , filter(ft)
{

  if (ish(1) < 2)
    throw_error (modname, "Number of pixels in the CT reconstruction less than 2.");
  if (!ish(0))
    throw_error (modname, "Zero projections in sinogram.");

  filt_window.resize(zidth);
  filter.fill(filt_window);
  filt_window *= zidth / (float) ish(1);
  if ( contrast == Contrast::REF )
    filt_window(0)=0.0;
  planF = safe_fftwf_plan_r2r_1d (zidth, 0, FFTW_R2HC);
  planB = safe_fftwf_plan_r2r_1d (zidth, 0, FFTW_HC2R);

  clSlice(clAllocArray<float>(area(osh), CL_MEM_WRITE_ONLY));
  clSino(clAllocArray<float>(area(ish), CL_MEM_READ_ONLY));
  blitz::Array<cl_float2, 1> angles(ish(0));
  if (abs(arc)<=1.0) // if it was a step then converts into arc
    arc *= ish(0)-1;
  for (size_t i = 0; i < ish(0); i++) {
    float th = arc * M_PI * i / ( ish(0) * 180.0 );
    angles(i).s[0] = sinf(th);
    angles(i).s[1] = cosf(th);
  }
  clAngles(blitz2cl(angles, CL_MEM_READ_ONLY));

  static const string oclsrc = {
    #include "ct.cl.includeme"
  };
  program = initProgram( oclsrc, program, modname);
  if (!program)
    throw_error(modname, "Failed to compile CT CL program.");
  kernelSino(program, "fbp");
  kernelSino.setArg(0, clSino());
  kernelSino.setArg(1, clSlice());
  kernelSino.setArg(2, (cl_int) ish(1));
  kernelSino.setArg(3, (cl_int) ish(0));
  kernelSino.setArg(5, clAngles());

}


/// \brief Destructor
CTrec::~CTrec(){
  safe_fftw_destroy_plan(planF);
  safe_fftw_destroy_plan(planB);
}


void CTrec::prepare_sino(Map &sinogram) {

  if (sinogram.shape() != ish )

  sinogram.reference(safe(sinogram));
  if (contrast == Contrast::ABS)
    deAbs(sinogram);

  const int zShift = ( zidth - ish(1) ) / 2;
  const int thetas = sinogram.rows();
  Line zsinoline(zidth); // zero-padded sinoline.

  if ( contrast != Contrast::FLT ) {
    for (int iTheta = 0 ; iTheta < thetas ; iTheta++) {
      Line sinoline = sinogram(iTheta, all);
      zsinoline = 0.0;
      zsinoline(blitz::Range(zShift, zShift+ish(1))) = sinoline;
      filter_line(zsinoline, filt_window, &planF, &planB);
      sinoline = zsinoline(blitz::Range(zShift, zShift+ish(1)));
    }
  }

  if ( contrast == Contrast::REF ) {
    for (int iTheta = 0 ; iTheta < thetas ; iTheta++) {
      Line sinoline = sinogram(iTheta, all);
      partial_sum( sinoline.begin(), sinoline.end(), sinoline.begin() );
    }
  }

}




/// \brief Core reconstruction.
///
/// @param sinogram Input sinogram. After the reconstruction
/// represents filtered sinogram.
/// @param center Rotation center.
///
void
CTrec::reconstruct(Map &sinogram, Map & slice, float center, float pixelSize) {

  if ( sinogram.shape() != ish )
    throw_error ( modname, "Shape of input sinogram (" + toString(sinogram.shape()) + ")"
                           " does not match initial (" + toString(ish) + ").");
  if ( abs(center) >= ish(1)/2 )
    throw_error(modname, "The rotation center "+toString(center)+
                         " is outside the image width " + toString(ish(1)) + ".");
  if ( pixelSize <= 0 ) {
    warn(modname, "Impossible pixel size (" + toString(pixelSize) + ")."
    " Using 1.0 instead.");
    pixelSize=1.0;
  }

  prepare_sino(sinogram);
  blitz2cl(sinogram, clSino());
  kernelSino.setArg(4, (cl_float) center);
  kernelSino.exec(osh);
  slice.resize(osh);
  cl2blitz(clSlice(), slice);
  // \Delta\Theta = \pi/thetas comes from the integration over \Theta.
  // The pixelSize is missing inside the CT algorithm in the filtering function Filter::fill().
  slice *= M_PI / (pixelSize * ish(0));

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











