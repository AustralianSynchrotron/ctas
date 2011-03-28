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
#include "poptmx.h"
#include <algorithm>
#include <numeric> /* partial_sum */
#include <time.h>
#include <pthread.h>
#include <gsl/gsl_sf_bessel.h> // for Bessel functions




#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif


using namespace std;


long
nof_threads(long _threads){
  if (_threads)
	return _threads;

#ifdef _WIN32
#ifndef _SC_NPROCESSORS_ONLN
  SYSTEM_INFO info;
  GetSystemInfo(&info);
#define sysconf(a) info.dwNumberOfProcessors
#define _SC_NPROCESSORS_ONLN
#endif
#endif

  long nProcessorsOnline = sysconf(_SC_NPROCESSORS_ONLN);
  if (nProcessorsOnline == -1) {
	warn ("thread number",
		  "Unable to read online processor count.");
	return 1;
  } else {
	return nProcessorsOnline;
  }
}





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
Filter::Filter(const string & _name, float _as)
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
  case NONE:     return "NONE";
  case RAMP:     return "RAMP";
  case BARLETT:  return "BARLETT";
  case WELCH:    return "WELCH";
  case PARZEN:   return "PARZEN";
  case HANN:     return "HANN";
  case HAMMING:  return "HAMMING";
  case BLACKMAN: return "BLACKMAN";
  case LANCKZOS: return "LANCKZOS";
  case KAISER:   return "KAISER";
  case GAUSS:    return "GAUSS";
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
Filter::fill(Line & filt, int pixels) const {

  if ( ! pixels ) pixels = filt.size();
  else filt.resize(pixels);
  if ( ! pixels )
	throw_error(modname, "Zero-size filter requested.");

  filt = 0.0;
  filt(0) = 0.25;
  for ( int pix = 1 ; pix < pixels/2 ; pix += 2)
    filt(pix) = -1.0/(pix*pix*M_PI*M_PI);
  fftwf_plan planZ = fftwf_plan_r2r_1d (pixels, filt.data(), filt.data(),
										FFTW_HC2R, FFTW_ESTIMATE);
  fftwf_execute(planZ);
  fftwf_destroy_plan(planZ);

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
operator==(const Filter & a, const Filter & b){
  return a.filter() == b.filter();
}

bool
operator!=(const Filter & a, const Filter & b){
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
  " and represents the \\alpha parameter of the KAISER filter or \\sigma"
  " parameter of the GAUSS filter. Description of the filtering functions"
  " and their graphs can be found in the html documentation.";

string
type_desc (Filter*){
  return "STRING[:FLOAT]";
}

bool
_conversion (Filter* _val, const string & in) {
  float alsig;
  std::string::size_type idx=in.find(':');
  if (idx == std::string::npos){
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
filter_line(Line & ln, const Line & f_win,
			const fftwf_plan *planF, const fftwf_plan *planB){
  float *lnp = ln.data();
  fftwf_execute_r2r( *planF, lnp, lnp);
  ln *= f_win;
  fftwf_execute_r2r( *planB, lnp, lnp);
  ln /= ln.size(); // normalization of the data.
}

/// \brief Projects one line onto the plane.
///
/// @param sino Line to be projected.
/// @param result Plane to project to.
/// @param Theta Rotation angle.
/// @param center Deviation of the rotation center.
///
static void
project_line(const Line & sino, Map & result, float Theta, float center) {

  int pixels = sino.size();
  float abcenter = abs(center);
  int nPp = pixels/2;
  float radius =  nPp - abcenter;

  float *resultp = result.data();
  const float *sinop = sino.data();

  int ncos_theta = (int) (TR_conf * cos(Theta));
  int nsin_theta = (int) (TR_conf * sin(Theta));
  int t_axis_cp  = (int)
	(TR_conf *
	 ( center + ( 1 - cos(Theta) - sin(Theta) ) * pixels / 2.0 ));

  int t_axis, delta;
  float *tresultp;
  for ( int xpix = (int)(abcenter) ; xpix < (int)(pixels-abcenter) ; xpix++){
    delta = (nPp - xpix);
    delta = (int) (sqrt(radius*radius - delta*delta ) - 1) ;
    t_axis = t_axis_cp + xpix * ncos_theta;
    tresultp = resultp + pixels*xpix;
    for ( int ypix = nPp - delta; ypix < nPp+delta ; ypix++)
      *(tresultp + ypix) +=
        *( sinop + ( t_axis + ypix * nsin_theta ) / TR_conf );
  }
}


/// \brief Projects the sinogram.
///
/// @param sinogram Sinogram to be projected
/// @param result The result to store the final projection.
/// @param center Rotation center.
///
static inline void
project_sino(const Map &sinogram, Map &result, float center) {
  //int pixels = sinogram.columns();
  int thetas = sinogram.rows();
  for (int iTheta = 0 ; iTheta < thetas ; iTheta++) {
	Line sinoline = sinogram(iTheta, blitz::Range::all());
	project_line(sinoline, result, (M_PI * iTheta)/thetas, center);
  }
}







/// \brief Thread distributor for the line-per-thread approach.
///
/// This class collects all the information and data needed for the
/// multithreaded backprojection and then can be used by
/// independent threads to give them next portion of work: one line
/// with the rotation angle of this line. I.e. it distributes the work
/// over any number of threads.
///
class line_distributor {

private:

  int iTheta;					///< Current projection index.
  int thetas;					///< Total number of projections.
  const Map & sinogram;        ///< (Filtered!) sinogram.
  Map & _result;				///< Result array.
  float _center;				///< Deviation of the rotation axis.

  pthread_mutex_t lock;         ///< Thread mutex used in the data distribution.

public:

  /// \brief Constructor.
  ///
  /// @param _sinogram  (Filtered!) input sinogram.
  /// @param _res Output result.
  /// @param _cent Deviation of the rotation axis.
  ///
  line_distributor(const Map & _sinogram, Map & _res, float _cent)
	: sinogram(_sinogram), _result(_res), _center(_cent) {
    iTheta = 0;
	thetas = sinogram.rows();
    if ( pthread_mutex_init(&lock, NULL) != 0 )
      throw_error("line_distributor", "Failed to initialize the mutex.");
  }

  /// \brief Destructor.
  inline ~line_distributor(){
    pthread_mutex_destroy(&lock);
  }

  /// \brief Resulting array.
  ///
  /// @return Array where the result is strored.
  ///
  inline Map & result() const {return _result;}

  /// \brief Rotation center.
  ///
  /// @return Deviation of the rotation axis from the center of the sinogram.
  ///
  inline float center() const {return _center;}

  /// \brief Destributor.
  ///
  /// @param Theta Rotation angle of the line.
  /// @param sinoline Line to be projected.
  ///
  /// @return \c true if the thread should process the job, \c false if the
  /// global work is finished and the thread should exit.
  ///
  inline bool distribute(float *Theta, Line & sinoline){
    pthread_mutex_lock(&lock);
	bool returned = iTheta < thetas;
	if ( returned ) {
	  sinoline.reference( sinogram(iTheta, blitz::Range::all()) );
	  *Theta = (M_PI * iTheta)/thetas;
	  iTheta++;
	}
    pthread_mutex_unlock(&lock);
    return returned;
  }

};

/// \brief Work for thread in the line-per-thread approach.
///
/// @param _args Arguments: distributor.
///
/// @return No return.
///
static inline void *
project_line_thr ( void * _args) {
  line_distributor * args = (line_distributor *) _args;
  Map & result = args->result();
  float center = args->center();

  Line sinoline;
  float Theta;
  while ( args->distribute( &Theta, sinoline )  )
    project_line ( sinoline, result, Theta, center);
  return 0;
}

/// \brief Projects the sinogram using multithreaded approach.
///
/// @param sinogram Sinogram to be projected
/// @param result The result to store the final projection.
/// @param center Rotation center.
/// @param threads Number of threads.
///
static inline void
project_sino( const Map &sinogram, Map &result, float center, int threads ) {

  if (threads == 1) {
	project_sino (sinogram, result, center);
	return;
  }

  // Here I will assume that all threads can write to the same result array
  // without the interference. However, it is not absolutely correct, but
  // even if a small number of threads are interfering, the influence
  // can be hardly noticed.
  //
  // If you want to avoid the interference effect, you should create
  // line_distributor for each thread with it's own result array and then
  // sum all these results after the threads had finished.

  //int pixels = sinogram.columns();
  //int thetas = sinogram.rows();
  line_distributor args(sinogram, result, center);
  vector<pthread_t> ntid(threads);

  for (int ith = 0 ; ith < threads ; ith++)
    if ( pthread_create( & ntid[ith], NULL, project_line_thr, &args ) )
      throw_error("project sino in thread", "Can't create thread.");

  for (int ith = 0 ; ith < threads ; ith++)
    pthread_join( ntid[ith], 0);

}






const string CTrec::modname = "reconstruction";

/// The filtering is performed in the domain this times wider
/// than the number of pixels (CTrec::_pixels). Must be >= 1.
const float CTrec::zPad = 2.0;





/// \brief Core reconstruction (currently universal for all types of contrast).
///
/// Other reconstruction functions ( CTrec::reconstruct_*() ) are in their
/// place to allow possible future branching of the reconstruction
/// methods/algorithms. Currently all of them just call this universal
/// reconstruction function
///
/// @param sinogram Input sinogram. After the reconstruction
/// represents filtered sinogram.
/// @param result Reconstructed result.
/// @param center Rotation center.
///
void
CTrec::reconstruct_uni(Map &sinogram, Map &result, float center) const {

  if ( _contrast == Contrast::ABS )
    sinogram = -log(sinogram);

  const int zShift = (int)(_pixels*(zPad-1)/2);
  const int thetas = sinogram.rows();
  Line zsinoline((int)(_pixels*zPad)); // zero-padded sinoline.

  if ( _contrast != Contrast::FLT ) {
    for (int iTheta = 0 ; iTheta < thetas ; iTheta++) {

      Line sinoline = sinogram(iTheta, blitz::Range::all());

      zsinoline = 0;
      zsinoline(blitz::Range(zShift, zShift+_pixels)) = sinoline;
      filter_line(zsinoline, filt_window, &planF, &planB);
      sinoline = zsinoline(blitz::Range(zShift, zShift+_pixels));

      if ( _contrast == Contrast::REF )
        partial_sum( sinoline.begin(), sinoline.end(), sinoline.begin() );

    }
  }

  project_sino(sinogram, result, center, _threads);

}

/// \brief Core function to add a projection line into the reconstruction.
///
/// @param Theta Projection angle.
/// @param sinoline Projection data.
/// @param result Output result.
/// @param center Deviation of the rotation center.
///
void
CTrec::addLine_uni(Line &sinoline, Map &result, const float Theta, const float center) const {

  if ( _contrast == Contrast::ABS )
    sinoline = -log(sinoline);

  if ( _contrast != Contrast::FLT ) {

    const int zShift = (int)(_pixels*(zPad-1)/2);
    Line zsinoline((int)(_pixels*zPad)); // zero-padded sinoline.

    zsinoline = 0;
    zsinoline(blitz::Range(zShift, zShift+_pixels)) = sinoline;
    filter_line(zsinoline, filt_window, &planF, &planB);
    sinoline = zsinoline(blitz::Range(zShift, zShift+_pixels));

  }

  if ( _contrast == Contrast::REF )
    partial_sum( sinoline.begin(), sinoline.end(), sinoline.begin() );

  project_line(sinoline, result, Theta, center);

}



/// \brief Actual reconstruction.
///
/// @param sinogram Input sinogram. Can be altered by the reconstruction process.
/// @param result Output result.
/// @param center Deviation of the rotation center.
///
void
CTrec::reconstruct(Map &sinogram, Map &result, const float center) const {


  if ( sinogram.columns() != _pixels )
	throw_error ( modname, "The width of the input array"
				  " (" + toString(sinogram.columns()) + ") does not match the"
				  " requested reconstruction width "
				  " (" + toString(_pixels) + ")." );
  if ( abs(center) >= _pixels/2 )
	throw_error(modname, "The rotation center is outside the image:"
				" Image width: " + toString(_pixels) + ", the deviation"
				" of the rotation axis from the center of the image: "
				+ toString(center) + ".");


  result.resize(_pixels, _pixels);
  result = 0.0;

  if ( ! sinogram.isStorageContiguous() ) sinogram.reference( sinogram.copy() );
  if ( ! result  .isStorageContiguous() ) result  .reference( result  .copy() );

  if (_contrast == Contrast::ABS) unzero(sinogram);

  (this->*_reconstruct)(sinogram, result, center);

}

/// \brief Add one line to the reconstruction.
///
/// @param Theta Projection angle.
/// @param sinoline Projection data.
/// @param result Output result.
/// @param center Deviation of the rotation center.
///
void
CTrec::addLine(Line &sinoline, Map &result, const float Theta, const float center) const {

  if ( sinoline.size() != _pixels )
	throw_error ( modname, "The width of the input sinogram line"
				  " (" + toString(sinoline.size()) + ") does not match the"
				  " requested reconstruction width "
				  " (" + toString(_pixels) + ")." );
  if ( result.shape() != Shape(_pixels, _pixels) )
	throw_error(modname, "The size of the result array"
                " (" + toString(result.columns()) + ":" + toString(result.rows()) + ")"
				" does not match the requested reconstruction width"
                " (" + toString(_pixels) + ").");
  if ( abs(center) >= _pixels/2 )
	throw_error(modname, "The rotation center is outside the image:"
				" Image width: " + toString(_pixels) + ", the deviation"
				" of the rotation axis from the center of the image: "
				+ toString(center) + ".");

  if ( ! sinoline.isStorageContiguous() ) sinoline.reference( sinoline.copy() );
  if ( ! result  .isStorageContiguous() ) result  .reference( result  .copy() );

  if (_contrast == Contrast::ABS) unzero(sinoline);

  (this->*_addLine)(sinoline, result, Theta, center);



}



/// \brief Destructor
CTrec::~CTrec(){
  fftwf_destroy_plan(planF);
  fftwf_destroy_plan(planB);
}

/// \brief Width of the reconstructed image.
///
/// @return Width of the reconstructed image.
///
int
CTrec::pixels() const {
  return _pixels;
}

/// \brief Type of the contrast.
///
/// @return Type of the contrast.
///
Contrast
CTrec::contrast() const {
  return _contrast;
}

/// \brief Number of threads.
///
/// @return Number of threads.
///
int
CTrec::threads() const {
  return _threads;
}

/// \brief Type of the filter function.
///
/// @return Type of the filter function.
///
Filter
CTrec::filter() const {
  return _filter;
}

/// \brief Constructor
///
/// @param px Width of the reconstructed image.
/// @param cn Type of the contrast.
/// @param tr Number of threads.
/// @param ft Type of the filter function.
///
CTrec::CTrec(int px, Contrast cn, int tr, const Filter & ft)
  : _pixels(px), _contrast(cn), _threads(tr), _filter(ft) {
  pixels(_pixels);
  filter(_filter);
  contrast(_contrast);
  threads(_threads);
  choose_algorithm();
}

/// \brief Constructor
///
/// @param px Width of the reconstructed image.
/// @param cn Type of the contrast.
/// @param ft Type of the filter function.
///
CTrec::CTrec(int px, Contrast cn, const Filter & ft)
  : _pixels(px), _contrast(cn), _threads(0), _filter(ft) {
  pixels(_pixels);
  filter(_filter);
  contrast(_contrast);
  threads(_threads);
  choose_algorithm();
}

/// \brief Sets _pixels, allocates FFT plans.
///
/// I made this method private because will have to reallocate
/// FFT plans, resize and refill the filter window, etc.
/// If you want to reconstruct with different width, you'd better
/// create new CTrec object for this. May be I will change this
/// behaviour in the future
///
/// @param px Width of the sinograms to be reconstructed
///
void
CTrec::pixels(int px){
  _pixels=px;
  if (_pixels <= 1)
	throw_error (modname, "Number of pixels in the CT reconstruction"
				 + toString(_pixels) + ": less or equal to 1.");
  planF = fftwf_plan_r2r_1d ((int)(_pixels*zPad), 0, 0, FFTW_R2HC, FFTW_ESTIMATE);
  planB = fftwf_plan_r2r_1d ((int)(_pixels*zPad), 0, 0, FFTW_HC2R, FFTW_ESTIMATE);
  filter(_filter);
}

/// \brief Changes type of the contrast.
///
/// @param cn New contrast type.
///
void
CTrec::contrast(Contrast cn){
  _contrast=cn;
  choose_algorithm();
}

/// \brief Changes type of the filter function.
///
/// @param ft New type of the filter function.
///
void
CTrec::filter(const Filter & ft){
  filt_window.resize((int)(_pixels*zPad));
  _filter = ft;
  _filter.fill(filt_window);
  filt_window *= zPad;
  if ( _contrast == Contrast::REF ) filt_window(0)=0.0;
}

/// \brief Changes number of threads.
///
/// @param tr New number of threads.
///
void
CTrec::threads(int tr=1){
  _threads = nof_threads(tr);
  choose_algorithm();
}


/// \brief Assigns one of the algorithms to the CTrec::_reconstruct pointer
///
/// The assignment is done on the basis of the pre-defined members of the class.
/// This method should be called whenever any algoritm-defining parameter
/// is altered.
///
void
CTrec::choose_algorithm(){
  switch (_contrast.contrast()) {
  case Contrast::ABS:
    _reconstruct = ( _threads == 1 ) ?
	  &CTrec::reconstruct_abs : &CTrec::reconstruct_abs_thr;
    _addLine = &CTrec::addLine_abs;
	break;
  case Contrast::REF:
    _reconstruct = ( _threads == 1 ) ?
	  &CTrec::reconstruct_ref : &CTrec::reconstruct_ref_thr;
    _addLine = &CTrec::addLine_ref;
	break;
  case Contrast::PHS:
    _reconstruct = &CTrec::reconstruct_uni;
    _addLine = &CTrec::addLine_uni;
    break;
  case Contrast::FLT:
    _reconstruct = &CTrec::reconstruct_uni;
    _addLine = &CTrec::addLine_uni;
    break;
  default :
    warn(modname, "Do not know how to reconstruct contrast type \""
         + _contrast.name() + "\". Will use standard reconstruction method.");
    _reconstruct = &CTrec::reconstruct_uni;
    _addLine = &CTrec::addLine_uni;
	break;
  }
}



/// \brief Core reconstruction: absorption
///
/// @param sinogram Input sinogram. After the reconstruction
/// represents filtered sinogram.
/// @param result Reconstructed result.
/// @param center Rotation center.
///
void
CTrec::reconstruct_abs(Map &sinogram, Map &result, float center) const {
  reconstruct_uni(sinogram, result, center);
}

/// \brief Core reconstruction: refraction
///
/// @param sinogram Input sinogram. After the reconstruction
/// represents filtered sinogram.
/// @param result Reconstructed result.
/// @param center Rotation center.
///
void
CTrec::reconstruct_ref(Map &sinogram, Map &result, float center) const {
  reconstruct_uni(sinogram, result, center);
}

/// \brief Core reconstruction: absorption, parallel approach.
///
/// @param sinogram Input sinogram. After the reconstruction
/// represents filtered sinogram.
/// @param result Reconstructed result.
/// @param center Rotation center.
///
void
CTrec::reconstruct_abs_thr(Map &sinogram, Map &result, float center) const {
  reconstruct_uni(sinogram, result, center);
}


/// \brief Core reconstruction: refraction, parallel approach
///
/// @param sinogram Input sinogram. After the reconstruction
/// represents filtered sinogram.
/// @param result Reconstructed result.
/// @param center Rotation center.
///
void
CTrec::reconstruct_ref_thr(Map &sinogram, Map &result, float center) const {
  reconstruct_uni(sinogram, result, center);
}


/// \brief Core addLine: absorption.
///
/// @param sinoline Projection data.
/// @param result Output result.
/// @param Theta Projection angle.
/// @param center Deviation of the rotation center.
///
void
CTrec::addLine_abs(Line &sinoline, Map &result, const float Theta, const float center) const {
  addLine_uni(sinoline, result, Theta, center);
}

/// \brief Core addLine: refraction.
///
/// @param sinoline Projection data.
/// @param result Output result.
/// @param Theta Projection angle.
/// @param center Deviation of the rotation center.
///
void
CTrec::addLine_ref(Line &sinoline, Map &result, const float Theta, const float center) const {
  addLine_uni(sinoline, result, Theta, center);
}

void
CTrec::finilize(Map & result, int thetas, float pixelSize) {
  // \Delta\Theta = \pi/thetas comes from the integration over \Theta.
  // The pixelSize is missing inside the CT algorithm in the filtering function Filter::fill().
  result *= M_PI / ( pixelSize * thetas);
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
    planF = fftwf_plan_r2r_1d (pixels, 0, 0, FFTW_R2HC, FFTW_ESTIMATE),
    planB = fftwf_plan_r2r_1d (pixels, 0, 0, FFTW_HC2R, FFTW_ESTIMATE);

  float cur_sin = sin(angle);
  float cur_cos = cos(angle);
  float plane_cos = (plane-center)*cur_sin;
  float Rcenter = pixels*0.5 + center;

  for (int ycur = 0 ; ycur < thetas ; ycur++){

	Line ln = projection(ycur, blitz::Range::all());

	// filtering
	filter_line(ln, f_win, &planF, &planB);
	if ( contrast == Contrast::REF )
	  partial_sum( ln.begin(), ln.end(), ln.begin() );

	// projecting
	for (int xcur = 0 ; xcur < pixels ; xcur++) {
	  float di = ( xcur - Rcenter )*cur_cos - plane_cos + Rcenter;
	  di = (di < 0) ?
		0 :
		(di >=pixels ) ? pixels - 1 : di;
	  result(ycur, xcur) += projection(ycur, int(di));
	}

  }

  fftwf_destroy_plan(planF);
  fftwf_destroy_plan(planB);

}










const string CenterOptionDesc=
  "Deviation of the rotation axis from the center of the sinogram.\n"
  "Allows you to erase some minor artifacts in the reconstructed image"
  " which come from the inaccurate rotation position. In most real"
  " CT or TS experiments the rotation center of the sample"
  " is not exactly in the center of the recoded projection. Moreover,"
  " the rotation axis may be inclined from the vertical orientation."
  " If the inclination of the rotation axis is noticeable for your desired"
  " spatial resolution then you should rotate the images before the actual"
  " reconstruction. You can do it in the batch using \"mogrify\" command"
  " from the ImageMagick package with the -rotate option.";
