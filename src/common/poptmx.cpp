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
/// @brief All functions defined in poptmx.h.
///

#include"poptmx.h"
#include <unistd.h>

#include<limits>
#include<algorithm>
#include<numeric>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<cstdlib>
#include<cmath>


#ifdef _WIN32
#  include <windows.h>
#else
#  include<termios.h>
#  include<sys/ioctl.h>
#endif


using namespace std;

namespace poptmx {



/// \cond
struct ToLower{ char operator() (char c) const { return tolower(c); } };
struct ToUpper{ char operator() (char c) const { return toupper(c); } };

static inline string
upper(string str){
  transform (str.begin(), str.end(), str.begin(), ToUpper());
  return str;
}

static inline string
lower(string str){
  transform (str.begin(), str.end(), str.begin(), ToLower());
  return str;
}
/// \endcond



/// \brief Constructor.
///
/// @param _terr Sets CtasErr::terr
/// @param mod   Sets CtasErr::module
/// @param msg   Sets CtasErr::message
///
Err::Err(Err::ErrTp _terr, const string & mod, const string & msg)
    : terr(_terr)
    , module(mod)
    , message(msg)
{}

/// \brief Prints the error into the standard error stream.
void
Err::report() const {
  switch ( terr ) {
  case Err::WARN:  cerr << "WARNING!";     break;
  case Err::ERR:   cerr << "ERROR!";       break;
  case Err::FATAL: cerr << "FATAL ERROR!"; break;
  }
  cerr << " In module \'" << module << "\'. " << message << endl;
}



/// \brief Warns about parsing error
///
/// @param type_name Type of the variable.
/// @param in Input string which caused the error.
/// @param msg Description of the error.
///
/// @return The error
///
static inline Err
warn_parse(const string & type_name, const string & in,
           const string &msg){
  return warn("parsing "+type_name, "String \""+in+"\": " +
              ( msg.empty() ? string("<no info>") : msg ) + ".");
}





/// \brief Returns current terminal width.
///
/// @return Terminal width
///
static inline int
get_terminal_width(){
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO info;
  HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);
  if (fd == INVALID_HANDLE_VALUE)
    return 0;
  return GetConsoleScreenBufferInfo (fd, &info) ? info.dwSize.X - 1 : 0;
#else
  winsize size;
  return ( ioctl (STDIN_FILENO, TIOCGWINSZ, &size )<0 ) ?  0 : size.ws_col - 1;
#endif
}


/// \brief Prints the string into the table.
///
/// Decomposes the input string into the array of the strings
/// with the length no greater than the "space" value. The cut
/// points are the space-characters ' ' or newline-characters
/// '\n' if any or any other point of the string if none. The
/// newline-characters, if found, always finish the element.
/// If space==string::npos or 0 then only the newline-characters
/// forms new element in the output array.
///
/// @param instr Input string.
/// @param space Maximum length of the decomposed string.
///
/// @return Array of the decomposed strings.
///
static string
print_table(const string & instr, string::size_type space=string::npos) {

  if ( instr.empty() )
    return string();
  if( space == string::npos)
    space=get_terminal_width();

  string out;
  string::size_type idx=0, nidx;
  while (idx != string::npos) {

    if ( nidx = instr.find('\n', idx),
         nidx != string::npos && nidx < idx+space )  {
      out += instr.substr(idx, nidx-idx) + "\n";
      idx = nidx+1;
    } else if ( idx+space > instr.length() ) { // end reached
      out += instr.substr(idx);
      idx = string::npos;
    } else if ( nidx = instr.rfind(' ', idx+space),
                nidx != string::npos && nidx > idx ) {
      out += instr.substr(idx, nidx-idx) + "\n";
      idx = nidx+1;
    } else {
      out += instr.substr(idx, space-1) + "\n";
      idx += space-1;
    }

  }

  return out;

}


/// Adds the addition after each new-line character found in the string.
///
/// @param original Original string.
/// @param addition The addition to add.
/// @param beginning If \c true then also adds the addition in the
/// beginning of the string, if \c false then only after the new-lines.
///
/// @return Upgraded string.
///
void
prefix_out(const string & original, const string & prefix = "",
           bool firstline=true){
  if(original.empty())
    return;

  if (firstline)
    cout << prefix;

  string::size_type idx=0, nidx=0;

  while ( nidx = original.find('\n', idx),
          nidx != string::npos && nidx != original.length()-1 ) {
    cout << original.substr(idx,nidx-idx) << endl
         <<  prefix;
    idx=nidx+1;
  }
  cout << original.substr(idx);

}



static const int indent=2;		///< Number of spaces for the indentation.
static const string sindent=string(indent, ' '); ///< Indenting string















/// Constructor
///
/// @param _kind kind
/// @param _val val
/// @param _convert convert
/// @param _char_name char_name
/// @param _long_name long_name
/// @param _short_desc short_desc
/// @param _long_desc long_desc
/// @param _arg_desc arg_desc
/// @param _dflt dflt
/// @param _isarray isarray
///
Option::Option
( Kind _kind, void * _val, int (*_convert)(void*, const std::string &) ,
  const char _char_name, const std::string & _long_name,
  const std::string & _short_desc, const std::string & _long_desc,
  const std::string & _arg_desc, const std::string & _dflt,
  bool _isarray) :
  kind(_kind),
  val(_val),
  long_name(_long_name),
  char_name(_char_name),
  arg_desc(_arg_desc),
  short_desc(_short_desc),
  long_desc(_long_desc),
  convert(_convert),
  dflt(_dflt),
  isarray(_isarray),
  counter(0)
{
  if ( ( kind == ARGUMENT || kind == OPTION ) && ( !val || !_convert )  )
    throw_error("construct option",
                "Arguments and options must always have non-zero value"
                " and conversion function.");
  if ( kind == ARGUMENT && ( short_desc.empty() || long_name.empty() ) )
    throw_error("construct option",
                "Arguments must always have \"short description\" and"
                " name fields filled.");
  if ( kind == OPTION && short_desc.empty() )
    throw_error("construct option",
                "Options must always have \"short description\" field.");
}


/// Prints general description of the option: short name (or spaces if
/// keep_format is \c true) plus the delimiter delim plus the long_name
///
/// @param delim Delimiter between short and long names of the option
/// @param keep_format Tells if the missing parts (short name or delimiter)
///                    should be filled with the spaces.
///
/// @return General representation of the option.
///
std::string
Option::delim_opt(const std::string & delim, bool keep_format) const {
  if (kind == OPTION) {

    string ret;

    if (char_name) ret += string("-") + string(1, char_name);
    else if (keep_format) ret += "  ";

    if ( char_name && ! long_name.empty() ) ret += delim;
    else if (keep_format) ret += string(delim.length(), ' ');

    ret += "--" + long_name;

    return ret;

  } else if (kind == ARGUMENT) {
    return "<" + long_name + ">";
  } else {
    return "";
  }
}


/// @param keep_format Tells if the missing parts (short name or delimiter)
///                    should be filled with the spaces.
///
/// @return Short description of the entry.
///
string
Option::desc(bool keep_format) const {
  return delim_opt(", ", keep_format);
}


void
Option::usage() const {
  if (kind == OPTION)
    cout << delim_opt(" ");
  else if (kind == ARGUMENT)
    cout << "<" << long_name << (isarray ? "..." : "")  << ">";
}


void
Option::Usage() const {
  if (kind == OPTION) {
    cout << "[" << delim_opt("|");
    if ( ! arg_desc.empty() ) cout << "=" << arg_desc;
    cout << "]";
  } else if (kind == ARGUMENT) {
    cout << "<" << long_name << " (" << arg_desc << ")" << ">";
  }
}


/// @param descwidth The width of the description substring. The description
/// is filled with spaces to match this size.
/// @param argwidth The width of the type description substring. The argument
/// name is filled with spaces to match this size.
///
void
Option::help(const int descwidth, const int argwidth) const {

  string descs = desc(true);
  int const_part = indent + descwidth + indent + argwidth + indent;

  if (kind == OPTION || kind == ARGUMENT) {
    cout << sindent << descs << string(descwidth-descs.length(),' ')
         << sindent << arg_desc << string(argwidth-arg_desc.length(),' ')
         << sindent;
    prefix_out
      ( print_table(short_desc, get_terminal_width()-const_part),
        string(const_part, ' '), false);
    cout << endl;
  } else if (kind == NOTE) {
    prefix_out(short_desc, sindent, false);
    if ( ! short_desc.empty() ) cout << endl;
  }

}


void
Option::Help() const {

  const int width = get_terminal_width();

  if (kind == OPTION) {
    cout << sindent << delim_opt(", ") << sindent << arg_desc << endl;
    if ( ! short_desc.empty()) {
      prefix_out( print_table(short_desc, width-2*indent), sindent+sindent);
      cout << endl;
    }
    if ( ! long_desc.empty()) {
      prefix_out( print_table(long_desc, width-2*indent), sindent+sindent);
      cout << endl;
    }
  } else if (kind == ARGUMENT) {
    cout << sindent << "<" << long_name << sindent << "(" << arg_desc << ")>" << endl;
    if ( ! short_desc.empty()) {
      prefix_out( print_table(short_desc, width-2*indent), sindent+sindent);
      cout << endl;
    }
    if ( ! long_desc.empty()) {
      prefix_out( print_table(long_desc, width-2*indent), sindent+sindent);
      cout << endl;
    }
  } else if (kind == NOTE) {
    if ( ! short_desc.empty()) {
      cout << print_table(short_desc, width);
      cout << endl;
    }
    if ( ! long_desc.empty()) {
      prefix_out( print_table(long_desc, width-2*indent), sindent+sindent);
      cout << endl;
    }
  }

}



void
Option::man() const {

  switch (kind) {

  case OPTION :

    cout << ".TP" << endl;
    if (char_name) cout << "\\fB\\-" << char_name << "\\fR";
    if ( char_name && ! long_name.empty() ) cout << ", ";
    if ( ! long_name.empty() ) cout << "\\fB\\-\\-" << long_name << "\\fR";
    if ( ! arg_desc.empty() ) cout << "=\\fI" << arg_desc << "\\fR";
    cout << endl;

    if ( ! short_desc.empty() || ! long_desc.empty() ) {
      cout << ".RS" << endl;
      cout << short_desc;
      if ( ! short_desc.empty() ) cout << endl;
      if ( ! short_desc.empty() && ! long_desc.empty() ) cout << ".br" << endl;
      prefix_out(long_desc, ".br\n",false);
      if ( ! long_desc.empty() ) cout << endl;
      cout << ".RE" << endl;
    }

    break;

  case ARGUMENT :

    cout << ".TP" << endl;
    cout << "\\fB<" << long_name << ">\\fR" << endl;

    if ( ! short_desc.empty() || ! long_desc.empty() ) {
      cout << ".RS" << endl;
      cout << short_desc;
      if ( ! short_desc.empty() ) cout << endl;
      if ( ! short_desc.empty() && ! long_desc.empty() ) cout << ".br" << endl;
      prefix_out(long_desc, ".br\n",false);
      if ( ! long_desc.empty() ) cout << endl;
      cout << ".RE" << endl;
    }

    break;

  case NOTE:
    cout << ".RS" << endl;
    if ( short_desc.empty() ) {
      prefix_out(long_desc, ".br\n",false);
      cout << endl;
    } else if ( long_desc.empty() ) {
      cout << ".SH " << short_desc << endl;
    } else {
      cout << ".SS " << short_desc << endl
           << ".br" << endl;
      prefix_out(long_desc, ".br\n",false);
      cout << endl;
    }
    cout << ".RE" << endl;
    break;

  case MAN:
    if ( ! short_desc.empty() )
      cout << ".br" << endl
           << ".SH " << short_desc << endl
           << ".br" << endl;
    cout << long_desc << endl;
    break;

  default:
    break;

  }

}


bool
Option::require_arg() const {
  return ! arg_desc.empty();
}


/// Converts the input string into the value.
///
/// @param acquire string to be converted
///
/// @return number of words parsed.
///
bool
Option::parse(const string & acquire){
  if ( !val || !convert )
    throw_error("parse entry", "Attempt to perform conversion on the entry"
                " which has no converter. Looks like a bug in the code.");
 if (counter && ! isarray)
   throw_error("parse entry", "Option \"" + delim_opt("|") + "\""
               " cannot be used more than one time.");
  int words = convert(val, acquire);
  if ( words < 0 ) {
    string errmsg;
    errmsg += "Failed to convert the string \"";
    errmsg += acquire;
    errmsg += "\" to the value of the parameter \"";
    errmsg += delim_opt("|");
    if ( ! arg_desc.empty() )
      errmsg += string(" of type \"") + arg_desc + string("\"");
    errmsg += ".";
    throw_error("parse entry", errmsg);
  }
  ++counter;
  return words != 0;
}
















typedef list<string>::const_iterator ListS;	///< Just to short the text





#ifdef POPTMX_MAN_KEY
const string OptionTable::manLN = POPTMX_MAN_KEY;
#else
const string OptionTable::manLN = "-very-long-key-phrase-to-output-man-pages";
#endif

const string OptionTable::pntrMark = "##PNTR##";


/// @param _general_desc General description of the table (program description).
/// @param _general_long_desc Long description of the table (program).
/// @param _general_synopsis Synopsis of the program.
///
OptionTable::OptionTable(const string & _general_desc,
                         const string & _general_long_desc,
                         const string & _general_synopsis) :
  general_desc(_general_desc), general_long_desc(_general_long_desc),
  general_synopsis(_general_synopsis) {

  name("");
  helpme=false;
  useme=false;
  beverb=false;
  auto_help=0;
  auto_use=0;
  auto_verb=0;

}




int
OptionTable::size() const {
  return options.size();
}

/// Finds the entry in the options with the Option.val equal to _val.
///
/// @param _val The value to search for.
///
/// @return The pointer to the table entry or 0-pointer if no entry is found.
///
OptionTable::ListO
OptionTable::find(const void * _val) const {
  return find_if(options.begin(), options.end(),
                 [&_val](const Option& opt){return opt.val == _val;});
}

/// Finds the entry in the options with the Option.char_name equal to _char_name.
///
/// @param _char_name The value to search for.
///
/// @return The pointer to the table entry or 0-pointer if no entry is found.
///
OptionTable::ListO
OptionTable::find(char _char_name) const {
  if ( ! _char_name )
    return options.end();
  return find_if(options.begin(), options.end(),
                 [&_char_name](const Option& opt){
                   return opt.char_name == _char_name && ( opt.kind == ARGUMENT || opt.kind == OPTION );});
}

/// Finds the entry in the options with the Option.long_name equal to _long_name.
///
/// @param _long_name The value to search for.
///
/// @return The pointer to the table entry or 0-pointer if no entry is found.
///
OptionTable::ListO
OptionTable::find(const string & _long_name) const {
  if ( _long_name.empty() )
    return options.end();
  return find_if(options.begin(), options.end(),
                 [&_long_name](const Option& opt){
                   return opt.long_name == _long_name && ( opt.kind == ARGUMENT || opt.kind == OPTION );});
}

/// Finds the first of the argument entries in the table which can take one
/// more value: i.e. either an argument which was not met before, or the
/// argument which accept multiple entries.
///
/// @return The pointer to the table entry or 0-pointer if no entry is found.
///
OptionTable::ListO
OptionTable::find() const {
  return find_if(options.begin(), options.end(),
                 [](const Option& opt){return opt.kind == ARGUMENT && ( opt.isarray || ! opt.counter ) ; });
}


/// Finds the first of the argument entries in the table which can take one
/// more value: i.e. either an argument which was not met before, or the
/// argument which accept multiple entries.
///
/// @return The pointer to the table entry or 0-pointer if no entry is found.
///
OptionTable::ListO
OptionTable::has_array() const {
  return find_if(options.begin(), options.end(),
                 [](const Option& opt){return opt.kind == ARGUMENT && opt.isarray; });
}



bool
OptionTable::parse(int argc, char *argv[]){

  // Reread all possible pointers.
  replacePointers(general_desc);
  replacePointers(general_long_desc);
  for (ListO icur=options.begin() ; icur != options.end() ; ++icur) {
    replacePointers(icur->short_desc);
    replacePointers(icur->long_desc);
  }

  if ( !argv )
    throw_error("parse parameters", "Zero-pointer to the parameters' array.");
  if ( argc < 1 )
    throw_error("parse parameters", "argc is not what it is meant to be.");

  const std::list<std::string> Argv(argv, argv+argc);
  const ListS cliend = Argv.end();


  // Prepares general information.
  if (general_name.empty())
    name(Argv.front());
  if ( general_synopsis.empty() ) {
    general_synopsis = name() + " [OPTIONS...]";
    for (ListO icur=options.begin() ; icur != options.end() ; ++icur)
      if ( icur->kind == ARGUMENT)
        general_synopsis += " <" + icur->long_name + (icur->isarray ? "..." : "") + ">";
  }

  // Triggers man if needed.
  ListS clicur = Argv.begin();
  while ( ++clicur != cliend)
    if ( *clicur == manLN ) {
      man();
      return false;
    }

  // Empty options
  if (options.empty()) {
    unused = list<string>(++(Argv.begin()),cliend);
    return true;
  }

  // Parses CLI phrase.
  bool no_more_options = false;
  clicur = ++(Argv.begin());
  while ( clicur != cliend) {

    const ListO tblend = options.end();

    if ( clicur->empty() ) {
      warn("parse parameters", "Empty string as the parameter. Will skip.");
      ++clicur;

    } else if ( clicur->at(0) != '-' || no_more_options ) {   // argument
      ListO tblarg = find();
      if (tblarg==tblend)
        unused.push_back(*clicur);
      else
        tblarg->parse(*clicur);
      ++clicur;

    } else if ( *clicur == "-" ) { // special case
      ++clicur;

    } else if ( *clicur == "--" ) { // end of options
      no_more_options = true;
      ++clicur;

    } else if ( ! clicur->compare(0,2,"--") ) { // long option
      ListO tblopt = find(clicur->substr(2));
      if (tblopt==tblend)
        throw_error("parse CLI", "Unknown option \"" + *clicur + "\".");
      ++clicur;
      if ( tblopt->require_arg() && clicur == cliend )
        throw_error("parse option", "Option \"" + tblopt->delim_opt("|") +"\""
                    " is missing required argument.");
      tblopt->parse ( clicur == cliend ? string("") : *clicur );
      if ( tblopt->require_arg() )
        ++clicur;


    } else { // short option
      string elements=*clicur++;
      for (unsigned el = 1 ; el < elements.length() ; ++el) {
        char short_name=elements[el];
        ListO tblopt = find(short_name);
        if (tblopt==tblend)
          throw_error("parse CLI", "Unknown option \"-" + string(1,short_name) + "\".");
        if ( tblopt->require_arg() && clicur == cliend )
          throw_error("parse option", "Option \"" + tblopt->delim_opt("|") +"\""
                      " is missing required argument.");
        tblopt->parse ( clicur == cliend ? string("") : *clicur );
        if ( tblopt->require_arg() )
          ++clicur;
      }
    }

  }

  if ( auto_help && *auto_help ) {
    auto_verb && *auto_verb   ?  Help() : help() ;
    return false;
  } else if ( auto_use && *auto_use ) {
    auto_verb && *auto_verb  ?  Usage() : usage() ;
    return false;
  }
  return true;

}










void
OptionTable::usage() const {

  cout << general_desc << endl
       << name();
  for_each(options.begin(), options.end(),
           [](const Option& opt) { cout << ' '; opt.usage(); });
  cout << endl;

  if ( auto_help || auto_verb ) {
    cout << "Use options ";
    if (auto_help) cout << find(auto_help)->delim_opt("|");
    if (auto_help && auto_verb) cout << " ";
    if (auto_verb) cout << find(auto_verb)->delim_opt("|");
    cout << " for more info." << endl;
  }

}

void
OptionTable::Usage() const {
  cout << general_desc << endl
       << print_table(general_long_desc) << endl
       << "USAGE:" << endl;
  prefix_out(general_synopsis, sindent);
  cout << endl
       << "All parameters:" << endl;
  for (ListO icur=options.begin() ; icur != options.end() ; ++icur){
    cout << " ";
    icur->Usage();
  }
  cout << endl;

  if ( auto_help || auto_verb ) {
    cout << "Use options ";
    if (auto_use)  cout << find(auto_use)->delim_opt("|") << " ";
    if (auto_help) cout << find(auto_help)->delim_opt("|") << " ";
    if (auto_verb) cout << find(auto_verb)->delim_opt("|");
    if (auto_help && auto_verb) cout << " and their combinations";
    cout << " for more info." << endl;
  }

}

void
OptionTable::help() const {

  cout << general_desc << endl
       << "USAGE:" << endl;
  prefix_out(general_synopsis, sindent);
  cout << endl;

  int descwidth=0, argwidth=0;
  for (ListO icur=options.begin() ; icur != options.end() ; ++icur) {
    int curlen;
    if ( descwidth < (curlen = icur->desc(true).length() ) )
      descwidth = curlen;
    if ( argwidth  < (curlen = icur->arg_desc.length() ) )
      argwidth = curlen;
  }

  for (ListO icur=options.begin() ; icur != options.end() ; ++icur)
    icur->help(descwidth, argwidth);

}

void
OptionTable::Help() const {

  cout << print_table(general_desc) << endl
       << print_table(general_long_desc) << endl
       << "USAGE:" << endl;
  prefix_out(print_table(general_synopsis), sindent, true);
  cout << endl;

  for (ListO icur=options.begin() ; icur != options.end() ; ++icur)
    icur->Help();

}




void
OptionTable::man() const {

//  string version="";
//#ifdef VERSION
//  version=VERSION;
//#endif

  string package="";
#ifdef PACKAGE_STRING
  package=PACKAGE_STRING;
#elif PACKAGE
  package=PACKAGE;
#else
  package=name();
#endif

  string date="";
#ifdef _config_date
  date=_config_date;
#endif

  cout << ".TH " << upper(name()) << " \"1\" \"" << date << "\" \""
       << package << "\" \"User Commands\"" << endl
       << ".SH NAME" << endl
       << name() << " \\- " << general_desc << endl
       << ".SH SYNOPSIS" << endl;
  prefix_out(general_synopsis, ".br\n.B ");
  cout << endl
       << ".SH DESCRIPTION" << endl
       << ".PP" << endl;
  prefix_out(general_long_desc, ".br\n",false);
  cout << endl;

  for (ListO icur=options.begin() ; icur != options.end() ; ++icur) {
    cout << "./ START OPTION" << endl;
    icur->man();
    cout << "./ END OPTION" << endl
         << "./" << endl;
  }

}







/// General interface to add the entry into the table. All other interfaces
/// should use this one.
///
/// @param opt Entry to be added.
///
OptionTable &
OptionTable::add(const Option &opt){

  if ( has(opt.char_name) )
    throw_error("add option", (string)
                "Option with the char name '" + opt.char_name +
                "' already exists in the table (description: \"" +
                find(opt.char_name)->short_desc + "\").");
  if ( has(opt.long_name ) )
    throw_error("add option",
                "Option with the long name \"" + opt.long_name +
                "\" already exists in the table (description: \"" +
                find(opt.long_name)->short_desc + "\").");
  if ( opt.kind == ARGUMENT && opt.isarray && has_array() != options.end() )
      throw_error("add option", "Cannot add the argument \"" + opt.desc() + "\""
                  " to the table because it already has the array-argument "
                  " \"" + has_array()->desc() + "\".");

  options.push_back(opt);
  return *this;

}




/// Most abstract and most flexible user interface to add an entry into the
/// table. Should be used only when the user wants to redefine one of the standard
/// type entry.
///
/// @param _kind Option::kind
/// @param _val Option::val
/// @param _convert Option::convert
/// @param _char_name Option::char_name
/// @param _long_name Option::long_name
/// @param _short_desc Option::short_desc
/// @param _long_desc Option:: long_desc
/// @param _arg_desc Option::arg_desc
/// @param _is_array Option::isarray
///
OptionTable &
OptionTable::add(Kind _kind, void * _val, int (*_convert)(void*, const string &),
                 const char _char_name, const string & _long_name,
                 const string & _short_desc, const string & _long_desc,
                 const string & _arg_desc, const std::string & _dflt,
                 bool _is_array){
  return add( Option(_kind, _val, _convert, _char_name, _long_name,
                     _short_desc, _long_desc, _arg_desc, _dflt, _is_array) );
}

/// Most abstract and most flexible user interface to add an argument into the
/// table. Should be used only when the user wants to redefine one of the standard
/// type entry.
///
/// @param _kind Option::kind
/// @param _val Option::val
/// @param _convert Option::convert
/// @param _long_name Option::long_name
/// @param _short_desc Option::short_desc
/// @param _long_desc Option:: long_desc
/// @param _arg_desc Option::arg_desc
/// @param _is_array Option::isarray
///
OptionTable &
OptionTable::add(Kind _kind, void * _val, int (*_convert)(void*, const string &),
                 const string & _long_name,
                 const string & _short_desc, const string & _long_desc,
                 const string & _arg_desc, const std::string & _dflt,
                 bool _is_array){
  if ( _kind != ARGUMENT )
    throw_error("add argument", "You've got the wrong add function for your kind.");
  return add ( Option(_kind, _val, _convert, 0, _long_name,
                      _short_desc, _long_desc, _arg_desc, _dflt, _is_array) );
}


/// Adds new note or manual page string to the entry table. If adding the note
/// just copies the *_desc into the appropriate elements of the entry.
/// If adding man-page then behaviour is more complex:
///   If the long description is empty then adds the short description would appear
///   in the man file as is, without any changes. Otherwise will add the
///   short description as the section header and long description as the
///   section's text.
///
/// @param _kind Kind of entry to add. In this context can be ::NOTE or ::MAN
/// @param _short_desc Note which would appear in the help messages and manual page
/// @param _long_desc Note which would appear in the verbose help message
///                   and manual page
///
OptionTable &
OptionTable::add(Kind _kind, const string & _short_desc, const string & _long_desc){
  if ( _kind == NOTE )
    return add(_kind, 0, 0, 0, "", _short_desc, _long_desc, "", "", false);
  else if ( _kind == MAN ) {
    if ( _long_desc.empty() )
      return add(_kind, 0, 0, 0, "", "", _short_desc, "", "", false);
    else
      return add(_kind, 0, 0, 0, "", _short_desc, _long_desc, "", "", false);
  } else {
    throw_error("add description", "You've got the wrong add function for your kind.");
    return *this;
  }
}


/// First puts the general_desc and general_long_desc into the note-entry of the
/// table. Then adds all entries of the _val to the table one-by-one.
///
/// @param _val The table to be incorporated.
///
OptionTable &
OptionTable::add( const OptionTable & _val ){
  if ( ! _val.general_desc.empty() || ! _val.general_long_desc.empty() )
    add( NOTE, _val.general_desc, _val.general_long_desc);
  if ( ! _val.options.size() )
    warn("add table", "Empty table to add.");
  for (ListO icur=_val.options.begin() ; icur != _val.options.end() ; ++icur)
    add(*icur);
  return *this;
}



/// @param _beverb The variable to be updated by the option-trigger
/// if 0-pointer, then uses internal one (OptionTable::beverb);
OptionTable &
OptionTable::add_verbose(bool *_beverb){
  if ( ! _beverb )
    _beverb=&beverb;
  add( OPTION, _beverb, 'v', "verbose", "Verbose output.", "");
  auto_verb = _beverb;
  return *this;
}

/// @param _useme The variable to be updated by the option-trigger
/// if 0-pointer, then uses internal one (OptionTable::useme);
OptionTable &
OptionTable::add_usage(bool *_useme){
  if ( ! _useme )
    _useme = &useme;
  add( OPTION, _useme, '?', "usage", "Outputs brief usage message.", "");
  auto_use = _useme;
  return *this;
}

/// @param _helpme The variable to be updated by the option-trigger
/// if 0-pointer, then uses internal one (OptionTable::helpme);
OptionTable &
OptionTable::add_help(bool *_helpme){
  if ( ! _helpme )
    _helpme = &helpme;
  add( OPTION, _helpme, 'h', "help", "Outputs help message.", "");
  auto_help = _helpme;
  return *this;
}


/// @param _beverb The variable to be updated by the option-trigger
/// if 0-pointer, then uses internal one (OptionTable::beverb);
/// @param _useme The variable to be updated by the option-trigger
/// if 0-pointer, then uses internal one (OptionTable::useme);
/// @param _helpme The variable to be updated by the option-trigger
/// if 0-pointer, then uses internal one (OptionTable::helpme);
OptionTable &
OptionTable::add_standard_options(bool *_beverb, bool *_helpme, bool *_useme){
  add(NOTE, "Standard options.", "");
  add_verbose(_beverb);
  add_usage(_useme);
  add_help(_helpme);
  find(auto_help)->long_desc=
    "When combined with the "
    "\"" + find(auto_verb)->delim_opt("|") + "\""
    " option may output more detailed message.";
  return *this;
}


/// Tells if the entry with the _val exists in the table.
///
/// @param _val - entry value to search for.
///
/// @return \c true if found, \c false otherwise.
///
bool
OptionTable::has(const void * _val) const {
  return find(_val) != options.end();
}


/// Tells if the entry with the _val exists in the table.
///
/// @param _long_name - long name to search for.
///
/// @return \c true if found, \c false otherwise.
///
bool
OptionTable::has(const string & _long_name) const {
  return find(_long_name) != options.end();
}


/// Tells if the entry with the _val exists in the table.
///
/// @param _char_name - entry value to search for.
///
/// @return \c true if found, \c false otherwise.
///
bool
OptionTable::has(const char _char_name) const {
  return find(_char_name) != options.end();
}



/// @param _val The value to check.
///
/// @return If _val is non-zero - number of the times the entry was
/// triggered from the CLI phrase. If _val is zero - number of times
/// all entries were triggered.
int
OptionTable::count(const void * _val) const {
  if ( ! _val )
    return accumulate(options.begin(), options.end(), 0,
                      [](int sum, const Option& opt){return sum + opt.counter;});
  if (!has(_val))
    throw_error("check option", "Could not find the option in the table.");
  return find(_val)->counter;
}

/// If the entry does not exist in the table and the pointer is non-zero,
/// the returned string is the pointer mark which later, after the entry
/// was added to the table, can be replaced using the ::replacePointers(string &).
///
/// @param _val The value to check
///
/// @return Description.
///
string
OptionTable::desc(const void * _val) const {
  if ( ! _val )
    throw_error("check option", "Zero-pointer.");
  if ( ! has(_val) ) {
    char s[128];
    //snprintf (s, 128, "%p", _val); // does not work on WIN32
    sprintf (s, "%p", _val);
    return pntrMark + s + pntrMark;
  }
  return find(_val)->desc();
}



/// Scans the string for the pointer marks, reads the pointer and replaces the mark with
/// the description given by ::desc(const void*).
///
/// @param str string to be changed.
///
void
OptionTable::replacePointers(std::string & str){
  size_t pos;
  while ( (pos=str.find(pntrMark)) != string::npos ) {
    void * pntr;
    if ( sscanf(str.c_str()+pos+pntrMark.length(), "%p", &pntr) != 1 )
      throw_error("reread pointer", "Found pointer mark, but failed to read"
                  " the pointer from the string \""+str+"\".");
    if ( ! has(pntr) )
      throw_error("reread pointer", "Did not find requested pointer in the table.");
    int sz = str.find(pntrMark, pos+1);
    if (sz == string::npos)
      throw_error("reread pointer", "Could not find end of the pointer in the string \""+str+"\".");
    sz += pntrMark.length() - pos ;
    str.replace(pos, sz, desc(pntr));
  }
}




string
OptionTable::name() const {
#ifdef _WIN32
  char fname[254], ext[254];
  _splitpath_s( general_name.c_str(), 0, 0, 0, 0, fname, 250, ext, 250);
  return string(fname)+string(ext);
#else
  size_t found = general_name.rfind('/');
  if (found==string::npos)
    return general_name;
  else
    return general_name.substr(found+1);
#endif
}

/// May be useful when you need just to print usage/verbose message before
/// parsing the CLI phrase.
///
/// @param _name New command name (usually set to argv[0]).
///
void
OptionTable::name(const string & _name) {
  general_name=_name;
}








// \defgroup Predef Predefined types.
// The "_conversion()" functions in this group are used for the
// parsing the "in" string into the, _val. The "type_desc()" functions
// just give the name of the argument type to be used as the Option::arg_desc.
//
// @{


/// Sets the value to \c true.
///
/// @param _val value to be updated
///
/// @return \c true
///
int
_conversion (bool* _val, const string &) {
  * _val = true;
  return 0;
}

/// Prints type name.
/// @return type name.
string
type_desc (bool*){
  return "";
}



/// Copies the "in" string into _val.
///
/// @param _val value to be updated.
/// @param in New value.
///
/// @return \c true
///
int
_conversion (string * _val, const string & in){
  * _val = in;
  return 1;
}

/// Prints type name.
/// @return type name.
string
type_desc (string*){
  return "STRING";
}



/// Converts the string "in" into char _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(char * _val, const string & in){
  if (in.length() != 1) {
    warn_parse("char", in, "the string is not single-char");
    return -1;
  }
  *_val = in[0];
  return 1;
}

/// Prints type name.
/// @return type name.
string
type_desc(char*){
  return "CHAR";
}





/// Converts the string "in" into unsigned char _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(unsigned char * _val, const string & in){
  if (in.length() != 1) {
    warn_parse("unsigned char", in, "the string is not single-char");
    return -1;
  }
  *_val = in[0];
  return 1;
}

/// Prints type name.
/// @return type name.
string
type_desc(unsigned char*){
  return "UCHAR";
}



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
bool num_conversion(T * _val, const std::string & in) {
  const std::string modname="numeric conversion";
  char * tail = 0 ;
  errno = 0;
  auto inval = spec_conversion<T>(in, &tail);
  *_val = (T)(inval);
  if (errno)
    warn(modname, "Failed to convert string \""+in+"\": value overflow");
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
             ( inval != 0.0 && fabsl(inval) < std::numeric_limits<T>::min() ) ) )
    warn(modname, "String \""+in+"\" contains float-point value outside type range.");
  else
    return true;
  return false;
}



/// Converts the string "in" into short integer _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(short * _val, const string & in){
  return num_conversion(_val, in);
}

/// Prints type name.
/// @return type name.
string
type_desc(short*){
  return "SHORT";
}



/// Converts the string "in" into unsigned short integer _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(unsigned short * _val, const string & in){
  return num_conversion(_val,in);
}

/// Prints type name.
/// @return type name.
string
type_desc(unsigned short*){
  return "USHORT";
}





/// Converts the string "in" into integer _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(int * _val, const string & in){
  return num_conversion(_val, in);
}

/// Prints type name.
/// @return type name.
string
type_desc(int*){
  return "INT";
}




/// Converts the string "in" into unsigned integer _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(unsigned int * _val, const string & in){
  return num_conversion(_val, in);
}

/// Prints type name.
/// @return type name.
string
type_desc(unsigned int*){
  return "UINT";
}






/// Converts the string "in" into long integer _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(long* _val, const string & in){
  return num_conversion(_val, in);
}

/// Prints type name.
/// @return type name.
string
type_desc(long*){
  return "LONG";
}



/// Converts the string "in" into unsigned long integer _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(unsigned long* _val, const string & in){
  return num_conversion(_val, in);
}

/// Prints type name.
/// @return type name.
string
type_desc(unsigned long*){
  return "ULONG";
}


#ifndef _WIN32

/// Converts the string "in" into long long integer _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(long long* _val, const string & in){
  return num_conversion(_val, in);
}

/// Prints type name.
/// @return type name.
string
type_desc(long long*){
  return "LLONG";
}




/// Converts the string "in" into unsigned long long integer _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(unsigned long long* _val, const string & in){
  return num_conversion(_val, in);
}

/// Prints type name.
/// @return type name.
string
type_desc(unsigned long long*){
  return "ULLONG";
}

#endif



/// Converts the string "in" into float _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(float* _val, const string & in){
  return num_conversion(_val, in);
}

/// Prints type name.
/// @return type name.
string
type_desc(float*){
  return "FLOAT";
}



/// Converts the string "in" into double float _val.
///
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
///
int
_conversion(double* _val, const string & in){
  return num_conversion(_val, in);
}

/// Prints type name.
/// @return type name.
string
type_desc(double*){
  return "DOUBLE";
}




/// @}


}

