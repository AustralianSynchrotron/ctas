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
/// @date   Thu Jul 17 12:58:59 2008
///
/// @brief %Reconstruction interface.
///
/// All the functions which perform actual CT and TS reconstruction go here.
///



#ifndef _H_KERNEL_H_
#define _H_KERNEL_H_

#include "common.h"
#include <fftw3.h>

#ifdef _WIN32

#  pragma warning(disable: 4251)
#  ifdef KERNEL_EXPORTS
#    define KERNEL_API __declspec(dllexport)
#  else
#    define KERNEL_API __declspec(dllimport)
#  endif

#else

//#  include "../../config.h"
#  define KERNEL_API

#endif


/// \defgroup CT CT and TS reconstruction.
///
/// Where actual \CT and \TS reconstruction is done.
///
/// @{


/// \brief Filtering window function.
///
/// This class holds information about filtering window function and
/// can prepare the filtering window.
///
/// Known filters: Filter::Ftype
/// <IMG SRC="../suppl/filters.png">
///
/// If you want to add a new filter:
/// -# Add your filter into the Filter::Ftype enumeration.
/// -# Add your filter to the Filter::Filter(const std::string &, float) constructor
/// -# Add your filter to the Filter::name() function.
/// -# Add your filter to the Filter::fill() function: additional "case" in the
///    switch-case construction
/// -# Add your filter to the Filter::includeFilterTable() option table
/// -# If your new filter requires additional parameter(s) then you can add
///    them using the Filter::alsig as the example.
class KERNEL_API Filter {

public:

  /// Filter types.
  typedef enum {

    /// No filter: 1
    NONE=0,

    /// Ramp: \f$ |k| \f$
    RAMP,

    /// Barlett: \f$ |k|(1-|k|) \f$
    BARLETT,

    /// Welch: \f$ |k|(1-k^2) \f$
    WELCH,

    /** Parzen:
    \f[ |k|\times\left\{ \begin{array}{lll}
    1-6k^2+3|k|^3  &  :  &  |k|\leq1/2    \cr
    2(1+|k|^3)     &  :  &   1/2<|k|\leq1
    \end{array} \right. \f]
    */
    PARZEN,

    /// Hann: \f$ |k|0.5(1+\cos{\pi k}) \f$
    HANN,

    /// Hamming: \f$ |k|(0.54+0.46\cos{\pi k}) \f$
    HAMMING,

    /// Blackman: \f$ |k|(0.42+0.5\cos{\pi k}+0.08\cos{2\pi k}) \f$
    BLACKMAN,

    /// Lanckzos: \f[ |k|\frac{\sin{\pi k}}{k} \f]
    LANCKZOS,

    /// Kaiser: \f[ |k|\frac{I_0(\alpha\sqrt{1-k^2})}{I_0(\alpha)} \f]
    KAISER,

    /// Gauss: \f[ |k|2^{-(k/\sigma)^2} \f]
    GAUSS

  } Ftype ;

private:

  static const std::string modname; ///< Module name.
  Ftype filttp;         ///< Filter type
  float alsig;          ///< Additional parameter required by some filters

public:

  Filter(Ftype _tp=RAMP, float _as=0); ///< Trivial constructor
  Filter(const std::string &_name, float _as=0);  ///< Constructor from name

  Ftype filter() const ;    ///< Filter
  std::string name() const;     ///< Returns name

  Line &fill(Line &filt, int pixels = 0) const;   ///< Fills the window

};


/// \brief Compare filters.
///
/// @param a first filter.
/// @param b second filter.
///
/// @return \c true if the filters are equal, \c false otherwise.
///
bool KERNEL_API
operator==(const Filter & a, const Filter & b);

/// \brief Compare filters.
///
/// @param a first filter.
/// @param b second filter.
///
/// @return \c false if the filters are equal, \c true otherwise.
///
bool KERNEL_API
operator!=(const Filter & a, const Filter & b);

/// Description of the option giving the Filter type.
extern KERNEL_API const std::string FilterOptionDesc;

/// \brief Prints type name.
/// To be used in the CLI parsing via "poptmx" library
/// @return type name.
std::string KERNEL_API type_desc (Filter*);

/// \brief Converts the string "in" into the Filter _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
bool KERNEL_API _conversion (Filter* _val, const std::string & in);



/// \brief CT reconstruction class
///
/// This class first collects all information needed for the CT reconstruction
/// (size, contrast type, filter, etc.) and then performs the reconstruction
/// using either CTrec::reconstruct or CTrec::addLine methods
///
class KERNEL_API CTrec {

private:

  static const float zPad;    ///< Zero-padding coefficient.

  const int _width;          ///< Width of the reconstructed image.
  const int _projections;          ///< Number of projections in sinogram.

  int projection_counter;          ///< Counting projections.
  bool nextAddLineResets;          ///< If true, resets _result to 0 on next addLine.

  // Settable parameters
  Contrast _contrast;     ///< Type of the contrast.
  Filter _filter;       ///< Type of the filter function.

  Map _result;

  // divergent parameters.
  Line filt_window;       ///< The array containing the filter window.
  fftwf_plan planF;       ///< Forward FFT transformation.
  fftwf_plan planB;       ///< Backward FFT transformation.

#ifdef OPENCL_FOUND

  static pthread_mutex_t ctrec_lock;

  static cl_int err;
  static cl_program program;

  mutable cl_kernel kernelSino;
  mutable cl_kernel kernelLine;

  cl_mem clSlice;
  cl_mem clSinoImage;
  cl_mem  clAngles;
  cl_sampler clSinoSampler;

#endif // OPENCL_FOUND

  void prepare_sino(Map & sinogram);

  /// \brief Prepares internal storage for new data.
  ///
  /// Mainly to be used in the conjunction with the ::addLine(),
  /// between two complete reconstructions.
  ///
  void reset();

public:

  CTrec(const Shape &sinoshape, Contrast cn, const Filter &ft);

  ~CTrec();

  static Map reconstruct(Map &sinogram, Contrast cn, const Filter &ft=Filter(),
                         const float center=0, float pixelSize=1.0);

  const Map & reconstruct(Map &sinogram, const float center, float pixelSize=1.0);

  /// Adding one sinogram line to the reconstruction array.
  void addLine(Line &sinoline, const float Theta, const float center);

  /// \brief Finalizes the reconstruction and returns the result.
  ///
  /// Intended to be used in the conjunction with the ::addLine(), as
  /// ::reconstruct() finilizes the results. After calling this method,
  /// next call to ::addLine() will first reset the internal results to 0.
  ///
  /// @param pixelSize Physical size of the pixel.
  ///
  const Map & result(float pixelSize=1.0);

  static const std::string modname; ///< Module name.

  // Set/get parameters.
  int width() const ;
  int projections() const;
  void contrast(Contrast cn);
  Contrast contrast() const ;
  void filter(const Filter & ft);
  Filter filter() const ;

};



/// Add one projection to the TS image.
///
/// The projection array is modified during the reconstruction:
/// each line in the array is filtered (and converted to the field if
/// refraction contrast is in use).
///
/// @param projection Array with the projection (filtered projection on exit).
/// @param result Resulting array.
/// @param filter Fourier filter.
/// @param center deviation of the rotation axis from the center of the sinogram.
/// @param contrast Type of contrast
/// @param angle Angle at which the projection was recorded.
/// @param plane Plane of the reconstruction.
///
void KERNEL_API
ts_add( Map &projection, Map &result, const Filter & filter,
		const float center, const Contrast contrast,
		const float angle, const int plane);



/// Description of the option giving the rotation center.
extern KERNEL_API const std::string CenterOptionDesc;


/// \brief Number of threads for the process.
///
/// @param _threads Requested number of threads (0 for auto).
///
/// @return Number of threads for the architecture where the process is running
/// if automatic number of threads was requested and just _threads if set in stone.
///
long KERNEL_API
nof_threads(long _threads=0);


/// @}



#endif // _H_KERNEL_H_
