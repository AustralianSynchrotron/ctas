#include "common.h"

#ifndef EXTARNAL_WORLD_H
#define EXTARNAL_WORLD_H


Path COMMON_API
imageFile(const std::string & filedesc);


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
PixelSize(const Path & filename);


/// \brief Image sizes.
///
/// Reads image size and returns them as the Shape vector.
///
/// @param filename Image filename.
///
/// @return Image sizes.
///
Shape  COMMON_API
ImageSizes(const Path & filename);


/// \brief Image sizes.
///
/// Read image sizes and return them as width and hight.
///
/// @param filename Image filename.
/// @param width Width of the image.
/// @param hight Height of the image.
///
void COMMON_API
ImageSizes(const Path & filename, int *width, int *hight);


/// \brief Check the shape of the image.
///
/// Throws error if the shape of the image is not equal to ::shp.
///
/// @param filename Image filename.
/// @param shp The expected shape.
///
void COMMON_API
BadShape(const Path & filename, const Shape & shp);


/// \brief Load image.
///
/// Similar to ReadImage(), but uses preopened image object to read.
///
/// @param filename Image filename.
/// @param storage  Array to load the image into.
///
void COMMON_API
ReadImage(const Path & filename, Map & storage );


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
ReadImage(const Path & filename, Map & storage, const Shape & shp);


/// \brief Load one line of the image.
///
/// Reads one line of the image from the file, resizes the array
/// to fit and stores the line into the array.
///
/// @param filename File to read the image from.
/// @param storage  Array to load the image into.
/// @param idx  The index of the line to be read.
///
void COMMON_API
ReadImageLine(const Path & filename, Line & storage, int idx);


/// \brief Load one line of the image.
///
/// Same as ReadImageLine(const Path &, Line &, int),
/// but checks the image shape before loading and throws the error if the
/// shape is different from ::shp.
///
/// @param filename File to read the image from.
/// @param storage  Array to load the image into.
/// @param idx  The index of the line to be read.
/// @param shp The expected shape.
///
void COMMON_API
ReadImageLine(const Path & filename, Line & storage, int idx, const Shape & shp);


/// \brief Load several lines of the image.
///
/// Reads lines of the image which are found in the ::idxs vector.
/// If a line in the ::idxs is outside the image size then warns and
/// fills the corresponding line with zeros. The lines are stored in
/// the order they found in the ::idx vector. Array ::storage is resized
/// to contain all lines. If the ::idxs array is empty - reads the whole image.
///
/// @param filename File to read the image from.
/// @param storage  Array to load the image into.
/// @param idxs vector of lines to be read. If empty, reads the whole image.
///
void COMMON_API
ReadImageLine(const Path & filename, Map & storage,
        const std::vector<int> & idxs);


/// \brief Load several lines of the image.
///
/// Same as ReadImageLine(const Path &, Map &, const std::vector<int> &),
/// but checks the image shape before loading and throws the error if the
/// shape is different from ::shp.
///
/// @param filename File to read the image from.
/// @param storage  Array to load the image into.
/// @param idxs vector of lines to be read. If empty - reads the whole image.
/// @param shp The expected shape.
///
void COMMON_API
ReadImageLine(const Path & filename, Map & storage,
        const std::vector<int> & idxs, const Shape & shp);




/// Description of the output result mask.
extern const std::string COMMON_API MaskDesc;

void COMMON_API
ReadVolume(const std::vector<Path> & filelist, Volume & storage, bool verbose=false);

inline void COMMON_API
ReadVolume(const Path & filename, Volume & storage, bool verbose=false) {
  ReadVolume(std::vector<Path>(1, filename), storage, verbose );
}



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
SaveImage(const Path & filename, const Map & storage, bool saveint=false);


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
SaveImage(const Path & filename, const Map & storage,
    float minval, float maxval );



void COMMON_API
SaveVolume(const Path & filedesc, Volume & storage,
           bool verbose, const std::string & slicedesc, float mmin, float mmax);


inline void COMMON_API
SaveVolume(const Path & filedesc, Volume & storage,
           bool verbose, const std::string & slicedesc) {
  SaveVolume(filedesc, storage, verbose, slicedesc, 0, 0);
}

inline void COMMON_API
SaveVolume(const Path & filedesc, Volume & storage,
           bool verbose) {
  SaveVolume(filedesc, storage, verbose, "", 0, 0);
}

inline void COMMON_API
SaveVolume(const Path & filedesc, Volume & storage,
           const std::string & slicedesc) {
  SaveVolume(filedesc, storage, false, slicedesc, 0, 0);
}

inline void COMMON_API
SaveVolume(const Path & filedesc, Volume & storage) {
  SaveVolume(filedesc, storage, false, "", 0, 0);
}

inline void COMMON_API
SaveVolume(const Path & filedesc, Volume & storage,
           bool verbose,
           float mmin, float mmax) {
  SaveVolume(filedesc, storage, verbose, "", mmin, mmax);
}

inline void COMMON_API
SaveVolume(const Path & filedesc, Volume & storage,
           const std::string & slicedesc,
           float mmin, float mmax) {
  SaveVolume(filedesc, storage, false, slicedesc, mmin, mmax);
}

inline void COMMON_API
SaveVolume(const Path & filedesc, Volume & storage,
           float mmin, float mmax) {
  SaveVolume(filedesc, storage, false, "", mmin, mmax);
}















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
