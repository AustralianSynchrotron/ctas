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
/// @author antonmx <antonmx@synchrotron.org.au>
/// @date   Thu Jul 17 12:51:00 2008
///
/// @brief Definitions of function declared in common.h
///
/// Here the functions declared in common.h are defined.
///

#include "common.world.h"
#include <algorithm>
#include <functional>
#include <unistd.h>
#include <stdarg.h>

using namespace std;

#ifdef _WIN32
#  define _CRT_SECURE_NO_WARNINGS 1 // needed to avoid wornings in MS VC++
#endif



/// \cond
struct ToLower{ char operator() (char c) const { return tolower(c); } };
struct ToUpper{ char operator() (char c) const { return toupper(c); } };
/// \endcond

string
upper(string str){
  transform (whole(str), str.begin(), ToUpper());
  return str;
}

string
lower(string str){
  transform (whole(str), str.begin(), ToLower());
  return str;
}


string
toString(const string fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  const int MAXLINE = 4096;
  char buf[MAXLINE];
  vsnprintf(buf, MAXLINE, fmt.c_str(), ap);
  va_end(ap);
  return string(buf);
}




const clock_t startTV = clock();
clock_t prevTV = startTV;
const time_t startTM = time(0);
time_t prevTM = startTM;

void prdn( const string & str ) {
  clock_t nowTV = clock();
  double start_elapsed_tv = double( nowTV - startTV ) / CLOCKS_PER_SEC;
  double prev_elapsed_tv = double( nowTV - prevTV ) / CLOCKS_PER_SEC;
  time_t nowTM = time(0);
  int start_elapsed_tm = nowTM - startTM;
  int prev_elapsed_tm = nowTM - prevTM;
  printf("DONE %s:  %i(%f)  %i(%f)\n", str.c_str(),
         prev_elapsed_tm, prev_elapsed_tv, start_elapsed_tm, start_elapsed_tv);
  fflush(stdout);
  prevTV=nowTV;
  prevTM=nowTM;
}


/*
void
prdn(const string fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  const int MAXLINE = 4096;
  char buf[MAXLINE];
  vsnprintf(buf, MAXLINE, fmt.c_str(), ap);
  va_end(ap);
  prdn(string(buf));
}
*/


/// \brief Constructor.
///
/// @param _terr Sets CtasErr::terr
/// @param mod   Sets CtasErr::module
/// @param msg   Sets CtasErr::message
///
CtasErr::CtasErr(ErrTp _terr, const string & mod, const string & msg)
  : terr(_terr)
  , module(mod)
  , message(msg)
  , reported(false)
{}

/// \brief Prints the error into the standard error stream.
void
CtasErr::report() const {
  if (reported)
    return;
  switch ( terr ) {
  case WARN:
    cerr << "WARNING!";
    break;
  case ERR:
    cerr << "ERROR!";
    break;
  case FATAL:
    cerr << "FATAL ERROR!";
    break;
  }
  cerr << " In module \'" << module << "\'. " << message << endl;
  cerr.flush();
  reported=true;
}


string &
CtasErr::consume() {
  reported=true;
  return message;
}


CtasErr::ErrTp
CtasErr::type() const {
  return terr;
}


void
throw_error(const string & mod, const string & msg) {
  CtasErr err(CtasErr::ERR, mod, msg);
  err.report();
  throw err;
}

void
throw_bug(const string & mod){
  throw_error(mod, "Must never happen. This is the bug, please report to developers.");
}

CtasErr
warn(const string & mod, const string & msg){
  CtasErr err(CtasErr::WARN, mod, msg);
  err.report();
  return err;
}

void
exit_on_error(const string & mod, const string & msg){
  CtasErr err(CtasErr::FATAL, mod, msg);
  err.report();
  cerr << "!!! EXITING ON ERROR !!!" << endl;
  exit(1);
}



deque<string> split (const string & str, const string & delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  deque<string> res;
  while ((pos_end = str.find (delimiter, pos_start)) != string::npos) {
    res.push_back (str.substr(pos_start, pos_end - pos_start));
    pos_start = pos_end + delim_len;
  }
  res.push_back (str.substr (pos_start));
  return res;
}



#ifdef _WIN32
const string Path::DIRSEPARATOR = "\\";
#else
const string Path::DIRSEPARATOR = "/";
#endif

const Path Path::emptypath = Path();


/// Constructs the error which reports the unextractable element.
///
/// @param element The element whose extraction has failed.
///
void
Path::throw_unextracted(const string & element) const {
  throw_error
        ("class Path",
         "Could not extract " + element + " from path \"" + *this + "\".");
}

string
Path::drive () const {
#ifdef _WIN32
  char _drive[FILENAME_MAX];
  if ( _splitpath_s( c_str(), _drive, FILENAME_MAX, 0,0,0,0,0,0 ) )
        throw_unextracted("drive");
  return _drive;
#else
  return "";
#endif
}

string
Path::dir () const {
#ifdef _WIN32
  char _dir[FILENAME_MAX];
  if ( _splitpath_s( c_str(),0,0,_dir,FILENAME_MAX,0,0,0,0 ) )
        throw_unextracted("directory");
  return drive() + _dir;
#else
  string::size_type idx=this->rfind("/");
  return  idx == string::npos  ?  string("")  :  string(*this, 0, idx+1);
#endif
}

string
Path::name () const {
#ifdef _WIN32
  return title() + ext();
#else
  size_t found = this->rfind('/');
  if (found==string::npos)
    return *this;
  else
    return this->substr(found+1);
#endif
};


string
Path::title () const {
#ifdef _WIN32
  char _title[FILENAME_MAX];
  if ( _splitpath_s( c_str(), 0,0,0,0, _title, FILENAME_MAX, 0,0 ) )
        throw_unextracted("title");
  return _title;
#else
  string::size_type idx = name().rfind(".");
  if (idx==string::npos || idx == 0 )
        return name();
  else
        return name().substr(0, idx);
#endif
}

string
Path::dtitle () const {
  return dir() + title();
}


string
Path::ext () const {
#ifdef _WIN32
  char _ext[FILENAME_MAX];
  if ( _splitpath_s( c_str(), 0,0,0,0,0,0, _ext, FILENAME_MAX ) )
        throw_unextracted("extension");
  return _ext;
#else
  string::size_type
        dotidx = this->rfind("."),
        diridx = this->rfind("/");
  if ( dotidx != string::npos && ( diridx == string::npos || diridx+1 < dotidx ) )
        return this->substr(dotidx);
  else
        return "";
#endif
}


vector<string>
Path::elements( ) const {
#ifdef _WIN32
  // TODO
  return vector<string>;
#else
  vector<string> toRet;
  size_t poss = 0;
  while ( poss != string::npos) {
    size_t pose = this->find("/",poss);
    string elm = this->substr(poss, pose);
    if (elm.size())
      toRet.push_back(elm);
    poss = pose == string::npos ? pose : pose + 1;
  }
  return toRet;
#endif

}



bool
Path::isdir() const {
  return ! empty() && name().empty();
}

Path &
Path::bedir() {
  if ( ! empty() && ! isdir() )
        *this += DIRSEPARATOR;
  return *this;
}


bool
Path::isabsolute() const {
#ifdef _WIN32
  return ! drive().empty();
#else
  return ! empty() && (*this)[0] == '/';
#endif
}


const Path Path::home() {
  char * hm;
  #ifdef _WIN32
  //return getenv("USERPROFILE");
  hm = getenv("APPDATA");
  #else
  hm = getenv("HOME");
  #endif
  return Path(hm).bedir();
}


Path
upgrade(const Path & path, const string & addthis) {
  return path.dir() + Path(addthis) + path.name();
}


Path
cdpath(const Path & dir, const Path & file){
  Path ddir = dir;
  ddir.bedir();

  if ( file.empty() )
        return ddir;
  else if ( file.isabsolute() || ddir.empty() )
        return file;
  else
      // Windows version is likely to fail here. See Path operator+.
        return ddir + file;
}


string
type_desc (Path*) {
  return "PATH";
}

int
_conversion (Path* _val, const string & in) {
  *_val = Path(in);
  return 1;
}



string
mask2format(const string & mask, int maxslice){
  string format(mask);
  // replace all '%' by "%%"
  string::size_type pos = format.find('%');
  while ( pos != string::npos ) {
    format.insert(pos, "%");
    pos = format.find('%', pos+2);
  }
  //replace last '@' with the format expression.
  auto atpos = format.rfind('@');
  if (atpos == string::npos) {
    atpos = format.find_last_of('.');
    format.insert(atpos,"@");
    atpos = format.rfind('@');
  }
  format.replace( atpos, 1, "%0" + toString( toString(maxslice-1).length() ) + "u");
  return format;
}



/// \brief Adds or removes the element into the array.
///
/// Adds or removes the element numb to/from the array depending on negation.
///
/// @param arr The array to be updated.
/// @param numb The element to be added/removed to/from the array.
/// @param negate If \c true then removes, if \c false then adds to the array.
///
/// @return Updated array.
///
static inline deque<int> &
rmadd(deque<int> & arr, int numb, bool negate){
  if (negate)
        arr.erase( remove( whole(arr), numb ), arr.end() );
  else
        arr.push_back(numb);
  return arr;
}

/// \brief Parses the string to non-negative integer.
///
/// @param str The string to parse.
///
/// @return The result of parsing.
///
static inline int str2n(const string & str){
  return strtol ( str.c_str() , 0, 0);
  //long idx = strtol ( str.c_str() , 0, 0);
  //if ( ! idx )
  //      warn("string to int", "Zero slice. Must be positive. Set to 1.");
  //return idx ? idx-1 : 1;
}




deque<int>
slice_str2vec(const string & sliceS, int hight){
  // empty string
  if ( sliceS.empty() ) {
    deque<int> ret(abs(hight));
    for (int slice = 0 ; slice < abs(hight) ; slice++)
      ret[slice]=slice;
    return ret;
  }

  const char negatec = 'n';
  const string permitted_chars = string("0123456789,-") + negatec ;
  deque<int> sliceV; // the array to be returned as the result.

  // construct the vector of substrings and checks for the global negation.
  deque<string> subSV;
  bool negateall = true; // turns to false if an unnegated substring found.
  string::size_type startidx=0, endidx;
  do {

    // extract substring
    endidx = sliceS.find(',', startidx);
    string subS = sliceS.substr(startidx, ( endidx == string::npos
                        ? sliceS.length() : endidx-startidx) );
    startidx=endidx+1;

    // checks for the non permitted characters
    if ( subS.find_first_not_of(permitted_chars) != string::npos ) {
      warn("slice string", "Substring \""+ subS +"\" in the string describing set of slices"
                           " has character(s) not from the permitted set"
                           " \""+ permitted_chars +"\". Skipping.");
      subS.erase();
    }

    // check it and add to the vector of substrings.
    if ( ! subS.empty() ) {

      const string initS = subS;

      // modifies in regards to the negatec
      string::size_type lastneg = subS.rfind(negatec);
      if ( lastneg == string::npos )
        negateall = false;
      else if ( lastneg != 0 ) {
        warn("slice string", "Suspicious substring \""+ initS +"\" in the string describing set of slices:"
                             " it has '" + negatec + "' character not in the first position."
                             " Moving it to the begining. Is it what you meant?");
        // moves all negatec's to the beginning and erases it's duplicates.
        subS.erase( subS.begin(), stable_partition
                    ( whole(subS), bind(equal_to<char>(), placeholders::_1, negatec) ) - 1 );
      }

      // modifies in regards to '-'
      if ( count(whole(subS), '-') > 1 ) {
        warn("slice string", "Substring \""+ initS +"\" in the string describing set of slices"
                             " has more than one minus sign '-'. Everything between first"
                             " and last minuses is ignored. Is it what you meant?");
        subS = subS.substr(0,subS.find('-')) + subS.substr(subS.rfind('-') );
      }

      // make sure minus is surrounded by numbers.
      if ( subS[0] == '-' )
        subS = "0" + subS;
      if ( subS.substr(0,2) == "n-" )
        subS.insert(1,"0");
      if ( subS[subS.length()-1] == '-' )
        subS += toString(hight-1);

      subSV.push_back(subS);

    }

  } while ( string::npos != endidx );


  // Check for global negation.
  if ( ( negateall |= ( subSV[0] == string(1,negatec) ) ) )
    for ( int icur = 0 ; icur < abs(hight) ; icur++)
      sliceV.push_back(icur);
  //subSV.erase( remove( whole(subSV), string(1,negatec) ), subSV.end() ); // no "negate" strings

  // adds/removes substrings into the array of slices
  deque<string>::iterator subSVi = subSV.begin();
  while ( subSVi != subSV.end() ) {

    bool negatethis = (*subSVi)[0]==negatec;
    if (negatethis)
      (*subSVi).erase(0,1);
    if (negateall)
      negatethis = !negatethis;

    string::size_type minuspos = (*subSVi).find('-');
    if ( minuspos != string::npos ) {
      int
        rangeB = str2n( (*subSVi).substr(0,minuspos) ),
        rangeE = str2n( (*subSVi).substr(minuspos+1) );
      if (rangeE < 0) // hight <= 0 and open end
        rangeE = hight ? rangeB-hight-1 : rangeB;
      else if ( rangeB == rangeE )
        warn("slice string", "One of the substrings with ranges in the string describing set of slices"
                             " has equal ends of the ranges. Is it what you meant?");
      const int inc  =  rangeE < rangeB  ?  -1  :  1;
      for (int curS = rangeB ; (rangeE-curS)*inc >=0  ; curS+= inc)
        rmadd(sliceV, curS, negatethis);
    } else {
      rmadd(sliceV, str2n( *subSVi ), negatethis );
    }

    ++subSVi;

  }

  //// sort and remove duplicates, too large numbers
  //sort(whole(sliceV));
  //sliceV.erase( unique( whole(sliceV) ), sliceV.end() );
  if (hight<=0)
    return slice_str2vec(sliceS, 1 + *max_element(whole(sliceV)));

  sliceV.erase( find_if(whole(sliceV), bind(greater<int>(), placeholders::_1, hight-1 ) ) , sliceV.end());
  // last check
  if ( sliceV.empty() )
    warn("slice string",
         "String \""+sliceS+"\" describing set of up to "+toString(hight)+" slices leads to the empty range of slices." );
  return sliceV;

}





/// \brief Initializing constructor.
///
/// @param _showme Tells if the progress bar should be shown.
/// @param _message The description of the progress.
/// @param _steps Number of steps in the progress. If 0 then unknown.
///
ProgressBar::ProgressBar(bool _showme, const string & _message, int _steps)
  : showme(_showme)
  , message(_message)
  , proglock(PTHREAD_MUTEX_INITIALIZER)
{
  if ( ! showme ) return;
  setSteps(_steps);
}


void
ProgressBar::setSteps(int _steps) {
  //if (steps)
  //  warn("ProgressBar", "Resetting steps of the progress bar.");
  steps=_steps;
  step=0;
  waswidth=0;
  reservedChs=0;
}

void
ProgressBar::start(){
  if ( !showme ) return;
  if ( reservedChs )
    return; // Was already started

  cout << "Starting process";
  if (steps>1)
    cout << " (" + toString(steps) + " steps)";
  cout << ": " << message;
  if ( steps == 1 )
    cout << " ... ";
  else
    cout << "." << endl;
  fflush(stdout);

  int nums = toString(steps).length();
  reservedChs = 14 + 2*nums;
  fmt = steps
        ? "%" + toString(nums) + "i of " + toString(steps) + " [%s] %s"
        : string( "progress: %i" );
}

/// \brief Updates the progress bar.
///
/// @param curstep Current step. Advances +1 if zero.
///
void
ProgressBar::update(int curstep){
  if ( !showme ) return;
  pthread_mutex_lock(&proglock);
  try {
    start(); // Does nothing if already started

    step = curstep ? curstep+1 : step + 1;
    if ( steps && step >= steps ) {
      done();
      throw 1;
    }
    const int wdth = getwidth();
    if (!wdth) {
      cout << step;
      if (steps)
        cout << "/" << steps;
      cout << endl;
      fflush(stdout);
      throw 1;
    }
    const int progln = wdth - reservedChs;
    if ( progln <= 3 ) // if we have a very narrow terminal
      throw 1;

    string outS;
    if (steps) {
      string eqs = string(progln*step/steps, '=') + string(progln, ' ') ;
      eqs.erase(progln);
      string prc = toString("%5.1f%% ", (100.0*step)/steps);
      outS = toString(fmt, step, eqs.c_str(), prc.c_str() );
    } else {
      outS = toString(fmt, step);
    }

    cout << string(waswidth+1, '\b') << outS;
    fflush(stdout);
    waswidth = outS.length();

  }  catch (...) {}
  pthread_mutex_unlock(&proglock);
}


void
ProgressBar::done(){
  if ( !showme || ! reservedChs )
    return;
  const int wdth = getwidth();
  if (wdth && steps != 1) {
    int progln = wdth - reservedChs;
    if ( progln < 0 )  progln = 0; // if we have a very narrow terminal
    string eqs(progln, '=');
    cout << string(waswidth+1, '\r')
         << ( steps ?
                toString(fmt, steps, eqs.c_str(), "") :
                toString(fmt, step) + " steps." );
  }
  cout << " DONE." << endl;
  fflush(stdout);
  reservedChs = 0;
}


#ifdef _WIN32
#  include <windows.h>
#else
#  include<termios.h>
#  include<sys/ioctl.h>
#endif

int
ProgressBar::getwidth(){
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO info;
  HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);
  if (fd == INVALID_HANDLE_VALUE)
    return 0;
  return GetConsoleScreenBufferInfo (fd, &info) ? info.dwSize.X - 1 : 0;
  //return (info.srWindow.Right - info.srWindow.Left + 1);
#else
  winsize size;
  if ( ! isatty(fileno(stdout)) )
    return 0;
  else if ( ioctl (STDOUT_FILENO, TIOCGWINSZ, &size ) < 0  ||  size.ws_col < 2)
    return 1;
  else
    return size.ws_col - 1;
#endif
}




/// Lists all manual pages in the package. This string to be added to
/// every manual page.
const std::string SeeAlsoList =
  "ctas(1), ctas-bg(1), ctas-ct(1), ctas-ct-abs(1), ctas-ct-dei(1),"
  " ctas-ct-edei(1), ctas-ct-ipc(1),"
  " ctas-dei(1), ctas-edei(1), ctas-ipc(1), ctas-f2i(1), ctas-ff(1),"
  " ctas-sino(1), ctas-sino-abs(1), ctas-sino-dei(1), ctas-sino-ipc(1),"
  " ctas-ts(1), ctas-ct-line(1)";








/*
Path findCommon(const vector<Path>::const_iterator _bgn, const vector<Path>::const_iterator _end) {

  vector<Path> touse;
  for (vector<Path>::const_iterator crnt=_bgn ; crnt<_end ; crnt++)
    touse.push_back(imageFile(*crnt));
  const vector<Path>::const_iterator bgn = touse.begin(),
                                     end = touse.end();
  int len = bgn->length();
  if ( bgn>=end  ||  ! len )
    return *bgn;

  for (vector<Path>::const_iterator crnt=bgn ; crnt<end ; crnt++)
    if ( len != crnt->length() )
      len = 0;

  if (len) {
    string ret;
    for ( int idxc = 0 ; idxc < len ; idxc++ ) {
      char cchar = bgn->at(idxc);
      for (vector<Path>::const_iterator crnt=bgn+1 ; crnt<end ; crnt++)
        if ( cchar != crnt->at(idxc) )
          cchar=0;
      if (cchar)
        ret += cchar;
    }
    return ret;
  }

  bool keepGoing=true;
  int idxp=0;
  while ( keepGoing  &&  idxp < bgn->length() ) {
    char cchar = bgn->at(idxp);
    for (vector<Path>::const_iterator crnt=bgn ; crnt<end ; crnt++)
      keepGoing &= idxp < crnt->length()  &&  crnt->at(idxp) == cchar;
    if (keepGoing)
      idxp++;
  }
  const string prefix(*bgn, 0, idxp);

  keepGoing=true;
  int idxs=0;
  while ( keepGoing  &&  idxs < bgn->length() ) {
    char cchar = bgn->at( bgn->length() - 1 - idxs );
    for (vector<Path>::const_iterator crnt=bgn ; crnt<end ; crnt++)
      keepGoing &= idxs < crnt->length()  &&  crnt->length() - idxs > idxp  &&  crnt->at( crnt->length() - 1 - idxs ) == cchar;
    if (keepGoing)
      idxs++;
  }
  const string suffix(*bgn, bgn->length() - idxs, idxs);

  return prefix+suffix;


}
*/


