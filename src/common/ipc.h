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
/// @author antonmx <antonmx@amx-mac.synchrotron.org.au>
/// @date   Wed Jun 16 10:40:11 2010
///
/// @brief Inline phase contrast.
///
///


#ifndef _H_IPC_H_
#define _H_IPC_H_

#include "common.h"
#include <complex>
#include <fftw3.h>

#ifdef OPENCL_FOUND
#include <clFFT.h>
#endif


#ifdef _WIN32
#  pragma warning(disable: 4251)
#  ifdef LIBIPC_EXPORTS
#    define IPC_API __declspec(dllexport)
#  else
#    define IPC_API __declspec(dllimport)
#  endif

#else

//#  include "../../config.h"
#  define IPC_API

#endif

/// \brief 2D Array with complex data.
///
/// Two dimensional array of the std::complex<float> elements.
typedef blitz::Array< std::complex<float> ,2 > CMap;


/// \defgroup IPC Inline phase contrast (IPC).
///
/// Where all functions needed for the IPC processing go.
///
/// @{

/// Transfer of intensity.
///
/// Calculates the intensity distribution at the given distance from the object's
/// transmission function (usually denoted as T(x,y)). The intensity is calculated as
/// the square of the convolution of the Fresnel propagator and Transmission function.
///
/// @param tif Input object's transmission function.
/// @param out The resulting intensity distribution.
/// @param dd Pixel size.
/// @param lambda Wavelength.
/// @param dist Object-to-detector distance.
///
void IPC_API
propagate(const CMap & tif, Map & out, float dd, float lambda, float dist);


/// Simulates transmission function.
///
/// Calculates an ideal transmission function of the object consisting of three
/// enclosed spheres with different values of the complex refraction index.
///
/// @param tif The resulting array with the transmission function.
/// @param sh Sizes of the array.
/// @param bd The ration between the absorption and refraction indeces (\f$\beta/\delta\f$).
/// @param theta Rotation angle of the object.
/// @param dd Pixel size.
/// @param lambda Wavelength.
///
void IPC_API
simulateTif( CMap & tif, const Shape & sh, float bd,
             float theta, float dd, float lambda);


/// IPC processing.
///
/// This class holds all the information needed for the Inline Phase Contrast processing
/// and extracts one of the component (absorption or phase) from the input contrast.
/// The extraction is done utilizing the Modified Bronnikov Algorithm (MBA) for the phase
/// component and the Bronnikov-Aided Correction (BAC) for the absorption component.
///
class IPC_API IPCprocess{

public:

  /// Two contrast components available in the IPC experiment.
  typedef enum {
    ABS,                        ///< Absorption.
    PHS                         ///< Phase shift
  }  Component ;

  static const std::string componentDesc;  ///< Description of the ::Component.

private:

  static const std::string modname;	///< Module name.

  Shape sh;                     ///< Shape of the input contrasts.

  #ifdef OPENCL_FOUND

  static const char oclSource[]; ///< OpenCl source
  static const cl_program oclProgram; 
  
  mutable cl_mem mid;                 ///< Internally used array for the zero-padded data.
  cl_kernel kernelApplyAbsFilter;
  cl_kernel kernelApplyPhsFilter; 
  clfftPlanHandle clfft_plan;
  cl_int clfftExec(clfftDirection dir) const;

  #else // OPENCL_FOUND

  mutable CMap mid;             ///< Internally used array for the zero-padded data.
  Map phsFilter;                ///< FFT filter used for the extraction of the PHS component.
  Map absFilter;                ///< FFT filter used for the extraction of the ABS component.
  fftwf_plan fft_f;             ///< Forward 2D FFT plan.
  fftwf_plan fft_b;             ///< Backward 2D FFT plan

  #endif // OPENCL_FOUND

public:

  /// Constructor.
  ///
  /// @param _sh Shape of the input contrasts.
  /// @param alpha \f$\alpha\f$ parameter of the MBA. Theoretically of the (\f$\beta/\delta\f$) order.
  /// @param dist Object-to-detector distance.
  /// @param dd Pixel size.
  /// @param lambda Wavelength (needed only if \f$\alpha\f$!=0).
  IPCprocess(const Shape & _sh, float alpha, float dist, float dd=1.0, float lambda=1.0);

  /// Destructor.
  ~IPCprocess();

  /// The component extraction.
  ///
  /// Extracts the requested contrast component using the parameters given at the initialization.
  ///
  /// @param in Input array with the intensity distribution. Note: if you want to do the trivial
  ///           absorption correction (I/I_0) do it in prior.
  /// @param out Resulting array.
  /// @param comp Component to be extracted.
  /// @param param ABS: dgamma \f$\gamma\f$ parameter of the BAC method (theoretically must be 1.0).
  ///              PHS: alpha * lambda. 
  ///
  /// @note the arrays ::in and ::out must not be the same.
  void extract(const Map & in, Map & out, Component comp, const float param) const ;

  /// Desired shape of the input arrays.
  ///
  /// @return Predefined shape of the processing algorithm.
  ///
  inline const Shape & shape() const {return sh;}

};



/// \brief Prints type name.
/// To be used in the CLI parsing via "poptmx" library
/// @return type name.
inline std::string
type_desc (IPCprocess::Component*){
  return "STRING";
}

/// \brief Converts the string "in" into the IPCprocess::Component _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
bool IPC_API
_conversion (IPCprocess::Component* _val, const std::string & in);

inline std::string
toString(IPCprocess::Component comp) {
  switch (comp) {
  case IPCprocess::ABS : return "absorption";
  case IPCprocess::PHS : return "phase";
  default : throw_error("IPC component name", "Unknown component.");
  }
}



/// \cond
#ifdef _H_EXPERIMENT_H_
/// \endcond

/// \ingroup experiment
/// \brief Class which extracts IPC-processed projections from the input data.
class IPCexp : public Experiment {
private:

  static const std::string modname;  ///< Name of the module.
  AqSeries list0;               ///< Data acquired in the contact print plane.
  AqSeries listD;               ///< Data acquired at the distance.
  const IPCprocess & proc;             ///<  IPC processing.
  IPCprocess::Component comp; ///< Component to be extracted by the ::projection.

  float dd;                     ///< Pixel size.
  float dist;                   ///< Object-to-detector distance.
  float dgamma;                 ///< \f$\gamma\f$ parameter of the BAC

  mutable Map pr0;         ///< Some internal array used for the extraction.
  mutable Map prD;         ///< Some internal array used for the extraction.

public:

  /// Contructor.
  ///
  /// @param _listD Data acquired at the distance.
  /// @param _list0 Data acquired in the contact print plane. Can be left empty.
  /// @param _proc The IPC processor.
  /// @param _comp Component to be extracted.
  ///

  inline IPCexp(const AqSeries & _listD, const AqSeries & _list0,
                const IPCprocess & _proc, IPCprocess::Component _comp)  :
    Experiment(_listD.shape(), _listD.thetas()),
    list0(_list0),
    listD(_listD),
    proc(_proc),
    dd(1.0),
    dist(1.0),
    dgamma(0.0),
    pr0(sh),
    prD(sh)
  {
    if (list0.size()) {
      if ( list0.thetas() != thts )
        throw_error(modname, "Different sizes of the data aquired in different positions.");
      if ( list0.shape() != sh )
        throw_error(modname, "Different shapes of the data aquired in different positions.");
    }
    if ( sh != proc.shape() )
      throw_error(modname, "Different shape of the data and the processing algorithm.");
    component(_comp);
  }

  inline void projection(  int itheta, Map & proj,
                           const std::vector<int> & sliceV,
                           float angle=0, const Crop &crp = Crop() ) const {

    listD.projection(itheta, prD);

    if (list0.size()) {
      list0.projection(itheta, pr0);
      prD /= unzero(pr0);
    }

    proc.extract(prD, pr0, comp, dgamma);
    rotate(pr0, prD, angle);
    crop(prD,crp);

    proj.resize( Shape( sliceV.size() ? sliceV.size() : sh(0) , sh(1) ) );
    if (sliceV.size()) {
      for(int icur = 0 ; icur<sliceV.size() ; icur++)
        proj (icur, blitz::Range::all()) =
          prD (sliceV[icur], blitz::Range::all());
    } else {
      proj = prD;
    }

  }

  /// Sets the component to be extracted and the corresponding contrast type.
  ///
  /// @param _comp Component to be extracted.
  ///
  inline void component(IPCprocess::Component _comp) {
    comp = _comp;
    cntr = ( comp==IPCprocess::ABS ) ? Contrast::ABS : Contrast::PHS ;
  }

  /// Sets the \f$\gamma\f$.
  ///
  /// @param _dgamma \f$\gamma\f$ parameter of the BAC method (theoretically must be 1.0).
  ///
  inline void gamma(float _dgamma) {
    if ( std::abs(_dgamma)>1.0 ) // should set even smaller limit
      throw_error(modname, "Absolute value of gamma is greater than 1.0.");
    dgamma = _dgamma;
  }


};

const std::string
IPCexp::modname="IPC experiment";

/// \cond
#endif // _H_EXPERIMENT_H_
/// \endcond



/// @}


#endif // _H_IPC_H_
