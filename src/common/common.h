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
#include "../blitz-long/blitz/array.h"
//#include <blitz/array.h>

#ifdef _WIN32
#define NAN numeric_limits<float>::quiet_NaN();
static inline int isnan(double x){ return _isnan(x); }
#endif


/// \cond
#define PRDN printf("DONE\n"); fflush(stdout);
inline void prdn( int a ) { printf("DONE %i\n", a); fflush(stdout); }
/// \endcond



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
  std::string message;               ///< The message which describes the error
  std::string module;                ///< Name of the module where the error happened
  ErrTp terr;                   ///< Error severity

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

  bool isdir() const;			///< Tells if the path definitely represents the a directory.
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



struct Crop {
  unsigned int left;      ///< Crop from left
  unsigned int top;       ///< Crop from top
  unsigned int right;     ///< Crop from right
  unsigned int bottom;     ///< Crop from bottom
  inline Crop(unsigned int t=0, unsigned int l=0, unsigned int b=0, unsigned int r=0)
  : top(t), left(l), bottom(b), right(r) {}
};

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


/// \brief Rotate the array.
///
/// @param inarr Input array.
/// @param outarr Output array. Input and output must be different arrays.
/// @param angle Rotation angle.
/// @param crop Crop resulting image
/// @param bg Values for the pixels in the resulting image not existing in
///        original.
///
void rotate(const Map & inarr, Map & outarr, float angle,
            const Crop & crop = Crop(), float bg=NAN);

/// \brief Rotate selected lines of the array.
///
/// @param inarr Input array.
/// @param outarr Output array. Input and output must be different arrays.
/// @param sliceV Vector of lines to output in the rotated array.
///        The vector can be modified to leave only exclude the slices
///        out of the image boudaries.
/// @param angle Rotation angle.
/// @param crop Crop resulting image
/// @param bg Values for the pixels in the resulting image not existing in
///        original.
///
void rotateLines(const Map & inarr, Map & outarr, std::vector<int> & sliceV,
                 float angle, const Crop & crop = Crop(), float bg=NAN);



/// \brief Shape of an 2D array.
typedef blitz::TinyVector<long int,2> Shape;

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



/// @}











/// \defgroup CommonFunctions Common tasks.
///
/// Small useful functions and constants used many times
///
/// @{


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
inline std::string toString (const Shape & shp) { return toString("%u, %u", shp(1), shp(0));}
/// \endcond









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

public:

  ProgressBar(bool _showme=false, const std::string & _message="", int _steps=0);

  void update(int curstep=0);	///< Updates the progress bar
  void done();					///< Finalizes the progress bar.

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

bool clIsInited();

cl_program initProgram(char csrc[], size_t length, const std::string & modname);

template <class T> void
setArg (cl_kernel kern, cl_uint arg_idx, const T val,
        const std::string &modname) {

  if (!kern)
    throw_error(modname, "Setting arguments for invalid OpenCL kernel.");

  cl_int err =  clSetKernelArg (kern, arg_idx, sizeof(T), &val);

  if (err != CL_SUCCESS) {

    std::string errstr = "Could not set argument " + toString(arg_idx) +
                         " for OpenCL kernel";

    size_t len=0;
    cl_int eerr = clGetKernelInfo ( kern, CL_KERNEL_FUNCTION_NAME, 0, 0, &len);
    char *kernel_function = (char *) calloc(len, sizeof(char));
    if (kernel_function) {
      eerr = clGetKernelInfo ( kern, CL_KERNEL_FUNCTION_NAME,
                               len, kernel_function, 0);
      if (eerr == CL_SUCCESS)
        errstr += " \"" + std::string(kernel_function, len) + "\"";
      free(kernel_function);
    }

    throw_error(modname, errstr + ": " + toString(err) );

  }

}


#endif // OPENCL_FOUND


/// @}



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


/// \brief Load image from preopened object.
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

#endif // _H_CTAS_H_
