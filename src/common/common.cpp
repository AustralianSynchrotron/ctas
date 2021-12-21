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
#include <tiffio.h>
#include <H5Cpp.h>
#include <fcntl.h> // for the libc "open" function see bug description in the SaveImageFP function.
#include <climits>
#include <ctime>
#include <cmath>
#include <ctype.h>
#include <unordered_map>

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
  if (  crp.left + crp.right >= inarr.shape()(2)  ||
        crp.top + crp.bottom >= inarr.shape()(1)  ||
        crp.face + crp.back >= inarr.shape()(0) ) {
    warn("Crop3 array", "Cropping (" + toString(crp) + ")"
         " is larger than array size.");
    return;
  }

  outarr.resize( inarr.shape()(0) - crp.face  - crp.back,
                 inarr.shape()(1) - crp.top  - crp.bottom,
                 inarr.shape()(2) - crp.left - crp.right);
  outarr = inarr( blitz::Range(crp.face, inarr.shape()(0)-1-crp.back),
                  blitz::Range(crp.top, inarr.shape()(0)-1-crp.bottom ),
                  blitz::Range(crp.left, inarr.shape()(1)-1-crp.right ) );

}

void
crop(Volume & io_arr, const Crop3 & crp) {
  Volume outarr;
  crop(io_arr, outarr, crp);
  if( io_arr.data() == outarr.data() )
    return;
  io_arr.resize(outarr.shape());
  io_arr=outarr.copy();
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
    return;
  }

  outarr.resize( inarr.shape()(0) - crp.top  - crp.bottom,
                 inarr.shape()(1) - crp.left - crp.right);
  outarr = inarr( blitz::Range(crp.top, inarr.shape()(0)-1-crp.bottom ),
                  blitz::Range(crp.left, inarr.shape()(1)-1-crp.right ) );

}

void
crop(Map & io_arr, const Crop & crp) {
  Map outarr;
  crop(io_arr, outarr, crp);
  if( io_arr.data() == outarr.data() )
    return;
  io_arr.resize(outarr.shape());
  io_arr=outarr.copy();
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















long nof_threads(long _threads) {
  if (_threads)
    return _threads;

#ifdef _WIN32
#ifndef _SC_NPROCESSORS_ONLN
  SYSTEM_INFO info;
  GetSystemInfo(&info);
#define sysconf(a) info.dwNumberOfProcessors
#define _SC_NPROCESSORS_ONLN
#endif
#endif

  long nProcessorsOnline = sysconf(_SC_NPROCESSORS_ONLN);
  if (nProcessorsOnline == -1) {
    warn ("thread number",
          "Unable to read online processor count.");
    return 1;
  } else {
    return nProcessorsOnline;
  }

}

static const int run_threads = nof_threads();


class ThreadDistributor {

private :

  pthread_mutex_t idxLock; // to protect index incriment
  pthread_mutex_t startLock; // to protect start signal.
  pthread_cond_t startCond; // to be signalled after all threads has started

  long int currentidx;
  std::vector<pthread_t> threads;

  void * arg;
  bool (*sub_routine0) ();
  bool (*sub_routine1) (long int);
  bool (*sub_routine2) (void *, long int);
  bool sub_routine (long int idx) {
    if (sub_routine0) return sub_routine0();
    if (sub_routine1) return sub_routine1(idx);
    if (sub_routine2) return sub_routine2(arg, idx);
    return false;
  }

  inline ThreadDistributor( bool (*_sub_routine0)(),
                     bool (*_sub_routine1)(long int),
                     bool (*_sub_routine2) (void *, long int),
                     void * _arg)
    : idxLock(PTHREAD_MUTEX_INITIALIZER)
    , startLock(PTHREAD_MUTEX_INITIALIZER)
    , startCond(PTHREAD_COND_INITIALIZER)
    , currentidx(0)
    , arg(_arg)
    , sub_routine0(_sub_routine0)
    , sub_routine1(_sub_routine1)
    , sub_routine2(_sub_routine2)
  {}

  ThreadDistributor( bool (*_sub_routine)() )
    : ThreadDistributor(_sub_routine, 0, 0, 0)
  {}

  ThreadDistributor( bool (*_sub_routine)(long int) )
    : ThreadDistributor(0, _sub_routine, 0, 0)
  {}

  ThreadDistributor( bool (*_sub_routine) (void *, long int), void * _arg )
    : ThreadDistributor(0, 0, _sub_routine, _arg)
  {}

  long int distribute() {
    long int idx;
    pthread_mutex_lock(&idxLock);
    idx = currentidx++;
    pthread_mutex_unlock(&idxLock);
    return idx;
  }


  static void * in_thread (void * vdist) {
    ThreadDistributor * dist = (ThreadDistributor*) vdist;
    while ( dist->sub_routine(dist->distribute()) ) {}
    return 0;
  }


  void start() {
    pthread_t thread;
    for (int ith = 0 ; ith < run_threads ; ith++)
      if ( pthread_create( & thread, NULL, in_thread, this ) )
        warn("Thread operation", "Can't create thread.");
      else
        threads.push_back(thread);
    pthread_cond_signal(&startCond);
  }

  void finish() {
    for (int ith = 0 ; ith < threads.size() ; ith++)
      pthread_join( threads[ith], 0);
  }


public:

  static void execute( bool (*_thread_routine) (void *, long int), void * _arg ) {
    ThreadDistributor dist(_thread_routine, _arg);
    dist.start();
    dist.finish();
  }

  static void execute( bool (*_thread_routine) (long int)) {
    ThreadDistributor dist(_thread_routine);
    dist.start();
    dist.finish();
  }

  static void execute( bool (*_thread_routine)()) {
    ThreadDistributor dist(_thread_routine);
    dist.start();
    dist.finish();
  }

};






void InThread::execute() {
  ThreadDistributor::execute(inThread, this);
}

void InThread::execute( bool (*_thread_routine) (long int)) {
  ThreadDistributor::execute(_thread_routine);
}

void InThread::execute( bool (*_thread_routine)()) {
  ThreadDistributor::execute(_thread_routine);
}



#ifdef OPENCL_FOUND

//#include<libconfig.h++>
//using namespace libconfig;

static bool clInited = false;

bool clIsInited() {

  if (clInited)
    return true;

  cl_int err;

  cl_uint nof_platforms;
  err = ::clGetPlatformIDs(0, 0, &nof_platforms);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not get number of OpenCL platforms: " + toString(err) );
    return false;
  }

  vector<cl_platform_id> platforms(nof_platforms);
  vector<cl_device_id> devices;

  err = clGetPlatformIDs(nof_platforms, platforms.data(), 0);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not get OpenCL platforms: " + toString(err) );
    return false;
  }

  for (int plidx=0; plidx < platforms.size() ; plidx++ ) {

    cl_uint nof_devices = 0;

    err = clGetDeviceIDs( platforms[plidx], CL_DEVICE_TYPE_GPU, 0, 0, &nof_devices);
    if (err != CL_SUCCESS) {
      warn("OpenCLinit", "Could not get OpenCL number of GPU devices of a platform: "
           + toString(err) );
    } else {

      vector<cl_device_id> platform_devices(nof_devices);
      err = clGetDeviceIDs( platforms[plidx], CL_DEVICE_TYPE_GPU,
                            nof_devices, platform_devices.data(), 0);
      if (err != CL_SUCCESS) {
        warn("OpenCLinit", "Could not get OpenCL GPU devices of a platform: "
             + toString(err) );
      } else {

        for (int devidx=0; devidx<platform_devices.size(); devidx++) {

          cl_device_id dev = platform_devices[devidx];
          bool errHappened=false;

          cl_bool devIsAvailable;
          err = clGetDeviceInfo(dev, CL_DEVICE_AVAILABLE,
                                sizeof(cl_bool), &devIsAvailable, 0);
          if (err != CL_SUCCESS) {
            warn("OpenCLinit", "Could not get OpenCL device info \"CL_DEVICE_AVAILABLE\": "
                 + toString(err) );
            errHappened=true;
          }

          cl_bool devCompilerIsAvailable;
          err = clGetDeviceInfo(dev, CL_DEVICE_COMPILER_AVAILABLE, sizeof(cl_bool),
                                &devCompilerIsAvailable, 0);
          if (err != CL_SUCCESS) {
            warn("OpenCLinit", "Could not get OpenCL device info \"CL_DEVICE_COMPILER_AVAILABLE\": "
                 + toString(err) );
            errHappened=true;
          }

          cl_device_exec_capabilities devExecCapabilities;
          err = clGetDeviceInfo(dev, CL_DEVICE_EXECUTION_CAPABILITIES, sizeof(cl_device_exec_capabilities),
                                &devExecCapabilities, 0);
          if (err != CL_SUCCESS) {
            warn("OpenCLinit", "Could not get OpenCL device info \"CL_DEVICE_EXECUTION_CAPABILITIES\": "
                 + toString(err) );
            errHappened=true;
          }

          if ( ! errHappened &&
               devIsAvailable &&
               devCompilerIsAvailable &&
               ( devExecCapabilities & CL_EXEC_KERNEL ) )
            devices.push_back(dev);

        }

      }

    }

  }


  if (devices.empty()) {

    warn("OpenCLinit", "No OpenCL devices found.");
    return false;

  } else { // more than one device found

    int idx=-1;
    cl_ulong devmem, devmaxmem=0;
    for (int devidx=0; devidx < devices.size(); devidx++) {
      err = clGetDeviceInfo(devices[devidx], CL_DEVICE_GLOBAL_MEM_SIZE,
                            sizeof(cl_ulong),  &devmem, 0);
      if (err == CL_SUCCESS  &&  devmem > devmaxmem) {
        devmaxmem = devmem;
        idx=devidx;
      }
    }

    if (idx >= 0)
      CL_device = devices[idx];

    /* TODO:
     * complete this part to read device name from the config
     * file instead of choosing the device with maximum memory
     * above.
     */

  }

  cl_platform_id platform;
  err = clGetDeviceInfo(CL_device, CL_DEVICE_PLATFORM, sizeof(cl_platform_id),  &platform, 0);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not get OpenCL device info \"CL_DEVICE_PLATFORM\": "
         + toString(err) );
    return false;
  }

  CL_context = clCreateContext(0, 1, &CL_device, 0, 0, &err);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not create OpenCL context: " + toString(err) );
    return false;
  }

  CL_queue = clCreateCommandQueueWithProperties(CL_context, CL_device, NULL, &err);
  //CL_queue = clCreateCommandQueue(CL_context, CL_device, 0, &err);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not create OpenCL queue: " + toString(err) );
    return false;
  }

  clInited = true;
  return clInited;

}




cl_program initProgram(const char csrc[], size_t length, const string & modname) {

  if ( ! clIsInited() )
    return 0;

  const char * src = csrc;

  cl_int err;

  cl_program program =
    clCreateProgramWithSource( CL_context, 1, &src, &length, &err);
  if (err != CL_SUCCESS) {
    warn(modname, "Could not load OpenCL program: " + toString(err) );
    return 0;
  }

  err = clBuildProgram( program, 0, 0, "", 0, 0);
  if (err != CL_SUCCESS) {

    warn(modname, (string) "Could not build OpenCL program: " + toString(err) +
    ". More detailsd below:" );

    cl_build_status stat;
    err = clGetProgramBuildInfo(program, CL_device, CL_PROGRAM_BUILD_STATUS,
                                sizeof(cl_build_status), &stat, 0);
    if (err != CL_SUCCESS)
      warn(modname, "Could not get OpenCL program build status: " + toString(err) );
    else
      warn(modname, "   Build status: " + toString(stat));

    size_t len=0;
    err=clGetProgramBuildInfo(program, CL_device, CL_PROGRAM_BUILD_OPTIONS,
                              0, 0, &len);
    char * buildOptions = (char*) calloc(len, sizeof(char));
    if (buildOptions)
      err=clGetProgramBuildInfo(program, CL_device, CL_PROGRAM_BUILD_OPTIONS,
                                len, buildOptions, 0);
    if (err != CL_SUCCESS)
      warn(modname, "Could not get OpenCL program build options: " + toString(err) );
    else
      warn(modname, "   Build options: \"" + string(buildOptions, len) + "\"");
    if (buildOptions)
      free(buildOptions);

    err = clGetProgramBuildInfo(program, CL_device, CL_PROGRAM_BUILD_LOG,
                                0, 0, &len);
    char * buildLog = (char*) calloc(len, sizeof(char));
    if (buildLog)
      err = clGetProgramBuildInfo(program, CL_device, CL_PROGRAM_BUILD_LOG,
                                  len, buildLog, 0);
    if (err != CL_SUCCESS)
      warn(modname, "Could not get OpenCL program build log: " + toString(err) );
    else
      warn(modname, "   Build log:\n" + string(buildLog, len));
    if (buildLog)
      free(buildLog);

    warn(modname, "\n");

    return 0;

  }

  return program;

}


cl_kernel createKernel(cl_program program, const std::string & name) {
  cl_int clerr;
  cl_kernel kern = clCreateKernel ( program, name.c_str(), &clerr);
  if (clerr != CL_SUCCESS)
    throw_error("createKernel",
                "Could not create OpenCL kernel \"" + name + "\": " + toString(clerr));
  return kern;
}


std::string kernelName(cl_kernel kern) {
  if (!kern)
    throw_error("kernelName", "Invalid OpenCL kernel.");
  size_t len=0;
  cl_int clerr = clGetKernelInfo ( kern, CL_KERNEL_FUNCTION_NAME, 0, 0, &len);
  if ( clerr != CL_SUCCESS )
    throw_error("kernelName", "Could not get OpenCL kernel name size: " + toString(clerr));
  char *kernel_function = (char *) calloc(len, sizeof(char));
  std::string name;
  if (kernel_function) {
    clerr = clGetKernelInfo ( kern, CL_KERNEL_FUNCTION_NAME, len, kernel_function, 0);
    if (clerr == CL_SUCCESS)
      name = std::string(kernel_function, len);
    free(kernel_function);
    if (clerr != CL_SUCCESS)
      throw_error("kernelName", "Could not get OpenCL kernel name: " + toString(clerr));
  }
  return name;
}


cl_int execKernel(cl_kernel kern, size_t size) {
  cl_int clerr = clEnqueueNDRangeKernel( CL_queue, kern, 1, 0,  & size, 0, 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to execute OpenCL kernel \"" + kernelName(kern) + "\": " + toString(clerr));
  clerr = clFinish(CL_queue);
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to finish OpenCL kernel \"" + kernelName(kern) + "\": " + toString(clerr));
  return clerr;
}

cl_int execKernel(cl_kernel kern, const Shape & sh) {
  size_t sizes[2] = {size_t(sh(1)), size_t(sh(0))};
  cl_int clerr = clEnqueueNDRangeKernel( CL_queue, kern, 2, 0, sizes, 0, 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to execute OpenCL kernel \"" + kernelName(kern) + "\": " + toString(clerr));
  clerr = clFinish(CL_queue);
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to finish OpenCL kernel \"" + kernelName(kern) + "\": " + toString(clerr));
  return clerr;
}

cl_int execKernel(cl_kernel kern, const Shape3 & sh) {
  size_t sizes[3] = {size_t(sh(2)), size_t(sh(1)), size_t(sh(0))};
  cl_int clerr = clEnqueueNDRangeKernel( CL_queue, kern, 3, 0, sizes, 0, 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to execute OpenCL kernel \"" + kernelName(kern) + "\": " + toString(clerr));
  clerr = clFinish(CL_queue);
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to finish OpenCL kernel \"" + kernelName(kern) + "\": " + toString(clerr));
  return clerr;
}


/*

cl_mem clAllocFImage(const Shape sh, cl_mem_flags flag) {
  cl_int err;
  const cl_image_desc ifdesc(
    { CL_MEM_OBJECT_IMAGE2D, size_t(sh(1)), size_t(sh(0)), 1, 1, 0, 0, 0, 0, {0} } ) ;
  cl_mem clImage = clCreateImage( CL_context, flag, &clfimage_format, &ifdesc, 0, &err);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not create OpenCL image: " + toString(err) );
  return clImage;
}

cl_mem blitz2clfi(const Map & storage, cl_mem clStorage, cl_mem_flags flag) {
  const size_t origin[3] = {0,0,0};
  const size_t region[3] = {size_t(storage.shape()(1)), size_t(storage.shape()(0)),1};
  cl_int err = clEnqueueWriteImage( CL_queue, clStorage, CL_TRUE, origin, region
                                  , 0, 0, storage.data(), 0, 0, 0);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not write OpenCL image: " + toString(err) );
  return clStorage;
}

cl_mem blitz2clfi(const Map & storage, cl_mem_flags flag) {
  cl_mem clStorage = clAllocFImage(storage.shape(), flag);
  return blitz2clfi(storage, clStorage, flag);
}


cl_mem clAllocFImage(const Shape3 sh, cl_mem_flags flag) {
  cl_int err;
  const cl_image_desc ifdesc(
    { CL_MEM_OBJECT_IMAGE3D, size_t(sh(2)), size_t(sh(1)), size_t(sh(0)), 1, 0, 0, 0, 0, {0} } ) ;
  cl_mem clImage = clCreateImage( CL_context, flag, &clfimage_format, &ifdesc, 0, &err);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not create OpenCL image: " + toString(err) );
  return clImage;
}

cl_mem blitz2clfi(const Volume & storage, cl_mem clStorage, cl_mem_flags flag) {
  const size_t origin[3] = {0,0,0};
  const size_t region[3] = {size_t(storage.shape()(2)),
                            size_t(storage.shape()(1)),
                            size_t(storage.shape()(0))};
  cl_int err = clEnqueueWriteImage( CL_queue, clStorage, CL_TRUE, origin, region
                                  , 0, 0, storage.data(), 0, 0, 0);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not write OpenCL image: " + toString(err) );
  return clStorage;
}

cl_mem blitz2clfi(const Volume & storage, cl_mem_flags flag) {
  cl_mem clStorage = clAllocFImage(storage.shape(), flag);
  return blitz2clfi(storage, clStorage, flag);
}


static void clImageInfo(cl_mem image, size_t region[3]) {
  cl_int clerr;
  if (  CL_SUCCESS != (clerr = clGetImageInfo(image, CL_IMAGE_WIDTH,   sizeof(size_t), region,   0) )
     || CL_SUCCESS != (clerr = clGetImageInfo(image, CL_IMAGE_HEIGHT,  sizeof(size_t), region+1, 0) )
     || CL_SUCCESS != (clerr = clGetImageInfo(image, CL_IMAGE_DEPTH,   sizeof(size_t), region+2, 0) )  )
    throw_error("execFill", "Failed to get image parameter: " + toString(clerr));
}


cl_int fillClImage(cl_mem image, float val) {
  const size_t origin[3] = {0,0,0};
  size_t region[3];
  const cl_float4 valc = {{val,0,0,0}};
  clImageInfo(image, region);
  int clerr = clEnqueueFillImage(CL_queue, image, &valc, origin, region, 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("execFill", "Failed to execute OpenCL image fill: " + toString(clerr));
  clerr = clFinish(CL_queue);
  if (clerr != CL_SUCCESS)
    throw_error("execFill", "Failed to finish OpenCL image fill: " + toString(clerr));
  return clerr;
}


void clfi2blitz(cl_mem image, Map & storage) {
  const size_t origin[3] = {0,0,0};
  size_t region[3];
  clImageInfo(image, region);
  if ( region[2] > 1 )
    throw_error("Read CLimage", "Can't read 3D image into Map array.");
  storage.resize(region[1], region[0]);
  if ( ! storage.size() )
    throw_error("Read CLimage", "Empty image.");
  cl_int clerr = clEnqueueReadImage(CL_queue, image, CL_TRUE, origin, region
                                    , 0, 0, storage.data(), 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("Read CLimage", "Failed to read image.");
}

void clfi2blitz(cl_mem image, Volume & storage) {
  const size_t origin[3] = {0,0,0};
  size_t region[3];
  clImageInfo(image, region);
  storage.resize(region[2], region[1], region[0]);
  if ( ! storage.size() )
    throw_error("Read CLimage", "Empty image.");
  cl_int clerr = clEnqueueReadImage(CL_queue, image, CL_TRUE, origin, region
                                    , 0, 0, storage.data(), 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("Read CLimage", "Failed to read image.");
}


*/

#endif // OPENCL_FOUND




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












#ifdef _WIN32
#  define STATIC_MAGICK
#  define MAGICK_STATIC_LINK
#endif
#define MAGICKCORE_QUANTUM_DEPTH 32
#define MAGICKCORE_HDRI_ENABLE 1
#include<Magick++.h>



/// \brief initializes image IO libraries
///
/// ImageMagick: allow reading up to 10k x 10k into memory, not HDD.
/// libTIFF: suppress warnings.
static bool
initImageIO(){

#ifdef MAGICKLIB_NAMESPACE
  using namespace MagickLib;
#else
  using namespace MagickCore;
#endif

  //MagickSizeType Msz = (numeric_limits<MagickSizeType>::max)();
  //SetMagickResourceLimit ( AreaResource , 10000 * 10000 * 4);
  //SetMagickResourceLimit ( FileResource , 1024 * 1024);
  //SetMagickResourceLimit ( DiskResource , Msz);
  //SetMagickResourceLimit ( MapResource , Msz);
  //SetMagickResourceLimit ( MemoryResource , Msz);

  // BUG in ImageMagick If I don'd do it here the TIFFSetWarningHandler
  // is called later in the code and causes
  // ../../magick/exception.c:1036: ThrowMagickExceptionList: Assertion `exception->signature == MagickSignature' failed.
  // whenever TIFFOpen is called
  try { Magick::Image imag; imag.ping("a.tif"); } catch (...) {}
  TIFFSetWarningHandler(0);

  return true;

}

static const bool imageIOinited = initImageIO();



Path imageFile(const std::string & filedesc) {
  return filedesc.substr(0 , filedesc.find(":"));
}






vector<string> split (string s, string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  string token;
  vector<string> res;
  while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
    token = s.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }
  res.push_back (s.substr (pos_start));
  return res;
}


struct HDFdesc {

  Path name;
  string data;
  int sliceDim;
  string slices;

  HDFdesc(const string & filedesc)
    : sliceDim(0)
  {

    const string modmname = "HDF desc";
    vector<string> hdfRd = split(filedesc, ":");
    if ( hdfRd.size() < 2  ||  hdfRd.size() > 3 ) // not HDF5
      return;
    name=hdfRd[0];
    data=hdfRd[1];
    slices  =  hdfRd.size() < 3  ||  hdfRd[2].empty()   ?   "Z"  :  hdfRd[2] ;
    switch ( slices.at(0) ) {
      case 'x':
      case 'X':
        sliceDim=2;  slices.erase(0,1); break;
      case 'y':
      case 'Y':
        sliceDim=1;  slices.erase(0,1); break;
      case 'z':
      case 'Z':
        sliceDim=0;  slices.erase(0,1); break;
      default:
        sliceDim=0;
    }

  }

  string id() const {
    return name + ":" + data + ":" + (sliceDim == 0 ? "z" : (sliceDim == 1 ? "y" : "x")) ;
  }

  bool isValid() const { return ! name.empty() ; }


};




struct HDFread : public HDFdesc {

public :

  mutable Shape shape;
  mutable vector<int> indices;

private :

  H5::H5File hdfFile;
  H5::DataSet dataset;
  H5::DataSpace dataspace;
  int rank;
  blitz::Array<hsize_t, 1> cnts;
  H5::DataSpace memspace;

public :

  HDFread(const string & filedesc)
    : HDFdesc(filedesc)
  {

    const string modmname = "HDF read";
    if ( ! isValid() )
      throw CtasErr(CtasErr::WARN, modmname, "Not an hdf file \"" + filedesc + "\".");

#ifdef H5F_ACC_SWMR_READ
    hdfFile = H5::H5File(name, H5F_ACC_RDONLY | H5F_ACC_SWMR_READ);
#else
    hdfFile = H5::H5File(name, H5F_ACC_RDONLY);
#endif
    dataset = H5::DataSet(hdfFile.openDataSet(data));
    dataspace = H5::DataSpace(dataset.getSpace());
    rank = dataspace.getSimpleExtentNdims();
    cnts.resize(rank);
    memspace = H5::DataSpace();

    try {

      dataspace.getSimpleExtentDims( cnts.data(), NULL);
      if ( rank == 2 ) {
        indices.push_back(0);
        shape = Shape(cnts(0), cnts(1));
      } else if ( rank == 3 ) {
        int idx=0, odx=0;
        while (idx<rank) {
          if (idx != sliceDim)
            shape(odx++) = cnts(idx);
          idx++;
        }
        indices = slice_str2vec(slices, cnts(sliceDim));
      } else {
        throw CtasErr(CtasErr::ERR, modmname, "Dataset is not 2D or 3D in " + filedesc);
      }

      hsize_t mcnts[2] = { hsize_t(shape(0)), hsize_t(shape(1))}; ///
      hsize_t moffs[2] = {0, 0};
      memspace.setExtentSimple( 2, mcnts );
      memspace.selectHyperslab( H5S_SELECT_SET, mcnts, moffs);
      if (sliceDim==2) // need to transpose what I read in YZ plane
        shape = Shape(shape(1),shape(0));

    } catch( ... ) {
      exit_on_error(modmname, "Error getting info from " + filedesc);
    }

  }

  ~HDFread() {
    complete();
  }

  void complete() {
    if ( ! indices.size() )
      return;
    indices.clear();
    dataset.close();
    hdfFile.close();
  }


  void read(int idx, Map & storage) {

    if ( ! indices.size() )
      throw_error("HDF read", "File " + name + " was previously closed.");
    if ( idx >= indices.size() )
      throw_error("HDF read", "Index is beyond slices to read from " + name + ".");

    blitz::Array<hsize_t, 1> offs(rank), cnth(rank);
    offs=0;
    cnth=cnts;
    if ( rank == 3 ) {
      offs(sliceDim) = indices.at(idx);
      cnth(sliceDim) = 1;
    }

    storage.resize(shape);
    Map rd( sliceDim==2 ? Shape(shape(1),shape(0)) : shape ); // to be transposed

    H5::DataSpace localDataspace(dataspace);
    localDataspace.selectHyperslab( H5S_SELECT_SET, cnth.data(), offs.data() );
    dataset.read( rd.data(), H5::PredType::NATIVE_FLOAT, memspace, localDataspace );
    storage  =  (sliceDim==2)  ?  rd.transpose(blitz::secondDim, blitz::firstDim)  :  rd;

  }

};





struct HDFwrite : public HDFdesc {

public :

  const Shape shape;
  size_t zsize;

private :

  H5::H5File hdfFile;
  H5::DataSet dataset;
  H5::DataSpace dataspace;
  blitz::Array<hsize_t, 1> cnts;
  H5::DataSpace memspace;
  pthread_mutex_t proglock; // HDF does not like multithread writing

public :

  HDFwrite(const HDFdesc & desc, Shape _sh, size_t _zsize)
    : HDFdesc(desc)
    , shape(_sh)
    , zsize(_zsize)
    , cnts(3)
    , proglock(PTHREAD_MUTEX_INITIALIZER)
  {

    const string modmname = "HDF write";
    if ( ! zsize * area(shape) )
      throw CtasErr(CtasErr::WARN, modmname, "Zerro size to write.");
    if ( ! isValid() )
      throw CtasErr(CtasErr::WARN, modmname, "Invalid hdf file \"" + id() + "\".");

    int idx=0, odx=0;
    while (idx<3) {
      if (idx != sliceDim)
        cnts(idx) = shape(odx++);
      idx++;
    }
    cnts(sliceDim) = zsize; // first will be used once as the 3D dimensions

    try {

     hdfFile = H5::H5File(name, H5F_ACC_TRUNC);
     float fillvalue = 0.0;   /* Fill value for the dataset */
     H5::DSetCreatPropList plist;
     plist.setFillValue(H5::PredType::NATIVE_FLOAT, &fillvalue);
     dataspace = H5::DataSpace(3, cnts.data());
     dataset = H5::DataSet(hdfFile.createDataSet(data, H5::PredType::NATIVE_FLOAT, dataspace, plist) );

     hsize_t mcnts[2] = { hsize_t(shape(0)), hsize_t(shape(1))};
     hsize_t moffs[2] = {0, 0};
     memspace.setExtentSimple( 2, mcnts );
     memspace.selectHyperslab( H5S_SELECT_SET, mcnts, moffs);

    } catch( ... ) {
      throw_error(modmname, "Error creating HDF file for " + id());
    }

    cnts(sliceDim) = 1; // from now used only to write 2D slices

  }

  HDFwrite(const string & filedesc, Shape _sh, size_t _zsize)
    : HDFwrite(HDFdesc(filedesc), _sh, _zsize)
  {}

  ~HDFwrite() {
    complete();
  }

  void complete() {
    if ( ! zsize )
      return;
    zsize=0;
    dataset.close();
    hdfFile.close();
  }


  void write(int idx, const Map & storage) {

    if ( ! zsize ) {
      warn("HDF write", "File \"" + id() + "\" was previously closed and no more write possible.");
      return;
    }
    if ( idx >= zsize ) {
      warn("HDF write", "Index " + toString(idx) + " to write is beyond initially requested size"
                        + toString(zsize) + ". Write is ignored.");
      return;
    }
    if ( storage.shape() != shape ) {
      warn("HDF write", "Shape of the slice to write " + toString(storage.shape()) + " is different from"
                        " initially requested shape" + toString(shape) + ".");
      return;
    }

    Map _storage;
    if ( storage.isStorageContiguous()  &&  storage.stride() == Shape(shape(1),1) )
      _storage.reference(storage);
    else {
      _storage.resize(shape);
      _storage = storage;
    }

    blitz::Array<hsize_t, 1> offs(3), cnth(3);
    offs=0;
    cnth=cnts;
    offs(sliceDim) = idx;
    cnth(sliceDim) = 1;

    pthread_mutex_lock(&proglock);
    try {
      H5::DataSpace localDataspace(dataspace);
      localDataspace.selectHyperslab( H5S_SELECT_SET, cnth.data(), offs.data() );
      dataset.write( _storage.data(), H5::PredType::NATIVE_FLOAT, memspace, localDataspace );
    } catch (...) {}
    pthread_mutex_unlock(&proglock);

  }

};














float
PixelSize(const Path & filename) {
  static const float defaultSize = 1.0;
  Magick::Image imag;
  try { imag.ping(filename); }    catch ( Magick::WarningCoder err ) {}
  const Magick::Geometry dens = (Magick::Geometry) imag.density();
  float res = (float) dens.width();
  if ( ! dens.isValid() || ! res ) {
    warn("pixel size", "The resolution of the image \""+ filename+ "\"" "is invalid.");
    return defaultSize;
  }
  if ( res != dens.height() )
    warn("pixel size", "The resolutions of the image \""+ filename+ "\""
         "in vertical and horizontal directions differ.");

  switch ( imag.resolutionUnits() ) {
  case Magick::PixelsPerInchResolution :
    return 25400.0f / res;
  case Magick::PixelsPerCentimeterResolution :
    return 10000.0f / res;
  default:
    warn("pixel size", "Undefined resolution units of the image \""+ filename+ "\".");
    return defaultSize;
  }
}


Shape
ImageSizes(const Path & filename){

  try {

    return HDFread(filename).shape;

  } catch (CtasErr err) {

    Magick::Image imag;
    try {
      imag.ping(filename);
    }
    catch ( Magick::WarningCoder err ) {}
    catch ( Magick::Exception & error) {
      throw_error("get image size", "Could not read image file\""+filename+"\"."
                          " Caught Magick++ exception: \""+error.what()+"\".");
    }
    return Shape( imag.rows(), imag.columns() );

  }

}


void
ImageSizes(const Path & filename, int *width, int *hight){
  Shape shp = ImageSizes(filename);
  if (width)
    *width = shp(1);
  if (hight)
    *hight = shp(0);
}


void
BadShape(const Path & filename, const Shape & shp){
  Shape ashp = ImageSizes(filename);
  if ( ashp != shp )
    throw_error("load image", "The shape of the image"
                "\"" + filename + "\"  (" + toString(ashp) + ") is not equal"
                " to the requested shape (" + toString(shp)  + ").");
}




/// Loads an HDF5 image.
///
/// @param filedesc HDF5 file and description of the dataset slice
/// @param storage The array to store the image.
///
static void
ReadImage_HDF5 (const Path & filedesc, Map & storage ) {
  try {
    HDFread(filedesc).read(0,storage);
  } catch( ... ) {
    throw CtasErr(CtasErr::ERR, "HDF read", "Failed to read HDF5 from " + filedesc);
  }
}





/// Loads an image (lines) using TIFF library.
///
/// @param filename Name of the image
/// @param storage The array to store the image.
/// @param idxs The indexes of the line to read.
///        if empty then reads whole image.
///
static void
ReadImageLine_TIFF (const Path & filename, Map & storage,
                    const vector<int> & idxs ) {

  static const string modname = "load image tiff";

  TIFF *tif = TIFFOpen(filename.c_str(), "r");
  if( ! tif )
    throw CtasErr(CtasErr::WARN, modname,
                  "Could not read tif from file\"" + filename + "\".");

  uint32_t width = 0, height = 0;
  uint16_t spp = 0, bps = 0, fmt = 0, photo;

  if ( ! TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) )
    throw warn(modname,
               "Image \"" + filename + "\" has undefined width.");

  if ( ! TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) )
    throw warn(modname,
               "Image \"" + filename + "\" has undefined height.");

  if ( ! TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp) || spp != 1 )
    throw warn(modname,
               "Image \"" + filename + "\" has undefined samples per pixel"
               " or is not a grayscale.");
  if ( spp != 1 )
    throw warn(modname,
               "Image \"" + filename + "\" is not grayscale.");

  if ( ! TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps) )
    throw warn(modname,
               "Image \"" + filename + "\" has undefined bits per sample.");
  if ( bps != 8 && bps != 16 && bps != 32 )
    throw warn(modname,
               "Image \"" + filename + "\" has nonstandard " + toString(bps) +
               " bits per sample. Do not know how to handle it.");

  if ( ! TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &fmt) ) {
    string warnadd;
    if (bps != 32) {
      warnadd = toString(bps) +
           " bits per sample suggests unsigned integer format.";
      fmt = SAMPLEFORMAT_UINT;
    } else {
      warnadd = "32 bits per sample suggests float-point format.";
      fmt = SAMPLEFORMAT_IEEEFP;
    }
    // Gives to many warnings
    /*
    warn(modname,
         "Image \"" + filename + "\" has undefined sample format."
         " Guessing! " + warnadd);
    */
  }
  if ( fmt != SAMPLEFORMAT_UINT &&
       fmt != SAMPLEFORMAT_INT &&
       fmt != SAMPLEFORMAT_IEEEFP )
    throw warn(modname,
               "Image \"" + filename + "\" has unsupported sample format.");

  if ( ! TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photo) ||
       photo != PHOTOMETRIC_MINISBLACK )
    throw warn(modname,
         "Image \"" + filename + "\" has undefined or unsupported"
         " photometric interpretation.");

  const int readheight = idxs.size() ? idxs.size() : height;
  storage.resize(readheight,width);

  tdata_t buf = _TIFFmalloc(TIFFScanlineSize(tif));

  for (uint curidx = 0; curidx < readheight; curidx++) {

    uint32_t row = idxs.size() ? idxs[curidx] : curidx;

    if ( row >= height || row < 0 ) {

      warn("load imagelines tiff",
      "The index of the line to be read (" + toString(row) + ")"
      " is outside the image boundaries (" + toString(height) + ").");

      storage(curidx, all) = 0.0;

    } else {

      if ( TIFFReadScanline(tif, buf, row) < 0 ) {
        _TIFFfree(buf);
        throw warn(modname,
                   "Failed to read line " + toString(row) +
                   " in image \"" + filename + "\".");
      }

      #define blitzArrayFromData(type) \
        blitz::Array<type,1> ( (type *) buf, \
                               blitz::shape(width), \
                               blitz::neverDeleteData)

      switch (fmt) {
      case SAMPLEFORMAT_UINT :
        if (bps==8)
          storage(curidx, all) = 1.0 * blitzArrayFromData(uint8_t);
        else if (bps==16)
          storage(curidx, all) = 1.0 * blitzArrayFromData(uint16_t);
        else if (bps==32)
          storage(curidx, all) = 1.0 * blitzArrayFromData(uint32_t);
        break;
      case SAMPLEFORMAT_INT :
        if (bps==8)
          storage(curidx, all) = 1.0 * blitzArrayFromData(int8_t);
        else if (bps==16)
          storage(curidx, all) = 1.0 * blitzArrayFromData(int16_t);
        else if (bps==32)
          storage(curidx, all) = 1.0 * blitzArrayFromData(int32_t);
        break;
      case SAMPLEFORMAT_IEEEFP :
        storage(curidx, all) = blitzArrayFromData(float);
        break;
      }

    }

  }

  _TIFFfree(buf);
  TIFFClose(tif);

}

/// Loads an image (lines) using TIFF library.
///
/// @param filename Name of the image
/// @param storage The array to store the image.
/// @param idxs The indexes of the line to read.
///        if empty then reads whole image.
///
inline static void
ReadImageLine_TIFF (const Path & filename, Line & storage, int idx) {
  Map _storage;
  ReadImageLine_TIFF(filename, _storage, vector<int>(1,idx) );
  storage=_storage(0,all);
}


/// Loads an image using ImageMagick library.
///
/// @param filename Name of the image
/// @param storage The array to store the image.
///
inline static void
ReadImage_TIFF (const Path & filename, Map & storage) {
  ReadImageLine_TIFF(filename, storage, vector<int>() );
}




/// Loads an image using ImageMagick library.
///
/// @param filename Name of the image
/// @param storage The array to store the image.
///
static void
ReadImage_IM (const Path & filename, Map & storage ){

  Magick::Image imag;
  try { imag.read(filename); }
  catch ( Magick::WarningCoder err ) {}
  catch ( Magick::Exception & error) {
    throw_error("load image IM", "Could not read image file\""+filename+"\"."
                " Caught Magick++ exception: \""+error.what()+"\".");
  }
  if ( imag.type() != Magick::GrayscaleType )
    warn("load image IM",
         "Input image \"" + filename + "\" is not grayscale.");

  const int
    width = imag.columns(),
    hight = imag.rows();
  storage.resize( hight, width );

  // below might be buggy - see notes in SaveImageINT_IM
  /*
  const Magick::PixelPacket
    * pixels = imag.getConstPixels(0,0,width,hight);
  float * data = storage.data();
  for ( int k = 0 ; k < hight*width ; k++ )
    *data++ = (float) Magick::ColorGray( *pixels++  ) .shade();
  */

  /* Replacement for the buggy block */
  for (ArrIndex curw = 0 ; curw < width ; curw++)
    for (ArrIndex curh = 0 ; curh < hight ; curh++)
      storage(curh,curw) = Magick::ColorGray(imag.pixelColor(curw, curh)).shade();
  /* end replacement */

}


void
ReadImage (const Path & filename, Map & storage ){
  try { ReadImage_HDF5(filename, storage); }
  catch (CtasErr err) {
    try { ReadImage_TIFF(filename, storage); }
    catch (CtasErr err) {
      if (err.type() != CtasErr::WARN)
        throw;
      ReadImage_IM(filename, storage);
    }
  }
}


void
ReadImage(const Path & filename, Map & storage, const Shape & shp){
  BadShape(filename, shp);
  ReadImage(filename, storage);
}








/// \brief Reads one line of the image using ImageMagick library.
///
/// @param filename The name of the file with the image.
/// @param storage Line to read into.
/// @param idx The index of the line to read.
///
static void
ReadImageLine_IM (const Path & filename, Line & storage, int idx){

  Magick::Image imag;
  try { imag.read(filename); } catch ( Magick::WarningCoder err ) {}
  if ( imag.type() != Magick::GrayscaleType )
    warn("load imageline IM",
         "Input image \"" + filename + "\" is not grayscale.");

  const int width = imag.columns();
  if ( idx < 0 || (unsigned) idx >= imag.rows() )
    throw_error("load imageline IM",
                "The index of the line to be read (" + toString(idx) + ")"
                " is outside the image boundaries (" + toString(imag.rows()) + ").");
  storage.resize(width);

  // below might be buggy - see notes in SaveImageINT_IM
  /*
  const Magick::PixelPacket
    * pixels = imag.getConstPixels(0,idx,width,1);
  float * data = storage.data();
  for ( int k = 0 ; k < width ; k++ )
    *data++ = (float) Magick::ColorGray( *pixels++  ) .shade();
  */

  /* Replacement for the buggy block */
  for (ArrIndex curw = 0 ; curw < width ; curw++)
    storage(curw) = Magick::ColorGray(imag.pixelColor(curw, idx)).shade();
  /* end replacement */

}


void
ReadImageLine (const Path & filename, Line & storage, int idx) {
  try { ReadImageLine_TIFF(filename, storage, idx); }
  catch (CtasErr err) {
    if (err.type() != CtasErr::WARN)
      throw;
    ReadImageLine_IM(filename, storage, idx);
  }
}


void
ReadImageLine(const Path & filename, Line & storage, int idx,
              const Shape &shp) {
  BadShape(filename, shp);
  ReadImageLine(filename, storage, idx);
}




/// \brief Reads many line of the image using ImageMagick library.
///
/// @param filename The name of the file with the image.
/// @param storage Array to read into.
/// @param idxs The indexes of the line to read.
///
static void
ReadImageLine_IM (const Path & filename, Map & storage,
                  const vector<int> &idxs) {

  Magick::Image imag;
  try { imag.read(filename); } catch ( Magick::WarningCoder err ) {}
  if ( imag.type() != Magick::GrayscaleType )
    warn("load imagelines IM",
                 "Input image \"" + filename + "\" is not grayscale.");

  const int width = imag.columns();
  const int hight = imag.rows();
  const int readheight = idxs.size() ? idxs.size() : hight;
  storage.resize( readheight, width );

  for ( ArrIndex curel = 0 ; curel < readheight ; curel++ ){
    int cursl = idxs.size() ? idxs[curel] : curel;
    if ( cursl >= hight ) {
      warn("load imagelines IM",
           "The index of the line to be read (" + toString(cursl) + ")"
           " is outside the image boundaries (" + toString(hight) + ").");
      storage(curel, all ) = 0.0;
    } else {
      // below might be buggy - see notes in SaveImageINT_IM
      /*
      const Magick::PixelPacket *pixels = imag.getConstPixels(0,cursl,width,1);
      for ( ArrIndex k = 0 ; k < width ; k++ )
        storage( (ArrIndex) curel, k) =
          (float) Magick::ColorGray( *pixels++  ) .shade();
      */
      /* Replacement for the buggy block */
      for (ArrIndex curw = 0 ; curw < width ; curw++)
        storage(curel, curw) = Magick::ColorGray(imag.pixelColor(curw, cursl)).shade();
      /* end replacement */

    }

  }

}



void
ReadImageLine (const Path & filename, Map & storage,
               const vector<int> & idxs) {

  if ( ! idxs.size() ) {
    ReadImage(filename, storage);
    return;
  }

  try { ReadImageLine_TIFF(filename, storage, idxs); }
  catch (CtasErr err) {
    if (err.type() != CtasErr::WARN)
      throw;
      ReadImageLine_IM(filename, storage, idxs);
  }

}



void
ReadImageLine(const Path & filename, Map & storage,
                          const vector<int> & idxs, const Shape & shp){
  BadShape(filename, shp);
  ReadImageLine(filename, storage, idxs);
}








const std::string MaskDesc =
  "Used when there is a need to describe a stack of output files. In this case"
  " the special character represents the number of the output file in the stack."
  " The result mask is a string which forms the names of the processed slices."
  " The mask should (but not must) contain the '@' character which denotes the"
  " position where the slice number will be inserted:\n"
  "    If there is no '@' in the mask, it is inserted together with the '-'"
  " prefix right before the file extension (if any) or at the end of the mask"
  " (if no extension).\n"
  "    If there are more than one '@' character in the mask, only the last of"
  " them is replaced by the slice number.\n"
  "For example the mask\n"
  "    ../path/to/@/result/rec-@-@@.png\n"
  "produces file names similar to this (for the 13th slice):\n"
  "    ../path/to/@/result/rec-@-@013.png\n"
  "Note that the path(s) to the files must exist: the program will not create"
  " any directory.";







class ReadVolInThread : public InThread {

  vector< pair<Path,int> > slicelist;
  Volume & storage;
  Shape sh;
  pthread_mutex_t proglock;
  unordered_map<string,HDFread> hdfs;

public:

  ReadVolInThread(const std::vector<Path> & filelist, Volume & _storage, bool verbose=false)
    : storage(_storage)
    , InThread(verbose , "reading volume")
  {

    if ( ! filelist.size() ) {
      storage.free();
      return;
    }

    size_t curSz=0;
    sh = ImageSizes(filelist[0]);
    for ( vector<Path>::const_iterator curI = filelist.begin() ; curI < filelist.end() ; curI++ )
      try {
        hdfs.insert({*curI,HDFread(*curI)});
        HDFread & hdf = hdfs.at(*curI);
        slicelist.push_back(make_pair(*curI,curSz));
        curSz += hdf.indices.size();
      } catch (...) {
        slicelist.push_back(make_pair(*curI,curSz));
        curSz++;
      }
    bar.setSteps(curSz);
    storage.resize(curSz, sh(0), sh(1));
    if ( ! storage.size() )
      return;

  }


  bool inThread (long int idx) {

    if ( idx >= slicelist.size())
      return false;

    Map slice(sh);
    const pair<Path,int> & slpr = slicelist.at(idx);
    if (hdfs.count(slpr.first)) {
      HDFread & hdf = hdfs.at(slpr.first);
      if ( hdf.shape != sh )
        throw_error("Reading volume", "Missmatching image shape in " + hdf.id() + ".");
      for (int idxV=0 ; idxV<hdf.indices.size() ; idxV++ ) {
        hdf.read(idxV, slice);
        storage(slpr.second+idxV, all, all) = slice;
        bar.update();
      }
    } else {
      ReadImage(slpr.first, slice, sh);
      storage(slpr.second, all, all) = slice;
      bar.update();
    }

    return true;

  }

};


void
ReadVolume(const std::vector<Path> & filelist, Volume & storage, bool verbose) {
  ReadVolInThread(filelist, storage, verbose).execute();
}




class StackWriter {

private:

  const size_t zsize;
  HDFwrite * hdfFile;
  string sliceformat;
  const float mincon;
  const float maxcon;

public:

  StackWriter(const Path & filedesc, Shape _sh, size_t _zsize, float mmin, float mmax)
    : zsize(_zsize)
    , hdfFile(0)
    , mincon(mmin)
    , maxcon(mmax)
  {
    HDFdesc hdfdesc(filedesc);
    if ( hdfdesc.isValid() )
      hdfFile = new HDFwrite(filedesc, _sh, _zsize);
    else {
      if (zsize==1)
        sliceformat=filedesc;
      else {
        const Path outmask = string(filedesc).find('@') == string::npos
                           ? filedesc.dtitle() + "-@" + filedesc.extension()
                           : string(filedesc) ;
        sliceformat = mask2format(outmask, zsize);
      }
    }
  }

  ~StackWriter() {
    complete();
  }

  void complete() {
    HDFwrite * hh = hdfFile;
    hdfFile = 0;
    if (hh)
      hh->complete();
    delete hh;
  }

  void put(int idx, const Map & storage) {
    if (hdfFile) {
      if (mincon==maxcon)
        hdfFile->write(idx, storage);
      else {
        Map _stor(storage.shape());
        for (ArrIndex y = 0 ; y < storage.shape()(0) ; y++) {
          for (ArrIndex x = 0 ; x < storage.shape()(1) ; x++) {
            float val = storage(y,x);
            if (val < mincon)
              _stor(y,x) = mincon;
            else if (val > maxcon)
              _stor(y,x) = maxcon;
            else
              _stor(y,x) = val;
          }
        }
        hdfFile->write(idx, storage);
      }
    } else {
      const Path fileName =  zsize == 1  ?  sliceformat : Path(toString(sliceformat, idx));
      if (mincon==maxcon)
        SaveImage(fileName, storage);
      else
        SaveImage(fileName, storage, mincon, maxcon);
    }
  }


};



class SaveVolInThread : public InThread {

private:

  const Volume & vol;
  StackWriter writer;
  int sliceDim;
  Shape ssh;
  vector<int>indices;
  unordered_map<pthread_t,Map> maps;

  bool inThread (long int idx) {

    if (idx >= indices.size())
      return false;
    const pthread_t me = pthread_self();
    lock();
    if ( ! maps.count(me) )  // first call
      maps.insert({me, Map(ssh)});
    Map & cur = maps.at(me) ;
    unlock();

    const int idi = indices[idx];
    switch ( sliceDim ) {
      case 2: cur = vol(all, all, idi); break;
      case 1: cur = vol(all, idi, all); break;
      case 0: cur = vol(idi, all, all); break;
    }
    writer.put(idi, cur);
    bar.update();
    return true;

  }


public:

  SaveVolInThread(const string & filedesc, const Volume & _vol, bool verbose,
                  const std::string & slicedesc, float mmin, float mmax)
    : vol(_vol)
    , writer(filedesc, faceShape(vol.shape()), vol.shape()(0), mmin, mmax)
    , InThread(verbose , "saving volume")
  {

    Shape3 vsh(vol.shape());
    string sindex = slicedesc.size()  ?  slicedesc  :  "Z";
    switch ( sindex.at(0) ) {
      case 'x':
      case 'X':
        sindex.erase(0,1);
        sliceDim=2;
        ssh = Shape(vsh(0),vsh(1));
        break;
      case 'y':
      case 'Y':
        sindex.erase(0,1);
        sliceDim=1;
        ssh = Shape(vsh(0),vsh(2));
        break;
      case 'z':
      case 'Z':
        sindex.erase(0,1);
      default:
        sliceDim=0;
        ssh = Shape(vsh(1),vsh(2));
    }
    indices = slice_str2vec(sindex, vsh(sliceDim));
    bar.setSteps(indices.size());

  }

  static void execute(const string & filedesc, const Volume & _vol, bool verbose,
                      const std::string & slicedesc, float mmin, float mmax) {
    SaveVolInThread(filedesc, _vol, verbose, slicedesc, mmin, mmax)
        .InThread::execute();
  }

};


void SaveVolume(const Path & filedesc, Volume & storage, bool verbose,
                const string & slicedesc, float mmin, float mmax) {
  SaveVolInThread::execute(filedesc, storage, verbose, slicedesc, mmin, mmax);
}






/// Saves image in integer format using ImageMagick library.
///
/// @param filename file to save image into.
/// @param storage array with the image.
///
static void
SaveImageINT_IM (const Path & filename, const Map & storage){

  if ( ! storage.size() ) {
    warn("save image", "Zero-sized array for image.");
    return;
  }

  const int
    width = storage.columns(),
    hight = storage.rows();

  Map _storage;
  if ( storage.isStorageContiguous()  &&  storage.stride() == Shape(width,1) )
    _storage.reference(storage);
  else {
    _storage.resize(storage.shape());
    _storage = storage;
  }

  Magick::Image imag( width, hight, "I", Magick::FloatPixel, _storage.data() );
  imag.classType(Magick::DirectClass);
  imag.type( Magick::GrayscaleType );
  imag.depth(8);
  //imag.magick("TIFF"); // saves to tif if not overwritten by the extension.
  try { imag.write(filename); }
  catch ( Magick::Exception & error) {
    throw_error("save image IM", "Could not write image file\""+filename+"\"."
                        " Caught Magick++ exception: \""+error.what()+"\".");
  }

}




namespace blitz {

/// Limits the number to the [0;1] region
///
/// @param x number to be limited
///
/// @return limited number
///
float
limit01(float x){
  return ( x < 0.0 ) ?
    0.0 :
    ( x > 1.0 ? 1.0 : x ) ;
}

/// \cond
BZ_DECLARE_FUNCTION(limit01);
/// \endcond

}



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
static void
SaveImageINT (const Path &filename, const Map &storage,
              float minval=0, float maxval=0 ) {

  if ( ! storage.size() ) {
    warn("save image",
         "Zero-sized array for image '" + filename + "': won't save." );
    return;
  }

  const int width = storage.columns();

  Map _storage;
  if ( storage.isStorageContiguous()  &&  storage.stride() == Shape(width,1) )
    _storage.reference(storage);
  else {
    _storage.resize(storage.shape());
    _storage = storage;
  }

  Map stor(_storage.shape());
  if (minval == maxval) {
    minval = (blitz::min)(_storage);
    maxval = (blitz::max)(_storage);
  }
  if (minval == maxval) {
    warn("save image",
         "All elements in the image '" + filename + "' have the same value.");
    if      ( minval < 0.0 ) stor = 0.0;
    else if ( minval > 1.0 ) stor = 1.0;
    else                     stor = minval;
  } else {
    const string modname = "Limit array";
    stor = ( _storage - minval ) / (maxval-minval);
    stor = limit01(stor);
  }
  SaveImageINT_IM(filename, stor);

}




/// \brief Save the array into float point TIFF image.
///
/// Stores the array in the float-point TIFF file. Be careful: limited number
/// of editors/viewers/analyzers support float point format.
///
/// @param filename the name of the image file image.
/// @param storage the array to be written to the image.
///
void
SaveImageFP (const Path & filename, const Map & storage){

  if ( ! storage.size() ) {
    warn("save image", "Zero-sized array for image.");
    return;
  }

  const int
    width = storage.columns(),
    hight = storage.rows();

  Map _storage;
  if ( storage.isStorageContiguous()  &&  storage.stride() == Shape(width,1) )
    _storage.reference(storage);
  else {
    _storage.resize(storage.shape());
    _storage = storage;
  }

  // BUG in libtiff
  // On platforms (f.e. CentOS) the TIFFOpen function fails,
  // while TIFFFdOpen works well. On the MS Windows the
  // TIFFFdOpen does not work, while TIFFOpen does.

  int fd=0;
#ifdef _WIN32
  TIFF *image = TIFFOpen(filename.c_str(), "w");
#else
  fd = open (filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (fd < 1)
        throw_error("save float-point image",
                                "Could not open file \"" + filename + "\" for writing.");
  TIFF *image = TIFFFdOpen(fd, filename.c_str(), "w");
#endif

  if( ! image ) {
        if (fd) close(fd);
        throw_error("save float-point image",
                                "Could create tif from file\"" + filename + "\".");
  }

  // We need to set some values for basic tags before we can add any data
  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, hight);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 32);
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, hight);
  TIFFSetField(image, TIFFTAG_SAMPLEFORMAT,SAMPLEFORMAT_IEEEFP);
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

  int wret = TIFFWriteRawStrip(image, 0, (void*) _storage.data(), width*hight*4);
  TIFFClose(image);
  if (fd) close(fd);
  if ( -1 == wret )
        throw_error("save 32-bit image",
                                "Could not save image to file \"" + filename + "\".");

}


void
SaveImage(const Path & filename, const Map & storage, bool saveint){
  if (saveint) SaveImageINT(filename, storage);
  else SaveImageFP(filename, storage);
}

void
SaveImage(const Path & filename, const Map & storage,
          float minval, float maxval ){
  SaveImageINT(filename, storage, minval, maxval);
}
















void
SaveData ( const Path filename, ... ) {

  va_list ap;
  va_start(ap, filename);

  vector<const Line*> storage;
  while ( const Line *curstor = va_arg(ap,const Line*) )
    storage.push_back(curstor);
  va_end(ap);

  int nof_args = storage.size();
  if ( ! nof_args ) {
    warn("write data", "No arrays provided for output. Nothing to do." );
    return;
  }

  int size = storage.front()->size();
  if ( ! size ) {
    warn("write data", "Empty arrays provided for output. Nothing to do." );
    return;
  }
  for (int icur = 0 ; icur < nof_args ; icur++)
    if ( storage[icur]->size() != size )
      throw_error("write data",
                  "The size of the array in the position " + toString(icur) +
                                  " (" + toString(storage[icur]->size()) + ") does not match the"
                                  " size of the first array (" + toString(size) + ").");

  FILE *funcf = fopen( filename.c_str(), "w" );
  if ( ! funcf )
    throw_error("write data", "Could not open output file \"" + filename + "\".");

  for (int element = 0 ; element < size ; element++) {
    bool printok = true;
    int curarray = -1;
    while (printok && ++curarray < nof_args)
      printok = fprintf( funcf, "%G ", (*storage[curarray])(element) ) >= 0;
    if ( ! printok ||
                 fseek (funcf, -1, SEEK_CUR) || // removes last space
                 fprintf(funcf, "\n") < 0 ) {
      fclose (funcf);
      throw_error("write data", "Could not print into output file"
                                  " \"" + filename + "\" in position " + toString(element) + ".");
    }
  }

  fclose (funcf);

}


void
LoadData ( const Path filename, ... ) {

  vector<Line*> storage;

  va_list ap;
  va_start(ap, filename);
  while ( Line * curstor = va_arg(ap, Line*) )
    storage.push_back( curstor );
  va_end(ap);

  int nof_args = storage.size();
  if ( ! nof_args ) {
    warn("read data", "No arrays provided for input. Nothing to do." );
    return;
  }
  vector< vector<float> > data_read(nof_args);

  FILE *funcf = fopen( filename.c_str(), "r" );
  if ( ! funcf )
    throw_error("read data", "Could not open input file \"" + filename + "\".");

  bool this_is_the_end = false;
  while ( ! this_is_the_end ) {
    for ( int curarray = 0 ; curarray < nof_args ; curarray++ ) {
      float toread;
      if ( fscanf( funcf, "%f ", &toread ) != 1 ) {
        fclose (funcf);
        throw_error("read data", "Could not scan float from input file"
                                        " \"" + filename + "\" at line " +
                                        toString(data_read[curarray].size() + 1) + ", position " +
                                        toString(curarray + 1) + ".");
      }
      data_read[curarray].push_back(toread);
    }
    this_is_the_end = fscanf(funcf, "\n") < 0 || feof(funcf);
  }

  fclose (funcf);

  int size = data_read[0].size();
  for ( int curarray = 0 ; curarray < nof_args ; curarray++ ) {
    (*storage[curarray]).resize(size);
    for (int element = 0 ; element < size ; element++)
      (*storage[curarray])(element) = data_read[curarray][element];
  }

}






