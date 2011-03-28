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

#  include "../../config.h"
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

  static const std::string modname;	///< Module name.
  Ftype filttp;					///< Filter type
  float alsig;					///< Additional parameter required by some filters

public:

  Filter(Ftype _tp=RAMP, float _as=0); ///< Trivial constructor
  Filter(const std::string & _name, float _as=0);	///< Constructor from name

  Ftype filter() const ;		///< Filter
  std::string name() const;			///< Returns name

  Line & fill(Line & filt, int pixels = 0) const; ///< Fills the window

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
/// I designed the CT as the class not as a function in order to provide easy
///  way to plug in any new contrast and/or algorithm (f.e. GPU-, FPGA-, bla-bla-bla-based).
///
/// In order to include new reconstruction algorithm you should follow these
/// steps:
/// -# Add the method which implements you algorithm. See CTrec::reconstruct_abs()
///    and other CTrec::reconstruct_* methods for example (currently all
///    of them just call the universal reconstruction CTrec::reconstruct_uni(),
///    they are added into the class just for the architecture's flexibility).
/// -# Set the CTrec::_reconstruct and CTrec::_addLine pointers to your method
///    from the previous step. Please do it ONLY within CTrec::choose_algorithm().
///    The choice of the pointer may depend on any condition you can imagine
///    and any parameter(s) you have added in the first step.
/// -# If needed, add new member(s) into the class which are required for the
///    algorithm (see CTrec::_filter for example). For these new member(s)
///    create set/get methods (see CTrec::filter() and
///    CTrec::filter(const Filter &) for example).
/// -# If it is needed, add the new constructor, or update existing one.
///
///    Voala: you can initialize the reconstruction and reconstruct your
///    sinogram using CTrec::reconstruct() method (f.e. see ct.cpp ).
///
class KERNEL_API CTrec {

private:

  static const std::string modname;	///< Module name.
  static const float zPad;		///< Zero-padding coefficient.

  // Settable parameters
  int _pixels;					///< Width of the reconstructed image.
  Contrast _contrast;			///< Type of the contrast.
  int _threads;					///< Number of threads.
  Filter _filter;				///< Type of the filter function.

  void pixels(int px);			///< Pixels

  // divergent parameters.
  Line filt_window;				///< The array containing the filter window.
  fftwf_plan planF;				///< Forward FFT transformation.
  fftwf_plan planB;				///< Backward FFT transformation.

  // Implementations of the reconstruction algorithms.
  void reconstruct_uni(Map &sinogram, Map &result, float center) const;
  void reconstruct_abs(Map &sinogram, Map &result, float center) const;
  void reconstruct_abs_thr(Map &sinogram, Map &result, float center) const;
  void reconstruct_ref(Map &sinogram, Map &result, float center) const;
  void reconstruct_ref_thr(Map &sinogram, Map &result, float center) const;

  void addLine_ref(Line &sinoline, Map &result, const float Theta, const float center) const;
  void addLine_abs(Line &sinoline, Map &result, const float Theta, const float center) const;
  void addLine_uni(Line &sinoline, Map &result, const float Theta, const float center) const;

  /// \brief Actual reconstruction function.
  ///
  /// Pointer to one of the reconstruction functions (see above) which
  /// would be used in the reconstruction algorithm.
  void (CTrec::*_reconstruct) (Map &, Map &, float) const;

  /// \brief Actual function which adds the sinogram line to the result.
  ///
  /// Pointer to one of the addLine functions (see above) which
  /// would be used in the reconstruction algorithm.
  void (CTrec::*_addLine) (Line&, Map&, const float, const float) const;

  void choose_algorithm();		///< Sets _reconstruct

public:

  CTrec(int px, Contrast cn, int tr=0, const Filter & ft=Filter());
  CTrec(int px, Contrast cn, const Filter & ft);
  ~CTrec();

  // Set/get parameters.
  int pixels() const ;
  void contrast(Contrast cn);
  Contrast contrast() const ;
  void threads(int tr);
  int threads() const ;
  void filter(const Filter & ft);
  Filter filter() const ;

  /// Actual reconstruction of a single sinogram.
  void reconstruct(Map &sinogram, Map &result, const float center) const;

  /// Adding one sinogram line to the reconstruction.
  void addLine(Line &sinoline, Map &result, const float Theta, const float center) const;

  /// \brief Finalize the calculations.
  ///
  /// The result of the reconstruction procedure is not normalized to represent
  /// the real physical values because the CT algorithm, for the simplicity does not know
  /// all parameters needed. This function will do the normalization of the result array.
  /// If the array was reconstructed using the CTrec::reconstruct() or CTrec::addLine() methods,
  /// with the correct physical values on input, after the normalization it will represent
  /// correct values of:
  /// \f$\mu\f$ - for the Contrast::ABS
  /// \f$\delta\f$ - for the Contrast::PHS and Contrast::REF.
  ///
  /// @param result The array to be normalized
  /// @param thetas Number of projections used in the CT reconstruction.
  /// @param pixelSize Physical size of the pixel
  ///
  static void finilize(Map & result, int thetas, float pixelSize=1.0);

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
