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

#include<algorithm>
#include <stdarg.h>
#include <string.h> // for memcpy
#include <unistd.h>
#include "common.h"


using namespace std;


/// \brief Constructor.
///
/// @param _terr Sets CtasErr::terr
/// @param mod   Sets CtasErr::module
/// @param msg   Sets CtasErr::message
///
CtasErr::CtasErr(ErrTp _terr, const string & mod, const string & msg){
  terr = _terr;
  module = mod;
  message = msg;
}

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
}


CtasErr::ErrTp
CtasErr::type() const {
  return terr;
}


void
throw_error(const string & mod, const string & msg){
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

bool
_conversion (Path* _val, const string & in) {
  *_val = Path(in);
  return true;
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

bool
_conversion (Contrast* _val, const string & in) {
  *_val=Contrast(in);
  return true;
}














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
ProgressBar::ProgressBar(bool _showme, const string & _message, int _steps) :
  showme(_showme), message(_message), steps(_steps) {

  if ( ! showme ) return;

  step = 0;
  waswidth = 0;
  reservedChs = 0;

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

/// \brief Updates the progress bar.
///
/// @param curstep Current step. Advances +1 if zero.
///
void
ProgressBar::update(int curstep){

  if ( !showme || !reservedChs ) return; // Uninitialized progress bar.

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











#ifdef _WIN32
#  define STATIC_MAGICK
#  define MAGICK_STATIC_LINK
#endif
#include<Magick++.h>
#include<FreeImage.h>


static bool
CtasMagickInited = false; ///< \c true if ImageMagick has been initialized.

/// \brief initializes ImageMagick library
///
/// Use it in all binaries to allow reading up to 10k x 10k into memory, not HDD.
static inline void
initMagick(){

#ifdef MAGICKLIB_NAMESPACE
  using namespace MagickLib;
#else
  using namespace MagickCore;
#endif

  if ( CtasMagickInited )
    return;

  MagickSizeType Msz = (numeric_limits<MagickSizeType>::max)();
  SetMagickResourceLimit ( AreaResource , 10000 * 10000 * 4);
  SetMagickResourceLimit ( FileResource , 1024 * 1024);
  SetMagickResourceLimit ( DiskResource , Msz);
  SetMagickResourceLimit ( MapResource , Msz);
  SetMagickResourceLimit ( MemoryResource , Msz);

  CtasMagickInited = true;

}



/// Loads the FreeImage image.
///
/// For further use with FreeImage library's functions.
/// Do not forget to unload image afterwards.
///
/// @param filename Name of the image.
///
/// @return
///
static FIBITMAP *
FImageLoader(const Path & filename) {

  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  FIBITMAP *dib=0;
  fif = FreeImage_GetFileType(filename.c_str(), 0);
  if(fif == FIF_UNKNOWN)
    fif = FreeImage_GetFIFFromFilename(filename.c_str());
  if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
    dib = FreeImage_Load(fif, filename.c_str(), 0);
  
  if( ! dib )
	throw warn("load image FI", "Could not load image \"" + filename + "\".");

  FREE_IMAGE_COLOR_TYPE tp = FreeImage_GetColorType(dib);
  if ( tp != FIC_MINISBLACK  &&  tp != FIC_MINISWHITE ) {
    FreeImage_Unload(dib);
	throw warn("load image FI", "Input image \"" + filename +
			   "\" is not grayscale.");
  }

  return dib;

}



float
PixelSize(const Path & filename) {
  static const float defaultSize = 1.0;
  Magick::Image imag;
  try { imag.ping(filename); }    catch ( Magick::WarningCoder err ) {}
  Magick::Geometry dens = imag.density();
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
  Magick::Image imag;
  try { imag.ping(filename); }
  catch ( Magick::WarningCoder err ) {}
  catch ( Magick::Exception & error) {
    throw_error("get image size", "Could not read image file\""+filename+"\"."
		        " Caught Magick++ exception: \""+error.what()+"\".");
  }
  return Shape( imag.rows(), imag.columns() );
}


void
ImageSizes(const Path & filename, int *width, int *hight){
  Shape shp = ImageSizes(filename);
  *width = shp(1);
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



#  include<climits>


/// Reads FreeImage image into the array.
///
/// @param storage The array to read into.
/// @param dib Image to read.
///
template <class StClass> static inline void
fip2arr(Map & storage, FIBITMAP *dib ){

  const double coeff = numeric_limits<StClass>::is_integer ?
	1.0/(numeric_limits<StClass>::max)()  :  1.0 ;
  StClass * pixels = (StClass *)  FreeImage_GetBits(dib);
  const int
	bps = FreeImage_GetBPP(dib),
	hight =  FreeImage_GetHeight(dib),
	width =  FreeImage_GetWidth(dib),
	shift =  FreeImage_GetPitch(dib) / (bps/8);

  storage.resize(hight,width);
  blitz::Array<StClass,2> tarr(hight,width);

  for (int y=0 ; y < hight ; y++)
	memcpy(tarr.data() + y*width, pixels + (hight-y-1)*shift, width*bps/8);
  storage = blitz::cast<float>(tarr) * coeff;

}

/// Loads an image using FreeImage library.
///
/// @param filename Name of the image
/// @param storage The array to store the image.
///
static void
ReadImage_FI (const Path & filename, Map & storage ){

  FIBITMAP *dib = FImageLoader(filename);
  if (!dib)
	  throw warn("load image FI", "FreeImage could not open image"
	             " \"" + filename + "\" for reading.");

  void (*convert)(Map &, FIBITMAP *);
  switch (  FreeImage_GetImageType(dib) ) {
  case FIT_UINT16 : convert = fip2arr<unsigned short>; break;
  case FIT_INT16  : convert = fip2arr<short>;          break;
  case FIT_UINT32 : convert = fip2arr<unsigned>;       break;
  case FIT_INT32  : convert = fip2arr<int>;            break;
  case FIT_FLOAT  : convert = fip2arr<float>;          break;
  case FIT_DOUBLE :
	warn("load image FI",
		 "Input image \"" + filename + "\" has pixels of double type."
		 " May give wrong result when converting from double to float.");
	convert = fip2arr<double>;
	break;

  default :
    FreeImage_Unload(dib);
	throw warn("load image FI", "Input image \"" + filename + "\""
			   " cannot be loaded with FreeImage: not single channel per pixel.");
  }

  convert(storage, dib);

  FreeImage_Unload(dib);

}

/// Loads an image using ImageMagick library.
///
/// @param filename Name of the image
/// @param storage The array to store the image.
///
static void
ReadImage_IM (const Path & filename, Map & storage ){

  initMagick();

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
  const Magick::PixelPacket 
    * pixels = imag.getConstPixels(0,0,width,hight);

  storage.resize( hight, width );
  float * data = storage.data();

  for ( int k = 0 ; k < hight*width ; k++ )
    *data++ = (float) Magick::ColorGray( *pixels++  ) .shade();

}


void
ReadImage (const Path & filename, Map & storage ){
  try { ReadImage_FI(filename, storage); }
  catch (CtasErr err) {
	if (err.type() != CtasErr::WARN) throw err;
	// warn("load image", "Loading of image file \"" + filename + "\""
	// " with FreeImage failed. Trying to load using ImageMagick.");
	ReadImage_IM(filename, storage);
  }
}


void
ReadImage(const Path & filename, Map & storage, const Shape & shp){
  BadShape(filename, shp);
  ReadImage(filename, storage);
}






/// Reads one line of the FreeImage image. 
///
/// @param storage The array to store the line
/// @param dib The image to read from.
/// @param idx The index of the line to read.
///
template <class StClass> static inline void
fip2ln(Line & storage, FIBITMAP *dib, int idx){

  const double coeff = numeric_limits<StClass>::is_integer ?
	1.0/(numeric_limits<StClass>::max)()  :  1.0 ;
  StClass * pixels = (StClass *)  FreeImage_GetBits(dib);
  const int
	bps = FreeImage_GetBPP(dib),
	hight =  FreeImage_GetHeight(dib),
	width =  FreeImage_GetWidth(dib),
	shift =  FreeImage_GetPitch(dib) / (bps/8);

  storage.resize(width);
  blitz::Array<StClass,1> tarr(width) ;

  memcpy(tarr.data(), pixels + (hight-idx-1)*shift, width*bps/8);
  storage = blitz::cast<float>(tarr) * coeff;

}


/// \brief Reads one line of the image using FreeImage library. 
///
/// @param filename The name of the file with the image.
/// @param storage Line to read into.
/// @param idx The index of the line to read.
///
static void
ReadImageLine_FI (const Path & filename, Line & storage, int idx){

  FIBITMAP *dib = FImageLoader(filename);

  const int
	hight =  FreeImage_GetHeight(dib),
	width =  FreeImage_GetWidth(dib);

  if (idx >= hight || idx < 0 ) {
    FreeImage_Unload(dib);
    throw_error("load imageline FI",
                "The index of the line to be read (" + toString(idx) + ")"
				" is outside the image boundaries (" + toString(hight) + ").");
  }

  storage.resize( width );

  void (*convert)(Line &, FIBITMAP *dib, int);
  switch ( FreeImage_GetImageType(dib) ) {
  case FIT_UINT16 : convert = fip2ln<unsigned short>; break;
  case FIT_INT16  : convert = fip2ln<short>;          break;
  case FIT_UINT32 : convert = fip2ln<unsigned>;       break;
  case FIT_INT32  : convert = fip2ln<int>;            break;
  case FIT_FLOAT  : convert = fip2ln<float>;          break;
  case FIT_DOUBLE :
	warn("load imageline FI",
		 "Input image \"" + filename + "\" has pixels of double type."
		 " May give wrong result when converting from double to float.");
	convert = fip2ln<double>;
	break;

  default :
    FreeImage_Unload(dib);
	throw warn("load imageline FI", "Input image \"" + filename + "\""
			   " cannot be loaded with FreeImage: not single channel per pixel.");
  }

  convert(storage, dib, idx);

  FreeImage_Unload(dib);

}


/// \brief Reads one line of the image using ImageMagick library. 
///
/// @param filename The name of the file with the image.
/// @param storage Line to read into.
/// @param idx The index of the line to read.
///
static void
ReadImageLine_IM (const Path & filename, Line & storage, int idx){

  initMagick();
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

  const Magick::PixelPacket
    * pixels = imag.getConstPixels(0,idx,width,1);
  storage.resize( width );
  float * data = storage.data();

  for ( int k = 0 ; k < width ; k++ )
    *data++ = (float) Magick::ColorGray( *pixels++  ) .shade();

}


void
ReadImageLine (const Path & filename, Line & storage, int idx){
  try { ReadImageLine_FI(filename, storage, idx); }
  catch (CtasErr err) {
  	if (err.type() != CtasErr::WARN)
  	  throw err;
	// Loading of image line from file \"" + filename + "\"
	// with FreeImage failed. Trying to load using ImageMagick.
	ReadImageLine_IM(filename, storage, idx);
  }
}


void
ReadImageLine(const Path & filename, Line & storage, int idx,
			  const Shape & shp){
  BadShape(filename, shp);
  ReadImageLine(filename, storage, idx);
}



/// \brief Reads many lines of the image using FreeImage library. 
///
/// @param filename The name of the file with the image.
/// @param storage Array to read into.
/// @param idxs The indexes of the line to read.
///
static void
ReadImageLine_FI (const Path & filename, Map & storage,
				  const vector<int> & idxs){

  FIBITMAP *dib = FImageLoader(filename);
  const int
	hight =  FreeImage_GetHeight(dib),
	width =  FreeImage_GetWidth(dib);

  storage.resize( idxs.size(), width );

  void (*convert)(Line &, FIBITMAP *dib, int);
  switch ( FreeImage_GetImageType(dib) ) {
  case FIT_UINT16 : convert = fip2ln<unsigned short>; break;
  case FIT_INT16  : convert = fip2ln<short>;          break;
  case FIT_UINT32 : convert = fip2ln<unsigned>;       break;
  case FIT_INT32  : convert = fip2ln<int>;            break;
  case FIT_FLOAT  : convert = fip2ln<float>;          break;
  case FIT_DOUBLE :
	warn("load imagelines FI",
		 "Input image \"" + filename + "\" has pixels of double type."
		 " May give wrong result when converting from double to float.");
	convert = fip2ln<double>;
	break;

  default :
    FreeImage_Unload(dib);
	throw warn("load imagelines FI", "Input image \"" + filename + "\""
			   " cannot be loaded with FreeImage: not single channel per pixel.");
  }

  for ( unsigned curel = 0 ; curel < idxs.size() ; curel++ ){
	int cursl = idxs[curel];
	Line curline = storage(curel, blitz::Range::all());
	if ( cursl >= hight || cursl < 0 ) {
	  warn("load imagelines FI",
		   "The index of the line to be read (" + toString(cursl) + ")"
		   " is outside the image boundaries (" + toString(hight) + ").");
	  curline = 0.0;
	} else {
	  convert( curline, dib, cursl);
	}

  }

  FreeImage_Unload(dib);

}

/// \brief Reads many line of the image using ImageMagick library. 
///
/// @param filename The name of the file with the image.
/// @param storage Array to read into.
/// @param idxs The indexes of the line to read.
///
static void
ReadImageLine_IM (const Path & filename, Map & storage,
			   const vector<int> & idxs){

  initMagick();
  Magick::Image imag;
  try { imag.read(filename); } catch ( Magick::WarningCoder err ) {}
  if ( imag.type() != Magick::GrayscaleType )
    warn("load imagelines IM",
		 "Input image \"" + filename + "\" is not grayscale.");

  const int width = imag.columns();
  const int hight = imag.rows();
  storage.resize( idxs.size(), width );

  for ( unsigned curel = 0 ; curel < idxs.size() ; curel++ ){
	int cursl = idxs[curel];
	if ( cursl >= hight ) {
	  warn("load imagelines IM",
		   "The index of the line to be read (" + toString(cursl) + ")"
		   " is outside the image boundaries (" + toString(hight) + ").");
	  storage(curel, blitz::Range::all() ) = 0.0;
	} else {
	  const Magick::PixelPacket * pixels = imag.getConstPixels(0,cursl,width,1);
	  for ( int k = 0 ; k < width ; k++ )
		storage( (int) curel, k) = (float) Magick::ColorGray( *pixels++  ) .shade();
	}

  }

}



void
ReadImageLine (const Path & filename, Map & storage,
			   const vector<int> & idxs){
  if ( ! idxs.size() ) {
    ReadImage(filename, storage);
    return;
  }
  try { ReadImageLine_FI(filename, storage, idxs); }
  catch (CtasErr err) {
	if (err.type() != CtasErr::WARN)
	  throw err;
	/* warn("load image lines",
		 "Loading of image lines from file \"" + filename + "\""
		 " with FreeImage failed. Trying to load using ImageMagick."); */
	ReadImageLine_IM(filename, storage, idxs);
  }
}



void
ReadImageLine(const Path & filename, Map & storage,
			  const vector<int> & idxs, const Shape & shp){
  BadShape(filename, shp);
  ReadImageLine(filename, storage, idxs);
}






/// Saves image in integer format using FreeImage library.
///
/// @param filename file to save image into.
/// @param storage array with the image.
///
static void
SaveImageINT_FI (const Path & filename, const Map & storage){

  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename.c_str());
  if( fif == FIF_UNKNOWN )
	throw warn("save image FI", "The output filename \"" + filename + "\""
			   " does not correspond to any format supported by FreeImage");
  if ( ! FreeImage_FIFSupportsWriting(fif) )
	throw warn("save image FI", "FreeImage cannot write to this file format.");

  const int
    width = storage.columns(),
    hight = storage.rows(),
	bps = 16;

  FIBITMAP *dib =  FreeImage_AllocateT(FIT_UINT16, width, hight, bps, 0, 0, 0);
  if (!dib)
    throw warn("save image FI", "FreeImage could not allocate memory for the image.");

  const int shift = FreeImage_GetPitch(dib) / (bps/8);
  unsigned short * pixels = (unsigned short *) FreeImage_GetBits(dib);

  blitz::Array<unsigned short,2> tarr( storage.shape() ) ;
  tarr = (numeric_limits<unsigned short>::max)() * storage;

  for (int y=0 ; y < hight ; y++)
	memcpy( pixels + (hight-y-1)*shift, tarr.data()+ y*width, width*bps/8);

  bool saveSuc = FreeImage_Save(fif, dib, filename.c_str(), 0);
  FreeImage_Unload(dib);
  if ( ! saveSuc )
	throw warn("save image FI", "FreeImage could not save image"
			   " to output file \"" + filename + "\".");

}


/// Saves image in integer format using ImageMagick library.
///
/// @param filename file to save image into.
/// @param storage array with the image.
///
static void
SaveImageINT_IM (const Path & filename, const Map & storage){

  initMagick();

  const int
    width = storage.columns(),
    hight = storage.rows();

  Magick::Image imag( Magick::Geometry(width, hight), "black" );
  imag.classType(Magick::DirectClass);
  imag.type( Magick::GrayscaleType );
  imag.depth(16);
  imag.magick("TIFF"); // saves to tif if not overwritten by the extension.

  const float *data = storage.data();
  Magick::PixelPacket * pixels = imag.getPixels(0,0,width,hight);
  Magick::ColorGray colg;
  for ( int k = 0 ; k < hight*width ; k++ )
    *pixels++ = Magick::PixelPacket( ( colg.shade( *data++ ), colg ) );

  imag.syncPixels();
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
    (0.0f) :
    ( x > 1.0 ? 1.0f : x ) ;
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
SaveImageINT (const Path & filename, const Map & storage,
			  float minval=0, float maxval=0 ){

  if ( ! storage.size() ) {
    warn("save image",
		 "Zero-sized array for image '" + filename + "': won't save." );
    return;
  }

  Map stor(storage.shape());
  if (minval == maxval) {
	  minval = (blitz::min)(storage);
    maxval = (blitz::max)(storage);
  }
  if (minval == maxval) {
	warn("save image",
		 "All elements in the image '" + filename + "' have the same value.");
	if      ( minval < 0.0 ) stor = 0.0;
	else if ( minval > 1.0 ) stor = 1.0;
	else                     stor = minval;
  } else {
	stor = ( storage - minval ) / (maxval-minval);
	stor = limit01(stor);
  }

  try { SaveImageINT_FI(filename, stor); }
  catch (CtasErr err) {
  	if (err.type() != CtasErr::WARN)
  	  throw err;
	// Saving with FreeImage failed. Trying ImageMagick.
	SaveImageINT_IM(filename, stor);
  }

}



#  include <tiffio.h>
#  include <fcntl.h> // for the libc "open" function see bug description in the SaveImageFP function.

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

  int wret = TIFFWriteRawStrip(image, 0, (void*) storage.data(), width*hight*4);
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
