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
/// @brief  Header file with common classes needed in the EDEI processing.
///
/// Have to say that the module with the EDEI processing (this file and deilib.cpp)
/// is the ugliest in the package. I do not know if it is my fault, or the nature of
/// algorithm, but this is the fact.
/// 


#ifndef _H_EDEI_H_
#define _H_EDEI_H_

#include "../common/common.h"
#include "../common/poptmx.h"



#ifdef _WIN32

//#pragma warning(disable: 4251)
#ifdef LIBEDEI_EXPORTS
#define EDEI_API __declspec(dllexport)
#else
#define EDEI_API __declspec(dllimport)
#endif

#else

#define EDEI_API

#endif

 
/// \defgroup edeiedei EDEI processing.
/// Functions in this group are used to process \RC and/or \FD data files
/// and extract the components from two  image contrasts via the EDEI method
/// @{




/// \brief Auxiliary class: just wrap around inter_tp enumeration.
class EDEI_API Interp {

public:

  /// \brief Determines the meaning of the input +/- points in the EDEIprocess::mpinter.
  enum inter_tp {
	ALPHA,                 ///< Angle \f$\alpha\f$ (x-axis).
	NUMBER,                ///< Number of the string in the \RC file.
	REFLECT,               ///< Reflectivity (y-axis).
	INTENS                 ///< Intensity (y-axis).
  };

private:
  inter_tp mpinter;				///< The interpretation.

public:

  static const std::string optionDesc; ///< Description of the type.

  std::string name() const ;			///< Prints name.
  inter_tp type() const;		///< The interpretation type.

  Interp(inter_tp _mpinter=REFLECT); ///< Constructor.
  Interp(const std::string & _name);	///< Constructor from name.

};




/// \brief Compare interpretations.
///
/// @param a first Interp.
/// @param b second Interp.
///
/// @return \c true if the interpretations are equal, \c false otherwise.
///
bool EDEI_API
operator==(const Interp & a, const Interp & b);

/// \brief Compare interpretations.
///
/// @param a first Interp.
/// @param b second Interp.
///
/// @return \c false if the interpretations are equal, \c true otherwise.
///
bool EDEI_API
operator!=(const Interp & a, const Interp & b);


/// Description of the option giving the Interp type.
extern EDEI_API const std::string InterpOptionDesc;

/// \brief Prints type name.
/// To be used in the CLI parsing via "poptmx" library
/// @return type name.
EDEI_API std::string
type_desc (Interp*);

/// \brief Converts the string "in" into the Interp _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
EDEI_API bool
_conversion (Interp* _val, const std::string & in);





/// Structure with the elements needed for the initialization of the ::EDEIprocess
struct EDEI_API EDEIoptions {

  Path RCname;                  ///< Name of the \RC data file.
  Path FDname;                  ///< Name of the \FD data file.
  float Gm;                     ///< Value determining the minus position.
  float Gp;                     ///< Value determining the plus position.
  Interp mpinter;               ///< Interpretation of the ::Gm, ::Gp.
  bool smooth;                  ///< Smooth RC to avoid non-mono regions.
  float acof;                   ///< X-axis (angle) conversion factor.

  const static std::string rcOptionDesc; ///< Description of the RC option.
  const static std::string fdOptionDesc; ///< Description of the FD option.
  const static std::string rcOptionShortDesc; ///< Short description of the RC option.
  const static std::string fdOptionShortDesc; ///< Short description of the FD option.

  EDEIoptions() : Gm(0.5), Gp(0.5), mpinter(Interp::REFLECT),
                  smooth(false), acof(1.0) {}; ///< Constructor.

  poptmx::OptionTable options();   ///< Option table.


};


/// \brief Store, process \RC, \FD, extract contrasts.
///
/// This class is used for loading, storing, saving, processing
/// the rocking curve of the analyzer from the point
/// of view of the EDEI method.
/// 
/// Also member functions of this class perform EDEI processing
/// of the contrasts basing on the preloaded \RC, \FD.
///
/// Basically the usage of this class assumes following steps:
/// - Setting parameters (all non-const public variables). It can be done
///   either by hand or via the command line options using POPT_TABLERC()
///   option table parsed by the libpopt.
/// - Loading \RC (loadRC()) or \FD (loadFD()).
/// - If you need to convert \FD to \RC just call to saveFD()
///   (see rc2fd.cpp for example).
/// - Extract contrast(s) via refract() and/or absorb().
class EDEI_API EDEIprocess {

private:

  static const std::string modname;	///< Module name.
  static const int function_size = 65535; ///< Size of the ::AL and ::PM arrays.

  mutable Line AL;              ///< \brief Array \f$\alpha(F)\f$
  ///< \f[ \alpha = F^{-1}\left( \frac{I_L-I_H}{I_L+I_H} \right) \f]
  mutable Line PM;              ///< Array \f$ I_L+I_H \f$

  void loadRC(const Path & filename) const; ///< Loads RC.

  float  Gm;            ///< Value determining the minus position.
  float  Gp;            ///< Value determining the plus position.
  bool smooth;                   ///< Smooth RC to avoid non-mono regions.
  Interp mpinter;               ///< Interpretation of the ::Gm, ::Gp values.
  float acof;                   ///< Multiplication of this coefficient by the
  ///< value in the first column of the \RC gives actual angle in radian.

public:

  /// Two contrast components available in the EDEI experiment.
  typedef enum {
    ABS,                        ///< Apparent absorption (abs+small-angle scat.).
    REF                         ///< Clean refraction.
  }  Component ;

  static const std::string componentDesc;  ///< Description of the ::Component.

  /// Constructor.
  EDEIprocess(const Path & RCname, float _Gm, float _Gp, Interp _mpinter,
               bool _smooth, float _acof);

  void saveFD(const Path & filename) const; ///< Save \FD

  float extract(float minus, float plus, Component comp) const ;

  template<int N_rank> void
    extract (const blitz::Array<float, N_rank> & minus,
             const blitz::Array<float, N_rank> & plus,
             Component comp,
             blitz::Array<float, N_rank> & out) const;

};

/// \brief Prints type name.
/// To be used in the CLI parsing via "poptmx" library
/// @return type name.
inline std::string
type_desc (EDEIprocess::Component*){
  return "STRING";
}

/// \brief Converts the string "in" into the EDEIprocess::Component _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
bool EDEI_API
_conversion (EDEIprocess::Component* _val, const std::string & in);

inline std::string
toString(EDEIprocess::Component comp) {
  switch (comp) {
  case EDEIprocess::ABS : return "absorption";
  case EDEIprocess::REF : return "refraction";
  default : throw_error("EDEI component name", "Unknown component.");
  }
}



/// \brief The same as ::extract, but works on array.
///
/// @param minus Array of the minus intensities.
/// @param plus Array of plus intensities.
/// @param comp Component to be extracted.
/// @param out  Array of the extracted component depending on the ::comp.
///
template<int N_rank> void
EDEIprocess::extract (const blitz::Array<float, N_rank> & minus,
                      const blitz::Array<float, N_rank> & plus,
                      Component comp,
                      blitz::Array<float, N_rank> & out) const {

  if ( minus.shape() != plus.shape() )
    throw_error(modname, "Geometry of the minus and plus"
                " arrays do not match." );
  if ( minus.shape() != out.shape() )
    out.resize(minus.shape());

  typename blitz::Array<float, N_rank>::iterator
    ocur = out.begin(), oend=out.end();
  typename blitz::Array<float, N_rank>::const_iterator
    pcur = plus.begin(), mcur = minus.begin();

  while (ocur != oend)
    *ocur++ = extract(*mcur++, *pcur++, comp);

}




/// \cond
#ifdef _H_EXPERIMENT_H_
/// \endcond

/// \ingroup experiment
/// \brief Class which extracts EDEI-processed projections from the input data.
class EDEIexp : public Experiment {
private:

  static const std::string modname;  ///< Name of the module
  AqSeries Mlist;              ///< Data acquired in minus position of the analyzer
  AqSeries Plist;              ///< Data acquired in plus position of the analyzer
  EDEIprocess proc;             ///< DEI processing
  EDEIprocess::Component comp;  ///< Component to be extracted by the ::projection.


public:

  /// Constructor
  ///
  /// @param _proc EDEI processing class.
  /// @param _comp Component to be extracted.
  /// @param _Mlist Data acquired in minus position of the analyzer
  /// @param _Plist  Data acquired in plus position of the analyzer
  ///
  inline EDEIexp(const EDEIprocess & _proc, EDEIprocess::Component _comp,
                 const AqSeries & _Mlist, const AqSeries & _Plist)  :
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


  inline void projection(int itheta, Map & proj,
                         const std::vector<int> & sliceV = std::vector<int>() ) const {
    Map prM(sh), prP(sh);
    Mlist.projection(itheta, prM, sliceV);
    Plist.projection(itheta, prP, sliceV);
    proc.extract( prM, prP, comp, proj);
  }

  /// Sets _comp and _contrast.
  ///
  /// @param _comp Component to be extracted.
  ///
  inline void component(EDEIprocess::Component _comp) {
    comp = _comp;
    cntr = ( comp == EDEIprocess::ABS ) ? Contrast::ABS : Contrast::REF ;
  }

};

const std::string EDEIexp::modname="EDEI experiment";

/// @}

/// \cond
#endif // _H_EXPERIMENT_H_
/// \endcond


/// @}



#endif // _H_DEI_H_
