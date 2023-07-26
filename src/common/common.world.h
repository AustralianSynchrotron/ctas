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


#ifndef COMMON_H
#define COMMON_H


#include <cmath>
#include <deque>
#include <string>
#include <vector>
#include <iostream>



#ifdef _WIN32
#define NAN numeric_limits<float>::quiet_NaN();
static inline int fisnan(double x){ return _isnan(x); }
static inline int fisok(double x){ return _isnormal(x) || _fpclassify(x) == FP_ZERO; }
#else
// without optimize attribute the comparison to FP_ZERO gives true for x=NaN on GCC. WTF?!!!
__attribute__((optimize("O0")))
static inline bool fisok(double x){ return std::isnormal(x) || ( std::fpclassify(x) == FP_ZERO ) ; }
#endif

#define whole(container) (container).begin(), (container).end()

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
std::string
upper(std::string str);

/// \brief Convert string to lower case
///
/// @param str Input string.
///
/// @return New string which represents input string converted into the lower case.
///
std::string
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
std::string
toString(const std::string fmt, ...);


/// \brief Prints the value to string.
///
/// @param n number to be printed
///
/// @return string with printed number
///
inline std::string toString (long double   n)   { return toString("%g", n); }
inline std::string toString (double        n)   { return toString("%g", n); }
inline std::string toString (float         n)   { return toString("%g", n); }
inline std::string toString (long int      n)   { return toString("%i", n); }
inline std::string toString (int           n)   { return toString("%i", n); }
inline std::string toString (long unsigned n)   { return toString("%u", n); }
inline std::string toString (unsigned      n)   { return toString("%u", n); }


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

std::deque<std::string>
splits (const std::string & str, const std::string & delimiters);



/// \defgroup Error Error handling.
/// Functions in this group are used for the
/// error parsing, printing, throwing and so on.
///
/// @{

/// Error type.
class  CtasErr{
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
void
throw_error(const std::string & mod, const std::string & msg);

/// \brief Reports and throws the bug.
///
/// Constructs the error with the message that it must never happen,
/// reports and then throws it.
///
/// @param mod Module where the error happened.
///
void
throw_bug(const std::string & mod);


/// \brief Constructs the warning and reports it.
///
/// @param mod Module where the error happened
/// @param msg Description
///
/// @return the error generated in warning.
///
CtasErr
warn(const std::string & mod, const std::string & msg);


/// \brief Constructs the error, reports and then exits.
///
/// @param mod Module where the error happened
/// @param msg Description
///
void
exit_on_error(const std::string & mod, const std::string & msg);

/// @}


template<class T,
         class = typename std::enable_if< std::is_integral<T>::value &&
                                          std::is_signed<T>::value >::type >
long long int spec_conversion(const std::string & in, char ** tail) {
  return strtoll(in.c_str(), tail, 0);
}

template<class T,
         class = typename std::enable_if< std::is_integral<T>::value &&
                                          std::is_unsigned<T>::value >::type >
unsigned long long int spec_conversion(const std::string & in, char ** tail) {
  return strtoull(in.c_str(), tail, 0);
}

template<class T,
         class = typename std::enable_if<std::is_floating_point<T>::value>::type>
long double spec_conversion(const std::string & in, char ** tail) {
  return strtold(in.c_str(), tail);
}

template<class T,
         class = typename std::enable_if<std::is_arithmetic<T>::value>::type >
bool parse_num(const std::string & in, T * val) {
  const std::string modname="numeric conversion";
  char * tail = 0 ;
  errno = 0;
  auto inval = spec_conversion<T>(in, &tail);
  *val = (T)(inval);
  if (errno)
    warn(modname, "Failed to convert string \""+in+"\" (errno: "+toString(errno)+")");
  else if (tail == in.c_str())
    warn(modname, "String \""+in+"\" does not represent a numeric value");
  else if (tail && *tail)
    warn(modname, "String \""+in+"\" contains tail \"" + std::string(tail) + "\".");
  else if (std::is_integral<T>() && (
             inval < std::numeric_limits<T>::min() ||
             inval > std::numeric_limits<T>::max() ) )
    warn(modname, "String \""+in+"\" contains integer value outside type range.");
  else if (std::is_floating_point<T>() && (
             inval > std::numeric_limits<T>::max() ||
             fabsl(inval) < std::numeric_limits<T>::min() ) )
    warn(modname, "String \""+in+"\" contains float-point value outside type range.");
  else
    return true;
  return false;
}





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
class  Path : public std::string {
  using std::string::string;

private:

  const static std::string DIRSEPARATOR; ///< Directory separator
  void throw_unextracted(const std::string & element) const;
  ///< throws error in extracting an element.

public:

  /// \brief Constructor from string
  /// @param str the initializing string.
  inline Path(const std::string & str = std::string()) : std::string(str) {};

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
std::string
type_desc (Path*);

/// \brief Converts the string "in" into the Path _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c 1 if success, \c -1 otherwise.
int
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
Path
upgrade(const Path & path, const std::string & addthis);

/// \brief Converts the mask string to the format string.
///
/// @param mask Mask string (read ::MaskDesc).
/// @param maxslice Maximum allowed slice (total number of slices).
///
/// @return The format string which ready to be used with the fmt2s() function.
///
std::string
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
Path
cdpath(const Path & dir, const Path & file);



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
std::deque<int>
slice_str2vec(const std::string & sliceS, int hight);


/// \brief CLI progress bar (PB).
///
/// This class is used to visualize a progress.
class   ProgressBar {

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

  explicit ProgressBar(bool _showme=false, const std::string & _message="", int _steps=0);
  ~ProgressBar() {pthread_mutex_destroy(&proglock);}

  void setSteps(int _steps);
  void start();
  void update(int curstep=0);	///< Updates the progress bar
  void done();					///< Finalizes the progress bar.
  bool isShown() const {return showme;}

};




/// Lists all manual pages in the package. This string to be added to
/// every manual page.
extern const std::string
SeeAlsoList;



#endif // COMMON_H
