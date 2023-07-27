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

#include "common.world.h"
#include "matrix.world.h"
#include "parallel.world.h"
#include "physical.world.h"

#include <fftw3.h>
#include <list>

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




pthread_mutex_t fftw_lock = PTHREAD_MUTEX_INITIALIZER;

inline fftwf_plan safe_fftwf_plan_r2r_1d(int n, float *inout, fftw_r2r_kind kind) {
  pthread_mutex_lock(&fftw_lock);
  fftwf_plan ret = fftwf_plan_r2r_1d(n, inout, inout, kind, FFTW_ESTIMATE);
  pthread_mutex_unlock(&fftw_lock);
  return ret;
}

inline void safe_fftw_destroy_plan(fftwf_plan plan) {
  pthread_mutex_lock(&fftw_lock);
  fftwf_destroy_plan(plan);
  pthread_mutex_unlock(&fftw_lock);
}


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

  static Ftype fromName(const std::string &_name);

public:

  Filter(Ftype _tp=RAMP, float _as=0)
    : filttp(_tp)
    , alsig(_as)
  {
    if (filttp == KAISER  && alsig < 0.0 )
      throw_error(modname, "Alpha parameter of Kaiser filter must be non-negative (set "+toString(alsig)+"). ");
    if (filttp == GAUSS  && alsig <= 0.0 )
      throw_error(modname, "Sigma parameter of Guass filter must be strictly positive (set "+toString(alsig)+"). ");
  }

  Filter(const std::string &_name, float _as=0)
    : Filter(fromName(_name), _as) {};  ///< Constructor from name

  inline Ftype filter() const {return filttp;}    ///< Filter
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
inline bool KERNEL_API
operator==(const Filter & a, const Filter & b) {
  return a.filter() == b.filter();
}

/// \brief Compare filters.
///
/// @param a first filter.
/// @param b second filter.
///
/// @return \c false if the filters are equal, \c true otherwise.
///
inline bool KERNEL_API
operator!=(const Filter & a, const Filter & b)  {
  return a.filter() != b.filter();
}

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



class RingFilter {
private:
  size_t box;   ///< box of the ring filter.
  Line average;
  Line temp;

public:

  RingFilter(size_t width, size_t ringBox=0)
    : box(ringBox)
    , average(ringBox ? width : 0)
    , temp(ringBox ? width : 0)
  {}

  void apply(Map & sinogram);

};





/// \brief CT reconstruction class
///
/// This class first collects all information needed for the CT reconstruction
/// (size, contrast type, filter, etc.) and then performs the reconstruction
/// using either CTrec::reconstruct or CTrec::addLine methods
///
class KERNEL_API CTrec {

private:

  static const std::string modname; ///< Module name.

  const Shape<2> ish;
  const Shape<2> osh;
  const size_t zidth;          ///< zero-padded width
  Contrast contrast;     ///< Type of the contrast.

  Filter filter;       ///< Type of the filter function.
  Line filt_window;       ///< The array containing the filter window.
  Line zsinoline;         ///< 0-padded sinogram line.
  fftwf_plan planF;       ///< Forward FFT transformation.
  fftwf_plan planB;       ///< Backward FFT transformation.
  Map mysino;
  blitz::Array<cl_float2, 1> cossins;
  float recCof;

  class ForCLdev {

    static const std::string oclsrc;
    CLenv & cl;
    const CTrec & parent;
    CLprogram program;
    CLkernel kernelSino;
    CLmem clSlice;
    CLmem clSino;
    CLmem clAngles;
    cl_float recCof;
    pthread_mutex_t locker;

    bool checkReady();

  public:

    ForCLdev(CLenv & cl, const CTrec & _parent)
      : cl(cl)
      , parent(_parent)
      , program(oclsrc, cl.cont)
      , kernelSino(program, "fbp")
      , clAngles(0)
      , clSino(0)
      , clSlice(0)
      , recCof(1.0)
      , locker(PTHREAD_MUTEX_INITIALIZER)
    {};

    ~ForCLdev() {
      pthread_mutex_destroy(&locker);
    } ;

    int reconstruct(Map & fsin, Map & slice, float center) ;

    void setRecCof(float _recCof) ;

    bool sino(Map &sinogram) ;

    bool repeat(Map & slice, float center) ;

  };

  std::list<ForCLdev>  _envs;
  std::list<ForCLdev> & envs;

  bool switchToGPU = false;
  bool useCPU = false;
  pthread_mutex_t * gpuReleasedMutex;
  pthread_cond_t * gpuReleasedCondition;
  bool _gpuWasReleased = false;
  bool & gpuWasReleased;

  void prepare_sino(Map & sinogram);
  void reconstructOnCPU(Map & slice, float center);

public:

  CTrec(const Shape<2> &sinoshape, Contrast cn, const float anarc=180, const Filter &ft=Filter());
  CTrec(CTrec & other);
  ~CTrec();

  inline Shape<2> recShape(const float center=0.0) const {
    return Shape<2>(osh(0)-2*ceil(abs(center)),osh(1));
  };

  inline Crop<2> recCrop(const float center=0.0) const {
    const float cacent = ceil(abs(center));
    return Crop<2>(Segment(cacent,-cacent), Segment());
  };

  void setPhysics(float pixelSize=0.0, float lambda=0.0);
  void sino(Map &sinogram);
  void repeat(Map & slice, float center);
  void reconstruct(Map &sinogram, Map & slice, float center);

  void doOnGPU() {switchToGPU=true;}
  void allowCPU() {useCPU=true;}

  inline static void reconstruct(Map &sinogram, Map & slice
                                 , Contrast cn, float arc=180, const Filter &ft=Filter()
                                 , float center=0, float pixelSize=0.0, float lambda=0.0) {
    CTrec rec(sinogram.shape(), cn, arc, ft);
    rec.setPhysics(pixelSize, lambda);
    rec.reconstruct(sinogram, slice, center);
  }


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







float raxis(Map & sino, const float anarc, const float maxDev, int algo=0);
float raxis(Map & proj0, Map & proj180, const float maxDev );
float raxis(Map & proj0, Map & proj180);



/// @}



#endif // _H_KERNEL_H_
