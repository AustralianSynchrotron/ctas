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
/// @author antonmx <antonmx@asw062.synchrotron.org.au>
/// @date   Mon Mar 16 13:39:22 2009
///
/// @brief Template-oriented library for CLI parsing.
///
/// This is just another command line interface parsing library. I wrote it
/// because could not find any other one (among dozens or even hundreds)
/// which could satisfy me. The architecture of this library was strongly
/// influenced by the libpopt (part of the rpm package) which I used for
/// several years. The characteristic features of my library are as follows:
/// - Object-oriented approach and architecture: C++/STL.
/// - Cross-platform approach: compiles even on MS Windows.
/// - No external libraries needed.
/// - Can parse both arguments and options.
/// - Template-based architecture: the parsing method is defined by the
///   type of the variable to be updated.
/// - Easy inclusion of any new types.
/// - Possibility to redefine the standard parse procedures for any type.
/// - Possibility to automatically generate help, usage and verbose options.
/// - Possibility to automatically generate manual pages.
/// - The entries to the option table appear in the help and usage messages
///   and manual pages exactly as they were added to the table: no
///   argument/option and/or alphabetical sort is used.
///
/// Writing this library I do assume a few things about the CLI logic:
///
/// - The element to be parsed may have only one meaning (certain type and
///   the variable to be updated). This assumption is different from the libpopt
///   where any option could have it's argument representing different types.
///   I think it may provoke errors in the CLI.
///   However you can overcame this limitation defining your own type or
///   redefining the parsing method of a predefined one.
/// - The option's argument may consist only of one CLI word in the phrase.
///   It is not difficult to write a support for the multi-element option's
///   argument, but it will make the command line harder to read and understand.
/// - The option may NOT be defined as "having or not having the argument".
///   Also different from the libpopt behaviour for the same reasons.
/// - The same option name (short or long) cannot be used in different
///   entries.
/// - The same element can be used more than once in the CLI input phrase
///   only if the corresponding variable is an std::deque. In this case each
///   next appearance of the element in the CLI phrase will add the element
///   to the end of the deque.
/// - The CLI input processing is case sensitive.

// TODO: Separate all functions in OptionTable into the ones which
// can be used only before or only after parse() called.

// TODO: Add possible "=" between option name and argument.


#ifndef _H_PPOPT_H_
#define _H_PPOPT_H_


#include<iostream>
#include<string>
#include<deque>
#include<list>


#ifdef _WIN32
#  pragma warning(disable: 4251)
#  ifdef POPTMX_EXPORTS
#    define POPTMX_API __declspec(dllexport)
#  else
#    define POPTMX_API __declspec(dllimport)
#  endif
#else
#  define POPTMX_API
#endif



namespace poptmx {

/// \defgroup poptmx CLI parsing.
///
/// @{





/// Error type
class POPTMX_API Err{

public:
  /// Error severity
  enum ErrTp {
  WARN,                        ///< Warning
  ERR,                         ///< Error
  FATAL                        ///< Fatal error
  } ;

private:
  std::string message;               ///< The message which describes the error
  std::string module;                ///< Name of the module where the error happened
  ErrTp terr;           ///< Error severity

public:

  /// Constructor
  Err(ErrTp _terr, const std::string & mod, const std::string & msg);
  void report() const ;			///< Reports the error

};


/// \brief Constructs the error, reports and then throws it.
///
/// @param mod Module where the error happened.
/// @param msg Description.
///
static inline void
throw_error(const std::string & mod, const std::string & msg){
  Err err(Err::ERR, mod, msg);
  err.report();
  throw err;
}

/// \brief Constructs the warning and reports it.
///
/// @param mod Module where the error happened
/// @param msg Description
///
/// @return the error generated in warning.
///
static inline Err
warn(const std::string & mod, const std::string & msg){
  Err err(Err::WARN, mod, msg);
  err.report();
  return err;
}





/// Type of the element in the table of the known options.
typedef enum {
  OPTION,						///< Option
  ARGUMENT,						///< Argument
  NOTE,							///< Note
  MAN,							///< Manual page
  // ALIAS,						//< Alias to the option(s)
  PROC							///< Post-parse processing function
} Kind;


/// Single entry in the option table. Represents all types of entries.
class POPTMX_API Option {

public:

  void * val;					///< Pointer to the value the parser to update.
  int counter;					///< Number of parser already invoked.
  bool isarray;					///< Tells if the entry represents the array.

  /// Type of the entry.
  Kind kind;					///< Type of the entry.
  std::string dflt;             ///< Default value of the option/argument.

  /// Function which converts the Option::acquire into the Option::val.
  int (*convert)(void*, const std::string &);

  char   char_name;				///< Short name of the option.
  std::string long_name;		///< Long name of the option / argument name.
  std::string arg_desc;			///< Description of the input type.
  std::string short_desc;		///< Short description.
  std::string long_desc;		///< Long description.

  Option( Kind _kind, void * _val, int (*_convert)(void*, const std::string &),
      const char _char_name, const std::string & _long_name,
      const std::string & _short_desc, const std::string & _long_desc,
      const std::string & _arg_desc, const std::string & _dflt,
          bool _isarray);

  bool parse(const std::string & acquire);	///< Parser: invokes Option::convert.
  /// Description of the entry: general form;
  std::string delim_opt(const std::string & delim, bool keep_format=false) const ;
  std::string desc(bool keep_format=false) const ; ///< Prints the description.
  bool require_arg() const;		///< Tells if the entry requires an argument for parsing.

  void usage() const ;	///< Usage.
  void Usage() const ;	///< Verbose usage.

  void help(const int descwidth=0, const int argwidth=0) const; ///< Help.

  void Help() const ; ///< Verbose help.
  void man() const; ///< Manual page.

};


/// @class poptmx::OptionTable
///
/// \brief The table of known options
///
/// The main (and the only) class to be used by the end user.
/// It stores the information about the program including
/// the general description, all known options and arguments,
/// parses the command line phrase basing on the elements,
/// and may output the usage and/or help messages as well as
/// prepare the manual page for the program.
///
/// Within this class the variables ending with "*_long_desc" would be shown in
/// the verbose help message and will go to the manual pages. The variables
/// ending with "*_desc" would be shown in both verbose and non-verbose help
/// messages.
class POPTMX_API OptionTable {

private:

  mutable std::list<Option> options;	///< The table containing all options, arguments, etc.
  typedef std::list<Option>::iterator ListO; ///< Just to short the text

  /// Short description of the program. If the table is included into
  /// another option table, this description becomes the short_desc of
  /// the note preceeding the included elements.
  std::string general_desc;

  /// Long description of the program. If the table is included into
  /// another option table, this description becomes the long_desc of
  /// the note preceeding the included elements.
  std::string general_long_desc;

  /// The string with the synopsis of the program: to be found in the
  /// manual page and help messages. If not set then it is automatically
  /// generated from the table of options.
  std::string general_synopsis;

  /// Program name (as invoked).
  std::string general_name;

  bool helpme;					///< To be used for the help, if none is provided.
  bool useme;					///< To be used for the usage, if none is provided.
  bool beverb;				///< To be used for the verbose, if none is provided.
  bool * auto_help;				///< Indicates that autohelp was used.
  bool * auto_use;				///< Indicates that autouse was used.
  bool * auto_verb;				///< Indicates that autoverb was used.


  ListO find(const void * _val) const; ///< Finds entry by value
  ListO find(char _char_name) const; ///< Finds entry by short name
  ListO find(const std::string & _long_name) const; ///< Finds entry by long name
  ListO find() const; ///< Finds first parse-able argument.
  ListO has_array() const; ///< True if the table has an array argument.

  OptionTable & add(const Option & opt);	///< Adds the entry in to the table

  static const std::string manLN; ///< The key phrase which triggers the man()

  static const std::string pntrMark; ///< Prefixes and suffixes the printed pointer in the text.

  /// Replaces all pointer marks.
  void replacePointers(std::string & str);

public:


  /// Constructor
  OptionTable(const std::string & _general_desc="",
        const std::string & _general_long_desc="",
        const std::string & _general_synopsis="");

  /// Incorporates preexisting option table into the table.
  OptionTable &
  add(const OptionTable & _val);

  /// Abstract interface to add an entry into the table.
  OptionTable &
  add( Kind _kind, void * _val, int (*_convert)(void*, const std::string &),
     const char _char_name, const std::string & _long_name,
     const std::string & _short_desc, const std::string & _long_desc,
     const std::string & _arg_desc, const std::string & _dflt, bool _is_array);

  /// Abstract interface to add the argument into the table.
  OptionTable &
  add( Kind _kind, void * _val, int (*_convert)(void*, const std::string &),
     const std::string & _long_name,
     const std::string & _short_desc, const std::string & _long_desc,
     const std::string & _arg_desc, const std::string & _dflt, bool _is_array);

  /// Simplified interface to add the option into the table.
  template<class BClass> OptionTable &
  add( Kind _kind, BClass * _val,
     const char _char_name, const std::string & _long_name,
     const std::string & _short_desc, const std::string & _long_desc, const std::string & _dflt="");

  /// Simplified interface to add multiple-appearing option into the table.
  template<class BClass> inline OptionTable &
  add( Kind _kind, std::deque<BClass> * _val,
     const char _char_name, const std::string & _long_name,
     const std::string & _short_desc, const std::string & _long_desc, const std::string & _dflt="");

  /// Simplified interface to add the argument into the table.
  template<class BClass> OptionTable &
  add( Kind _kind, BClass * _val,
     const std::string & _long_name,
     const std::string & _short_desc, const std::string & _long_desc, const std::string & _dflt="");

  /// Simplified interface to add multiple-appearing argument into the table.
  template<class BClass> inline OptionTable &
  add( Kind _kind, std::deque<BClass> * _val,
     const std::string & _long_name,
     const std::string & _short_desc, const std::string & _long_desc, const std::string & _dflt="");

  /// Adds note or man.
  OptionTable &
  add( Kind _kind, const std::string & _short_desc, const std::string & _long_desc="");

  OptionTable &
  add_verbose(bool *_beverb=0);	///< Includes standard verbose option.

  OptionTable &
  add_help(bool *_helpme=0); ///< Includes standard help option.

  OptionTable &
  add_usage(bool *_useme=0); ///< Includes standard usage option.

  /// Includes all three standard options;
  OptionTable &
  add_standard_options(bool *_beverb=0, bool *_helpme=0, bool *_useme=0);



  /// Parses the whole CLI phrase.
  ///
  /// @param argc Number of the arguments in the argv.
  /// @param argv Pointer to the array of the arguments. The last element must always
  /// be 0-pointer.
  ///
  /// @return \c false if help or usage man message were printed, \c true otherwise.
  ///
  bool parse(int argc, char *argv[]);



  ///The array of the CLI words which left unprocessed by the parser.
  std::list<std::string> unused;

  /// Tells if the entry with the _val exists in the table.
  bool has(const void * _val) const ;

  /// Tells if the entry with the _long_name exists in the table.
  bool has(const std::string & _long_name) const ;

  /// Tells if the entry with the _char_name exists in the table.
  bool has(const char _char_name) const ;


  /// Returns number of times the entry was processed in parse().
  int count(const void * _val=0) const ;

  /// Returns the description of the entry with the value.
  std::string desc(const void * _val) const ;

  int size() const;				///< Size of the table

  void usage() const;	///< Constructs the usage message.
  void Usage() const;	///< Constructs the verbose usage message.
  void help() const;		///< Constructs the help message.
  void Help() const;		///< Constructs the verbose help message.
  void man() const;		///< Manages man pages

  std::string name() const ;	///< Returns the command name as invoked.
  void name(const std::string & _name); ///< Sets the command name.

};



int POPTMX_API _conversion(bool* _val, const std::string &);
std::string POPTMX_API type_desc (bool*);

int POPTMX_API _conversion (std::string * _val, const std::string & in);
std::string POPTMX_API type_desc (std::string*);

int POPTMX_API _conversion(char * _val, const std::string & in);
std::string POPTMX_API type_desc(char*);

int POPTMX_API _conversion(unsigned char * _val, const std::string & in);
std::string POPTMX_API type_desc(unsigned char*);

int POPTMX_API _conversion(short * _val, const std::string & in);
std::string POPTMX_API type_desc(short*);

int POPTMX_API _conversion(unsigned short * _val, const std::string & in);
std::string POPTMX_API type_desc(unsigned short*);

int POPTMX_API _conversion(int * _val, const std::string & in);
std::string POPTMX_API type_desc(int*);

int POPTMX_API _conversion(unsigned int * _val, const std::string & in);
std::string POPTMX_API type_desc(unsigned int*);

int POPTMX_API _conversion(long* _val, const std::string & in);
std::string POPTMX_API type_desc(long*);

int POPTMX_API _conversion(unsigned long* _val, const std::string & in);
std::string POPTMX_API type_desc(unsigned long*);

int POPTMX_API _conversion(long long* _val, const std::string & in);
std::string POPTMX_API type_desc(long long*);

int POPTMX_API _conversion(unsigned long long* _val, const std::string & in);
std::string POPTMX_API type_desc(unsigned long long*);

int POPTMX_API _conversion(float* _val, const std::string & in);
std::string POPTMX_API type_desc(float*);

int POPTMX_API _conversion(double* _val, const std::string & in);
std::string POPTMX_API type_desc(double*);






/// The layer between the real type-overloaded function which parses the string
/// into value and the function which can be inserted into the Option entry as
/// Option::convert (of type bool (*)(void*, const std::string &) )
///
/// @param _val The value to be updated when parsing the "in" string.
/// @param in The string to be parsed into the _val.
///
/// @return \c true if the conversion is successful and \c false if failed.
///
template<class BClass> inline int
conversion(void* _val, const std::string & in){
  return _conversion( (BClass *) _val, in);
}


/// The interface to add the option into the table which argument's type has
/// defined "bool _conversion ( BClass * , const std::string & )" and
/// "std::string type_desc (BClass*)" functions.
///
/// @param _kind Kind of entry to add. In this context can be ::OPTION.
/// @param _val Option::val
/// @param _char_name Option::char_name
/// @param _long_name Option::long_name
/// @param _short_desc Option::short_desc
/// @param _long_desc Option::long_desc
///
template<class BClass> inline OptionTable &
OptionTable::add( Kind _kind,   BClass * _val,
          const char _char_name, const std::string & _long_name,
          const std::string & _short_desc, const std::string & _long_desc, const std::string & _dflt){
  if ( _kind != poptmx::OPTION )
  throw_error("add option", "You've got the wrong add function for your kind.");
  return add(_kind, _val, conversion<BClass>, _char_name, _long_name,
       _short_desc, _long_desc, type_desc(_val), _dflt, false );
}

/// The interface to add into the table the argument which type has
/// defined "bool _conversion ( BClass * , const std::string & )" and
/// "std::string type_desc (BClass*)" functions.
///
/// @param _kind Kind of entry to add. In this context can be ::ARGUMENT.
/// @param _val Option::val
/// @param _long_name Option::long_name
/// @param _short_desc Option::short_desc
/// @param _long_desc Option::long_desc
///
template<class BClass> inline OptionTable &
OptionTable::add( Kind _kind, BClass * _val,
          const std::string & _long_name,
          const std::string & _short_desc, const std::string & _long_desc, const std::string & _dflt){
  if ( _kind != poptmx::ARGUMENT )
  throw_error("add argument", "You've got the wrong add function for your kind.");
  return add(_kind, _val, conversion<BClass>, 0, _long_name,
       _short_desc, _long_desc, type_desc(_val), _dflt, false );
}





/// The layer between the real type-overloaded function which parses the string
/// into value and the function which first converts the string into a temp value
/// and then inserts it into the array of values. Needed here to be inserted into
/// the Option entry Option::convert (of type bool (*)(void*, const std::string &) ).
///
/// @param _val The array to accept the newly parsed value from the "in" string.
/// @param in The string to be parsed into the value.
///
/// @return \c true if the conversion is successful and \c false if failed.
///
template<class BClass> inline int
arr_conversion(void* _val, const std::string & in){
  BClass var;
  int returned = conversion<BClass>(&var, in);
  if (returned >= 0)
  ( (std::deque<BClass> *) _val ) ->push_back(var);
  return returned;
}


/// The interface to add the multi-acceptable option into the table. The option's
/// argument's type has  defined "bool _conversion ( BClass * , const std::string & )"
/// and "std::string type_desc (BClass*)" functions.
///
/// @param _kind Kind of entry to add. In this context can be ::OPTION
/// @param _val Option::val
/// @param _char_name Option::char_name
/// @param _long_name Option::long_name
/// @param _short_desc Option::short_desc
/// @param _long_desc Option::long_desc
///
template<class BClass> inline OptionTable &
OptionTable::add( Kind _kind, std::deque<BClass> * _val,
          const char _char_name, const std::string & _long_name,
          const std::string & _short_desc, const std::string & _long_desc, const std::string & _dflt){
  if ( _kind != OPTION )
  throw_error("add option", "You've got the wrong add function for your kind.");
  BClass var;
  return add(_kind, _val, arr_conversion<BClass>, _char_name, _long_name,
       _short_desc, _long_desc, type_desc(& var)+"...", _dflt, true);
}

/// The interface to add the multi-acceptable argument into the table. The argument's
/// type must have defined "bool _conversion ( BClass * , const std::string & )"
/// and "std::string type_desc (BClass*)" functions.
///
/// @param _kind Kind of entry to add. In this context can be ::ARGUMENT
/// @param _val Option::val
/// @param _long_name Option::long_name
/// @param _short_desc Option::short_desc
/// @param _long_desc Option::long_desc
///
template<class BClass> inline OptionTable &
OptionTable::add( Kind _kind, std::deque<BClass> * _val,
          const std::string & _long_name,
          const std::string & _short_desc, const std::string & _long_desc, const std::string & _dflt){
  if ( _kind != ARGUMENT )
  throw_error("add argument", "You've got the wrong add function for your kind.");
  BClass var;
  return add( _kind, _val, arr_conversion<BClass>, 0, _long_name,
        _short_desc, _long_desc, type_desc(&var)+"...", _dflt, true);
}






/// @}


}






#endif // _H_PPOPT_H_
