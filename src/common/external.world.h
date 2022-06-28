#ifndef EXTARNAL_WORLD_H
#define EXTARNAL_WORLD_H

#include "common.h"
#include <unordered_map>
#include <deque>
#include <algorithm>




class ImagePath : public Path {
private:

  std::string _desc;

  static std::pair<std::string,std::string> splitIn(const std::string & pthndsc) {
    const size_t splt = std::min(pthndsc.find(':'), pthndsc.size());
    return std::make_pair(pthndsc.substr(0,splt), pthndsc.substr(splt));
  }

public:

  inline ImagePath(){}

  inline ImagePath(const std::string & str)
    : Path(splitIn(str).first)
    , _desc(splitIn(str).second)
  {}

  inline ImagePath(const ImagePath & other)
    : Path(other)
    , _desc(other._desc)
  {}

  inline ImagePath & operator=(const ImagePath & other) {
    std::string::operator=(other);
    _desc = other._desc;
    return *this;
  }

  inline const std::string & desc() const {return _desc;}
  inline const std::string repr() const {return *this + _desc;}

};

inline int COMMON_API
_conversion (ImagePath* _val, const std::string & in) {
  *_val = ImagePath(in);
  return 1;
}


template<> struct std::hash<ImagePath> {
  std::size_t operator()(ImagePath const& s) const noexcept  {
    return std::hash<std::string>{}(s.repr());
  }
};






/// \defgroup IO Image and data file handling.
///
/// Loading, saving and other possible operations with the
/// image/data -> array -> image/data conversions.
///
/// @{

/// \brief Pixel size of the image.
///
/// Calculated from the dpi (dots per inch).
///
/// @param filename Image filename.
///
/// @return pixel size in micron.
///
float  COMMON_API
PixelSize(const ImagePath & filename);


/// \brief Image sizes.
///
/// Reads image size and returns them as the Shape vector.
///
/// @param filename Image filename.
///
/// @return Image sizes.
///
Shape  COMMON_API
ImageSizes(const ImagePath & filename);



/// \brief Check the shape of the image.
///
/// Throws error if the shape of the image is not equal to ::shp.
///
/// @param filename Image filename.
/// @param shp The expected shape.
///
void COMMON_API
BadShape(const ImagePath & filename, const Shape & shp);



/// \brief Load image checking the shape.
///
/// Same as ReadImage(const Path & , Map &), but checks the image shape
/// before loading and throws the error if the shape is different from :::shp.
///
/// @param filename Image filename.
/// @param storage  Array to load the image into.
/// @param shp The expected shape.
///
void COMMON_API
ReadImage(const ImagePath & filename, Map & storage, const Shape & shp = Shape());



/// \brief Save the array into image.
///
/// Stores the array in the float-point TIFF or integer image depending.
/// on requested type. Be careful: limited number
/// of editors/viewers/analyzers support float-point format.
///
/// @param filename the name of the image file image.
/// @param storage the array to be written to the image.
/// @param saveint save image as 16-bit integer.
///
void COMMON_API
SaveImage(const ImagePath & filename, const Map & storage, bool saveint=false);


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
void COMMON_API
SaveImage(const ImagePath & filename, const Map & storage, float minval, float maxval );







/// Description of the output result mask.
extern const std::string COMMON_API MaskDesc;

void COMMON_API
ReadVolume(const std::deque<ImagePath> & filelist, Volume & storage, bool verbose=false);

inline void COMMON_API
ReadVolume(const ImagePath & filename, Volume & storage, bool verbose=false) {
  ReadVolume(std::deque<ImagePath>(1, filename), storage, verbose );
}


void COMMON_API
SaveVolume(const ImagePath & filedesc, Volume & storage,
           bool verbose, const std::string & slicedesc, float mmin, float mmax);


inline void COMMON_API
SaveVolume(const ImagePath & filedesc, Volume & storage,
           bool verbose, const std::string & slicedesc) {
  SaveVolume(filedesc, storage, verbose, slicedesc, 0, 0);
}

inline void COMMON_API
SaveVolume(const ImagePath & filedesc, Volume & storage,
           bool verbose) {
  SaveVolume(filedesc, storage, verbose, "", 0, 0);
}

inline void COMMON_API
SaveVolume(const ImagePath & filedesc, Volume & storage,
           const std::string & slicedesc) {
  SaveVolume(filedesc, storage, false, slicedesc, 0, 0);
}

inline void COMMON_API
SaveVolume(const ImagePath & filedesc, Volume & storage) {
  SaveVolume(filedesc, storage, false, "", 0, 0);
}

inline void COMMON_API
SaveVolume(const ImagePath & filedesc, Volume & storage,
           bool verbose,
           float mmin, float mmax) {
  SaveVolume(filedesc, storage, verbose, "", mmin, mmax);
}

inline void COMMON_API
SaveVolume(const ImagePath & filedesc, Volume & storage,
           const std::string & slicedesc,
           float mmin, float mmax) {
  SaveVolume(filedesc, storage, false, slicedesc, mmin, mmax);
}

inline void COMMON_API
SaveVolume(const ImagePath & filedesc, Volume & storage,
           float mmin, float mmax) {
  SaveVolume(filedesc, storage, false, "", mmin, mmax);
}





class ReadVolumeBySlice {
private:
  void * guts;
public:
  ReadVolumeBySlice(const std::deque<ImagePath> & filelist = std::deque<ImagePath>() );
  ReadVolumeBySlice(const ImagePath & file);
  ~ReadVolumeBySlice();
  void add(const std::deque<ImagePath> & filelist);
  void add(const ImagePath & fileind);
  void read(uint sl, Map & trg);
  size_t slices() const;
};


class SaveVolumeBySlice {
private:
  void * guts;
public:
  SaveVolumeBySlice(const ImagePath & filedesc, Shape _sh, size_t _zsize, float mmin=0, float mmax=0);
  ~SaveVolumeBySlice();
  void save(uint sl, const Map & trg);
  size_t slices() const;
};













/// \brief Loads any amount of lines from data file.
///
/// @param filename the name of the data file.
/// @param ... Any number of the lines. Be careful:
/// I supposed that all arguments are of type const Line*,
/// a null pointer must be passed as the last such argument.
///
// Don't use the reference type "const Path &" here: will
// not work on Windows
void COMMON_API
LoadData(const Path filename, ... );

void COMMON_API
LoadData ( const Path filename, Map & storage );



/// \brief Save any amount of lines into data file.
///
/// @param filename the name of the data file.
/// @param ... Any number of the lines. Be careful:
/// I supposed that all arguments are of type const Line*,
/// a null pointer must be passed as the last such argument.
///
// Don't use the reference type "const Path &" here: will
// not work on Windows
void COMMON_API
SaveData(const Path filename, ... );

/// @}


#endif // AAA_H
