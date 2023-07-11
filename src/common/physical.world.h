#ifndef PHYSICS_H
#define PHYSICS_H

#include "common.world.h"
#include "matrix.world.h"


/// \brief Type of the input contrast.
///
/// Affects the behaviour of the reconstruction algorithm. See the CTRec class for
/// more details.
///
/// When you need to include your own type of contrast follow these steps:
/// -# Add your contrast to the Contrast::Cntype enumeration.
/// -# Add your contrast to the Contrast::Contrast(const std::string &) constructor
/// -# Add your contrast to the Contrast::name() method.
/// -# Update the ContrastOptionDesc string as appropriate.
class  Contrast {

public:
  /// Known contrast types.
  typedef enum {
    ABS,						///< Absorption
    REF,						///< Refraction
    PHS,                        ///< Phase
    FLT                         ///< Pre-filtered: ready for backprojection, no filtering needed.
  } Cntype ;

private:
  static const std::string modname;	///< Module name.
  Cntype _contrast;				///< Type of the contrast.

public:

  Contrast(Cntype _cn=FLT);		  ///< Constructor from contrast type.
  Contrast(const std::string & _name); ///< Constructor from name.

  Cntype contrast() const;		///< Contrast type.
  std::string name() const;			///< Name of the contrast type.

  static const std::string Desc; ///< Description of the option giving the contrast type.

};

/// \brief Compare contrasts.
///
/// @param a first contrast.
/// @param b second contrast.
///
/// @return \c true if the contrasts are equal, \c false otherwise.
///
bool
operator==(const Contrast & a, const Contrast & b);

/// \brief Compare contrasts.
///
/// @param a first contrast.
/// @param b second contrast.
///
/// @return \c false if the contrasts are equal, \c true otherwise.
///
bool
operator!=(const Contrast & a, const Contrast & b);

/// \brief Prints type name.
/// To be used in the CLI parsing via "poptmx" library
/// @return type name.
std::string
type_desc (Contrast*);

/// \brief Converts the string "in" into the Contrast _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c 1 if success, \c -1 otherwise.
int
_conversion (Contrast* _val, const std::string & in);

/// converts absorption contrast into the integrated attentuation
/// -log(I/I_0). Checks for zero and sub-zero values.
void deAbs(Map & arr);



/// Description of the option giving the rotation center.
extern const std::string
CenterOptionDesc;

/// Description of the --resolution option used in many modules.
extern const std::string
ResolutionOptionDesc;

/// The note added to many options.
extern const std::string
NeedForQuant;







#endif // PHYSICS_H
