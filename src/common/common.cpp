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

#ifdef _WIN32
#  define _CRT_SECURE_NO_WARNINGS 1 // needed to avoid wornings in MS VC++
#endif

#include <algorithm>
#include <stdarg.h>
#include <string.h> // for memcpy
#include <unistd.h>
#include <climits>
#include <ctime>
#include <cmath>
#include <ctype.h>

#include "common.h"
#include "poptmx.h"



using namespace std;

const clock_t startTV = clock();
clock_t prevTV = startTV;

void prdn( const string & str ) {
  clock_t nowTV = clock();
  double start_elapsed = double( nowTV - startTV ) / CLOCKS_PER_SEC;
  double prev_elapsed = double( nowTV - prevTV ) / CLOCKS_PER_SEC;
  printf("DONE %s:  %f  %f\n", str.c_str(), prev_elapsed, start_elapsed);
  fflush(stdout);
  prevTV=nowTV;
}

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
{}

/// \brief Prints the error into the standard error stream.
void
CtasErr::report() const {
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
  return title() + extension();
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
Path::extension () const {
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
  format.replace( format.rfind('@'), 1,
                  "%0" + toString( toString(maxslice).length() ) + "u");
  return format;
}












const string
Contrast::modname="contrast";

/// \brief Constructor.
///
/// @param _cn Contrast type.
Contrast::Contrast(Cntype _cn)
  : _contrast(_cn) {}

/// \brief Constructor from name
///
/// @param _name Contrast name.
Contrast::Contrast(const string & _name) {
  string name = upper(_name);
  if ( name == "A" || name == "ABS" || name == "ABSORPTION" ) _contrast = ABS;
  else if ( name == "R" || name == "REF" || name == "REFRACTION" ) _contrast = REF;
  else if ( name == "P" || name == "PHS" || name == "PHASE" ) _contrast = PHS;
  else if ( name == "F" || name == "FLT" || name == "FILTERED" ) _contrast = FLT;
  else throw_error("contrast type", "The string \""+ _name +"\""
                                   " does not describe any known contrast.");
}

/// \brief Type of contrast
///
/// @return Type of contrast
///
Contrast::Cntype
Contrast::contrast() const {
  return _contrast;
}


/// \brief Name of the contrast.
///
/// @return Name of the contrast.
///
string
Contrast::name() const {
  switch (_contrast) {
  case REF:     return "REFRACTION";
  case ABS:     return "ABSORPTION";
  case PHS:     return "PHASE";
  case FLT:     return "FILTERED";
  default :
          throw_bug(__FUNCTION__);
          return "";
  }
}

const std::string Contrast::Desc =
            "Must be one of the following strings (case insensitive):\n"
            "A, ABS, ABSORPTION - for the absorption contrast\n"
            "R, REF, REFRACTION - for the refraction contrast\n"
            "P, PHS, PHASE      - for the phase contrast\n"
            "F, FLT, FILTERED   - for the pre-filtered contrast";



bool
operator==(const Contrast & a, const Contrast & b){
  return a.contrast() == b.contrast();
}


bool
operator!=(const Contrast & a, const Contrast & b){
  return a.contrast() != b.contrast();
}

string
type_desc (Contrast*){
  return "STRING";
}

int
_conversion (Contrast* _val, const string & in) {
  *_val=Contrast(in);
  return 1;
}







string
type_desc (Crop3*){
  return "UINT:UINT:UINT:UINT:UINT:UINT";
}

int
_conversion (Crop3* _val, const string & in) {
  int l, r, t, b, f, k;
  int scanres = sscanf( in.c_str(), "%i:%i:%i:%i:%i:%i", &t, &l, &b, &r, &f, &k);
  if (scanres != 6  &&  scanres != 4) // try , instead of :
    scanres = sscanf( in.c_str(), "%i,%i,%i,%i,%i,%i", &t, &l, &b, &r, &f, &k);
  if (scanres==4) {
    f=0;
    k=0;
  } else if (scanres!=6)
    return -1;
  if ( l<0 || r<0 || t<0 || b<0 || f<0 || k<0 )
    return -1;
  *_val = Crop3(t, l, b, r, f , k);
  return 1;
}



void
crop(const Volume & inarr, Volume & outarr, const Crop3 & crp) {

  if ( ! crp.left && ! crp.right && ! crp.top && ! crp.bottom && ! crp.face && ! crp.back ) {
    outarr.reference(inarr);
    return;
  }
  if (  crp.left + crp.right  >= inarr.shape()(2)  ||
        crp.top  + crp.bottom >= inarr.shape()(1)  ||
        crp.face + crp.back   >= inarr.shape()(0) ) {
    warn("Crop3 array", "Cropping (" + toString(crp) + ") is larger than array size.");
    outarr.free();
    return;
  }

  outarr.resize( inarr.shape()(0) - crp.face - crp.back,
                 inarr.shape()(1) - crp.top  - crp.bottom,
                 inarr.shape()(2) - crp.left - crp.right);
  outarr = inarr( blitz::Range(crp.face, inarr.shape()(0)-1-crp.back),
                  blitz::Range(crp.top,  inarr.shape()(0)-1-crp.bottom ),
                  blitz::Range(crp.left, inarr.shape()(1)-1-crp.right ) );

}

void
crop(Volume & io_arr, const Crop3 & crp) {

  if ( ! crp.left && ! crp.right && ! crp.top && ! crp.bottom && ! crp.face && ! crp.back )
    return;

  if (  crp.left + crp.right  >= io_arr.shape()(2)  ||
        crp.top  + crp.bottom >= io_arr.shape()(1)  ||
        crp.face + crp.back   >= io_arr.shape()(0) ) {
    warn("Crop3 array", "Cropping (" + toString(crp) + ") is larger than array size.");
    io_arr.free();
    return;
  }

  io_arr.reference( io_arr( blitz::Range(crp.face, io_arr.shape()(0)-1-crp.back),
                            blitz::Range(crp.top,  io_arr.shape()(0)-1-crp.bottom ),
                            blitz::Range(crp.left, io_arr.shape()(1)-1-crp.right ) ) );

}















string
type_desc (Crop*){
  return "UINT:UINT:UINT:UINT";
}

int
_conversion (Crop* _val, const string & in) {
  int l, r, t, b;
  int scanres = sscanf( in.c_str(), "%i:%i:%i:%i", &t, &l, &b, &r);
  if (scanres != 4) // try , instead of :
    scanres = sscanf( in.c_str(), "%i,%i,%i,%i", &t, &l, &b, &r);
  if ( 4 != scanres || l<0 || r<0 || t<0 || b<0 )
    return -1;
  *_val = Crop(t, l, b, r);
  return 1;
}



void
crop(const Map & inarr, Map & outarr, const Crop & crp) {

  if ( ! crp.left && ! crp.right && ! crp.top && ! crp.bottom ) {
    outarr.reference(inarr);
    return;
  }
  if (  crp.left + crp.right >= inarr.shape()(1)  ||
        crp.top + crp.bottom >= inarr.shape()(0) ) {
    warn("Crop array", "Cropping (" + toString(crp) + ")"
         " is larger than array size ("+toString(inarr.shape())+")");
    outarr.free();
    return;
  }

  outarr.resize( inarr.shape()(0) - crp.top  - crp.bottom,
                 inarr.shape()(1) - crp.left - crp.right);
  outarr = inarr( blitz::Range(crp.top,  inarr.shape()(0)-1-crp.bottom ),
                  blitz::Range(crp.left, inarr.shape()(1)-1-crp.right ) );

}

void
crop(Map & io_arr, const Crop & crp) {
  if ( ! crp.left && ! crp.right && ! crp.top && ! crp.bottom )
    return;
  if (  crp.left + crp.right >= io_arr.shape()(1)  ||
        crp.top + crp.bottom >= io_arr.shape()(0) ) {
    warn("Crop array", "Cropping (" + toString(crp) + ")"
         " is larger than array size ("+toString(io_arr.shape())+")");
    io_arr.free();
    return;
  }
  io_arr.reference( io_arr( blitz::Range(crp.top,  io_arr.shape()(0)-1-crp.bottom ),
                            blitz::Range(crp.left, io_arr.shape()(1)-1-crp.right ) ) );
}







#ifdef OPENCL_FOUND

const char binnSource[] = {
  #include "binn.cl.includeme"
};

const cl_program binnProgram =
  initProgram( binnSource, sizeof(binnSource), "Binn on OCL" );

#endif



const std::string Binn3OptionDesc =
  "3D Binning factor(s) X:Y:Z. If zero - averages over the whole dimension.";


string
type_desc (Binn3*) {
  return "UINT[:UINT:UINT]";
}

int
_conversion (Binn3* _val, const string & in) {

  int x=0, y=0, z=0;

  if ( in.find_first_of(",:") !=  string::npos ) {

    int scanres = sscanf( in.c_str(), "%i:%i:%i", &x, &y, &z);
    if (scanres != 3) // try , instead of :
      scanres = sscanf( in.c_str(), "%i,%i,%i", &x, &y, &z);
    if ( 3 != scanres || x<0 || y<0 || z<0)
      return -1;

  } else {
    int xy;
    if ( 1 != sscanf( in.c_str(), "%i", &xy ) || xy<0 )
      return -1;
    x=xy;
    y=xy;
    z=xy;

  }

  *_val = Binn3(x, y, z);
  return 1;

}


void
binn(const Volume & inarr, Volume & outarr, const Binn3 & ibnn) {

  if ( ibnn.x == 1 && ibnn.y == 1 && ibnn.z == 1) {
    outarr.reference(inarr);
    return;
  }
  Binn3 bnn( ibnn.x ? ibnn.x : inarr.shape()(2) ,
             ibnn.y ? ibnn.y : inarr.shape()(1) ,
             ibnn.z ? ibnn.z : inarr.shape()(0) );
  Shape3 osh(inarr.shape()(0) / bnn.z
               , inarr.shape()(1) / bnn.y
               , inarr.shape()(2) / bnn.x);
  outarr.resize(osh);

#ifdef OPENCL_FOUND

  try {

    CLmem clinarr(blitz2cl(inarr, CL_MEM_READ_ONLY));
    CLmem cloutarr(clAllocArray<float>(outarr.size(), CL_MEM_WRITE_ONLY));
    cl_kernel kernelBinn3 = createKernel(binnProgram, "binn3");

    setArg(kernelBinn3, 0, clinarr());
    setArg(kernelBinn3, 1, cloutarr());
    setArg(kernelBinn3, 2, (cl_int) bnn.x);
    setArg(kernelBinn3, 3, (cl_int) bnn.y);
    setArg(kernelBinn3, 4, (cl_int) bnn.z);
    setArg(kernelBinn3, 5, (cl_int) inarr.shape()(2));
    setArg(kernelBinn3, 6, (cl_int) inarr.shape()(1));
    setArg(kernelBinn3, 7, (cl_int) osh(2));
    setArg(kernelBinn3, 8, (cl_int) osh(1));

    execKernel(kernelBinn3, osh);
    cl2blitz(cloutarr(), outarr);

  }  catch (...) { // full volume was too big for the gpu

    Map inslice(inarr.shape()(1), inarr.shape()(0));
    CLmem clinslice(clAllocArray<float>(inslice.size(), CL_MEM_READ_ONLY));
    Map outslice(osh(1), osh(0));
    CLmem cloutslice(clAllocArray<float>(outslice.size()));
    Map tmpslice(osh(1), osh(0));
    CLmem cltmpslice(clAllocArray<float>(outslice.size()));

    cl_kernel kernelBinn2 = createKernel(binnProgram, "binn2");
    setArg(kernelBinn2, 0, clinslice());
    setArg(kernelBinn2, 1, cltmpslice());
    setArg(kernelBinn2, 2, (cl_int) bnn.x);
    setArg(kernelBinn2, 3, (cl_int) bnn.y);
    setArg(kernelBinn2, 4, (cl_int) inslice.shape()(1));
    setArg(kernelBinn2, 5, (cl_int) outslice.shape()(1));

    cl_kernel kernelAddTo = createKernel(binnProgram, "addToSecond");
    setArg(kernelAddTo, 0, cltmpslice());
    setArg(kernelAddTo, 1, cloutslice());

    cl_kernel kernelMulti = createKernel(binnProgram, "multiplyArray");
    setArg(kernelMulti, 0, cloutslice());
    setArg(kernelMulti, 1, (cl_float) bnn.z);

    for (int z = 0  ;  z < osh(0)  ;  z++ ) {
      fillClArray(cloutslice(), outslice.size(), 0);
      for (int cz=0 ; cz<bnn.z ; cz++) {
        inslice = inarr(z*bnn.z+cz, all, all);
        blitz2cl(inslice, clinslice());
        execKernel(kernelBinn2, outslice.shape());
        execKernel(kernelAddTo, outslice.size());
      }
      execKernel(kernelMulti, outslice.shape());
      cl2blitz(cloutslice(), outslice);
      outarr(z, all, all) = outslice;
    }

  }

#else // OPENCL_FOUND

  const size_t bsz = bnn.x * bnn.y * bnn.z;
  for (ArrIndex zcur = 0 ; zcur < osh(0) ; zcur++ )
    for (ArrIndex ycur = 0 ; ycur < osh(1) ; ycur++ )
      for (ArrIndex xcur = 0 ; xcur < osh(2) ; xcur++ )
        // BUG in blitz? But the following fails.
        //outarr(zcur,ycur,xcur) = mean(
        //  inarr( bnn.z  ?  blitz::Range(zcur*bnn.z, zcur*bnn.z+bnn.z-1)  :  all
        //       , bnn.y  ?  blitz::Range(ycur*bnn.y, ycur*bnn.y+bnn.y-1)  :  all
        //       , bnn.x  ?  blitz::Range(xcur*bnn.x, xcur*bnn.x+bnn.x-1)  :  all ));
      {
        float sum=0;
        for (int zbcur = 0 ; zbcur < bnn.z ; zbcur++ )
          for (int ybcur = 0 ; ybcur < bnn.y ; ybcur++ )
            for (int xbcur = 0 ; xbcur < bnn.x ; xbcur++ )
              sum += inarr( zcur*bnn.z+zbcur, ycur*bnn.y+ybcur, xcur*bnn.x+xbcur );
        outarr(zcur,ycur,xcur) = sum / bsz;
      }

#endif // OPENCL_FOUND

}

void
binn(Volume & io_arr, const Binn3 & bnn) {
  Volume outarr;
  binn(io_arr, outarr, bnn);
  if( io_arr.data() == outarr.data() )
    return;
  io_arr.resize(outarr.shape());
  io_arr=outarr;
}





const string BinnOptionDesc =
  "2D Binning factor(s) X:Y. If zero - averages over the whole dimension.";

string
type_desc (Binn*) {
  return "UINT[:UINT]";
}

int
_conversion (Binn* _val, const string & in) {

  int x, y;

  if ( in.find_first_of(",:") !=  string::npos ) {

    int scanres = sscanf( in.c_str(), "%i:%i", &x, &y);
    if (scanres != 2) // try , instead of :
      scanres = sscanf( in.c_str(), "%i,%i", &x, &y);
    if ( 2 != scanres || x<0 || y<0 )
      return -1;

  } else {

    int xy;
    if ( 1 != sscanf( in.c_str(), "%i", &xy ) || xy<0 )
      return -1;
    x=xy;
    y=xy;

  }

  *_val = Binn(x, y);
  return 1;

}


void
binn(const Map & inarr, Map & outarr, const Binn & ibnn) {

  if ( ibnn.x == 1 && ibnn.y == 1 ) {
    outarr.reference(inarr);
    return;
  }
  Binn bnn( ibnn.x ? ibnn.x : inarr.shape()(1) ,
            ibnn.y ? ibnn.y : inarr.shape()(0) );
  outarr.resize(inarr.shape()(0) / bnn.y, inarr.shape()(1) / bnn.x);

#ifdef OPENCL_FOUND

  CLmem clinarr(blitz2cl(inarr, CL_MEM_READ_ONLY));
  CLmem cloutarr(clAllocArray<float>(outarr.size(), CL_MEM_WRITE_ONLY));
  cl_kernel kernelBinn2 = createKernel(binnProgram, "binn2");

  setArg(kernelBinn2, 0, clinarr());
  setArg(kernelBinn2, 1, cloutarr());
  setArg(kernelBinn2, 2, (cl_int) bnn.x);
  setArg(kernelBinn2, 3, (cl_int) bnn.y);
  setArg(kernelBinn2, 4, (cl_int) inarr.shape()(1));
  setArg(kernelBinn2, 5, (cl_int) outarr.shape()(1));

  execKernel(kernelBinn2, outarr.shape());
  cl2blitz(cloutarr(), outarr);

#else // OPENCL_FOUND

  for (ArrIndex ycur = 0 ; ycur < outarr.shape()(0) ; ycur++ )
    for (ArrIndex xcur = 0 ; xcur < outarr.shape()(1) ; xcur++ )
      outarr(ycur,xcur) = mean(
        inarr( bnn.y  ?  blitz::Range(ycur*bnn.y, ycur*bnn.y+bnn.y-1)  :  all
             , bnn.x  ?  blitz::Range(xcur*bnn.x, xcur*bnn.x+bnn.x-1)  :  all ) );

#endif // OPENCL_FOUND

}

void
binn(Map & io_arr, const Binn & bnn) {
  Map outarr;
  binn(io_arr, outarr, bnn);
  if( io_arr.data() == outarr.data() )
    return;
  io_arr.resize(outarr.shape());
  io_arr=outarr.copy();
}




void rotate(const Map & inarr, Map & outarr, float angle, float bg) {

  const Shape sh = inarr.shape();

  if ( abs( remainder(angle, M_PI/2) ) < 1.0/max(sh(0),sh(1)) ) { // close to a 90-deg step

    const int nof90 = round(2*angle/M_PI);

    if ( ! (nof90%4)  ) { // 360deg
      outarr.reference(inarr);
    } else if ( ! (nof90%2) ) { //180deg
      outarr.resize(sh);
      outarr=inarr.copy().reverse(blitz::firstDim).reverse(blitz::secondDim);
    } else if (  ( nof90 > 0 && (nof90%3) ) || ( nof90 < 0 && ! (nof90%3) ) ) { // 270deg
      outarr.resize(sh(1),sh(0));
      outarr=inarr.copy().transpose(blitz::firstDim, blitz::secondDim).reverse(blitz::secondDim);
    } else { // 90deg
      outarr.resize(sh(1),sh(0));
      outarr=inarr.copy().transpose(blitz::firstDim, blitz::secondDim).reverse(blitz::firstDim);
    }

    return;

  }

  const float cosa = cos(-angle), sina = sin(-angle);
  const int
    rwidth = abs( sh(1)*cosa ) + abs( sh(0)*sina),
    rheight = abs( sh(1)*sina ) + abs( sh(0)*cosa);

  const Shape shf(rheight, rwidth);
  outarr.resize(shf);

  if ( ! isnormal(bg) ) {
    bg=0;
    bg += mean( inarr( all, 0 ) );
    bg += mean( inarr( 0, all ) );
    bg += mean( inarr( all, sh(1)-1 ) );
    bg += mean( inarr( sh(0)-1, all ) );
    bg /= 4.0;
  }

  const float
    constinx = ( sh(1) + rheight*sina - rwidth*cosa ) / 2.0,
    constiny = ( sh(0) - rwidth*sina - rheight*cosa ) / 2.0;

  for ( ArrIndex x=0 ; x < shf(1) ; x++) {
    for ( ArrIndex y=0 ; y < shf(0) ; y++) {

      const float xf = x*cosa - y*sina + constinx;
      const float yf = x*sina + y*cosa + constiny;
      const ArrIndex flx = floor(xf), fly = floor(yf);

      if ( flx < 1 || flx >= sh(1)-1 || fly < 1  || fly >= sh(0)-1 ) {
        outarr(y, x)=bg;
      } else {
        float v0 = inarr(fly,flx) + ( inarr(fly,flx+1) - inarr(fly,flx) ) * (xf-flx);
        float v1 = inarr(fly+1,flx) + ( inarr(fly+1,flx+1) - inarr(fly+1,flx) ) * (yf-fly);
        outarr(y, x) = v0 + (v1-v0) * (yf-fly);
      }

    }
  }

}

void
rotate(Map & io_arr, float angle, float bg) {
  Map outarr;
  rotate(io_arr, outarr, angle, bg);
  if( io_arr.data() == outarr.data() )
    return;
  io_arr.resize(outarr.shape());
  io_arr=outarr.copy();
}




const string CropOptionDesc =
  "top:left:bottom:right. Cropping from the edges of image.";



const string Crop3OptionDesc =
  "top:left:bottom:right:face:back. 3D Cropping from the edges of volume.";


const string CenterOptionDesc=
  "Deviation of the rotation axis from the center of the sinogram.\n"
  "Allows you to erase some minor artifacts in the reconstructed image"
  " which come from the inaccurate rotation position. In most real"
  " CT or TS experiments the rotation center of the sample"
  " is not exactly in the center of the recoded projection. Moreover,"
  " the rotation axis may be inclined from the vertical orientation."
  " If the inclination of the rotation axis is noticeable for your desired"
  " spatial resolution then you should rotate the images before the actual"
  " reconstruction. You can do it in the batch using \"mogrify\" command"
  " from the ImageMagick package with the -rotate option.";

const std::string IntOptionDesc =
  "If this option is not set, the output format defaults to the"
  " 32-bit float-point TIFF (regardless of the extension)."
  " If it is set, the image format is derived from the output"
  " file extension (TIFF if the extension does not correspond"
  " to any format).";

const std::string NeedForQuant =
  "The value is required only to produce the physically correct output."
  " If not set the results are qualitative rather than quantitative.";

const std::string ResolutionOptionDesc =
  "Size of the image pixel in microns. If not present, the program attempts to"
  " to calculate it from the image dpi." + NeedForQuant;

/// Lists all manual pages in the package. This string to be added to
/// every manual page.
const std::string SeeAlsoList =
  "ctas(1), ctas-bg(1), ctas-ct(1), ctas-ct-abs(1), ctas-ct-dei(1),"
  " ctas-ct-edei(1), ctas-ct-ipc(1),"
  " ctas-dei(1), ctas-edei(1), ctas-ipc(1), ctas-f2i(1), ctas-ff(1),"
  " ctas-sino(1), ctas-sino-abs(1), ctas-sino-dei(1), ctas-sino-ipc(1),"
  " ctas-ts(1), ctas-ct-line(1)";







/// \cond
struct ToLower{ char operator() (char c) const { return tolower(c); } };
struct ToUpper{ char operator() (char c) const { return toupper(c); } };
/// \endcond

string
upper(string str){
  transform (str.begin(), str.end(), str.begin(), ToUpper());
  return str;
}

string
lower(string str){
  transform (str.begin(), str.end(), str.begin(), ToLower());
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









/// \brief Initializing constructor.
///
/// @param _showme Tells if the progress bar should be shown.
/// @param _message The description of the progress.
/// @param _steps Number of steps in the progress. If 0 then unknown.
///
ProgressBar::ProgressBar(bool _showme, const string & _message, int _steps)
  : showme(_showme)
  , message(_message)
  , steps(_steps)
  , proglock(PTHREAD_MUTEX_INITIALIZER)
{
  if ( ! showme ) return;
  step = 0;
  waswidth = 0;
  reservedChs = 0;
}

/// \brief Updates the progress bar.
///
/// @param curstep Current step. Advances +1 if zero.
///
void
ProgressBar::_update(int curstep){

  if ( !showme ) return; // Uninitialized progress bar.

  if ( !reservedChs ) {

    cout << "Starting process";
    if (steps) cout << " (" + toString(steps) + " steps)";
    cout << ": " << message << "." << endl;
    fflush(stdout);

    int nums = toString(steps).length();
    reservedChs = 14 + 2*nums;
    fmt = steps ?
          "%" + toString(nums) + "i of " + toString(steps) + " [%s] %4s" :
          string( "progress: %i" );

  }

  int progln = getwidth() - reservedChs;
  if ( progln <= 3 )  return; // if we have a very narrow terminal
  step = curstep ? curstep+1 : step + 1;

  if ( steps && step >= steps ) {
        done();
        return;
  }

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

}

void
ProgressBar::update(int curstep){
  pthread_mutex_lock(&proglock);
  try { _update(curstep); }  catch (...) {}
  pthread_mutex_unlock(&proglock);
}




void
ProgressBar::done(){

  if ( !showme || ! reservedChs ) return;

  int progln = getwidth() - reservedChs;
  if ( progln < 0 )  progln = 0; // if we have a very narrow terminal
  string eqs(progln, '=');

  cout << string(waswidth+1, '\r')
       << ( steps ?
            toString(fmt, steps, eqs.c_str(), "DONE. ") :
            toString(fmt, step) + " steps. DONE." )
       << endl
       << "Successfully finished " << message << "." << endl;
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
  return ( ioctl (STDOUT_FILENO, TIOCGWINSZ, &size ) < 0 ) ?  0 : size.ws_col - 1;
#endif
}















const string SliceOptionDesc=
  "The string describing the slices in the image stack which are to be"
  " processed. Must be a string consisting only of numbers, commas ','"
  " minus signs '-' and the character 'n'. First the string is divided"
  " into the substrings separated by comas and each substring is"
  " processed on it's own:\n"
  "    The string consisting only of numbers is read as the integer and"
  " added to the list of the reconstructed slices.\n"
  "    The string with the minus sign surrounded by the numbers adds all"
  " slices in the range into the list.\n"
  "    If the string starts with the minus then the range start is"
  " assumed to be 0.\n"
  "    If minus is the last character in the string then the range"
  " finishes at the maximum slice number - 1.\n"
  "    If the string has negation prefix 'n' then the slice(s) are"
  " excluded from the previously formed list.\n"
  "    If all substrings have 'n' prefix or the first substring contains"
  " only it, then the meaning of the whole string is \"all except ...\".\n"
  "    Two and more negations are interpreted as a single one.\n"
  "    If no slice string is given then all slices are reconstructed.\n"
  "For example the following string:\n"
  "    9,-4,6,20-400,n3,500-440,n450-470,800-,n920-910,915\n"
  "requests processing of the slices with numbers 0, 1, 2, 4, 6, 9, 20 to 400,"
  " 440 to 449, 471 to 500, 800 to 909, 915 and 921 to the end.";


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
static inline vector<int> &
rmadd(vector<int> & arr, int numb, bool negate){
  if (negate)
        arr.erase( remove( arr.begin(), arr.end(), numb ), arr.end() );
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




vector<int>
slice_str2vec(const string & sliceS, int hight){

  const char negatec = 'n';
  const string permitted_chars = string("0123456789,-") + negatec ;

  vector<int> sliceV; // the array to be returned as the result.

  // empty string
  if ( sliceS.empty() ) {
        for (int slice = 0 ; slice < hight ; slice++)
          sliceV.push_back(slice);
        return sliceV;
  }


  // construct the vector of substrings and checks for the global negation.
  vector<string> subSV;
  bool negateall = true; // turns to false if an unnegated substring found.
  string::size_type startidx=0, endidx;
  do {

    // extract substring
    endidx = sliceS.find(',', startidx);
    string subS = sliceS.substr(startidx, ( endidx == string::npos ?
                        sliceS.length() :
                        endidx-startidx) );
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
      if ( lastneg != string::npos ) {
        if ( lastneg != 0 ) {
          warn("slice string", "Suspicious substring \""+ initS +"\" in the string describing set of slices:"
                               " it has '" + negatec + "' character not in the first position."
                               " Moving it to the begining. Is it what you meant?");
          // moves all negatec's to the beginning and erases it's duplicates.
          subS.erase
          ( subS.begin(),
            stable_partition( subS.begin(), subS.end(),
                    bind2nd(equal_to<char>(),negatec) ) - 1 );
        }
      } else {
        negateall = false;
      }

      // modifies in regards to '-'
      if ( count(subS.begin(), subS.end(), '-') > 1 ) {
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
        subS = subS + toString(hight-1);

      subSV.push_back(subS);

    }

  } while ( string::npos != endidx );


  // Check for global negation.
  if ( ( negateall |= ( subSV[0] == string(1,negatec) ) ) )
        for ( int icur = 0 ; icur < hight ; icur++)
          sliceV.push_back(icur);
  subSV.erase( remove( subSV.begin(), subSV.end(), string(1,negatec) ),
                           subSV.end() ); // no "negate" strings

  // adds/removes substrings into the array of slices
  vector<string>::iterator subSVi = subSV.begin();
  const vector<string>::iterator subSVe=subSV.end();

  while ( subSVi != subSVe ) {

    bool negatethis = (*subSVi)[0]==negatec;
    if (negatethis) (*subSVi).erase(0,1);
      negatethis |= negateall;

    string::size_type minuspos = (*subSVi).find('-');
    if ( minuspos != string::npos ) {
      int
        rangeB = str2n( (*subSVi).substr(0,minuspos) ),
        rangeE = str2n( (*subSVi).substr(minuspos+1) );
      if ( rangeB > rangeE ) swap(rangeB,rangeE);
      if ( rangeB == rangeE )
        warn("slice string", "One of the substrings with ranges in the string describing set of slices"
                             " has equal ends of the ranges. Is it what you meant?");
      for (int curS = rangeB ; curS <= rangeE ; curS++ )
        rmadd(sliceV, curS, negatethis);
    } else {
      rmadd(sliceV, str2n( *subSVi ), negatethis );
    }

    subSVi++;

  }

  // sort and remove duplicates, too large numbers
  sort(sliceV.begin(), sliceV.end());
  sliceV.erase( unique( sliceV.begin(), sliceV.end() ), sliceV.end() );
  if ( sliceV.back() >= hight )
        warn("slice string", "The string describing set of slices includes slices beyond the size"
                             " of the input image (" + toString(hight) + "). These slices are ignored." );
  sliceV.erase( find_if( sliceV.begin(), sliceV.end(),
                         bind2nd( greater<int>(), hight-1 ) ),
                         sliceV.end() );

  // last check
  if ( sliceV.empty() )
        warn("slice string", "The string describing set of slices leads to the empty range of slices." );

  return sliceV;

}







