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
 *   MERCVHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
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
/// @date   Thu Jul 17 10:29:40 2008
///
/// @brief  Header file with common classes needed in the DEI processing.
///


#ifndef _H_DEI_H_
#define _H_DEI_H_

#include "../common/ctas.h"
#include <poptmx.h>



#ifdef _WIN32

//#pragma warning(disable: 4251)
#ifdef LIBDEI_EXPORTS
#define DEI_API __declspec(dllexport)
#else
#define DEI_API __declspec(dllimport)
#endif

#else

#define DEI_API

#endif


/// \defgroup deiedei DEI processing.
/// Functions in this group are used extract the components from two
/// image contrasts via the DEI method
/// @{

/// Structure with the elements needed for the initialization of the ::DEIprocess
struct DEIoptions {
  float  Rm;            ///< Reflectivity in the minus position.
  float  Rp;            ///< Reflectivity in the plus position.
  float  Gm;            ///< Derivative of the RC in the minus position.
  float  Gp;            ///< Derivative of the RC in the plus position.

  DEIoptions() : Rm(0.5), Rp(0.5), Gm(1.0), Gp(-1.0) {}; ///< Constructor.

  /// Constructs and returns the option table.
  ///
  /// @return Option table to be used via the poptmx library.
  ///
  poptmx::OptionTable options() {
    poptmx::OptionTable opt("DEI input parameters.",
                            "Options required for the DEI processing.");
    opt
      .add(poptmx::OPTION, &Rm,'m', "minus-rc",
           "Reflectivity in the minus position.", "", toString(Rm))
      .add(poptmx::OPTION, &Rp,'p', "plus-rc",
           "Reflectivity in the plus position.", "", toString(Rp))
      .add(poptmx::OPTION, &Gm,'M', "minus-drv",
           "Derivative of the RC in the minus position.", "", toString(Gm))
      .add(poptmx::OPTION, &Gp,'P', "plus-drv",
           "Derivative of the RC in the plus position.", "", toString(Gp));
    return opt;
  };

};




/// \brief Extract contrasts via DEI.
///
class DEI_API DEIprocess{

private:

  static const std::string modname;	///< Module name.
  const float  Rm;            ///< Reflectivity in the minus position.
  const float  Rp;            ///< Reflectivity in the plus position.
  const float  Gm;            ///< Derivative of the RC in the minus position.
  const float  Gp;            ///< Derivative of the RC in the plus position.

public:

  /// Two contrast components available in the DEI experiment.
  typedef enum {
    ABS,                        ///< Apparent absorption (abs+small-angle scat.).
    REF                         ///< Clean refraction.
  }  Component ;

  static const std::string componentDesc; ///< Description of the ::Component.

  DEIprocess(float _Gm=1.0, float _Gp=-1.0, float _Rm=0.5, float _Rp=0.5); ///< Constructor.

  float extract(float minus, float plus, Component comp) const ;

  template<int N_rank> blitz::Array<float, N_rank>
    extract(const blitz::Array<float, N_rank> & minus,
             const blitz::Array<float, N_rank> & plus,
             Component comp) const;

};

/// \brief Prints type name.
/// To be used in the CLI parsing via "poptmx" library
/// @return type name.
inline std::string
type_desc (DEIprocess::Component*){
  return "STRING";
}

/// \brief Converts the string "in" into the DEIprocess::Component _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
inline bool
_conversion (DEIprocess::Component* _val, const std::string & in) {
  std::string _in = upper(in);
  if (_in == "A" || _in == "ABS" || _in == "ABSORPTION" )
    *_val= DEIprocess::ABS;
  else if (_in == "R" || _in == "REF" || _in == "REFRACTION" )
    *_val= DEIprocess::REF;
  else
    throw_error("DEI component", "Unrecognized component\"" + _in + "\".");
  return true;
}

inline std::string
toString(DEIprocess::Component comp) {
  switch (comp) {
  case DEIprocess::ABS : return "absorption";
  case DEIprocess::REF : return "refraction";
  default : throw_error("DEI component name", "Unknown component.") ; return "" ;
  }
}





/// \brief The same as ::extract, but works on array.
///
/// @param minus Array of the minus intensities.
/// @param plus Array of plus intensities.
/// @param comp Component to be extracted.
///
/// @return Array of the extracted component
///         depending on the ::comp.
///
template<int N_rank> blitz::Array<float, N_rank>
DEIprocess::extract (const blitz::Array<float, N_rank> & minus,
                     const blitz::Array<float, N_rank> & plus,
                     Component comp) const {

  if ( minus.shape() != plus.shape() )
    throw_error(modname, "Geometry of the minus and plus"
                " arrays do not match." );

  blitz::Array<float, N_rank> out( minus.shape() );
  typename blitz::Array<float, N_rank>::iterator
    ocur = out.begin(), oend=out.end();
  typename blitz::Array<float, N_rank>::const_iterator
    pcur = plus.begin(), mcur = minus.begin();

  while (ocur != oend)
    *ocur++ = extract(*mcur++, *pcur++, comp);

  return out;
}

/// \cond
#ifdef _H_EXPERIMENT_H_
/// \endcond

/// \ingroup experiment

/// \brief Class which extracts DEI-processed projections from the input data.
class DEIexp : public Experiment {

private:

  static const std::string modname;  ///< Name of the module
  AqSeries Mlist;              ///< Data acquired in minus position of the analyzer
  AqSeries Plist;              ///< Data acquired in plus position of the analyzer
  DEIprocess proc;             ///< DEI processing
  DEIprocess::Component comp;  ///< Component to be extracted by the ::projection.

public:

  /// Constructor
  ///
  /// @param _proc DEI processing class.
  /// @param _comp Component to be extracted.
  /// @param _Mlist Data acquired in minus position of the analyzer
  /// @param _Plist  Data acquired in plus position of the analyzer
  ///
  inline DEIexp(const DEIprocess & _proc, DEIprocess::Component _comp,
                const AqSeries & _Mlist, const AqSeries & _Plist) :
    Experiment(_Mlist.shape(), _Mlist.thetas()),
    proc(_proc),
    Mlist(_Mlist),
    Plist(_Plist)
  {
    if ( Mlist.thetas() != Plist.thetas() )
      throw_error(modname, "Different sizes of the data aquired in minus and plus positions.");
    if ( Mlist.shape() != Plist.shape() )
      throw_error(modname, "Different shapes of the data aquired in minus and plus positions.");
    component(_comp);
  }

  inline void projection( int itheta, Map & proj,
                          const std::vector<int> & sliceV,
                          float angle=0, const Crop &crop = Crop() ) const {
    Map prM(sh), prP(sh);
    Mlist.projection(itheta, prM, sliceV, angle, crop);
    Plist.projection(itheta, prP, sliceV, angle, crop);
    proj = proc.extract( prM, prP, comp);
  }

  /// Sets _comp and _contrast.
  ///
  /// @param _comp Component to be extracted.
  ///
  inline void component(DEIprocess::Component _comp) {
    comp = _comp;
    cntr = ( comp == DEIprocess::ABS ) ? Contrast::ABS : Contrast::REF ;
  }

};

const std::string DEIexp::modname="DEI experiment";

/// \cond
#endif // _H_EXPERIMENT_H_
/// \endcond


/// @}



#endif // _H_DEI_H_
