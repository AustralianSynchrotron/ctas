#include "external.world.h"
#include "parallel.world.h"

#include <tiffio.h>
#include <H5Cpp.h>
#include <unordered_map>
#include <unistd.h>
#include <fstream>
#include <fcntl.h> // for the libc "open" function see bug description in the SaveImageFP function.



using namespace std;







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





deque<string> split (string s, string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  string token;
  deque<string> res;
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

  static const string & checkFiledesc(const string & filedesc) {
    if ( HDFdesc(filedesc).isValid() )
      return filedesc;
    else
      throw CtasErr(CtasErr::WARN, "HDFread",
                    "Not an hdf file \"" + filedesc + "\".");
  }

  HDFdesc(const string & filedesc)
    : sliceDim(0)
  {

    const string modmname = "HDF desc";
    deque<string> hdfRd = split(filedesc, ":");
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

  bool isValid() const { return name.length() ; }

  static bool isValid(const string & filedesc) { return HDFdesc(filedesc).isValid(); }

  static bool isValidHDF(const string & filedesc) {
    HDFdesc hdf(filedesc);
    return hdf.isValid() && H5::H5File::isHdf5(hdf.name);
  }

};


bool isHDFdesc(const std::string& filedesc) {
  return HDFdesc::isValidHDF(filedesc);
}





struct HDFread : public HDFdesc {

public :

  mutable Shape shape;
  mutable vector<int> indices;

private :

  H5::H5File hdfFile;
  H5::DataSet dataset;
  H5::DataSpace dataspace;
  H5::DataSpace memspace;
  int rank;
  blitz::Array<hsize_t, 1> cnts;

public :

  HDFread(const string & filedesc)
    : HDFdesc(checkFiledesc(filedesc))
  {

    const string modmname = "HDF read";

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
      throw_error(modmname, "Error getting info from " + filedesc);
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

  Shape3 sizeToRead() {
    return Shape3(indices.size(), shape(1), shape(0));
  }

};


Shape3 volumeShape(const string & filedesc) {
  try {
    HDFread hdf(filedesc);
    return hdf.sizeToRead();
  } catch (...) {
    return Shape3();
  }
}





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
      size_t poss = 0, pose;
      while ( (pose=data.find("/",poss)) != string::npos) {
        string elm = data.substr(poss, pose);
        if (elm.size())
          hdfFile.createGroup(data.substr(0,pose));
        poss = pose + 1;
      }
      float fillvalue = 0.0;   /* Fill value for the dataset */
      H5::DSetCreatPropList plist;
      plist.setFillValue(H5::PredType::NATIVE_FLOAT, &fillvalue);
      dataspace = H5::DataSpace(3, cnts.data());
      dataset = H5::DataSet(hdfFile.createDataSet(data, H5::PredType::NATIVE_FLOAT, dataspace, plist));

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
    : HDFwrite(HDFdesc(checkFiledesc(filedesc)), _sh, _zsize)
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

    Map _storage(safe(storage));
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




pthread_mutex_t mut(PTHREAD_MUTEX_INITIALIZER);


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

  const string modname = "load image tiff";

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

    pthread_mutex_lock(&mut); // I do not understand why,
    // but with no mutex storage(curidx,all) causes double free
    // or corruption on ASCI.
    Line ln(storage(curidx, all));
    pthread_mutex_unlock(&mut);


    if ( row >= height || row < 0 ) {

      warn("load imagelines tiff",
      "The index of the line to be read (" + toString(row) + ")"
      " is outside the image boundaries (" + toString(height) + ").");

      ln = 0.0f;

    } else {

      if ( TIFFReadScanline(tif, buf, row) < 0 ) {
        _TIFFfree(buf);
        TIFFClose(tif);
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
          ln = 1.0 * blitzArrayFromData(uint8_t);
        else if (bps==16)
          ln = 1.0 * blitzArrayFromData(uint16_t);
        else if (bps==32)
          ln = 1.0 * blitzArrayFromData(uint32_t);
        break;
      case SAMPLEFORMAT_INT :
        if (bps==8)
          ln = 1.0 * blitzArrayFromData(int8_t);
        else if (bps==16)
          ln = 1.0 * blitzArrayFromData(int16_t);
        else if (bps==32)
          ln = 1.0 * blitzArrayFromData(int32_t);
        break;
      case SAMPLEFORMAT_IEEEFP :
        ln = blitzArrayFromData(float);
        break;
      }

    }

    pthread_mutex_lock(&mut);
    ln.reference(Line(0));
    pthread_mutex_unlock(&mut);

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

  deque< pair<Path,int> > slicelist;
  Volume & storage;
  const float ang;
  const Crop crp;
  const Binn bnn;
  Shape ish;
  Shape rsh;
  Shape csh;
  Shape bsh;
  Shape sh;
  pthread_mutex_t proglock;
  unordered_map<string,HDFread> hdfs;
  unordered_map<pthread_t,Map> islices;
  unordered_map<pthread_t,Map> rslices;
  unordered_map<pthread_t,Map> cslices;
  unordered_map<pthread_t,Map> bslices;

public:

  ReadVolInThread(const std::deque<Path> & filelist, Volume & _storage,
                  float _angle, Crop _crop, Binn _binn, bool verbose=false)
    : InThread(verbose , "reading volume")
    , storage(_storage)
    , ang(_angle)
    , crp(_crop)
    , bnn(_binn)
  {

    if ( ! filelist.size() ) {
      storage.free();
      return;
    }

    size_t curSz=0;
    ish = ImageSizes(filelist[0]);
    rsh = shapeOnRotate(ish, ang);
    csh = Shape(rsh(0)-crp.top-crp.bottom, rsh(1)-crp.left-crp.right);
    bsh = shapeOnBinn(csh, bnn);
    sh = bsh;
    for ( deque<Path>::const_iterator curI = filelist.begin() ; curI < filelist.end() ; curI++ )
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

    pthread_t me = pthread_self();
    lock();
    if ( ! islices.count(me) ) {
      islices.insert({me,Map(ish)});
      rslices.insert({me,Map()});
      if (ang==0)
        rslices.at(me).reference(islices.at(me));
      else
        rslices.at(me).resize(rsh);
      cslices.insert({me,Map()});
      if (rsh==csh)
        cslices.at(me).reference(rslices.at(me));
      else
        cslices.at(me).resize(csh);
      bslices.insert({me,Map()});
      if (csh==bsh)
        bslices.at(me).reference(cslices.at(me));
      else
        bslices.at(me).resize(bsh);
      //rslices.insert({me, ang == 0   ? islices.at(me) : Map(rsh)});
      //cslices.insert({me, rsh == csh ? rslices.at(me) : Map(csh)});
      //bslices.insert({me, csh == bsh ? cslices.at(me) : Map(bsh)});
    }
    Map islice = islices.at(me);
    Map rslice = rslices.at(me);
    Map cslice = cslices.at(me);
    Map bslice = bslices.at(me);
    unlock();

    #define sliceTraining() { \
      if (ang!=0) \
        rotate(islice, rslice, ang, 0.0); \
      if (rsh != csh) \
        crop(rslice, cslice, crp); \
      if (csh != bsh) \
        binn(cslice, bslice, bnn); \
    }

    const pair<Path,int> & slpr = slicelist.at(idx);
    if (hdfs.count(slpr.first)) {
      HDFread & hdf = hdfs.at(slpr.first);
      if ( hdf.shape != sh )
        throw_error("Reading volume", "Missmatching image shape in " + hdf.id() + ".");
      for (int idxV=0 ; idxV<hdf.indices.size() ; idxV++ ) {
        hdf.read(idxV, islice);
        sliceTraining();
        storage(slpr.second+idxV, all, all) = bslice;
        bar.update();
      }
    } else {
      try {
        //lock();
        ReadImage(slpr.first, islice, sh);
        //unlock();
        sliceTraining();
        storage(slpr.second, all, all) = bslice;
      } catch (...) {
        warn("ReadVolume", "Slice " + toString(slpr.second) + " artificially filled with 0s.");
        storage(slpr.second, all, all) = 0;
      }
      bar.update();
    }

    #undef sliceTraining

    return true;

  }

};


void
ReadVolume(const std::deque<Path> & filelist, Volume & storage, bool verbose) {
  ReadVolInThread(filelist, storage, 0, Crop(), Binn(), verbose).execute();
}








struct _ReadVolBySlice  {

  deque<Path> ilist;
  unordered_map<string,HDFread> hdfs;
  size_t ssize;
  const float ang;
  const Crop crp;
  const Binn3 bnn;

  _ReadVolBySlice(float _angle, Crop _crop, Binn3 _binn)
    : ilist()
    , ssize(0)
    , ang(_angle)
    , crp(Crop())
    , bnn(Binn3())
  {}

  _ReadVolBySlice(const std::deque<Path> & filelist, float _angle, Crop _crop, Binn3 _binn)
    : ilist()
    , ssize(0)
    , ang(_angle)
    , crp(_crop)
    , bnn(_binn)
  {
    add(filelist);
  }


  void add(const Path & fileind) {
      ilist.push_back(fileind);
      try {
        hdfs.insert({fileind,HDFread(fileind)});
        ssize += hdfs.at(fileind).indices.size();
      } catch (...) {
        ssize++;
      }
  }


  void add(const std::deque<Path> & filelist) {
    for ( deque<Path>::const_iterator curI = filelist.begin() ; curI < filelist.end() ; curI++ )
      add(*curI);
  }


  bool read (long int idx, Map & out) {
    int cfirst=0;
    for (int cfl = 0 ; cfl < ilist.size() ; cfl++) {
      const Path flnm = ilist[cfl];
      if (hdfs.count(flnm)) {
        HDFread & hdf = hdfs.at(flnm);
        if (idx < cfirst + hdf.indices.size()) {
          hdf.read(idx-cfirst, out);
          return true;
        }
        cfirst += hdf.indices.size();
      } else {
        if (idx==cfirst){
          ReadImage(flnm, out);
          return true;
        }
        cfirst++;
      }
    }
    return false;
  }

};


ReadVolumeBySlice::ReadVolumeBySlice(const deque<Path> & filelist)
  : guts(new _ReadVolBySlice(filelist, 0, Crop(), Binn3()))
{}

ReadVolumeBySlice::ReadVolumeBySlice(const Path & file)
  : guts(new _ReadVolBySlice(deque<Path>(1, file), 0, Crop(), Binn3()))
{}

void ReadVolumeBySlice::add(const Path & fileind) {
  ((_ReadVolBySlice*) guts)->add(fileind);
}


void ReadVolumeBySlice::add(const std::deque<Path> & filelist) {
  ((_ReadVolBySlice*) guts)->add(filelist);
}



ReadVolumeBySlice::~ReadVolumeBySlice() {
  delete (_ReadVolBySlice*) guts;
}

void ReadVolumeBySlice::read(uint sl, Map& trg) {
  ((_ReadVolBySlice*) guts)->read(sl, trg);
}

size_t ReadVolumeBySlice::slices() const {
  return ((_ReadVolBySlice*) guts)->ssize;
}
















class StackWriter {

private:

  const size_t zsize;
  HDFwrite * hdfFile;
  string sliceformat;
  const float mincon;
  const float maxcon;

public:
  Path ofile;

public:

  StackWriter(const Path & filedesc, Shape _sh, size_t _zsize, float mmin, float mmax)
    : zsize(_zsize)
    , hdfFile(0)
    , mincon(mmin)
    , maxcon(mmax)
  {
    HDFdesc hdfdesc(filedesc);
    if ( hdfdesc.isValid() ) {
      hdfFile = new HDFwrite(filedesc, _sh, _zsize);
      ofile=hdfdesc.name;
    } else {
      if (zsize==1)
        sliceformat=filedesc;
      else {
        const Path outmask = string(filedesc).find('@') == string::npos
                           ? filedesc.dtitle() + "-@" + filedesc.ext()  :  string(filedesc) ;
        sliceformat = mask2format(outmask, zsize);
      }
      ofile=filedesc;
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


  size_t size() const {return zsize;}


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



SaveVolumeBySlice::SaveVolumeBySlice(const Path & filedesc, Shape _sh, size_t _zsize,
                                     float mmin, float mmax)
  : guts(new StackWriter(filedesc, _sh, _zsize, mmin, mmax))
{}

SaveVolumeBySlice::~SaveVolumeBySlice() {
  delete (StackWriter*) guts;
}

void SaveVolumeBySlice::save(uint sl, const Map& trg) {
  ((StackWriter*) guts)->put(sl, trg);
}

const Path SaveVolumeBySlice::savePath() const {
  return ((StackWriter*) guts)->ofile;
}

size_t SaveVolumeBySlice::slices() const {
  return ((StackWriter*) guts)->size();
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

  Map _storage(safe(storage));
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

  Map _storage(safe(storage));
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


  pthread_mutex_lock(&mut); // without it many fails.
  Map _storage(safe(storage));
  pthread_mutex_unlock(&mut);
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

  const int nof_args = storage.size();
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




void
LoadData ( const Path filename, Map & storage ) {

  ifstream infile(filename);
  string line;
  vector< vector<float> > data_read;
  while (getline(infile, line)) {
    line = line.substr(0, line.find('#'));
    istringstream iss(line);
    vector<float> nums;
    while (! iss.eof()) {
      string wrd;
      iss >> wrd;
      float num;
      if (stringstream(wrd) >> num)
        nums.push_back(num);
    }
    if ( data_read.size()  &&  nums.size()  &&  data_read[0].size() != nums.size() )
      throw_error("read data", "Line "+toString(data_read.size()+1)+" of data file \"" + filename + "\" "
                             + " contains number of elements different from the first striong.");
    if (nums.size())
      data_read.push_back(nums);
  }

  const Shape data_shape(data_read.size(), data_read.size() ? data_read[0].size() : 0);
  storage.resize(data_shape);
  for ( ArrIndex y=0 ; y < data_shape(0) ; y++)
    for ( ArrIndex x=0 ; x < data_shape(1) ; x++)
      storage(y,x) = data_read[y][x];

}


