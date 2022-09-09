#include "external.world.h"
#include "parallel.world.h"

#include <tiffio.h>
#include <hdf5.h>
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

  //InitializeMagick(0);
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
  TIFFSetErrorHandler(0);
  TIFFSetErrorHandlerExt(0);
  H5Eset_auto(H5E_DEFAULT, NULL, NULL);
  //H5::Exception::dontPrint();

  return true;

}

static const bool imageIOinited = initImageIO();


/*
Path imageFile(const std::string & filedesc) {
  return filedesc.substr(0 , filedesc.find(":"));
}
*/







struct HDFdesc {

  ImagePath name;
  string data;
  string slicesStr;
  int sliceDim;

  HDFdesc(const ImagePath & filedesc) {
    const string desc = trim(filedesc.desc(), ":");
    if (desc.empty()) // not HDF5
      return;
    name = filedesc;
    deque<string> hdfRd = split(desc, ":");
    data=hdfRd[0];
    slicesStr  =  hdfRd.size() < 2  ||  hdfRd[1].empty()   ?   "Z"  :  hdfRd[1] ;
    switch ( slicesStr.at(0) ) {
      case 'x':
      case 'X':
        sliceDim=2;  slicesStr.erase(0,1); break;
      case 'y':
      case 'Y':
        sliceDim=1;  slicesStr.erase(0,1); break;
      case 'z':
      case 'Z':
        sliceDim=0;  slicesStr.erase(0,1); break;
      default:
        sliceDim=0;
    }
  }

  string id() const {
    //return name + ":" + data + ":" + (sliceDim == 0 ? "z" : (sliceDim == 1 ? "y" : "x")) ;
    return name.repr();
  }

  inline bool isValid() const { return name.length() ; }
  inline bool isValidHDF() const { return isValid() && ( H5Fis_hdf5(name.c_str()) > 0 ); }
  static bool isValid(const ImagePath & filedesc) { return HDFdesc(filedesc).isValid(); }
  static bool isValidHDF(const ImagePath & filedesc) { return HDFdesc(filedesc).isValidHDF(); }

};


struct HDFrw : public HDFdesc {

public :

  inline HDFrw(const ImagePath & filedesc)
    : HDFdesc(filedesc)
    , iosh(0,0,0)
    , hdfFile(0)
    , file_fapl(0)
    , dataset(0)
    , dataset_dxpl(0)
    , filespace(0)
    //, rwLock(PTHREAD_MUTEX_INITIALIZER)
  {
    if (!isValid())
      throw CtasErr(CtasErr::ERR, "HDF i/o", "Not an HDF in \""+filedesc.repr()+"\".");
  }

  ~HDFrw() {
    complete();
    if (dataset_dxpl > 0)
      H5Pclose(dataset_dxpl);
    dataset_dxpl=0;
    if (file_fapl > 0)
      H5Pclose(file_fapl);
    file_fapl=0;
  }


private:
  Shape3 iosh;

public:
  inline Shape face() const {return sliceDim == 2
                    ?  Shape(iosh(2),iosh(1)) : Shape(iosh(1),iosh(2));}
  inline Shape ioface() const {return Shape(iosh(1),iosh(2));}
  inline size_t slices() const {return iosh(0);}
  //inline Shape3 sizes() const {return _shape);}


protected :

  int rank;
  hid_t hdfFile;
  hid_t file_fapl;
  hid_t dataset;
  hid_t dataset_dxpl;
  hid_t filespace;
  //pthread_mutex_t rwLock;
  blitz::Array<hsize_t, 1> cnts;

  void setFace(const Shape & fcsh) {
    iosh[1] = fcsh(sliceDim == 2 ? 1 : 0);
    iosh[2] = fcsh(sliceDim == 2 ? 0 : 1);
    const size_t ar = area(fcsh);
    if ( ! ar )
      return;
    if (  (dataset_dxpl = H5Pcreate(H5P_DATASET_XFER)) == H5I_INVALID_HID
       || H5Pset_buffer(dataset_dxpl, ar*sizeof(float), 0, 0) < 0 )
      dataset_dxpl=H5P_DEFAULT;
    size_t bsize;
    switch (sliceDim) {
      case 0: bsize = ar; break;
      case 1: bsize = iosh[1] ; break;
      case 2: bsize = 1; break;
    }
    if (  (file_fapl = H5Pcreate(H5P_FILE_ACCESS)) == H5I_INVALID_HID
       || H5Pset_sieve_buf_size(file_fapl, ar*sizeof(float)) < 0
       || H5Pset_cache(file_fapl, 0, 1, ar*sizeof(float), 1) < 0 )
      file_fapl = H5P_DEFAULT;
  }

  inline void setSlices(size_t _slices) {
    iosh[0]=_slices;
  }

  void complete() {
    if (filespace>0)
      H5Sclose(filespace);
    filespace=0;
    if (dataset>0)
      H5Dclose(dataset);
    dataset=0;
    if (hdfFile>0)
      H5Fclose(hdfFile);
    hdfFile=0;
  }

  hid_t local_fs(int idx, const Crop & crp = Crop()) {
    hid_t ret_fs=H5Scopy(filespace);
    if (ret_fs<0)
      throw_error("HDF i/o", "Failed to copy filespace accessing slice " +toString(idx)+ " from " + id() + ".");
    hsize_t offs[rank], mcns[rank];
    unsigned cdim = sliceDim == 2 ? 1 : 0;
    for (int dim=0; dim<rank ; dim++) {
      if (rank==3 && dim == sliceDim) {
        offs[dim]=idx;
        mcns[dim]=1;
      } else {
        offs[dim]=crp(cdim,0);
        mcns[dim]=cnts(dim)-crp(cdim,1);
        cdim += sliceDim == 2 ? -1 : 1;
      }
    }
    if ( H5Sselect_hyperslab(ret_fs, H5S_SELECT_SET, offs, NULL, mcns, NULL) < 0) {
      H5Sclose(ret_fs);
      throw_error("HDF i/o", "Failed to select hyperslab accessing slice " +toString(idx)+ " from " + id() + ".");
    }
    return ret_fs;
  }

};




struct HDFread : public HDFrw {

private :

  const static string modname;
  mutable vector<int> indices;

public :

  HDFread(const ImagePath & filedesc)
    : HDFrw(filedesc)
  {
    if (!isValidHDF())
      throw_error(modname, "No HDF file at "+id()+".");
    const Shape sh = ImageSizes(filedesc);
    setFace(sh);

#ifdef H5F_ACC_SWMR_READ
    hdfFile = H5Fopen(name.c_str(), H5F_ACC_RDONLY | H5F_ACC_SWMR_READ, H5P_DEFAULT);
#else
    hdfFile = H5Fopen(name.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
#endif
    if (  hdfFile<=0
       || (dataset = H5Dopen(hdfFile, data.c_str(), H5P_DEFAULT))<0
       || (filespace = H5Dget_space(dataset))<0
       || (rank = H5Sget_simple_extent_ndims(filespace))<0 ) {
      complete();
      throw_error(modname, "Failed to open data " + id() + " for reading.");
    }
    if (rank!=2 && rank !=3) {
      complete();
      throw_error(modname, "Dataset is not 2D or 3D in " + id());
    }
    cnts.resize(rank);
    if ( H5Sget_simple_extent_dims(filespace, cnts.data(), 0) != rank ) {
      complete();
      throw_error(modname, "Failed to read dataset size in " + id());
    }
    Shape chsh;
    if ( rank == 2 )
      chsh = Shape(cnts(0), cnts(1));
    else {
      int idx=0, odx=0;
      while (idx<rank) {
        if (idx != sliceDim)
          chsh(odx++) = cnts(idx);
        idx++;
      }
    }
    if (sliceDim==2) // need to transpose what I read in YZ plane
      chsh = Shape(chsh(1),chsh(0));
    if (chsh != sh)
      throw_error(modname, "Inconsistent read from file " + id());
    if ( rank == 2 )
      indices.push_back(0);
    else if ( rank == 3 )
      indices = slice_str2vec(slicesStr, cnts(sliceDim));
    setSlices(indices.size());
  }


  void read(uint idx, Map & storage, const Crop & crp = Crop()) {
    if ( ! hdfFile )
      throw_error(modname, "File " + name + " was previously closed.");
    if ( idx >= indices.size() )
      throw_error(modname, "Index is beyond slices to read from " + name + ".");

    hid_t lfillespace=local_fs(indices.at(idx), crp);
    storage.resize(crop(face(),crp));
    Map rd;
    if (sliceDim==2)
      rd.resize(storage.transpose(blitz::secondDim, blitz::firstDim).shape()); // must be swap of face
    else
      rd.reference(safe(storage, false));

    hsize_t mcnts[2] = {hsize_t(rd.shape()(0)), hsize_t(rd.shape()(1))},
            moffs[2] = {0, 0};
    hid_t memspace = H5Screate_simple(2, mcnts, 0);
    if (  memspace < 0
       || H5Sselect_hyperslab(memspace, H5S_SELECT_SET, moffs, NULL, mcnts, NULL) < 0 )
      throw_error("HDF i/o", "Failed to prepare memory space to access file " + name + ".");

    //pthread_mutex_lock(&rwLock);
    if ( H5Dread(dataset, H5T_NATIVE_FLOAT, memspace, lfillespace, H5P_DEFAULT, rd.data()) < 0)
      warn(modname, "Failed to read slice " +toString(idx)+ " from " + name + ".");
    //pthread_mutex_unlock(&rwLock);
    H5Sclose(memspace);

    if (sliceDim==2)
      rd.transposeSelf(blitz::secondDim, blitz::firstDim);
    if ( rd.data() != storage.data() )
      storage = rd;
    H5Sclose(lfillespace);
  }

};

const string HDFread::modname = "HDF read";





struct HDFwrite : public HDFrw {

private:

  const static string modname;
  const float mincon;
  const float maxcon;
  hid_t memspace;


  void createNewGroup() {
    hid_t lcpl=0, dcpl=0;
    if (  hdfFile<=0
       || (lcpl = H5Pcreate(H5P_LINK_CREATE)) == H5I_INVALID_HID
       || H5Pset_create_intermediate_group(lcpl, 1) < 0
       || H5Pset_char_encoding(lcpl, H5T_CSET_UTF8) < 0
       || (dcpl = H5Pcreate(H5P_DATASET_CREATE)) == H5I_INVALID_HID
       || H5Pset_fill_time(dcpl, H5D_FILL_TIME_NEVER) < 0
       || (filespace = H5Screate_simple(3, cnts.data(), NULL)) < 0
       || (dataset = H5Dcreate(hdfFile, data.c_str(), H5T_NATIVE_FLOAT, filespace, lcpl, dcpl, H5P_DEFAULT)) == H5I_INVALID_HID
       ) {
      complete();
      if (lcpl)
        H5Pclose(lcpl);
      if (dcpl)
        H5Pclose(dcpl);
    }
  }


public :

  HDFwrite( const ImagePath & filedesc, Shape _sh, const size_t zsize
          , float _mincon=0, float _maxcon=0)
    : HDFrw(filedesc)
    , mincon(_mincon)
    , maxcon(_maxcon)
  {
    rank = 3;
    setFace(_sh);
    setSlices(zsize);
    if ( ! zsize * area(face()) )
      throw warn(modname, "Zerro size to write.");

    cnts.resize(3);
    int idx=0, odx=0;
    while (idx<3) {
      if (idx != sliceDim)
        cnts(idx) = ioface()(odx++);
      idx++;
    }
    cnts(sliceDim) = zsize; // first will be used once as the 3D dimensions
    blitz::Array<hsize_t,1> tcnts(3);
#ifdef H5F_ACC_SWMR_WRITE
    hdfFile = H5Fopen(name.c_str(), H5F_ACC_RDWR | H5F_ACC_SWMR_WRITE, H5P_DEFAULT);
#else
    hdfFile = H5Fopen(name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
#endif
    if (hdfFile<=0) {
      complete();
    } else if ((dataset = H5Dopen(hdfFile, data.c_str(), H5P_DEFAULT))<=0) {
      createNewGroup();
    } else if (  (filespace = H5Dget_space(dataset)) <= 0
              || H5Sget_simple_extent_ndims(filespace) != 3
              || H5Sget_simple_extent_dims(filespace, tcnts.data(), 0) != rank ) {
        complete();
    } else {
        for (int idx=0 ; idx<3 ; idx++)
          if (idx != sliceDim  &&  ioface()(idx) != tcnts(idx))
            complete();
        if (zsize > tcnts(sliceDim))
          complete();
    }
    if (hdfFile<=0) {
#ifdef H5F_ACC_SWMR_WRITE
      hdfFile = H5Fcreate(name.c_str(), H5F_ACC_SWMR_WRITE | H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
#else
      hdfFile = H5Fcreate(name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
#endif
      if (hdfFile>0)
        createNewGroup();
    }
    if (hdfFile<=0) {
      complete();
      throw_error(modname, "Failed to open HDF5 file " + name + " for writing.");
    }

    hsize_t mcnts[2] = { hsize_t(ioface()(0)), hsize_t(ioface()(1))},
            moffs[2] = {0, 0};
    if ( (memspace = H5Screate_simple(2, mcnts, 0))<0
       || H5Sselect_hyperslab(memspace, H5S_SELECT_SET, moffs, NULL, mcnts, NULL) < 0 ) {
      complete();
      throw_error("HDF i/o", "Failed to prepare memory space to access file " + name + ".");
    }

  }


  ~HDFwrite() {
    if (memspace)
      H5Sclose(memspace);
  }

  void write(int idx, const Map & storage) {
    if ( hdfFile <= 0 )
      throw_error(modname, "File \"" + id() + "\" was previously closed and no more write possible.");
    if ( idx >= slices() )
      throw_error(modname, "Index " + toString(idx) + " to write is beyond initially requested size"
                        + toString(slices()) + ". Write is ignored.");
    if ( storage.shape() != face() )
      throw_error(modname, "Shape of the slice to write " + toString(storage.shape()) + " is different from"
                        " initially requested shape" + toString(face()) + ".");

    hid_t lfillespace=local_fs(idx);
    Map wr;
    if (sliceDim==2) {
      Map tmap(storage);
      wr.resize(ioface());
      wr = tmap.transpose(blitz::secondDim, blitz::firstDim);
    } else {
      wr.reference(safe(storage));
    }

    //pthread_mutex_lock(&rwLock);
    if ( H5Dwrite(dataset, H5T_NATIVE_FLOAT, memspace, lfillespace, H5P_DEFAULT, wr.data()) < 0)
      warn(modname, "Failed to write slice " +toString(idx)+ " to " + name + ".");
    //pthread_mutex_unlock(&rwLock);

    H5Sclose(lfillespace);

  }

};

const string HDFwrite::modname = "HDF write";













float
PixelSize(const ImagePath & filename) {
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
ImageSizes_HDF5(const ImagePath & filename){
  const string modname="HDF size";
  HDFdesc me(filename);
  if (!me.isValidHDF())
    throw CtasErr(CtasErr::ERR, modname, "Not an HDF in \""+filename.repr()+"\".");

  hid_t hdfFile(0)
      , dataset(0)
      , filespace(0);
  int rank(0);
  #define cleanup() \
    if (filespace>0) H5Sclose(filespace); \
    if (dataset>0) H5Dclose(dataset); \
    if (hdfFile>0) H5Fclose(hdfFile);

  if (  (hdfFile = H5Fopen(me.name.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT)) <0
     || (dataset = H5Dopen(hdfFile, me.data.c_str(), H5P_DEFAULT))<0
     || (filespace = H5Dget_space(dataset))<0
     || (rank = H5Sget_simple_extent_ndims(filespace))<0
     ) {
    cleanup();
    throw_error(modname, "Failed to open data " + filename.repr() + " for reading.");
  }
  if (rank!=2 && rank !=3) {
    cleanup();
    throw_error(modname, "Dataset is not 2D or 3D in " + filename.repr());
  }
  blitz::Array<hsize_t, 1> cnts(rank);
  if ( H5Sget_simple_extent_dims(filespace, cnts.data(), 0) != rank ) {
    cleanup();
    throw_error(modname, "Failed to read dataset size in " + filename.repr());
  }
  Shape ret;
  if ( rank == 2 )
    ret = Shape(cnts(0), cnts(1));
  else {
    int idx=0, odx=0;
    while (idx<rank) {
      if (idx != me.sliceDim)
        ret(odx++) = cnts(idx);
      idx++;
    }
  }
  if (me.sliceDim==2) // need to transpose what I read in YZ plane
    ret = Shape(ret(1),ret(0));
  cleanup();
  return ret;
  #undef cleanup
}


Shape
ImageSizes(const ImagePath & filename){
  try {
    return ImageSizes_HDF5(filename);
  } catch (CtasErr err) {
    Magick::Image imag;
    try {
      imag.ping(filename);
    } catch ( Magick::WarningCoder err ) {
    } catch ( Magick::Exception & error) {
      throw_error("get image size", "Could not read image file \""+filename+"\"."
                          " Caught Magick++ exception: \""+error.what()+"\".");
    } catch (...) {
      throw_error("get image size", "Could not read image file \""+filename+"\".");
    }
    return Shape( imag.rows(), imag.columns() );
  }
}




void
BadShape(const ImagePath & filename, const Shape & shp){
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
ReadImage_HDF5 (const Path & filedesc, Map & storage, const Crop & crp = Crop() ) {
  try {
    HDFread(filedesc).read(0,storage,crp);
  } catch( ... ) {
    throw CtasErr(CtasErr::ERR, "HDF read", "Failed to read HDF5 from " + filedesc);
  }
}




pthread_mutex_t tiffmut(PTHREAD_MUTEX_INITIALIZER);

static inline void
safelyCloseTIFFandThrow(TIFF *image, int fd, const string modname, const string report = string()) {
  pthread_mutex_lock(&tiffmut);
  if (image) TIFFClose(image);
  pthread_mutex_unlock(&tiffmut);
  if (fd>0) close(fd);
  if ( report.size() )
    throw warn(modname, report);
  return;
}

static inline void
safelyOpenTIFForThrow(TIFF **image, int *fd, const string modname, const Path & filename, bool rdonly) {
    // BUG in libtiff
    // On platforms (f.e. CentOS) the TIFFOpen function fails,
    // while TIFFFdOpen works well. On the MS Windows the
    // TIFFFdOpen does not work, while TIFFOpen does.
  pthread_mutex_lock(&tiffmut);
  #ifdef _WIN32
  *image = TIFFOpen(filename.c_str(), rdonly ? "r" : "w");
  #else
  *fd = rdonly
      ?  open (filename.c_str(), O_RDONLY)
      :  open (filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (*fd > 0)
    *image = TIFFFdOpen(*fd, filename.c_str(), rdonly ? "r" : "w");
  #endif
  pthread_mutex_unlock(&tiffmut);
  if (! *image)
    safelyCloseTIFFandThrow(*image, *fd, modname,
      "Failed to open TIFF file \"" + filename + "\" for " + (rdonly ? "reading" : "writing") + ".");
}



/// Loads an image (lines) using TIFF library.
///
/// @param filename Name of the image
/// @param storage The array to store the image.
/// @param idxs The indexes of the line to read.
///        if empty then reads whole image.
///

static void
ReadImage_TIFF (const Path & filename, Map & storage, const Crop & crp = Crop()) {

  const string modname = "load image tiff";

  int fd=0;
  TIFF *image = 0;
  safelyOpenTIFForThrow(&image, &fd, modname, filename, true);
  uint32_t width = 0, height = 0;
  uint16_t spp = 1, bps = 0, fmt = 0, photo;
  if (    ! TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width)
       || ! TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height)
       || ( TIFFGetField(image, TIFFTAG_SAMPLESPERPIXEL, &spp) && spp != 1 )
       || ! TIFFGetField(image, TIFFTAG_BITSPERSAMPLE, &bps)
       || ( bps != 8 && bps != 16 && bps != 32 )
       || ! TIFFGetField(image, TIFFTAG_PHOTOMETRIC, &photo)
       || photo != PHOTOMETRIC_MINISBLACK )
    safelyCloseTIFFandThrow(image, fd, modname,
      "Lost in the fields of tif image \"" + filename + "\".");
  if ( ! TIFFGetField(image, TIFFTAG_SAMPLEFORMAT, &fmt) )
    fmt =  bps != 32  ?  SAMPLEFORMAT_UINT  :  SAMPLEFORMAT_IEEEFP;
  if ( fmt != SAMPLEFORMAT_UINT &&
       fmt != SAMPLEFORMAT_INT &&
       fmt != SAMPLEFORMAT_IEEEFP )
    safelyCloseTIFFandThrow(image, fd, modname,
      "Image \"" + filename + "\" has unsupported sample format.");

  const Shape osh = crop(Shape(height,width),crp);
  storage.resize(osh);
  tdata_t buf = _TIFFmalloc(TIFFScanlineSize(image));
  for (uint curidx = crp.top; curidx < height - crp.bottom; curidx++) {

    //pthread_mutex_lock(&mut);
    if ( TIFFReadScanline(image, buf, curidx) < 0 ) {
      _TIFFfree(buf);
      //pthread_mutex_unlock(&mut);
      safelyCloseTIFFandThrow(image, fd, modname,
        "Failed to read line " + toString(curidx) +" in image \"" + filename + "\".");
    }
    Line ln(storage(curidx-crp.top, all));
    //pthread_mutex_unlock(&mut);

    #define blitzArrayFromData(type) \
      blitz::Array<type,1> ( ((type*)buf) + crp.left, blitz::shape(osh(1)), blitz::neverDeleteData)
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
    #undef blitzArrayFromData

  }

  //pthread_mutex_lock(&mut);
  //ln.reference(Line(0));
  _TIFFfree(buf);
  //pthread_mutex_unlock(&mut);
  safelyCloseTIFFandThrow(image, fd, modname, "");



}




/// Loads an image using ImageMagick library.
///
/// @param filename Name of the image
/// @param storage The array to store the image.
///
static void
ReadImage_IM (const Path & filename, Map & storage, const Crop & crp = Crop() ){

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
  const Shape osh = crop(Shape(hight,width),crp);
  storage.resize(osh);

  // below might be buggy - see notes in SaveImageINT_IM
  //const Magick::PixelPacket
  //  * pixels = imag.getConstPixels(0,0,width,hight);
  //float * data = storage.data();
  //for ( int k = 0 ; k < hight*width ; k++ )
  //  *data++ = (float) Magick::ColorGray( *pixels++  ) .shade();
  // Replacement for the buggy block:
  for (ArrIndex curh = crp.top ; curh < hight-crp.bottom ; curh++)
    for (ArrIndex curw = crp.left ; curw < width-crp.right ; curw++)
      storage(curh-crp.top, curw-crp.left) = Magick::ColorGray(imag.pixelColor(curw, curh)).shade();
  // end replacement *

}



void
ReadImage(const ImagePath & filename, Map & storage, const Crop & crp, const Shape & shp){
  try {
    const string ext = lower(filename.ext());
    if (area(shp))
      BadShape(filename, shp);
    if (HDFdesc::isValidHDF(filename))
      ReadImage_HDF5(filename, storage, crp);
    else if ( ext == ".tif" || ext == ".tiff")
      ReadImage_TIFF(filename, storage, crp);
    else
      ReadImage_IM(filename, storage, crp);
  } catch (...) {
    throw_error("Read image", "Failed to read image " + filename + ".");
  }
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








struct _ReadVolBySlice  {

  deque<ImagePath> ilist;
  // I have observed situation where using ImagePath as the key could not
  // descriminate between different keys despite producing different hash.
  //unordered_map<ImagePath,HDFread> hdfs;
  unordered_map<size_t,HDFread> hdfs;
  size_t ssize;
  Shape face;
  const float ang;
  const Crop crp;
  const Binn bnn;


  _ReadVolBySlice(float _angle, Crop _crop, Binn _binn)
    : ilist()
    , ssize(0)
    , face()
    , ang(_angle)
    , crp(Crop())
    , bnn(Binn())
  {}


  _ReadVolBySlice(const std::deque<ImagePath> & filelist, float _angle, Crop _crop, Binn _binn)
    : ilist()
    , ssize(0)
    , face()
    , ang(_angle)
    , crp(_crop)
    , bnn(_binn)
  {
    add(filelist);
  }


  void add(const ImagePath & fileind) {
      if (!ilist.size())
        face = ImageSizes(fileind);
      ilist.push_back(fileind);
      try {
        //hdfs.emplace(fileind, fileind);
        const size_t key = hash<string>{}(fileind.repr());
        hdfs.emplace(key, fileind);
        //ssize += hdfs.at(fileind).slices();
        ssize += hdfs.at(key).slices();
      } catch (...) {
        ssize++;
      }
  }


  void add(const std::deque<ImagePath> & filelist) {
    for ( deque<ImagePath>::const_iterator curI = filelist.begin() ; curI < filelist.end() ; curI++ )
      add(*curI);
  }


  void sliceTraining(const Map & in, Map & rslice, Map & cslice, Map & out) {
    if (ang!=0)
      rotate(in, rslice, ang, 0.0);
    else if (rslice.data()!=in.data())
      rslice.reference(in);
    if (crp != Crop())
      crop(rslice, cslice, crp);
    else if (cslice.data()!=rslice.data())
      cslice.reference(rslice);
    if (bnn != Binn())
      binn(cslice, out, bnn);
    else if (out.data()!=cslice.data())
      out.reference(cslice);
  }


  bool read (long int idx, Map & rd, Map & rslice, Map & cslice, Map & out) {
    int cfirst=0;
    for (int cfl = 0 ; cfl < ilist.size() ; cfl++) {
      const ImagePath flnm = ilist[cfl];
      //if (hdfs.count(flnm)) {
      const size_t key = hash<string>{}(flnm.repr());
      if (hdfs.count(key)) {
        HDFread & hdf = hdfs.at(key);
        if (idx < cfirst + hdf.slices()) {
          hdf.read(idx-cfirst, rd);
          sliceTraining(rd, rslice, cslice, out);
          return true;
        }
        cfirst += hdf.slices();
      } else {
        if (idx==cfirst){
          ReadImage(flnm, rd);
          sliceTraining(rd, rslice, cslice, out);
          return true;
        }
        cfirst++;
      }
    }
    return false;
  }


  bool read (long int idx, Map & out) {
    Map rd, rslice, cslice;
    return read(idx, rd, rslice, cslice, out);
  }


  size_t size() const {return ssize;}

};




class ReadVolInThread : public InThread {

  Volume & storage;
  _ReadVolBySlice reader;
  Shape sh;
  pthread_mutex_t proglock;
  unordered_map< pthread_t, deque<Map> > slices;

public:

  ReadVolInThread(const std::deque<ImagePath> & filelist, Volume & _storage,
                  float _angle, Crop _crop, Binn _binn, bool verbose=false)
    : InThread(verbose , "reading volume")
    , storage(_storage)
    , reader(filelist, _angle, _crop, _binn)
  {
    if ( ! filelist.size() ) {
      storage.free();
      return;
    }
    sh = ImageSizes(filelist[0]);
    sh = rotate(sh, _angle);
    sh = crop(sh, _crop);
    sh = binn(sh, _binn);
    bar.setSteps(reader.size());
    storage.resize(reader.size(), sh(0), sh(1));
    if ( ! storage.size() )
      return;
  }


  bool inThread (long int idx) {

    if ( idx >= reader.size() )
      return false;

    pthread_t me = pthread_self();
    lock();
    if ( ! slices.count(me) )
      slices.emplace(me, deque<Map>(4));
    deque<Map> & myslices = slices.at(me);
    unlock();
    Map islice = myslices[0];
    Map rslice = myslices[1];
    Map cslice = myslices[2];
    Map bslice = myslices[3];

    const bool ret = reader.read(idx, islice, rslice, cslice, bslice);
    if ( bslice.shape() != sh )
      throw_error("Reading volume", "Missmatching image shape.");
    storage(idx,all,all) = bslice;
    bar.update();
    return true;

  }

};


void
ReadVolume(const std::deque<ImagePath> & filelist, Volume & storage, bool verbose) {
  ReadVolInThread(filelist, storage, 0, Crop(), Binn(), verbose).execute();
}







ReadVolumeBySlice::ReadVolumeBySlice(const deque<ImagePath> & filelist)
  : guts(new _ReadVolBySlice(filelist, 0, Crop(), Binn()))
{}


void ReadVolumeBySlice::add(const ImagePath & fileind) {
  ((_ReadVolBySlice*) guts)->add(fileind);
}


void ReadVolumeBySlice::add(const std::deque<ImagePath> & filelist) {
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


Shape ReadVolumeBySlice::face() const {
  return ((_ReadVolBySlice*) guts)->face;
}















class _SaveVolumeBySlice {

private:

  const size_t zsize;
  HDFwrite * hdfFile;
  string sliceformat;
  const float mincon;
  const float maxcon;

public:

  _SaveVolumeBySlice(const ImagePath & filedesc, Shape _sh, size_t _zsize, float mmin, float mmax)
    : zsize(_zsize)
    , hdfFile(0)
    , mincon(mmin)
    , maxcon(mmax)
  {
    if ( HDFdesc::isValid(filedesc) )
      hdfFile = new HDFwrite(filedesc, _sh, _zsize, mincon, maxcon);
    else if ( zsize==1 )
      sliceformat=filedesc;
    else if ( filedesc.find('@') == string::npos )
      sliceformat = mask2format(filedesc.dtitle() + "-@" + filedesc.ext(), zsize);
    else
      sliceformat = mask2format(filedesc, zsize);
  }

  ~_SaveVolumeBySlice() {
    complete();
  }

  void complete() {
    if (hdfFile)
      delete hdfFile;
    hdfFile=0;
  }

  void save(int idx, const Map & storage) {
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
      const ImagePath fileName =  zsize == 1  ?  sliceformat : toString(sliceformat, idx);
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
  _SaveVolumeBySlice writer;
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
      maps.emplace(me, ssh);
    Map & cur = maps.at(me) ;
    unlock();

    const int idi = indices[idx];
    switch ( sliceDim ) {
      case 2: cur = vol(all, all, idi); break;
      case 1: cur = vol(all, idi, all); break;
      case 0: cur = vol(idi, all, all); break;
    }
    writer.save(idi, cur);
    bar.update();
    return true;

  }


public:

  SaveVolInThread(const ImagePath & filedesc, const Volume & _vol, bool verbose,
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

  static void execute(const ImagePath & filedesc, const Volume & _vol, bool verbose,
                      const std::string & slicedesc, float mmin, float mmax) {
    SaveVolInThread(filedesc, _vol, verbose, slicedesc, mmin, mmax)
        .InThread::execute();
  }

};


void SaveVolume(const ImagePath & filedesc, Volume & storage, bool verbose,
                const string & slicedesc, float mmin, float mmax) {
  SaveVolInThread::execute(filedesc, storage, verbose, slicedesc, mmin, mmax);
}



SaveVolumeBySlice::SaveVolumeBySlice(const ImagePath & filedesc, Shape _sh, size_t _zsize,
                                     float mmin, float mmax)
  : guts(new _SaveVolumeBySlice(filedesc, _sh, _zsize, mmin, mmax))
{}

SaveVolumeBySlice::~SaveVolumeBySlice() {
  delete (_SaveVolumeBySlice*) guts;
}

void SaveVolumeBySlice::save(uint sl, const Map& trg) {
  ((_SaveVolumeBySlice*) guts)->save(sl, trg);
}


size_t SaveVolumeBySlice::slices() const {
  return ((_SaveVolumeBySlice*) guts)->size();
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
SaveImageFP (const Path & filename, const Map & storage, uint attempts=3){

  static const string modname="save 32fp image";
  if (!attempts)
    throw_error(modname, "Failed to save image to file \"" + filename + "\" after all attempts.");
  if ( ! storage.size() ) {
    warn(modname, "Zero-sized array for image \"" + filename + "\".");
    return;
  }
  const int
    width = storage.columns(),
    hight = storage.rows();

  try {
    TIFF *image = 0;
    int fd=0;
    safelyOpenTIFForThrow(&image, &fd, modname, filename, false);
    if (   TIFFSetField(image, TIFFTAG_IMAGEWIDTH, width) != 1
        || TIFFSetField(image, TIFFTAG_IMAGELENGTH, hight) !=1
        || TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 32) != 1
        || TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1) != 1
        || TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, hight) != 1
        || TIFFSetField(image, TIFFTAG_SAMPLEFORMAT,SAMPLEFORMAT_IEEEFP) != 1
        || TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK) != 1 )
      safelyCloseTIFFandThrow(image, fd, modname, "Could not set a field in image \"" + filename + "\".");
    Map _storage(safe(storage));
    int wret = TIFFWriteRawStrip(image, 0, (void*) _storage.data(), width*hight*sizeof(float));
    safelyCloseTIFFandThrow(image, fd, modname,  (-1 != wret) ? "" : "Could not save image to file \"" + filename + "\"." );

  } catch (...) {
    return SaveImageFP(filename, storage, --attempts);
  }


}


void
SaveImage(const ImagePath & filename, const Map & storage, bool saveint){
  if (saveint) SaveImageINT(filename, storage);
  else SaveImageFP(filename, storage);
}

void
SaveImage(const ImagePath & filename, const Map & storage,
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


