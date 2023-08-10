#include "common.world.h"
#include "external.world.h"
#include "parallel.world.h"

#include <tiffio.h>
#include <hdf5.h>
#include <unordered_map>
#include <unistd.h>
#include <fstream>
#include <fcntl.h> // for the libc "open" function see bug description in the SaveImageFP function.
#include <sys/mman.h> // for mmap and related

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

  Magick::InitializeMagick(nullptr);
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
        sliceDim=blitz::thirdDim;  slicesStr.erase(0,1); break;
      case 'y':
      case 'Y':
        sliceDim=blitz::secondDim;  slicesStr.erase(0,1); break;
      case 'z':
      case 'Z':
        sliceDim=blitz::firstDim;  slicesStr.erase(0,1); break;
      default:
        sliceDim=blitz::firstDim;
    }
  }

  string id() const {
    return name.repr();
  }

  inline bool isValid() const { return name.length() ; }
  inline bool isValidHDF() const {
    return    isValid() && ( H5Fis_hdf5(name.c_str()) > 0 ) && ! data.empty()  ;
  }
  static bool isValid(const ImagePath & filedesc) { return HDFdesc(filedesc).isValid(); }
  static bool isValidHDF(const ImagePath & filedesc) { return HDFdesc(filedesc).isValidHDF(); }

};




bool ImagePath::isValidHDF() const {
  return HDFdesc::isValidHDF(*this);
}

static bool isValidHDF(const string & nm) {
  return HDFdesc::isValidHDF(nm);
}



struct HDFrw : public HDFdesc {

public :

  inline HDFrw(const ImagePath & filedesc)
    : HDFdesc(filedesc)
    , iosh(0l)
    , hdfFile(0)
    , file_fapl(0)
    , dataset(0)
    , dataset_dxpl(0)
    , filespace(0)
    , memspace(0)
    //, rwLock(PTHREAD_MUTEX_INITIALIZER)
  {
    if (!isValid())
      throw_error("HDF i/o", "Not an HDF in \""+filedesc.repr()+"\".");
  }


  #define closeMe(hid, al) \
    if (hid>0) { \
      H5##al ## close(hid);\
      hid = 0; \
    }

  void complete() {
    if (dataMmap.size()) {
      //float * datap = dataMmap.data();
      //munmap(datap, dataMmap.size()*sizeof(float) );
      // should be safe to leave data unmounted because dataMmap was the only
      // reference to the data pointer.
      dataMmap.reference(Volume());
    }
    closeMe(filespace, S);
    closeMe(dataset, D);
    closeMe(hdfFile, F);
  }

  ~HDFrw() {
    closeMe(dataset_dxpl, P);
    closeMe(file_fapl, P);
    closeMe(memspace, S);
    complete();
  }

  #undef closeMe

private:
  Shape<3> iosh;

public:
  inline Shape<2> face() const {return sliceDim == blitz::thirdDim
                    ?  Shape<2>(iosh(2),iosh(1)) : Shape<2>(iosh(1),iosh(2));}
  inline Shape<2> ioface() const {return Shape<2>(iosh(1),iosh(2));}
  inline size_t slices() const {return iosh(0);}
  //inline Shape3 sizes() const {return _shape);}

  template<class T> Shape<2> exclDim(const T & sh3, int dimension) {
    if (dimension>2)
      throw_error("face of volume", "Impossible excluded dimension " +toString(dimension)+ ".");
    Shape<2> ret;
    int idx=0, odx=0;
    while (idx<3) {
      if (idx != dimension)
        ret(odx++) = sh3(idx);
      idx++;
    }
    return ret;
  }


  template<class T> Shape<3> inclDim(const T & sh2, int dimension, long sz) {
    if (dimension>2)
      throw_error("shape volume", "Impossible included dimension " +toString(dimension)+ ".");
    Shape<3> ret;
    int idx=0, odx=0;
    while (idx<3) {
      ret(idx) = (idx == sliceDim) ? sz : sh2(odx++);
      idx++;
    }
    return ret;
  }

protected :

  int rank;
  hid_t hdfFile;
  hid_t file_fapl;
  hid_t dataset;
  hid_t dataset_dxpl;
  hid_t filespace;
  hid_t memspace;
  //pthread_mutex_t rwLock;
  blitz::Array<hsize_t, 1> cnts;
  mutable deque<int> indices;
  Volume dataMmap;

  void setFace(const Shape<2> & fcsh) {
    iosh[1] = fcsh(sliceDim == blitz::thirdDim ? 1 : 0);
    iosh[2] = fcsh(sliceDim == blitz::thirdDim ? 0 : 1);
    const size_t ar = size(fcsh);
    if ( ! ar )
      return;
    if (  (dataset_dxpl = H5Pcreate(H5P_DATASET_XFER)) == H5I_INVALID_HID
       || H5Pset_buffer(dataset_dxpl, ar*sizeof(float), 0, 0) < 0 )
      dataset_dxpl=H5P_DEFAULT;
    //size_t bsize;
    //switch (sliceDim) {
    //  case 0: bsize = ar; break;
    //  case 1: bsize = iosh[1] ; break;
    //  case 2: bsize = 1; break;
    //}
    if (  (file_fapl = H5Pcreate(H5P_FILE_ACCESS)) == H5I_INVALID_HID
       || H5Pset_sieve_buf_size(file_fapl, ar*sizeof(float)) < 0
       || H5Pset_cache(file_fapl, 0, 1, ar*sizeof(float), 1) < 0 )
      file_fapl = H5P_DEFAULT;
    hsize_t mcnts[2] = { hsize_t(ioface()(0)), hsize_t(ioface()(1))},
            moffs[2] = {0, 0};
    if ( (memspace = H5Screate_simple(2, mcnts, 0))<0
       || H5Sselect_hyperslab(memspace, H5S_SELECT_SET, moffs, NULL, mcnts, NULL) < 0 )
      throw_error("HDF i/o", "Failed to prepare memory space to access file " + name + ".");

  }

  inline void setSlices(size_t _slices) {
    iosh[0]=_slices;
  }




  hid_t local_fs(int idx, const Crop<2> & crp = Crop<2>()) {
    hid_t ret_fs=H5Scopy(filespace);
    if (ret_fs<0)
      throw_error("HDF i/o", "Failed to copy filespace accessing slice " +toString(idx)+ " from " + id() + ".");
    hsize_t offs[rank], mcns[rank];
    unsigned cdim = sliceDim == blitz::thirdDim ? 1 : 0;
    for (int dim=0; dim<rank ; dim++) {
      if (rank==3 && dim == sliceDim) {
        offs[dim]=idx;
        mcns[dim]=1;
      } else {
        offs[dim]=crp(cdim).begin();
        mcns[dim]=crp(cdim).size(cnts(dim));
        cdim += sliceDim == blitz::thirdDim ? -1 : 1;
      }
    }
    if ( H5Sselect_hyperslab(ret_fs, H5S_SELECT_SET, offs, NULL, mcns, NULL) < 0) {
      H5Sclose(ret_fs);
      throw_error("HDF i/o", "Failed to select hyperslab accessing slice " +toString(idx)+ " from " + id() + ".");
    }
    return ret_fs;
  }

  void mmapMeIfYouCan(bool writable) {

    if ( !hdfFile || !dataset || !file_fapl || dataMmap.size())
      return;

    string name_realpath( [&](){
      char * rp_name = realpath(name.c_str(),0);
      string toRet( rp_name ? rp_name : "" );
      free(rp_name);
      return toRet;
    }() );

    { // check if given path is in the list of mmap'able paths or in memory
      stringstream mmappath( [&](){
        const char * val = getenv("CTAS_MMAP_PATH") ;
        return string( val ? val : "" );
      }() );
      bool inMmapList=false;
      string tmp;
      while( ! inMmapList  &&  getline(mmappath, tmp, ':') )
        if ( size(tmp) && name_realpath.rfind(tmp, 0) == 0 )
          inMmapList=true;
      if ( ! inMmapList && name_realpath.rfind("/dev/shm/", 0) != 0 )
        return;
    }

    void * handle;
    if ( 0 > H5Fget_vfd_handle(hdfFile, file_fapl, & handle) )
      return;
    int fd = *((int*)handle);
    haddr_t offset = H5Dget_offset(dataset);
    if (offset<=0)
      return;
    hsize_t fsize;
    if ( 0 > H5Fget_filesize(hdfFile, &fsize) )
      return;
    if ( fsize - offset < product(cnts) * sizeof(float) && writable )  {
      // try to allocate file space by writing last element of the dataspace.
      if ( H5Dflush(dataset) < 0 ||
           H5Fflush(hdfFile, H5F_SCOPE_GLOBAL) < 0 )
        return;
      hid_t fs=H5Scopy(filespace);
      if (fs<=0)
        return;
      hsize_t offs[rank], mcns[rank];
      for (int dim=0; dim<rank ; dim++) {
        offs[dim] = cnts(dim)-1;
        mcns[dim] = 1;
      }
      if ( H5Sselect_hyperslab(fs, H5S_SELECT_SET, offs, NULL, mcns, NULL) < 0 ) {
        H5Sclose(fs);
        return;
      }
      hsize_t singleC = 1, singleO = 0;
      hid_t ms = H5Screate_simple(1, &singleC, 0);
      if (ms<=0)
        return;
      if ( H5Sselect_hyperslab(ms, H5S_SELECT_SET, &singleO, NULL, &singleC, NULL) < 0 ) {
        H5Sclose(ms);
        return;
      }
      float wrval(0);
      herr_t err = H5Dwrite(dataset, H5T_NATIVE_FLOAT, ms, fs, H5P_DEFAULT, &wrval);
      H5Sclose(fs);
      if ( err < 0 ||
           H5Dflush(dataset) < 0 ||
           H5Fflush(hdfFile, H5F_SCOPE_GLOBAL) < 0 )
        return;
      // size and offset must be updated
      if ( 0 >= (offset = H5Dget_offset(dataset)) ||
           0 > H5Fget_filesize(hdfFile, &fsize) )
        return;
    }
    if ( fsize - offset < product(cnts) * sizeof(float) )
      return;
    if ( ! H5Tequal(H5Dget_type(dataset), H5T_NATIVE_FLOAT) )
      return;
    hid_t dcpl;
    if ( (dcpl = H5Dget_create_plist(dataset)) == H5I_INVALID_HID )
      return;
    const H5D_layout_t layout = H5Pget_layout(dcpl); // must be H5D_CONTIGUOUS
    const int extfl = H5Pget_external_count(dcpl); // must be 0
    const int filters = H5Pget_nfilters(dcpl);
    H5Pclose(dcpl);
    if ( ( layout != H5D_CONTIGUOUS && layout != H5D_COMPACT )
         || extfl || filters )
      return;

    float * datap = (float*) ( offset + (char*)
      mmap(0, fsize, writable ? PROT_WRITE|PROT_READ : PROT_READ, MAP_SHARED, fd, 0) );
    if ( ! datap || datap == MAP_FAILED )
      return;
    const Shape<3> datash = cnts.size()==3
      ? Shape<3>(cnts(0),cnts(1),cnts(2))  :  Shape<3>(1,cnts(0),cnts(1));
    dataMmap.reference( Volume(datap, datash, blitz::neverDeleteData) );
    //prdn("MMAPED " + name);

  }

};




struct HDFread : public HDFrw {

private :

  const static string modname;

public :

  HDFread(const ImagePath & filedesc, bool overwrite=false)
    : HDFrw(filedesc)
  {
    if (!isValidHDF())
      throw_error(modname, "No HDF file at "+id()+".");
    const Shape<2> sh = ImageSizes(filedesc);
    setFace(sh);

    unsigned int h5openflags;
    if (overwrite) {
      //#ifdef H5F_ACC_SWMR_WRITE
      //h5openflags = H5F_ACC_RDWR | H5F_ACC_SWMR_WRITE;
      //#else
      h5openflags = H5F_ACC_RDWR;
      //#endif
    } else {
      #ifdef H5F_ACC_SWMR_READ
      h5openflags = H5F_ACC_RDONLY | H5F_ACC_SWMR_READ;
      #else
      h5openflags = H5F_ACC_RDONLY;
      #endif
    }
    hdfFile = H5Fopen(name.c_str(), h5openflags, H5P_DEFAULT);
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
    Shape<2> chsh =  rank == 2 ? Shape<2>(cnts(0), cnts(1))  :  exclDim(cnts, sliceDim);
    if (sliceDim==blitz::thirdDim) // need to transpose what I read in YZ plane
      chsh = Shape<2>(chsh(1),chsh(0));
    if (chsh != sh)
      throw_error(modname, "Inconsistent read from file " + id());
    //if (overwrite) {
      //#ifdef H5F_ACC_SWMR_WRITE
      //H5Fstart_swmr_write(hdfFile);
      //#endif
    //}
    if ( rank == 2 )
      indices.push_back(0);
    else if ( rank == 3 )
      indices = slice_str2vec(slicesStr, cnts(sliceDim));
    setSlices(indices.size());

    mmapMeIfYouCan(overwrite);

  }


  Map read(uint idx, Map & storage, const Crop<2> & crp = Crop<2>()) {
    if ( ! hdfFile )
      throw_error(modname, "File " + name + " was previously closed.");
    if ( idx >= indices.size() )
      throw_error(modname, "Index is beyond slices to read from " + name + ".");

    const ssize_t idxme = indices.at(idx);

    if (dataMmap.size()) {

      Map rd;
      switch (sliceDim) {
      case blitz::firstDim : rd.reference(dataMmap(idxme, all, all)); break;
      case blitz::secondDim: rd.reference(dataMmap(all, idxme, all)); break;
      case blitz::thirdDim : rd.reference(dataMmap(all, all, idxme)
                                          .transpose(blitz::secondDim, blitz::firstDim));
        break;
      default:
        throw_error(modname, "Bad slice dimension. Should never happen.");
      }
      return crp.apply(rd);

    } else {

      storage.resize(crp.shape(face()));
      Map rd;
      if (sliceDim==blitz::thirdDim)
        rd.resize(storage.transpose(blitz::secondDim, blitz::firstDim).shape()); // swap of face
      else
        rd.reference(safe(storage, false));

      hid_t lms;
      if (!crp && memspace)
        lms = memspace;
      else {
        hsize_t mcnts[2] = {hsize_t(rd.shape()(0)), hsize_t(rd.shape()(1))},
                moffs[2] = {0, 0};
        lms = H5Screate_simple(2, mcnts, 0);
        if (  lms < 0
           || H5Sselect_hyperslab(lms, H5S_SELECT_SET, moffs, NULL, mcnts, NULL) < 0 )
          throw_error(modname, "Failed to prepare memory space to access file " + name + ".");
      }
      hid_t lfs=local_fs(idxme, crp);
      //pthread_mutex_lock(&rwLock);
      if ( H5Dread(dataset, H5T_NATIVE_FLOAT, lms, lfs, H5P_DEFAULT, rd.data()) < 0)
        warn(modname, "Failed to read slice " +toString(idx)+ " from " + name + ".");
      //pthread_mutex_unlock(&rwLock);
      if (lms != memspace)
        H5Sclose(lms);
      H5Sclose(lfs);

      if (sliceDim==blitz::thirdDim)
        rd.transposeSelf(blitz::secondDim, blitz::firstDim);
      if ( rd.data() != storage.data() )
        storage = rd;
      return storage;

    }



  }


  void write(uint idx, Map & storage) {

    if ( ! hdfFile )
      throw_error(modname, "File " + name + " was previously closed.");
    unsigned intent;
    H5Fget_intent(hdfFile, &intent);
    if ( ! (intent & H5F_ACC_RDWR) )
      throw_error(modname, "Can't write into " + name + " which was open read-only.");
    if ( idx >= indices.size() )
      throw_error(modname, "Index is beyond slices to write to " + name + ".");
    if ( storage.shape() != face() )
      throw_error(modname, "Shape of the slice to write " + toString(storage.shape())
                  + " is different from existing shape" + toString(face()) + ".");
    const int idxme = indices.at(idx);

    if (dataMmap.size()) {

      Map wrto;
      switch (sliceDim) {
      case blitz::firstDim : wrto.reference(dataMmap(idxme, all, all)); break;
      case blitz::secondDim: wrto.reference(dataMmap(all, idxme, all)); break;
      case blitz::thirdDim : wrto.reference(dataMmap(all, all, idxme).transpose(blitz::secondDim, blitz::firstDim)); break;
      default:
        throw_error(modname, "Bad slice dimension. Should never happen.");
      }
      wrto = storage;

    } else {

      Map wr;
      if (sliceDim==blitz::thirdDim) {
        Map tmap(storage);
        wr.resize(ioface());
        wr = tmap.transpose(blitz::secondDim, blitz::firstDim);
      } else {
        wr.reference(safe(storage));
      }
      hid_t lfs=local_fs(idxme);
      //pthread_mutex_lock(&rwLock);
      if ( H5Dwrite(dataset, H5T_NATIVE_FLOAT, memspace, lfs, H5P_DEFAULT, wr.data()) < 0)
        warn(modname, "Failed to write slice " +toString(idxme)+ " to " + name + ".");
      //pthread_mutex_unlock(&rwLock);
      H5Sclose(lfs);

    }

  }

};

const string HDFread::modname = "HDF read";





struct HDFwrite : public HDFrw {

private:

  const static string modname;
  const float mincon;
  const float maxcon;

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

  HDFwrite( const ImagePath & filedesc, Shape<3> _sh, float _mincon=0, float _maxcon=0)
    : HDFrw(filedesc)
    , mincon(_mincon)
    , maxcon(_maxcon)
  {
    rank = 3;
    setFace(Shape<2>(_sh(1),_sh(2)));
    const int zsize = _sh(0);
    if ( ! zsize * size(face()) )
      throw warn(modname, "Zerro size to write.");
    indices = slice_str2vec(slicesStr, -zsize);
    if (!indices.size())
      throw_error(modname, "Empty slices to write.");
    const int maxIdx = *max_element(indices.begin(), indices.end());
    setSlices(maxIdx+1);
    cnts.resize(3);
    int idx=0, odx=0;
    while (idx<3) {
      if (idx != sliceDim)
        cnts(idx) = ioface()(odx++);
      idx++;
    }
    cnts(sliceDim) = slices(); // first will be used once as the 3D dimensions
    blitz::Array<hsize_t,1> tcnts(3);
//#ifdef H5F_ACC_SWMR_WRITE
//    hdfFile = H5Fopen(name.c_str(), H5F_ACC_RDWR | H5F_ACC_SWMR_WRITE, H5P_DEFAULT);
//    if (hdfFile>0)
//      H5Fstart_swmr_write(hdfFile);
//#else
    hdfFile = H5Fopen(name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
//#endif
    if (hdfFile<=0) {
      complete();
    } else if ((dataset = H5Dopen(hdfFile, data.c_str(), H5P_DEFAULT))<=0) {
      createNewGroup();
    } else if (  (filespace = H5Dget_space(dataset)) <= 0
              || H5Sget_simple_extent_ndims(filespace) != 3
              || H5Sget_simple_extent_dims(filespace, tcnts.data(), 0) != rank ) {
      complete();
    } else {
      const Shape<2> exFace = exclDim(tcnts, sliceDim);
      const long exIdx = tcnts(sliceDim);
      if ( ( exFace != ioface()  &&  ( warn(modname,
               "Existing shape of HDF data \"" + name+":"+data + "\" ("+toString(exFace)+")"
               " does not match output ("+toString(ioface())+")."), true) )
           ||
           ( maxIdx > exIdx-1  &&  ( warn(modname,
               "Existing size of HDF data \"" + name+":"+data + "\""
               " ("+toString(exIdx)+" slice"+(exIdx==1?"":"s")+")"
               " cannot accomodate index "+toString(maxIdx)+"."), true) )
           ) {
        complete();
        warn(modname, "Existing HDF data \"" + name+":"+data + "\". Will be overwritten.");
      }
    }
    if (hdfFile<=0) {
      //#ifdef H5F_ACC_SWMR_WRITE
      //hdfFile = H5Fcreate(name.c_str(), H5F_ACC_SWMR_WRITE | H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
      //#else
      hdfFile = H5Fcreate(name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
      //#endif
      if (hdfFile>0)
        createNewGroup();
      else {
        complete();
        if (isValidHDF()) {
          hbool_t locking, ignored;
          warn(modname, "Failed to open valid and existing HDF5 file \"" + name + "\".");
          if ( H5Pget_file_locking(file_fapl, &locking, &ignored) >= 0  &&  locking )
            warn(modname, "File \""+name+"\" is locked. You may remove it or disable locking by setting"
                          " evironment variable HDF5_USE_FILE_LOCKING=FALSE.");
        }
        throw_error(modname, "Failed to open HDF5 file " + name + " for writing.");
      }
    }

//#ifdef H5F_ACC_SWMR_WRITE
//    H5Fstart_swmr_write(hdfFile);
//#endif

    mmapMeIfYouCan(true);

  }



  int write(int idx, const Map & storage) {
    if ( hdfFile <= 0 )
      throw_error(modname, "File \"" + id() + "\" was previously closed and no more write possible.");
    if ( idx >= indices.size() )
      throw_error(modname, "Index " + toString(idx) + " to write is beyond initially requested size "
                        + toString(indices.size()) + ". Write is ignored.");
    if ( storage.shape() != face() )
      throw_error(modname, "Shape of the slice to write " + toString(storage.shape())
                  + " is different from initially requested shape" + toString(face()) + ".");
    const int idxme = indices.at(idx);

    if (dataMmap.size()) {

      Map wrto;
      switch (sliceDim) {
      case blitz::firstDim : wrto.reference(dataMmap(idxme, all, all)); break;
      case blitz::secondDim: wrto.reference(dataMmap(all, idxme, all)); break;
      case blitz::thirdDim : wrto.reference(dataMmap(all, all, idxme).transpose(blitz::secondDim, blitz::firstDim)); break;
      default:
        throw_error(modname, "Bad slice dimension. Should never happen.");
      }
      wrto = storage;

    } else {

      Map wr;
      if (sliceDim==blitz::thirdDim) {
        Map tmap(storage);
        wr.resize(ioface());
        wr = tmap.transpose(blitz::secondDim, blitz::firstDim);
      } else {
        wr.reference(safe(storage));
      }
      hid_t lfs=local_fs(idxme);
      //pthread_mutex_lock(&rwLock);
      if ( H5Dwrite(dataset, H5T_NATIVE_FLOAT, memspace, lfs, H5P_DEFAULT, wr.data()) < 0)
        warn(modname, "Failed to write slice " +toString(idxme)+ " to " + name + ".");
      //pthread_mutex_unlock(&rwLock);
      H5Sclose(lfs);

    }

    return idxme;

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



Shape<2>
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
  Shape<2> ret;
  if ( rank == 2 )
    ret = Shape<2>(cnts(0), cnts(1));
  else {
    int idx=0, odx=0;
    while (idx<rank) {
      if (idx != me.sliceDim)
        ret(odx++) = cnts(idx);
      idx++;
    }
  }
  if (me.sliceDim==blitz::thirdDim) // need to transpose what I read in YZ plane
    ret = Shape<2>(ret(1),ret(0));
  cleanup();
  return ret;
  #undef cleanup
}


Shape<2>
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
    return Shape<2>( imag.rows(), imag.columns() );
  }
}




void
BadShape(const ImagePath & filename, const Shape<2> & shp){
  Shape<2> ashp = ImageSizes(filename);
  if ( ashp != shp )
    throw_error("load image", "Shape of the image "
                "\"" + filename + "\"  (" + toString(ashp) + ") is not equal"
                " to the requested shape (" + toString(shp)  + ").");
}




/// Loads an HDF5 image.
///
/// @param filedesc HDF5 file and description of the dataset slice
/// @param storage The array to store the image.
///
static void
ReadImage_HDF5 (const ImagePath & filedesc, Map & storage, const Crop<2> & crp = Crop<2>() ) {
  try {
    Map rd = HDFread(filedesc).read(0,storage,crp);
    if (!areSame(rd,storage)) {
      storage.resize(rd.shape());
      storage=rd;
    }
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
ReadImage_TIFF (const Path & filename, Map & storage, const Crop<2> & crp = Crop<2>()) {

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
  if ( ! TIFFGetField(image, TIFFTAG_SAMPLEFORMAT, &fmt) ) // attempt to guess
    // fmt =  bps != 32  ?  SAMPLEFORMAT_UINT  :  SAMPLEFORMAT_IEEEFP;
    fmt = SAMPLEFORMAT_UINT;
  if ( fmt != SAMPLEFORMAT_UINT &&
       fmt != SAMPLEFORMAT_INT &&
       fmt != SAMPLEFORMAT_IEEEFP )
    safelyCloseTIFFandThrow(image, fd, modname,
      "Image \"" + filename + "\" has unsupported sample format.");

  const Shape<2> osh = crp.shape(Shape<2>(height,width));
  storage.resize(osh);
  tdata_t buf = _TIFFmalloc(TIFFScanlineSize(image));
  for (uint curidx = crp(0).begin(); curidx < crp(0).end(height); curidx++) {

    //pthread_mutex_lock(&mut);
    if ( TIFFReadScanline(image, buf, curidx) < 0 ) {
      _TIFFfree(buf);
      //pthread_mutex_unlock(&mut);
      safelyCloseTIFFandThrow(image, fd, modname,
        "Failed to read line " + toString(curidx) +" in image \"" + filename + "\".");
    }
    Line ln(storage(curidx-crp(0).begin(), all));
    //pthread_mutex_unlock(&mut);

    #define blitzArrayFromData(type) \
      blitz::Array<type,1> ( ((type*)buf) + crp(0).begin(), blitz::shape(osh(1)), blitz::neverDeleteData)
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
ReadImage_IM (const Path & filename, Map & storage, const Crop<2> & crp = Crop<2>() ){

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
  const Shape<2> osh = crp.shape(Shape<2>(hight,width));
  storage.resize(osh);

  // below might be buggy - see notes in SaveImageINT_IM
  //const Magick::PixelPacket
  //  * pixels = imag.getConstPixels(0,0,width,hight);
  //float * data = storage.data();
  //for ( int k = 0 ; k < hight*width ; k++ )
  //  *data++ = (float) Magick::ColorGray( *pixels++  ) .shade();
  // Replacement for the buggy block:
  for (ssize_t curh = crp(0).begin() ; curh < crp(0).end(hight) ; curh++)
    for (ssize_t curw = crp(1).begin() ; curw < crp(1).end(width) ; curw++)
      storage(curh-crp(0).begin(), curw-crp(1).begin()) = Magick::ColorGray(imag.pixelColor(curw, curh)).shade();
  // end replacement *

}



Map
ReadImage(const ImagePath & filename, Map & storage, const Crop<2> & crp, const Shape<2> & shp){
  try {
    const string ext = lower(filename.ext());
    if (size(shp))
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
  return storage;
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
  Shape<2> face;
  const bool writable;

  _ReadVolBySlice(bool overwrite=false)
    : ilist()
    , ssize(0)
    , face(0l)
    , writable(overwrite)
  {}


  _ReadVolBySlice(const std::deque<ImagePath> & filelist, bool overwrite=false)
    : _ReadVolBySlice(overwrite)
  {
    add(filelist);
  }


  void add(const ImagePath & fileind) {
      if (!ilist.size())
        face = ImageSizes(fileind);
      ilist.push_back(fileind);
      if (HDFdesc::isValidHDF(fileind)) {
        const size_t key = hash<string>{}(fileind.repr());
        hdfs.try_emplace(key, fileind, writable);
        ssize += hdfs.at(key).slices();
      } else {
        ssize++;
      }
  }


  void add(const std::deque<ImagePath> & filelist) {
    for ( auto curI = filelist.begin() ; curI < filelist.end() ; curI++ )
      add(*curI);
  }

  Map read (long int idx, Map & out, const Crop<2> & crp = Crop<2>()) {
    int cfirst=0;
    for (int cfl = 0 ; cfl < ilist.size() ; cfl++) {
      const ImagePath flnm = ilist[cfl];
      //if (hdfs.count(flnm)) {
      const size_t key = hash<string>{}(flnm.repr());
      if (hdfs.count(key)) {
        HDFread & hdf = hdfs.at(key);
        if (idx < cfirst + hdf.slices())
          return hdf.read(idx-cfirst, out, crp);
        cfirst += hdf.slices();
      } else {
        if (idx==cfirst)
          return ReadImage(flnm, out, crp);
        cfirst++;
      }
    }
    return out;
  }


  bool write (long int idx, Map & out) {
    if (!writable)
      throw_error("Volume reader", "This reader was open read-only. Writing is not permitted.");
    int cfirst=0;
    for (int cfl = 0 ; cfl < ilist.size() ; cfl++) {
      const ImagePath flnm = ilist[cfl];
      //if (hdfs.count(flnm)) {
      const size_t key = hash<string>{}(flnm.repr());
      if (hdfs.count(key)) {
        HDFread & hdf = hdfs.at(key);
        if (idx < cfirst + hdf.slices()) {
          hdf.write(idx-cfirst, out);
          return true;
        }
        cfirst += hdf.slices();
      } else {
        if (idx==cfirst){
          const Shape<2> imgsh=ImageSizes(flnm);
          if ( imgsh != out.shape())
            throw_error("Volume reader", "Shape of the output map (" + toString(out.shape())
                        + ") does not match that of the image " +flnm+ "("+toString(imgsh)+").");
          SaveImage(flnm, out);
          return true;
        }
        cfirst++;
      }
    }
    return false;
  }


  size_t size() const {return ssize;}

};




class ReadVolInThread : public InThread {

  Volume & storage;
  const Shape<2> ish;
  const ImageProc canonImgProc;
  const Shape<2> sh;
  unordered_map< pthread_t, ImageProc* > rdprocs;
  ReadVolumeBySlice reader;

public:

  ReadVolInThread(const std::deque<ImagePath> & filelist, Volume & storage,
                  float ang, Crop<2> crp, Binn<2> bnn, bool verbose=false)
    : InThread(verbose , "reading volume")
    , ish( filelist.size() ? ImageSizes(filelist[0]) : Shape<2>(0,0) )
    , storage(storage)
    , canonImgProc(ang, crp, bnn, ish)
    , sh(canonImgProc.shape())
    , reader(filelist)
  {
    if ( ! filelist.size() || ! reader.slices() || ! size(sh) ) {
      storage.free();
      return;
    }
    bar.setSteps(reader.slices());
    storage.resize(reader.slices(), sh(0), sh(1));
  }

  ~ReadVolInThread() {
    for (auto celem : rdprocs)
      if (celem.second)
        delete celem.second;
  }


  bool inThread (long int idx) {

    if ( idx >= reader.slices() )
      return false;

    pthread_t me = pthread_self();
    if ( ! rdprocs.count(me) ) {
      ImageProc * erdproc = new ImageProc(canonImgProc);
      lock();
      rdprocs.emplace(me, erdproc);
      unlock();
    }
    // lock(); // is it safe to avoid locking here?
    ImageProc & myrdproc = *rdprocs.at(me);
    // unlock();

    storage(idx,all,all) = myrdproc.read(reader, idx);
    bar.update();
    return true;

  }

};


void
ReadVolume(const std::deque<ImagePath> & filelist, Volume & storage, bool verbose) {
  ReadVolInThread(filelist, storage, 0, Crop<2>(), Binn<2>(), verbose).execute();
}







ReadVolumeBySlice::ReadVolumeBySlice(const deque<ImagePath> & filelist, bool overwrite)
  : guts(new _ReadVolBySlice(filelist, overwrite))
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

Map ReadVolumeBySlice::read(uint sl, Map& trg, const Crop<2> & crp) {
  return ((_ReadVolBySlice*) guts)->read(sl, trg, crp);
}

void ReadVolumeBySlice::readTo(uint sl, Map& trg, const Crop<2> & crp) {
  Map rmap = read(sl, trg, crp);
  if (!areSame(rmap, trg)) {
    trg.resize(rmap.shape());
    trg = rmap;
  }
}

bool ReadVolumeBySlice::write(uint sl, Map& trg) {
  return ((_ReadVolBySlice*) guts)->write(sl, trg);
}

size_t ReadVolumeBySlice::slices() const {
  return ((_ReadVolBySlice*) guts)->ssize;
}


Shape<2> ReadVolumeBySlice::face() const {
  return ((_ReadVolBySlice*) guts)->face;
}

Shape<3> ReadVolumeBySlice::shape() const {
  return  Shape<3>( slices(), face()(1), face()(0) );
}



Map ImageProc::read(function<Map()> doRot, function<Map()> noRot) {
  Map cmap;
  if (rotProc) {
    Map rdmap = doRot();
    Map rmap = rotProc.apply(rdmap, rotmap, reNAN);
    cmap.reference(crp.apply(rmap));
  } else {
    cmap.reference(noRot());
  }
  Map bmap = bnnProc.apply(cmap,bnnmap);
  return bmap;
}

Map ImageProc::read(const ImagePath & filename) {
  return read( [&filename,this](){return ReadImage(filename, readmap, ish);}
             , [&filename,this](){return ReadImage(filename, readmap, crp, ish);} );
}

Map ImageProc::read(ReadVolumeBySlice & volRd, uint sl) {
  if (volRd.face() != ish)
    throw_error("ImageProc", "Missmatch of input volume slice shape ("+toString(volRd.face())+")"
                             " with expected ("+toString(ish)+").");
  return read( [&volRd,sl,this](){return volRd.read(sl, readmap);}
             , [&volRd,sl,this](){return volRd.read(sl, readmap, crp);} );
}











class _SaveVolumeBySlice {

private:

  const size_t zsize;
  HDFwrite * hdfFile;
  string sliceformat;
  const float mincon;
  const float maxcon;

  // below members are to accumulate printed output to keep in order.
  pthread_mutex_t locker;
  unordered_map<int, string> texts;
  int nextOut;

public:

  _SaveVolumeBySlice(const ImagePath & filedesc, Shape<3> _sh, float mmin, float mmax)
    : zsize(_sh(0))
    , hdfFile(0)
    , mincon(mmin)
    , maxcon(mmax)
    , locker(PTHREAD_MUTEX_INITIALIZER)
    , nextOut(0)
  {
    if (filedesc == "-") { // special case for print out
      sliceformat = "-";
    } else if ( HDFdesc::isValid(filedesc) )
      hdfFile = new HDFwrite(filedesc, _sh, mincon, maxcon);
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

    if (sliceformat == "-")
      printf("\n");
  }

  ImagePath save(int idx, const Map & storage) {

    if (sliceformat == "-") {

      string myText;
      for (ssize_t y = 0 ; y < storage.shape()(0) ; y++) {
        for (ssize_t x = 0 ; x < storage.shape()(1) ; x++)
          myText += toString(storage(y,x)) + " ";
        if (storage.shape()(1)!=1)
          myText += " ";
      }
      if (storage.shape()(0)!=1)
        myText += " ";
      // below I assume that sooner or later all indexes starting
      // from idx=0 will come here and and come only once.
      pthread_mutex_lock(&locker);
      if (idx == nextOut) {
        cout << myText;
        while (texts.count(++nextOut)) {
          cout <<  texts[nextOut];
          texts.erase(nextOut);
        }
      } else {
        texts[idx] = myText;
      }
      pthread_mutex_unlock(&locker);
      return ImagePath();

    } else if (hdfFile) {

      int slIdx;
      if (mincon==maxcon)
        slIdx = hdfFile->write(idx, storage);
      else {
        Map _stor(storage.shape());
        for (ssize_t y = 0 ; y < storage.shape()(0) ; y++) {
          for (ssize_t x = 0 ; x < storage.shape()(1) ; x++) {
            float val = storage(y,x);
            if (val < mincon)
              _stor(y,x) = mincon;
            else if (val > maxcon)
              _stor(y,x) = maxcon;
            else
              _stor(y,x) = val;
          }
        }
        slIdx = hdfFile->write(idx, storage);
      }
      return hdfFile->name + ':' + hdfFile->data + ':' + hdfFile->slicesStr + toString(slIdx);

    } else {

      const ImagePath fileName =  zsize == 1  ?  sliceformat : toString(sliceformat, idx);
      if (mincon==maxcon)
        SaveImage(fileName, storage);
      else
        SaveImage(fileName, storage, mincon, maxcon);
      return fileName;

    }
  }


  size_t size() const {return zsize;}


};



class SaveVolInThread : public InThread {

private:

  const Volume & vol;
  int sliceDim;
  Shape<2> ssh;
  _SaveVolumeBySlice *writer = 0;
  std::deque<int>indices;
  unordered_map<pthread_t,Map> maps;

  bool inThread (long int idx) {

    if ( idx >= indices.size() || ! writer )
      return false;

    const pthread_t me = pthread_self();
    lock();
    if ( ! maps.count(me) )  // first call
      maps.emplace(me, 0);
    Map & cur = maps.at(me) ;
    unlock();
    cur.resize(ssh);

    const int idi = indices[idx];
    switch ( sliceDim ) {
    case 2: cur = vol(all, all, idi); break;
    case 1: cur = vol(all, idi, all); break;
    case 0: cur = vol(idi, all, all); break;
    }
    writer->save(idi, cur);
    bar.update();
    return true;

  }


public:

  SaveVolInThread(const ImagePath & filedesc, const Volume & _vol, bool verbose,
                  const std::string & slicedesc, float mmin, float mmax)
    : vol(_vol)
    , InThread(verbose , "saving volume")
  {

    Shape<3> vsh(vol.shape());
    string sindex = slicedesc.size()  ?  slicedesc  :  "Z";
    switch ( sindex.at(0) ) {
      case 'x':
      case 'X':
        sindex.erase(0,1);
        sliceDim=2;
        ssh = Shape<2>(vsh(0),vsh(1));
        break;
      case 'y':
      case 'Y':
        sindex.erase(0,1);
        sliceDim=1;
        ssh = Shape<2>(vsh(0),vsh(2));
        break;
      case 'z':
      case 'Z':
        sindex.erase(0,1);
      default:
        sliceDim=0;
        ssh = Shape<2>(vsh(1),vsh(2));
    }
    indices = slice_str2vec(sindex, vsh(sliceDim));
    writer = new _SaveVolumeBySlice(filedesc, Shape<3>(vsh(sliceDim), ssh(0), ssh(1)), mmin, mmax);
    bar.setSteps(indices.size());

  }

  ~SaveVolInThread() {
    if (writer) {
      delete writer;
      writer = 0;
    }
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



SaveVolumeBySlice::SaveVolumeBySlice(const ImagePath & filedesc, Shape<3> _sh, float mmin, float mmax)
  : guts(new _SaveVolumeBySlice(filedesc, _sh, mmin, mmax))
{}

SaveVolumeBySlice::~SaveVolumeBySlice() {
  delete (_SaveVolumeBySlice*) guts;
}

ImagePath SaveVolumeBySlice::save(uint sl, const Map& trg) {
  return ((_SaveVolumeBySlice*) guts)->save(sl, trg);
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

  const Shape<2> data_shape(data_read.size(), data_read.size() ? data_read[0].size() : 0);
  storage.resize(data_shape);
  for ( ssize_t y=0 ; y < data_shape(0) ; y++)
    for ( ssize_t x=0 ; x < data_shape(1) ; x++)
      storage(y,x) = data_read[y][x];

}



const string IntOptionDesc =
  "If this option is not set, the output format defaults to the"
  " 32-bit float-point TIFF (regardless of the extension)."
  " If it is set, the image format is derived from the output"
  " file extension (TIFF if the extension does not correspond"
  " to any format).";

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

const string DimSliceOptionDesc = "[slice dimension][slice(s)],"
" with [slice dimension] either x, y or z (default) being the perpendicular to the slicing plane"
" and [slice(s)]. " + SliceOptionDesc;


