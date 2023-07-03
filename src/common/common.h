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
#include <deque>
#include <vector>
#include <unordered_map>
#include <deque>
#include <algorithm>
#include <math.h>

#define BZ_THREADSAFE
//#define BZ_DEBUG_LOG_REFERENCES
#include "../blitz-long/blitz/array.h"
//#include <blitz/array.h>

#ifdef _WIN32
#define NAN numeric_limits<float>::quiet_NaN();
static inline int fisnan(double x){ return _isnan(x); }
static inline int fisok(double x){ return _isnormal(x) || _fpclassify(x) == FP_ZERO; }
#else
// without optimize attribute the comparison to FP_ZERO gives true for x=NaN on GCC with optimization. WTF?!!!
__attribute__((optimize("O0")))
static inline bool fisok(double x){ return std::isnormal(x) || ( std::fpclassify(x) == FP_ZERO ) ; }
#endif


inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v") {
  return s.erase(0, s.find_first_not_of(t));
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v"){
  return s.erase(s.find_last_not_of(t) + 1);
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v"){
  return ltrim(rtrim(s, t), t);
}

inline std::string trim(const std::string& s, const char* t = " \t\n\r\f\v"){
  std::string ss = s;
  return ltrim(rtrim(ss, t), t);
}

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
//void prdn(const std::string fmt = std::string(), ...);
template<class T> void prdn( const T & val ) { prdn(toString(val)); } ;


template<class T>
std::ostream & operator<<(std::ostream & o, const std::vector<T> & x) {
  o << '[';
  for (T val: x)
    o << val << ' ';
  o << ']';
  return o;
}


std::deque<std::string>
split (const std::string & str, const std::string & delimiter);




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
  mutable bool reported;

public:
  CtasErr(ErrTp _terr, const std::string & mod, const std::string & msg);
  //~CtasErr() {report();};
  ErrTp type() const;			///< Returns error type.
  void report() const;		    ///< Reports the error to the ctderr.
  std::string & consume();
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
  std::string ext () const; ///< Extracts the extension.
  std::string name () const;	///< Extracts file name.
  std::vector<std::string> elements () const;

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



typedef blitz::MyIndexType ArrIndex;


template<class T1, class T2, int N>
inline bool operator==( const blitz::TinyVector<T1,N> & t1
                      , const blitz::TinyVector<T2,N> & t2) {
  for (int dim=0; dim<N; dim++)
    if (t1[dim]!=t2[dim])
      return false;
  return true;
}

template<class T1, class T2, int N>
inline bool operator!=( const blitz::TinyVector<T1,N> & t1
                      , const blitz::TinyVector<T2,N> & t2) {
  return ! (t1 == t2);
}

template<int N>
inline std::string toString (const blitz::TinyVector<ArrIndex,N> & shp) {
  std::string toRet("{");
  for (int dim = 0; dim < N; ++dim)
    toRet += toString(shp[dim]) + (dim == N-1 ? "" : ", ");
  toRet += "}";
  return toRet;
}


template<int N>
struct Shape
  : public blitz::TinyVector<ArrIndex,N>
{
  Shape() : blitz::TinyVector<ArrIndex,N>() { for (int dim = 0; dim < N; ++dim) (*this)[dim] = 0; }
  Shape(ArrIndex hght, ArrIndex wdth) : blitz::TinyVector<ArrIndex,2>(hght,wdth) {}
  Shape(ArrIndex dpth, ArrIndex hght, ArrIndex wdth) : blitz::TinyVector<ArrIndex,3>(dpth,hght,wdth) {}
  template <class T>
  Shape(const blitz::TinyVector<T,N> & other) : blitz::TinyVector<ArrIndex,N>(other) {}
  template <class T>
  Shape& operator=(const blitz::TinyVector<T,N> & other) {
    for (int dim = 0; dim < N; ++dim)
      (*this)[dim] = other[dim];
    return *this;
  }
  // above template does not instantinate automatically to produce below assign operator
  Shape& operator=(const Shape & other) { return this->operator=<ArrIndex>(other); }
};

inline size_t area(const Shape<2> & sh) {
  return sh(0) * sh(1) ;
}

inline Shape<2> faceShape(const Shape<3> & sh) {
  return Shape<2>(sh(1), sh(2));
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
static const Map defaultMap;

/// \brief 3D Array with data.
///
/// Three dimensional array of the ::float elements.
typedef blitz::Array<float,3> Volume;

const blitz::Range all = blitz::Range::all();

inline blitz::Range dstR(size_t size, ssize_t shift) {
  return blitz::Range(std::max(0l, shift), size - 1 + std::min(0l, shift));
}

inline blitz::Range srcR(size_t size, ssize_t shift) {
  return blitz::Range(std::max(0l, -shift), size - 1 + std::min(0l, -shift));
}

#define dstRa(size, shift0, shift1) \
dstR(size(0), shift0), dstR(size(1), shift1)
#define srcRa(size, shift0, shift1) \
    srcR(size(0), shift0), srcR(size(1), shift1)

static blitz::firstIndex  i0;
static blitz::secondIndex i1;


// Returns the array which has elements stored continiously and contiguosly.
// That is data() method can be used safely. If input array is safe, it is
// returned as is, otherwise safe copy of it is returned.
template<class T, int N> const blitz::Array<T,N>
safe(const blitz::Array<T,N> & arr, bool preserve=true){
  if ( ! arr.size() )
    return arr;
  bool retRef = arr.isStorageContiguous();
  int previousStride = 1;
  for (int dim = N-1 ; dim >= 0 ; dim--) {
    retRef  &=  arr.stride(dim) == previousStride;
    previousStride *= arr.extent(dim);
  }
  if (retRef)
    return arr;
  blitz::Array<T,N> retArr(arr.shape());
  if (preserve)
    retArr = arr;
  return retArr;
}

template<class T, int N>
inline bool areSame(const blitz::Array<T,N> & arr1,
                    const blitz::Array<T,N> & arr2) {
  return arr1.data()   == arr2.data()   &&
         arr1.stride() == arr2.stride() &&
         arr1.shape()  == arr2.shape();
}



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

/// converts absorption contrast into the integrated attentuation
/// -log(I/I_0). Checks for zero and sub-zero values.
void deAbs(Map & arr);




struct PointF2D {
  float x;      ///< X coordinate
  float y;       ///< Y coordinate

  inline PointF2D(float _x=0, float _y=0)
    : x(_x), y(_y)
  {}

  inline PointF2D & operator+=(const PointF2D & rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  inline PointF2D & operator-=(const PointF2D & rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

};

inline bool operator==( const PointF2D & p1, const PointF2D & p2){
  return p1.x == p2.x && p1.y == p2.y;
}

inline bool operator!=( const PointF2D & p1, const PointF2D & p2){
  return p1.x != p2.x || p1.y != p2.y;
}



std::string
inline type_desc (PointF2D*) {
  return "FLOAT:FLOAT";
}

int
inline _conversion (PointF2D* _val, const std::string & in) {
  float x, y;
  if ( sscanf( in.c_str(), "%f:%f", &x, &y) != 2  &&
       sscanf( in.c_str(), "%f,%f", &x, &y) != 2 )
    return -1;
  *_val = PointF2D(x, y);
  return 1;
}








struct Crop {
  unsigned int top;       ///< Crop from top
  unsigned int left;      ///< Crop from left
  unsigned int bottom;     ///< Crop from bottom
  unsigned int right;     ///< Crop from right
  inline Crop(unsigned int t=0, unsigned int l=0, unsigned int b=0, unsigned int r=0)
    : top(t), left(l), bottom(b), right(r) {}
  explicit operator bool() const {
    return top || left || bottom || right;
  }
  inline unsigned int operator()(unsigned dim, unsigned edge) const {
    switch (dim) {
    case 0: return edge ? bottom : top  ;
    case 1: return edge ? right  : left ;
    default: throw_error("Crop", "Invalid dimension "+toString(dim)+".");
    }
    return 0;
  }
  inline unsigned int operator()(unsigned dim) const {
    switch (dim) {
    case 0: return bottom + top  ;
    case 1: return right  + left ;
    default: throw_error("Crop", "Invalid dimension "+toString(dim)+".");
    }
    return 0;
  }
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

/// \brief Crop the array by returning the reference to the portion of original array.
///
/// @param io_arr Input/output array.
/// @param crop Crop resulting image
///
const Map COMMON_API
crop(const Map & iarr, const Crop & crp);



inline Shape<2>
crop(const Shape<2> & sh, const Crop & crp) {
  const Shape<2> csz(sh(0) - crp.top  - crp.bottom, sh(1) - crp.left - crp.right);
  if ( csz(0) <= 0 || csz(1) <= 0 )
    throw_error("Crop", "Cropping is larger than the shape of input volume.");
  return csz;
}








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

  inline operator Crop() const {
    return Crop(top, left, bottom, right);
  }

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


/// \brief Crop3 the array.
///
/// @param inarr Input array.
/// @param outarr Output array.
/// @param crop Crop3 resulting image
///
inline void COMMON_API
crop(const Volume & inarr, Volume & outarr, const Crop & crp) {
  Crop3 crp3(crp);
  crop(inarr, outarr, crp3);
}

/// \brief Crop3 the array.
///
/// @param io_arr Input/output array.
/// @param crop Crop3 resulting image
///
inline void COMMON_API
crop(Volume & io_arr, const Crop & crp) {
  Crop3 crp3(crp);
  crop(io_arr, crp3);
}
















struct Binn {
  uint x;      ///< X binning
  uint y;       ///< Y binning
  inline Binn(uint _x=1, uint _y=1)
    : x(_x), y(_y)
  {}
  explicit operator bool() const {
    return x != 1 || y != 1;
  }
  explicit operator std::string() const {
    return x == y ? toString(x) : toString ("%ux%u", x, y);
  }
};

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


class BinnProc {
private:
  void * guts;
public:
  BinnProc(const Shape<2> & ish, const Binn & bnn);
  ~BinnProc();
  void operator() (const Map & imap, Map & omap);
};

inline uint binn(uint sz, uint bn) {
  //return bn ? (sz + bn - 1) / bn : 1;
  return bn ? sz/bn : 1 ;
}

inline Shape<2> binn(const Shape<2> & sh, const Binn & bnn) {
  return Shape<2>(binn(sh(0),bnn.y), binn(sh(1),bnn.x) );
}

inline void binn(const Map & imap, Map & omap, const Binn & bnn) {
  BinnProc(imap.shape(), bnn)(imap, omap);
}

inline void binn(Map & iomap, const Binn & bnn) {
  BinnProc(iomap.shape(), bnn)(iomap, iomap);
}







struct Binn3 {
  unsigned int x;      ///< X binning
  unsigned int y;       ///< Y binning
  unsigned int z;       ///< Z binning
  inline Binn3(unsigned int _x=1, unsigned int _y=1, unsigned int _z=1)
    : x(_x), y(_y), z(_z) {}
  inline operator Binn() const {
    return Binn(x,y);
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




Shape<2> rotate(const Shape<2> & sh, float angle);

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

public:

  ProgressBar(bool _showme=false, const std::string & _message="", int _steps=0);
  ~ProgressBar() {pthread_mutex_destroy(&proglock);}

  void setSteps(int _steps);
  void start();
  void update(int curstep=0);	///< Updates the progress bar
  void done();					///< Finalizes the progress bar.
  bool isShown() const {return showme;}

};


extern const std::string SliceOptionDesc;

extern const std::string DimSliceOptionDesc;


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
std::deque<int> COMMON_API
slice_str2vec(const std::string & sliceS, int hight);


#include "parallel.world.h"
#include "external.world.h"


#endif // _H_CTAS_H_
