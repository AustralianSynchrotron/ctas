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


/// @file
/// @author antonmx <antonmx@gmail.com>
/// @date   Thu Jul 17 10:29:40 2008
///
/// @brief  Header file with common functions and types
///
/// This file contains the declarations of the functions, types,
/// classes to be used in most of the project routines.




#ifndef _H_CTAS_H_
#define _H_CTAS_H_

#ifdef _WIN32

#  pragma warning(disable: 4251)
#  ifdef COMMON_EXPORTS
#    define COMMON_API __declspec(dllexport)
#  else
#    define COMMON_API __declspec(dllimport)
#  endif

#else

#  include "config.h"
#  define COMMON_API

#endif

#include <string>
#include <vector>
#include <math.h>
#include "../blitz-long/blitz/array.h"
//#include <blitz/array.h>

#ifdef _WIN32
#define NAN numeric_limits<float>::quiet_NaN();
static inline int fisnan(double x){ return _isnan(x); }
static inline int fisok(double x){ return _isnormal(x) || _fpclassify(x) == FP_ZERO; }
#else
// without optimize attribute the comparison to FP_ZERO gives true for x=NaN on GCC with optimization. WTF?!!!
__attribute__((optimize("O0"))) static inline bool fisok(double x){ return std::isnormal(x) || ( std::fpclassify(x) == FP_ZERO ) ; }
#endif


/// \brief Convert string to upper case
///
/// @param str Input string.
///
/// @return New string which represents input string converted into the upper case.
///
std::string COMMON_API
upper(std::string str);

/// \brief Convert string to lower case
///
/// @param str Input string.
///
/// @return New string which represents input string converted into the lower case.
///
std::string COMMON_API
lower(std::string str);

/// \brief Prints formatted message to string.
///
/// Like 'sprintf', but prints into the STL std::string
///
/// @param fmt format string.
/// @param ... whatever goes into the format string.
///
/// @return New STL string with the printed expression.
///
// Don't use the reference type "const string &" here: will
// not work on Windows
std::string COMMON_API
toString(const std::string fmt, ...);


/// \brief Prints the value to string.
///
/// @param n number to be printed
///
/// @return string with printed number
///
inline std::string toString (long double   n)   { return toString("%g", n); }
/// \cond
inline std::string toString (double        n)   { return toString("%g", n); }
inline std::string toString (float         n)   { return toString("%g", n); }
inline std::string toString (long int      n)   { return toString("%i", n); }
inline std::string toString (int           n)   { return toString("%i", n); }
inline std::string toString (long unsigned n)   { return toString("%u", n); }
inline std::string toString (unsigned      n)   { return toString("%u", n); }
/// \endcond


void prdn( const std::string & str = std::string() );
template<class T> void prdn( const T & val ) { prdn(toString(val)); } ;


template<class T>
std::ostream & operator<<(std::ostream & o, const std::vector<T> & x) {
  o << '[';
  for (T val: x)
    o << val << ' ';
  o << ']';
  return o;
}




/// \defgroup Error Error handling.
/// Functions in this group are used for the
/// error parsing, printing, throwing and so on.
///
/// @{

/// Error type.
class COMMON_API CtasErr{
public:
  /// Error severity
  typedef enum {
        WARN,                        ///< Warning
        ERR,                         ///< Error
        FATAL                        ///< Fatal error
  } ErrTp ;

private:
  ErrTp terr;                   ///< Error severity
  std::string module;                ///< Name of the module where the error happened
  std::string message;               ///< The message which describes the error

public:
  inline CtasErr(ErrTp _terr, const std::string & mod, const std::string & msg);
  inline ErrTp type() const;			///< Returns error type.
  inline void report() const;		    ///< Reports the error to the ctderr.
};


/// \brief Constructs the error, reports and then throws it.
///
/// @param mod Module where the error happened.
/// @param msg Description.
///
void COMMON_API
throw_error(const std::string & mod, const std::string & msg);

/// \brief Reports and throws the bug.
///
/// Constructs the error with the message that it must never happen,
/// reports and then throws it.
///
/// @param mod Module where the error happened.
///
void COMMON_API
throw_bug(const std::string & mod);


/// \brief Constructs the warning and reports it.
///
/// @param mod Module where the error happened
/// @param msg Description
///
/// @return the error generated in warning.
///
CtasErr COMMON_API
warn(const std::string & mod, const std::string & msg);


/// \brief Constructs the error, reports and then exits.
///
/// @param mod Module where the error happened
/// @param msg Description
///
void COMMON_API
exit_on_error(const std::string & mod, const std::string & msg);

/// @}







/// \defgroup Types Common classes.
///
/// Common type definitions, classes, structures, enums and
/// their friends.
///
/// @{


/// \brief %Path in the directory structure.
///
/// This class just a wrapper around std::string. It has some additional
/// useful functions which have sense only if the string represents a path.
/// Also a lot of platform specific code is hidden here.
class COMMON_API Path : public std::string {

private:

  const static std::string DIRSEPARATOR; ///< Directory separator
  void throw_unextracted(const std::string & element) const;
  ///< throws error in extracting an element.

public:

  /// \brief Constructor from string
  /// @param str the initializing string.
  inline Path(const std::string & str = std::string()) : std::string(str) {};

  /// \brief Constructor from C-string
  /// @param str the initializing string.
  inline Path(const char *str) : std::string(str) {};

  const static Path emptypath;

  std::string drive () const;	///< Extracts drive letter (has sense on MS WIN only).
  std::string dir () const;		///< Extracts directory.
  std::string title () const;	///< Extracts title (filename without the extension).
  std::string dtitle () const;	///< Extracts title with the preceding full path.
  std::string extension () const; ///< Extracts the extension.
  std::string name () const;	///< Extracts file name.

  bool isdir() const;			///< Tells if the path definitely represents a directory.
  bool isabsolute() const;		///< Tells if the path is absolute.

  Path & bedir();				///< Makes the path to be the directory (adds DIRSEPARATOR).

  static const Path home();

};

/// \brief Prints type name.
/// To be used in the CLI parsing via "poptmx" library
/// @return type name.
std::string COMMON_API
type_desc (Path*);

/// \brief Converts the string "in" into the Path _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c 1 if success, \c -1 otherwise.
int COMMON_API
_conversion (Path* _val, const std::string & in);


/// \brief Upgrades the filename with an addition.
///
/// Prefixes the base of the filename with the addition while the
/// path to the file remains unchanged.
///
/// @param path Path to upgrade.
/// @param addthis the upgrade string.
///
/// @return itself
///
Path COMMON_API
upgrade(const Path & path, const std::string & addthis);

/// \brief Converts the mask string to the format string.
///
/// @param mask Mask string (read ::MaskDesc).
/// @param maxslice Maximum allowed slice (total number of slices).
///
/// @return The format string which ready to be used with the fmt2s() function.
///
std::string COMMON_API
mask2format(const std::string & mask, int maxslice);


/// \brief Adds the path to the filename.
///
/// If the filename is absolute then just returns it as is. If the
/// filename is relative then prefixes it with the "dir" and returns.
///
/// @param dir Path to be added.
/// @param file File name to be updated with the path.
///
/// @return Updated filename.
///
Path COMMON_API
cdpath(const Path & dir, const Path & file);






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
class COMMON_API Contrast {

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
bool COMMON_API
operator==(const Contrast & a, const Contrast & b);

/// \brief Compare contrasts.
///
/// @param a first contrast.
/// @param b second contrast.
///
/// @return \c false if the contrasts are equal, \c true otherwise.
///
bool COMMON_API
operator!=(const Contrast & a, const Contrast & b);

/// \brief Prints type name.
/// To be used in the CLI parsing via "poptmx" library
/// @return type name.
std::string COMMON_API
type_desc (Contrast*);

/// \brief Converts the string "in" into the Contrast _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c 1 if success, \c -1 otherwise.
int COMMON_API
_conversion (Contrast* _val, const std::string & in);




struct PointF2D {
  float x;      ///< X coordinate
  float y;       ///< Y coordinate
  inline PointF2D(float _x=0, float _y=0)
  : x(_x), y(_y) {}
};

inline bool operator==( const PointF2D & p1, const PointF2D & p2){
  return p1.x == p2.x && p1.y == p2.y;
}

inline bool operator!=( const PointF2D & p1, const PointF2D & p2){
  return p1.x != p2.x || p1.y != p2.y;
}


std::string
type_desc (PointF2D*) {
  return "FLOAT:FLOAT";
}

int
_conversion (PointF2D* _val, const std::string & in) {
  float x, y;
  if ( sscanf( in.c_str(), "%f:%f", &x, &y) != 2  &&
       sscanf( in.c_str(), "%f,%f", &x, &y) != 2 )
    return -1;
  *_val = PointF2D(x, y);
  return 1;
}




/// \brief 1D line with data.
///
/// One dimensional array of the ::float elements.
/// Used for filters, sinogram's lines and so on.
typedef blitz::Array<float,1> Line;

/// \brief 2D Array with data.
///
/// Two dimensional array of the ::float elements.
/// Used for sinograms, input and output images etc.
typedef blitz::Array<float,2> Map;

/// \brief 3D Array with data.
///
/// Three dimensional array of the ::float elements.
typedef blitz::Array<float,3> Volume;

const blitz::Range whole = blitz::Range::all();








/// \brief Shape of an 2D array.
typedef blitz::TinyVector<blitz::MyIndexType,2> Shape;

inline std::string toString (const Shape & shp) { return toString("%u, %u", shp(1), shp(0));}

/// \brief Compare shapes.
///
/// @param sh1 first shape.
/// @param sh2 second shape.
///
/// @return \c true if the shapes are equal, \c false otherwise.
///
inline bool
operator==( const Shape & sh1, const Shape & sh2){
  return ( sh1(0)==sh2(0)  &&  sh1(1)==sh2(1) );
}

/// \brief Compare shapes.
///
/// @param sh1 first shape.
/// @param sh2 second shape.
///
/// @return \c false if the shapes are equal, \c true otherwise.
///
inline bool
operator!=( const Shape & sh1, const Shape & sh2){
  return ( sh1(0)!=sh2(0)  ||  sh1(1)!=sh2(1) );
}

size_t area(const Shape & sh) {return sh(0) * sh(1) ;}




/// \brief Shape of an 2D array.
typedef blitz::TinyVector<blitz::MyIndexType,3> Shape3;

inline std::string toString (const Shape3 & shp) { return toString("%u, %u, %u", shp(2), shp(1), shp(0));}

/// \brief Compare shapes.
///
/// @param sh1 first shape.
/// @param sh2 second shape.
///
/// @return \c true if the shapes are equal, \c false otherwise.
///
inline bool
operator==( const Shape3 & sh1, const Shape3 & sh2){
  return ( sh1(0)==sh2(0)  &&  sh1(1)==sh2(1)  &&  sh1(2)==sh2(2) );
}

/// \brief Compare shapes.
///
/// @param sh1 first shape.
/// @param sh2 second shape.
///
/// @return \c false if the shapes are equal, \c true otherwise.
///
inline bool
operator!=( const Shape3 & sh1, const Shape3 & sh2){
  return ( sh1(0)!=sh2(0)  ||  sh1(1)!=sh2(1)  ||  sh1(2)!=sh2(2) );
}

inline Shape faceShape(const Shape3 & sh) {
  return Shape(sh(1), sh(2));
}








struct Crop {
  unsigned int top;       ///< Crop from top
  unsigned int left;      ///< Crop from left
  unsigned int bottom;     ///< Crop from bottom
  unsigned int right;     ///< Crop from right
  inline Crop(unsigned int t=0, unsigned int l=0, unsigned int b=0, unsigned int r=0)
  : top(t), left(l), bottom(b), right(r) {}
};

inline std::string toString (const Crop & crp)  {
  return toString("%ut,%ul,%ub,%ur", crp.top, crp.left, crp.bottom, crp.right);
}


/// \brief Compare crops.
///
/// @param sh1 first crop.
/// @param sh2 second crop.
///
/// @return \c true if the crops are equal, \c false otherwise.
///
inline bool
operator==( const Crop & cr1, const Crop & cr2){
  return cr1.bottom == cr2.bottom && cr1.top == cr2.top &&
         cr1.left == cr2.left && cr1.right == cr2.right;
}

/// \brief Compare crops.
///
/// @param sh1 first crop.
/// @param sh2 second crop.
///
/// @return \c false if the crops are equal, \c true otherwise.
///
inline bool
operator!=( const Crop & cr1, const Crop & cr2){
  return cr1.bottom != cr2.bottom || cr1.top != cr2.top ||
  cr1.left != cr2.left || cr1.right != cr2.right;
}

std::string COMMON_API
type_desc (Crop*);

int COMMON_API
_conversion (Crop* _val, const std::string & in);

extern const std::string COMMON_API
CropOptionDesc;

/// \brief Crop the array.
///
/// @param inarr Input array.
/// @param outarr Output array.
/// @param crop Crop resulting image
///
void COMMON_API
crop(const Map & inarr, Map & outarr, const Crop & crp);

/// \brief Crop the array.
///
/// @param io_arr Input/output array.
/// @param crop Crop resulting image
///
void COMMON_API
crop(Map & io_arr, const Crop & crp);











struct Crop3 {

  unsigned int top;       ///< Crop3 from top
  unsigned int left;      ///< Crop3 from left
  unsigned int bottom;     ///< Crop3 from bottom
  unsigned int right;     ///< Crop3 from right
  unsigned int face;     ///< Crop3 from bottom
  unsigned int back;     ///< Crop3 from right

  inline Crop3(unsigned int t=0, unsigned int l=0, unsigned int b=0, unsigned int r=0, unsigned int f=0, unsigned int k=0)
  : top(t), left(l), bottom(b), right(r) , face(f), back(k) {}

  inline Crop3(const Crop & crp, unsigned int f=0, unsigned int k=0)
  : top(crp.top), left(crp.left), bottom(crp.bottom), right(crp.right) , face(f), back(k) {}

};

inline std::string toString (const Crop3 & crp)  {
  return toString("%ut,%ul,%ub,%ur,%uf,%uk", crp.top, crp.left, crp.bottom, crp.right, crp.face, crp.back);
}


/// \brief Compare crops.
///
/// @param sh1 first crop.
/// @param sh2 second crop.
///
/// @return \c true if the crops are equal, \c false otherwise.
///
inline bool
operator==( const Crop3 & cr1, const Crop3 & cr2){
  return cr1.bottom == cr2.bottom && cr1.top == cr2.top &&
         cr1.left == cr2.left && cr1.right == cr2.right &&
         cr1.face == cr2.face && cr1.back == cr2.back;
}

/// \brief Compare crops.
///
/// @param sh1 first crop.
/// @param sh2 second crop.
///
/// @return \c false if the crops are equal, \c true otherwise.
///
inline bool
operator!=( const Crop3 & cr1, const Crop3 & cr2){
  return cr1.bottom != cr2.bottom || cr1.top != cr2.top ||
  cr1.left != cr2.left || cr1.right != cr2.right ||
  cr1.face != cr2.face || cr1.back != cr2.back;
}

std::string COMMON_API
type_desc (Crop3*);

int COMMON_API
_conversion (Crop3* _val, const std::string & in);

extern const std::string COMMON_API
Crop3OptionDesc;

/// \brief Crop3 the array.
///
/// @param inarr Input array.
/// @param outarr Output array.
/// @param crop Crop3 resulting image
///
void COMMON_API
crop(const Volume & inarr, Volume & outarr, const Crop3 & crp);

/// \brief Crop3 the array.
///
/// @param io_arr Input/output array.
/// @param crop Crop3 resulting image
///
void COMMON_API
crop(Volume & io_arr, const Crop3 & crp);


















struct Binn3 {
  unsigned int x;      ///< X binning
  unsigned int y;       ///< Y binning
  unsigned int z;       ///< Z binning
  inline Binn3(unsigned int _x=1, unsigned int _y=1, unsigned int _z=1)
  : x(_x)
  , y(_y)
  , z(_z) {
    if (x<0) exit_on_error("Binn", "Binning factor less than 0." );
  }
};

inline std::string toString (const Binn3 & bnn) {
  return bnn.x == bnn.y && bnn.x == bnn.z ? toString(bnn.x) : toString ("%ux%ux%u", bnn.x, bnn.y, bnn.z);
}

/// \brief Compare binnings.
///
/// @param bn1 first binning.
/// @param bn2 second binning.
///
/// @return \c true if the binnings are equal, \c false otherwise.
///
inline bool
operator==( const Binn3 & bn1, const Binn3 & bn2){
  return bn1.x == bn2.x && bn1.y == bn2.y && bn1.z == bn2.z;
}

/// \brief Compare binnings.
///
/// @param bn1 first binning.
/// @param bn2 second binning.
///
/// @return \c true if the binnings are not equal, \c false otherwise.
///
inline bool
operator!=( const Binn3 & bn1, const Binn3 & bn2){
  return bn1.x != bn2.x || bn1.y != bn2.y || bn1.z != bn2.z;
}


std::string COMMON_API
type_desc (Binn3*);

int COMMON_API
_conversion (Binn3* _val, const std::string & in);

extern const std::string COMMON_API
Binn3OptionDesc;

/// \brief Apply binning to the array.
///
/// @param inarr Input array.
/// @param outarr Output array.
/// @param binn Binning factor
///
void COMMON_API
binn(const Volume & inarr, Volume & outarr, const Binn3 & bnn);

/// \brief Apply binning to the array.
///
/// @param io_arr Input/output array.
/// @param binn Binning factor
///
void COMMON_API
binn(Volume & io_arr, const Binn3 & bnn);



struct Binn {
  unsigned int x;      ///< X binning
  unsigned int y;       ///< Y binning
  inline Binn(unsigned int _x=1, unsigned int _y=1)
  : x(_x), y(_y) {
    if (x<0) exit_on_error("Binn", "Binning factor less than 0." );
  }
};

inline std::string toString (const Binn & bnn) {
  return bnn.x == bnn.y ? toString(bnn.x) : toString ("%ux%u", bnn.x, bnn.y);
}

/// \brief Compare binnings.
///
/// @param bn1 first binning.
/// @param bn2 second binning.
///
/// @return \c true if the binnings are equal, \c false otherwise.
///
inline bool
operator==( const Binn & bn1, const Binn & bn2){
  return bn1.x == bn2.x && bn1.y == bn2.y;
}

/// \brief Compare binnings.
///
/// @param bn1 first binning.
/// @param bn2 second binning.
///
/// @return \c true if the binnings are not equal, \c false otherwise.
///
inline bool
operator!=( const Binn & bn1, const Binn & bn2){
  return bn1.x != bn2.x || bn1.y != bn2.y;
}


std::string COMMON_API
type_desc (Binn*);

int COMMON_API
_conversion (Binn* _val, const std::string & in);

extern const std::string COMMON_API
BinnOptionDesc;

/// \brief Apply binning to the array.
///
/// @param inarr Input array.
/// @param outarr Output array.
/// @param binn Binning factor
///
void COMMON_API
binn(const Map & inarr, Map & outarr, const Binn & bnn);

/// \brief Apply binning to the array.
///
/// @param io_arr Input/output array.
/// @param binn Binning factor
///
void COMMON_API
binn(Map & io_arr, const Binn & bnn);







/// \brief Rotate the array.
///
/// @param inarr Input array.
/// @param outarr Output array. Input and output must be different arrays.
/// @param angle Rotation angle.
/// @param bg Values for the pixels in the resulting image not existing in
///        original.
///
void COMMON_API
rotate(const Map & inarr, Map & outarr, float angle, float bg=NAN);

/// \brief Rotate the array.
///
/// @param io_arr Input/output array.
/// @param angle Rotation angle.
/// @param bg Values for the pixels in the resulting image not existing in
///        original.
///
void COMMON_API
rotate(Map & io_arr, float angle, float bg=NAN);










/// @}











/// \defgroup CommonFunctions Common tasks.
///
/// Small useful functions and constants used many times
///
/// @{


/// Description of the option giving the rotation center.
extern const std::string COMMON_API
CenterOptionDesc;


/// Description of the -i, --int option used in many modules.
extern const std::string COMMON_API
IntOptionDesc;

/// Description of the --resolution option used in many modules.
extern const std::string COMMON_API
ResolutionOptionDesc;

/// The note added to many options.
extern const std::string COMMON_API
NeedForQuant;

/// Lists all manual pages in the package. This string to be added to
/// every manual page.
extern const std::string COMMON_API
SeeAlsoList;










/// \brief CLI progress bar (PB).
///
/// This class is used to visualize a progress.
class  COMMON_API ProgressBar {

private:
  bool showme;			///< Tells if the bar should be shown.
  std::string message;		///< Description of the process.
  int steps;			///< Total number of steps in the progress.
  int step;				///< Current step.
  int waswidth;			///< The width of the terminal in the previous step
  int reservedChs;		///< Symbols for the constant part of the PB.
  std::string fmt;			///< Format string used to print the numbers.
  int getwidth();		///< Returns current terminal width.
  pthread_mutex_t proglock;

  void _update(int curstep);

public:

  ProgressBar(bool _showme=false, const std::string & _message="", int _steps=0);

  void setSteps(int _steps) {steps=_steps;}
  void update(int curstep=0);	///< Updates the progress bar
  void done();					///< Finalizes the progress bar.
  bool isShown() const {return showme;}

};


/// Removes all (sub-)zeros from the array.
///
/// @param arr the array to be processed.
///
/// @return processed input array.
///
template<int N> blitz::Array<float,N> &
unzero(blitz::Array<float,N> & arr){
  if ( (blitz::min)(arr) <= 0.0 ) {
    warn("unzero", "Minimum in the array is sub-zero."
         " This should never happen with the absorption data.");
    float mina = (blitz::max)(arr)/100.0;
    if ( mina <= 0.0 )
      throw_error ("unzero", "Bad absorption data (maximum is sub-zero).");
    float *data = arr.data();
    for (int icur = 0 ; icur < arr.size() ; icur++ ) {
      if ( *data <= mina ) *data = mina;
      data++;
    }
  }
  return arr;
}

/// Removes all (sub-)zeros from the array.
///
/// @param arr the array to be processed.
///
/// @return processed array.
///
template<int N> inline blitz::Array<float,N>
unzero(const blitz::Array<float,N> & arr){
  blitz::Array<float,N> out(arr);
  return unzero(out);
}



/// @}


/// \defgroup OCL OpenCL
///
/// Preparing the OpenCL infrastructure.
///
/// @{



#ifdef OPENCL_FOUND

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.h>

cl_device_id CL_device;
cl_context CL_context;
cl_command_queue CL_queue;

class CLmem {
    cl_mem cl;
public:
    CLmem(cl_mem _cl=0) : cl(_cl) {}
    ~CLmem() { free(); }
    cl_mem operator()() const { return cl; }
    CLmem & operator()(cl_mem _cl) { cl=_cl; return *this; }
    void free() { if (cl) clReleaseMemObject(cl); cl=0; }
};


const cl_image_format clfimage_format({CL_R, CL_FLOAT});

bool clIsInited();

cl_program initProgram(const char csrc[], size_t length, const std::string & modname);

cl_kernel createKernel(cl_program program, const std::string & name);

std::string kernelName(cl_kernel kern);

cl_int execKernel(cl_kernel kern, size_t size=1);

cl_int execKernel(cl_kernel kern, const Shape & sh);

cl_int execKernel(cl_kernel kern, const Shape3 & sh);


template <class T> void
setArg (cl_kernel kern, cl_uint arg_idx, const T & val) {
  cl_int clerr = clSetKernelArg (kern, arg_idx, sizeof(T), &val);
  if (clerr != CL_SUCCESS)
    throw_error("setArg", "Could not set argument " + toString(arg_idx) +
                         " for OpenCL kernel \"" + kernelName(kern) + "\": " + toString(clerr));
}


template <class T>
cl_mem var2cl(cl_mem_flags flag = CL_MEM_WRITE_ONLY) {
  cl_int clerr;
  cl_mem clStorage = clCreateBuffer ( CL_context, flag, sizeof(T), 0, &clerr);
  if (clerr != CL_SUCCESS)
    throw_error("OpenCL", "Could not create OpenCL buffer: " + toString(clerr) );
  return clStorage;
}


template <class T>
T cl2var(const cl_mem & buff) {
  T var;
  cl_int clerr = clEnqueueReadBuffer(CL_queue, buff, CL_TRUE, 0, sizeof(T), &var, 0, 0, 0 );
  if (clerr != CL_SUCCESS)
    throw_error("OpenCL", "Could not read OpenCL buffer: " + toString(clerr) );
  return var;
}


template <typename T>
cl_mem clAllocArray(size_t arrSize, cl_mem_flags flag=CL_MEM_READ_WRITE) {
  cl_int err;
  const size_t iStorageSize = sizeof(T) * arrSize ;
  cl_mem clStorage = clCreateBuffer ( CL_context, flag, iStorageSize, 0, &err);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not create OpenCL buffer: " + toString(err) );
  return clStorage;
}


template <typename T, int N>
cl_mem blitz2cl(const blitz::Array<T,N> & storage, cl_mem clStorage, cl_mem_flags flag=CL_MEM_READ_WRITE) {

  blitz::Array<T,N> _storage;
  if ( storage.isStorageContiguous() )
    _storage.reference(storage);
  else {
    _storage.resize(storage.shape());
    _storage=storage;
  }

  cl_int err =
    clEnqueueWriteBuffer(  CL_queue, clStorage, CL_TRUE, 0, sizeof(T) * _storage.size(), _storage.data(), 0, 0, 0);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not write OpenCL buffer: " + toString(err) );
  return clStorage;

}


template <typename T, int N>
cl_mem blitz2cl(const blitz::Array<T,N> & storage, cl_mem_flags flag=CL_MEM_READ_WRITE) {
  cl_mem clStorage = clAllocArray<T>(storage.size(), flag);
  return blitz2cl<T,N>(storage, clStorage, flag);
}


template <typename T, int N>
blitz::Array<T,N> & cl2blitz(cl_mem clbuffer, blitz::Array<T,N> & storage) {

  blitz::Array<T,N> _storage;
  if ( storage.isStorageContiguous())
    _storage.reference(storage);
  else
    _storage.resize(storage.shape());

  cl_int err = clEnqueueReadBuffer(CL_queue, clbuffer, CL_TRUE, 0,
                                   sizeof(T) * _storage.size(),
                                   _storage.data(), 0, 0, 0 );
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not read OpenCL buffer: " + toString(err) );
  if ( storage.data() != _storage.data() )
    storage = _storage;
  return storage;

}

template <typename T>
cl_int fillClArray(cl_mem clStorage, size_t size, T val) {
  cl_int clerr = clEnqueueFillBuffer(CL_queue, clStorage, &val, sizeof(T), 0, sizeof(T) * size, 0,0,0);
  if (clerr != CL_SUCCESS)
    throw_error("Fill CL buffer", "Failed to fill the buffer: " + toString(clerr));
  return clerr;
}


/*
cl_int fillClImage(cl_mem image, float val);

cl_mem clAllocFImage(const Shape sh, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem blitz2clfi(const Map & storage, cl_mem clStorage, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem blitz2clfi(const Map & storage, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem clAllocFImage(const Shape3 sh, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem blitz2clfi(const Volume & storage, cl_mem clStorage, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem blitz2clfi(const Volume & storage, cl_mem_flags flag=CL_MEM_READ_WRITE);

void clfi2blitz(cl_mem clStorage, Map & storage);

void clfi2blitz(cl_mem clStorage, Volume & storage);
*/


#endif // OPENCL_FOUND


/// @}


extern const std::string SliceOptionDesc;

/// \brief Constructs the array of slices.
///
/// Constructs the array of slices to be processed from the slice string
/// (read ::SliceOptionDesc for more details).
///
/// @param sliceS Input ::SliceOptionDesc "slice string".
/// @param hight Height of the input projection(s).
///
/// @return The vector consisting of the numbers of slices to be processed.
///         The vector is sorted and duplicates are removed.
///
std::vector<int> COMMON_API
slice_str2vec(const std::string & sliceS, int hight);


Path COMMON_API
imageFile(const std::string & filedesc);


/// \defgroup IO Image and data file handling.
///
/// Loading, saving and other possible operations with the
/// image/data -> array -> image/data conversions.
///
/// @{

/// \brief Pixel size of the image.
///
/// Calculated from the dpi (dots per inch).
///
/// @param filename Image filename.
///
/// @return pixel size in micron.
///
float  COMMON_API
PixelSize(const Path & filename);


/// \brief Image sizes.
///
/// Reads image size and returns them as the Shape vector.
///
/// @param filename Image filename.
///
/// @return Image sizes.
///
Shape  COMMON_API
ImageSizes(const Path & filename);


/// \brief Image sizes.
///
/// Read image sizes and return them as width and hight.
///
/// @param filename Image filename.
/// @param width Width of the image.
/// @param hight Height of the image.
///
void COMMON_API
ImageSizes(const Path & filename, int *width, int *hight);


/// \brief Check the shape of the image.
///
/// Throws error if the shape of the image is not equal to ::shp.
///
/// @param filename Image filename.
/// @param shp The expected shape.
///
void COMMON_API
BadShape(const Path & filename, const Shape & shp);


/// \brief Load image.
///
/// Similar to ReadImage(), but uses preopened image object to read.
///
/// @param filename Image filename.
/// @param storage  Array to load the image into.
///
void COMMON_API
ReadImage(const Path & filename, Map & storage );


/// \brief Load image checking the shape.
///
/// Same as ReadImage(const Path & , Map &), but checks the image shape
/// before loading and throws the error if the shape is different from :::shp.
///
/// @param filename Image filename.
/// @param storage  Array to load the image into.
/// @param shp The expected shape.
///
void COMMON_API
ReadImage(const Path & filename, Map & storage, const Shape & shp);


/// \brief Load one line of the image.
///
/// Reads one line of the image from the file, resizes the array
/// to fit and stores the line into the array.
///
/// @param filename File to read the image from.
/// @param storage  Array to load the image into.
/// @param idx  The index of the line to be read.
///
void COMMON_API
ReadImageLine(const Path & filename, Line & storage, int idx);


/// \brief Load one line of the image.
///
/// Same as ReadImageLine(const Path &, Line &, int),
/// but checks the image shape before loading and throws the error if the
/// shape is different from ::shp.
///
/// @param filename File to read the image from.
/// @param storage  Array to load the image into.
/// @param idx  The index of the line to be read.
/// @param shp The expected shape.
///
void COMMON_API
ReadImageLine(const Path & filename, Line & storage, int idx, const Shape & shp);


/// \brief Load several lines of the image.
///
/// Reads lines of the image which are found in the ::idxs vector.
/// If a line in the ::idxs is outside the image size then warns and
/// fills the corresponding line with zeros. The lines are stored in
/// the order they found in the ::idx vector. Array ::storage is resized
/// to contain all lines. If the ::idxs array is empty - reads the whole image.
///
/// @param filename File to read the image from.
/// @param storage  Array to load the image into.
/// @param idxs vector of lines to be read. If empty, reads the whole image.
///
void COMMON_API
ReadImageLine(const Path & filename, Map & storage,
        const std::vector<int> & idxs);


/// \brief Load several lines of the image.
///
/// Same as ReadImageLine(const Path &, Map &, const std::vector<int> &),
/// but checks the image shape before loading and throws the error if the
/// shape is different from ::shp.
///
/// @param filename File to read the image from.
/// @param storage  Array to load the image into.
/// @param idxs vector of lines to be read. If empty - reads the whole image.
/// @param shp The expected shape.
///
void COMMON_API
ReadImageLine(const Path & filename, Map & storage,
        const std::vector<int> & idxs, const Shape & shp);




/// Description of the output result mask.
extern const std::string COMMON_API MaskDesc;

void COMMON_API
ReadVolume(const std::vector<Path> & filelist, Volume & storage, bool verbose=false);

inline void COMMON_API
ReadVolume(const Path & filename, Volume & storage, bool verbose=false) {
  ReadVolume(std::vector<Path>(1, filename), storage, verbose );
}



/// \brief Save the array into image.
///
/// Stores the array in the float-point TIFF or integer image depending.
/// on requested type. Be careful: limited number
/// of editors/viewers/analyzers support float-point format.
///
/// @param filename the name of the image file image.
/// @param storage the array to be written to the image.
/// @param saveint save image as 16-bit integer.
///
void COMMON_API
SaveImage(const Path & filename, const Map & storage, bool saveint=false);


/// \brief Save the array into integer image.
///
/// Stores the array in the integer-based image. If minval is equal to maxval
/// then the minimum and maximum values of the array data corresponds to black
/// and white respectively.
///
/// @param filename the name of the image file image.
/// @param storage the array to be written to the image.
/// @param minval the value corresponding to black.
/// @param maxval the value corresponding to white.
///
void COMMON_API
SaveImage(const Path & filename, const Map & storage,
    float minval, float maxval );


/// \brief Loads any amount of lines from data file.
///
/// @param filename the name of the data file.
/// @param ... Any number of the lines. Be careful:
/// I supposed that all arguments are of type const Line*,
/// a null pointer must be passed as the last such argument.
///
// Don't use the reference type "const Path &" here: will
// not work on Windows
void COMMON_API
LoadData(const Path filename, ... );


/// \brief Save any amount of lines into data file.
///
/// @param filename the name of the data file.
/// @param ... Any number of the lines. Be careful:
/// I supposed that all arguments are of type const Line*,
/// a null pointer must be passed as the last such argument.
///
// Don't use the reference type "const Path &" here: will
// not work on Windows
void COMMON_API
SaveData(const Path filename, ... );

/// @}



/// \brief Number of threads for the process.
///
/// @param _threads Requested number of threads (0 for auto).
///
/// @return Number of threads for the architecture where the process is running
/// if automatic number of threads was requested and just _threads if set in stone.
///
long COMMON_API
nof_threads(long _threads=0);






/// \brief Base class used for multithreading.
///
/// This virtual class helps to ease the implementation of multithread parallel computation.
/// The subclasses must implement only three entities:
/// 1. Data members - to store and make accessible all the data needed for threads to work.
/// 2. Constructor which initializes the data and, if needed, deconstructor to clean up.
/// 3. Pure virtual method bool inThread(long int).
///    This method is called from threads with the index to work on as the input.
///    It must return false if there left nothing to do in the queue and true otherwise.
///
/// Once it is done the execute() method can be used on the subclass to run the tasks.
/// Two static execute(...) methods are here to implement even easier computation for the cases
/// where no data are required and/or the index is not used for the inThread implementation:
/// just provide the input _thread_routine function.
class InThread {

private:

  virtual bool inThread(long int) = 0;

  static bool inThread(void * args, long int idx) {
    if (!args)
      throw_error("InThread", "Wrongly used class. Report to developers.");
    return ((InThread*)args)->inThread(idx);
  }

public:

  ProgressBar bar;

  InThread(bool verbose=false, const std::string procName = std::string(), int steps=0)
    : bar(verbose, procName, steps)
  {}

  void execute();
  static void execute( bool (*_thread_routine) (long int) );
  static void execute( bool (*_thread_routine) () );

};



#endif // _H_CTAS_H_
