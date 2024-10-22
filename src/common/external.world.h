#ifndef EXTARNAL_WORLD_H
#define EXTARNAL_WORLD_H


#include "common.world.h"
#include "matrix.world.h"
#include "flatfield.h"
#include <functional>





class ImagePath : public Path {
private:

  std::string _desc;

  static std::pair<std::string,std::string> splitIn(const std::string & pthndsc) {
    const size_t splt = std::min(pthndsc.find(':'), pthndsc.size());
    return std::make_pair(pthndsc.substr(0,splt), pthndsc.substr(splt));
  }

public:

  inline ImagePath(const std::string & str = std::string())
    : Path(splitIn(str).first)
    , _desc(splitIn(str).second)
  {}

  inline ImagePath(const char *str)
    : ImagePath(std::string(str))
  {};

  inline ImagePath(const ImagePath & other)
    : Path(other)
    , _desc(other._desc)
  {}

  inline ImagePath & operator=(const ImagePath & other) {
    std::string::operator=(other);
    _desc = other._desc;
    return *this;
  }

  inline const std::string repr() const {return *this + _desc;}
  inline const std::string & desc() const {return _desc;}
  inline ImagePath & desc(const std::string & __desc) { _desc=__desc ; return *this;}

};

inline int
_conversion (ImagePath* _val, const std::string & in) {
  *_val = ImagePath(in);
  return 1;
}


namespace std {
template<> struct hash<ImagePath> {
  size_t operator()(ImagePath const& s) const noexcept  {
    return hash<string>{}(s.repr());
  }
};
}





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
float
PixelSize(const ImagePath & filename);


/// \brief Image sizes.
///
/// Reads image size and returns them as the Shape vector.
///
/// @param filename Image filename.
///
/// @return Image sizes.
///
Shape<2>
ImageSizes(const ImagePath & filename);



/// \brief Check the shape of the image.
///
/// Throws error if the shape of the image is not equal to ::shp.
///
/// @param filename Image filename.
/// @param shp The expected shape.
///
void
BadShape(const ImagePath & filename, const Shape<2> & shp);



/// \brief Load image checking the shape.
///
/// Same as ReadImage(const Path & , Map &), but checks the image shape
/// before loading and throws the error if the shape is different from :::shp.
///
/// @param filename Image filename.
/// @param storage  Array to load the image into.
/// @param shp The expected shape.
///
Map
ReadImage( const ImagePath & filename, Map & storage
         , const Crop<2> & crp = Crop<2>(), const Shape<2> & shp = Shape<2>(0l));

inline Map
ReadImage(const ImagePath & filename, Map & storage, const Shape<2> & shp) {
  return ReadImage(filename, storage, Crop<2>(), shp);
}



/// \brief Save the array into image.
///
/// Stores the array in the float-point TIFF or integer image depending.
/// on requested type. Be careful: limited number
/// of editors/viewers/analyzers support float-point format.
///
/// @param filename the name of the image file image.
/// @param storage the array to be written to the image.
/// @param bpp if non-zero saves image as integer with corresponding bits per pixel.
/// @param minval the value corresponding to black.
/// @param maxval the value corresponding to white.
void
SaveImage(const ImagePath & filename, const Map & storage, int bpp=0, float minval=0, float maxval=0);






/// Description of the output result mask.
extern const std::string  MaskDesc;

void
ReadVolume(const std::deque<ImagePath> & filelist, Volume & storage, bool verbose=false);

inline void
ReadVolume(const ImagePath & filename, Volume & storage, bool verbose=false) {
  ReadVolume(std::deque<ImagePath>(1, filename), storage, verbose );
}


void
SaveVolume( const ImagePath & filedesc, Volume & storage, bool verbose
          , const std::string & slicedesc, int bpp=0, float minval=0, float maxval=0);



class ReadVolumeBySlice {
private:
  void * guts;
public:
  ReadVolumeBySlice(const std::deque<ImagePath> & filelist = std::deque<ImagePath>(), bool overwrite=false);
  inline ReadVolumeBySlice(bool overwrite)
    : ReadVolumeBySlice(std::deque<ImagePath>(), overwrite) {};
  inline ReadVolumeBySlice(const ImagePath & file, bool overwrite=false)
    : ReadVolumeBySlice(std::deque<ImagePath>(1, file), overwrite) {};
  ~ReadVolumeBySlice();
  void add(const std::deque<ImagePath> & filelist);
  void add(const ImagePath & fileind);
  Map read(uint sl, Map & trg, const Crop<2> & crp = Crop<2>());
  void readTo(uint sl, Map & trg, const Crop<2> & crp = Crop<2>());
  bool write(uint sl, Map & out);
  bool writable() const;
  size_t slices() const;
  Shape<2> face() const;
  Shape<3> shape() const;
};


class SaveVolumeBySlice {
private:
  void * guts;
public:
  SaveVolumeBySlice(const ImagePath & filedesc, Shape<3> sh, int bpp=0, float minval=0, float maxval=0);
  ~SaveVolumeBySlice();
  ImagePath save(uint sl, const Map & trg);
  size_t slices() const;
};





class ImageProc : public MapProc, public FlatFieldProc {
  using MapProc::MapProc;
  using FlatFieldProc::FlatFieldProc;
  static const std::string modname;
  Map readmap;
  Map read(std::function<Map()> doRot, std::function<Map()> noRot);
public:
  ImageProc( const Shape<2> & ish, const Map & bg, const Map & df, const Map & dg, const Map & ms
           , float ang, const Crop<2> & crp, const Binn<2> & bnn, float reNAN=NAN);
  Map read(const ImagePath & filename);
  Map read(ReadVolumeBySlice & volRd, uint sl);
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
void
LoadData(const Path filename, ... );

void
LoadData ( const Path filename, Map & storage );

inline Map
LoadData (const Path filename) {
  Map out;
  LoadData(filename, out);
  return out;
}


/// \brief Save any amount of lines into data file.
///
/// @param filename the name of the data file.
/// @param ... Any number of the lines. Be careful:
/// I supposed that all arguments are of type const Line*,
/// a null pointer must be passed as the last such argument.
///
// Don't use the reference type "const Path &" here: will
// not work on Windows
void
SaveData(const Path filename, ... );

/// @}




/// Description of the -i, --int option used in many modules.
extern const std::string
IntOptionDesc;

extern const std::string
SliceOptionDesc;

extern const std::string
DimSliceOptionDesc;



#endif // AAA_H
