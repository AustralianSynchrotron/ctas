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
  mutable CMap mid;             ///< Internally used array for the zero-padded data.
                                // Also used as the indicator of the need to process.

  static cl_program oclProgram;
  static pthread_mutex_t protectProgramCompilation;
  mutable CLmem clmid;                 ///< Internally used array for the zero-padded data.
  CLkernel kernelApplyAbsFilter;
  CLkernel kernelApplyPhsFilter;
  CLkernel kernelApply00;
  clfftPlanHandle clfft_plan;
  CLmem clfftTmpBuff;
  cl_int clfftExec(clfftDirection dir) const;

  /* no OPENCL_FOUND
  Map phsFilter;                ///< FFT filter used for the extraction of the PHS component.
  Map absFilter;                ///< FFT filter used for the extraction of the ABS component.
  fftwf_plan fft_f;             ///< Forward 2D FFT plan.
  fftwf_plan fft_b;             ///< Backward 2D FFT plan
  */

public:

  /// Constructor.
  ///
  /// @param _sh Shape of the input contrasts.
  /// @param d2b Ratio of the (\f$\delta/\beta\f$). Must be supplied multiplied by
  ///            M_PI * dist * lambda / dd^2
  IPCprocess(const Shape & _sh, float _d2b);

  /// Destructor.
  ~IPCprocess();

  const Shape sh;                     ///< Shape of the input contrasts.
  const float d2b;

  /// The component extraction.
  ///
  /// Extracts the requested contrast component using the parameters given at the initialization.
  ///
  /// @param in Input array with the intensity distribution. Note: if you want to do the trivial
  ///           absorption correction (I/I_0) do it in prior.
  /// @param out Resulting array.
  /// @param comp Component to be extracted.
  /// @param param ABS: dgamma \f$\gamma\f$ parameter of the BAC method (theoretically must be 1.0).
  ///              PHS: multiplier missing for the physical correct values (theoretically (dd/2.0*M_PI)^2/dist ).
  ///
  /// @note the arrays ::in and ::out must not be the same.
  void extract(const Map & in, Map & out, Component comp, const float param=1.0) const ;

  void extract(Map & io, Component comp, const float param=1.0) const {
    extract(io, io, comp, param);
  }

  static float coeff (float _d2b, float _dd, float _dist, float _lambda) {
    if ( _d2b < 0.0 || _dist <= 0.0 || _lambda <= 0.0 )
      return 1.0;
    float ipccoef = _dd * _dd / (4*M_PI*M_PI * _dist);
    if ( _d2b != 0.0 )
      ipccoef *= 4 * M_PI / (_lambda * _d2b);
    return ipccoef;
  }

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
  default : throw_error("IPC component name", "Unknown component."); return "";
  }
}



const std::string
d2bOptionDesc = "delta/beta ratio (0 for no absorption).";




/// @}


#endif // _H_IPC_H_
