#ifndef MATRIX_H
#define MATRIX_H

#include "common.world.h"
#include <algorithm>

#define BZ_THREADSAFE
//#define BZ_DEBUG_LOG_REFERENCES
#include "../blitz-long/blitz/array.h"
//#include <blitz/array.h>



/// \brief 1D line with data.
///
/// One dimensional array of the ::float elements.
/// Used for filters, sinogram's lines and so on.
typedef blitz::Array<float,1> Line;

/// \brief 2D Array with data.
///
/// Two dimensional array of the ::float elements.
/// Used for sinograms, input and output images etc.
typedef blitz::Array<float,2> Map;
static const Map defaultMap;

/// \brief 3D Array with data.
///
/// Three dimensional array of the ::float elements.
typedef blitz::Array<float,3> Volume;

static blitz::firstIndex  i0;
static blitz::secondIndex i1;



template<class T1, class T2, int N>
inline bool operator==( const blitz::TinyVector<T1,N> & t1
                      , const blitz::TinyVector<T2,N> & t2) {
  for (int dim=0; dim<N; dim++)
    if (t1[dim]!=t2[dim])
      return false;
  return true;
}

template<class T1, class T2, int N>
inline bool operator!=( const blitz::TinyVector<T1,N> & t1
                      , const blitz::TinyVector<T2,N> & t2) {
  return ! (t1 == t2);
}

template<int Dim, class T>
inline std::string toString (const blitz::TinyVector<T,Dim> & shp) {
  std::string toRet;
  for (int dim = 0; dim < Dim; ++dim)
    toRet += toString(shp[Dim-1-dim]) + (dim == Dim-1 ? "" : ",");
  return toRet;
}

template<class Ts, class Tt, int DimS, int DimT>
blitz::TinyVector<Tt,DimT> & equate(       blitz::TinyVector<Tt,DimT> & target
                                   , const blitz::TinyVector<Ts,DimS> & source) {
  for (int dim = 0; dim < DimT && dim < DimS; ++dim)
    target(DimT-1-dim) = source(DimS-1-dim);
  return target;
} ;



template<int Dim>
struct Shape
  : public blitz::TinyVector<ssize_t,Dim>
{
  //using blitz::TinyVector<ssize_t,Dim>::TinyVector;
  Shape(const blitz::TinyVector<ssize_t,Dim> & other) : blitz::TinyVector<ssize_t,Dim>(other) {}
  Shape() : blitz::TinyVector<ssize_t,Dim>(ssize_t(0)) {}
  Shape(ssize_t lngth) : blitz::TinyVector<ssize_t,1>(lngth) {}
  Shape(ssize_t hght, ssize_t wdth) : blitz::TinyVector<ssize_t,2>(hght,wdth) {}
  Shape(ssize_t dpth, ssize_t hght, ssize_t wdth) : blitz::TinyVector<ssize_t,3>(dpth,hght,wdth) {}
  template <class T>
  Shape(const blitz::TinyVector<T,Dim> & other) : blitz::TinyVector<ssize_t,Dim>(other) {}
  template <class T>
  Shape& operator=(const blitz::TinyVector<T,Dim> & other) {
    blitz::TinyVector<ssize_t,Dim>::operator=(other);
    return *this;
  }
  // above template does not instantinate automatically to produce below assign operator
  Shape& operator=(const Shape & other) { return this->operator=<ssize_t>(other); }
};

inline size_t area(const Shape<2> & sh) {
  return sh(0) * sh(1) ;
}

inline Shape<2> faceShape(const Shape<3> & sh) {
  return Shape<2>(sh(1), sh(2));
}


static const blitz::Range all = blitz::Range::all();

inline blitz::Range dstR(size_t size, ssize_t shift) {
  return blitz::Range(std::max(0l, shift), size - 1 + std::min(0l, shift));
}

inline blitz::Range srcR(size_t size, ssize_t shift) {
  return blitz::Range(std::max(0l, -shift), size - 1 + std::min(0l, -shift));
}

#define dstRa(size, shift0, shift1) \
dstR(size(0), shift0), dstR(size(1), shift1)
#define srcRa(size, shift0, shift1) \
    srcR(size(0), shift0), srcR(size(1), shift1)


class Segment {
private:
  static const std::string modname;
  ssize_t _from;
  ssize_t _to; // 0 or negative stands for "from the end"
  void check_throw();
public:
  Segment(ssize_t _from, ssize_t _to) : _from(_from), _to(_to) {check_throw();}
  Segment(const std::string & str=std::string());
  ssize_t begin() const {return _from;}
  ssize_t end(size_t orgsz=0) const { return _from + size(orgsz); }
  ssize_t size(size_t orgsz=0) const;
  explicit operator bool() const {return _from || _to;} ;
  std::string print() const { return toString("%u%c%u", _from, _to > 0 ? ':' : '-' , abs(_to) ); }
};

inline std::string toString(const Segment & seg) {
  return seg.print();
}

inline std::string type_desc (Segment*) {
  return "UINT[:+-]UINT";
}


template<int Dim>
class Crop : public blitz::TinyVector<Segment,Dim> {
  using blitz::TinyVector<Segment,Dim>::TinyVector;
private:
  static const std::string modname;
public:
  Crop(const std::string & str=std::string()) ;
  template<int DimO> Crop(Crop<DimO> & other) { this->operator=(other); }
  template<int DimO> Crop<Dim> & operator=(const Crop<DimO> & other) { equate(*this, other); return *this; } ;
  explicit operator bool() const { return std::any_of( whole(*this), [](const Segment & seg){return bool(seg);}); }
  Shape<Dim> apply(const blitz::TinyVector<ssize_t,Dim> & ish) const ;
  const blitz::Array<float,Dim> apply(const blitz::Array<float,Dim> & iarr) const ;
  void apply(const blitz::Array<float,Dim> & inarr, blitz::Array<float,Dim> & outarr) const ;
};

template<int Dim>
std::string type_desc (Crop<Dim>*) {
  return toString( blitz::TinyVector<std::string,Dim>("SEG") );
}

template<int Dim>
int _conversion (Crop<Dim>* _val, const std::string & in) {
  try { *_val = Crop<Dim>(in); }
  catch (...) {return -1;}
  return 1;
}

extern const std::string CropOptionDesc;








template<int Dim>
class BinnT : public blitz::TinyVector<float,Dim> {
  using blitz::TinyVector<float,Dim>::TinyVector;
private:
  static const std::string modname;
public:
  BinnT(float val=1.0f) : blitz::TinyVector<float,Dim>(val) {}
  BinnT(const std::string & str);
  template<int DimO> BinnT(BinnT<DimO> & other) { this->operator=(other); }
  template<int DimO> BinnT<Dim> & operator=(const BinnT<DimO> & other) { equate(*this, other); return *this;}
  explicit operator bool() const { return std::any_of( whole(*this), [](const float & bnn){return bnn != 1.0f;}); }
};

template<int Dim>
Shape<Dim> binn(const blitz::TinyVector<ssize_t,Dim> & ish, const BinnT<Dim> & bnn) {
  Shape<Dim> osh;
  //TODO
  return osh;
}

template<int Dim, class T>
const blitz::Array<T,Dim> binn(const blitz::Array<T,Dim> & iarr, const BinnT<Dim> & bnn) {
  if (!bnn)
    return iarr;
  const Shape<Dim> osh = binn(iarr.shape(), bnn);
  blitz::Array<T,Dim> toRet;
  // TODO
  return toRet;
}

template<int Dim, class T>
void binn(const blitz::Array<T,Dim> & inarr, blitz::Array<T,Dim> & outarr, const BinnT<Dim> & bnn) {
  const blitz::Array<T,Dim> & oarr = binn(inarr, bnn);
  outarr.resize(oarr.shape());
  outarr = oarr;
}






struct PointF2D {
  float x;      ///< X coordinate
  float y;       ///< Y coordinate

  inline PointF2D(float _x=0, float _y=0)
    : x(_x), y(_y)
  {}

  inline PointF2D & operator+=(const PointF2D & rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  inline PointF2D & operator-=(const PointF2D & rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

};

inline bool operator==( const PointF2D & p1, const PointF2D & p2){
  return p1.x == p2.x && p1.y == p2.y;
}

inline bool operator!=( const PointF2D & p1, const PointF2D & p2){
  return p1.x != p2.x || p1.y != p2.y;
}

std::string
inline type_desc (PointF2D*) {
  return "FLOAT,FLOAT";
}

int
inline _conversion (PointF2D* _val, const std::string & in) {
  float x, y;
  if ( sscanf( in.c_str(), "%f:%f", &x, &y) != 2  &&
       sscanf( in.c_str(), "%f,%f", &x, &y) != 2 )
    return -1;
  *_val = PointF2D(x, y);
  return 1;
}



// Returns the array which has elements stored continiously and contiguosly.
// That is data() method can be used safely. If input array is safe, it is
// returned as is, otherwise safe copy of it is returned.
template<class T, int N> const blitz::Array<T,N>
safe(const blitz::Array<T,N> & arr, bool preserve=true){
  if ( ! arr.size() )
    return arr;
  bool retRef = arr.isStorageContiguous();
  int previousStride = 1;
  for (int dim = N-1 ; dim >= 0 ; dim--) {
    retRef  &=  arr.stride(dim) == previousStride;
    previousStride *= arr.extent(dim);
  }
  if (retRef)
    return arr;
  blitz::Array<T,N> retArr(arr.shape());
  if (preserve)
    retArr = arr;
  return retArr;
}

template<class T, int N>
inline bool areSame(const blitz::Array<T,N> & arr1,
                    const blitz::Array<T,N> & arr2) {
  return arr1.data()   == arr2.data()   &&
         arr1.stride() == arr2.stride() &&
         arr1.shape()  == arr2.shape();
}



struct Binn {
  uint x;      ///< X binning
  uint y;       ///< Y binning
  inline Binn(uint _x=1, uint _y=1)
    : x(_x), y(_y)
  {}
  explicit operator bool() const {
    return x != 1 || y != 1;
  }
  explicit operator std::string() const {
    return x == y ? toString(x) : toString ("%ux%u", x, y);
  }
};

/// \brief Compare binnings.
///
/// @param bn1 first binning.
/// @param bn2 second binning.
///
/// @return \c true if the binnings are equal, \c false otherwise.
///
inline bool
operator==( const Binn & bn1, const Binn & bn2){
  return bn1.x == bn2.x && bn1.y == bn2.y;
}

/// \brief Compare binnings.
///
/// @param bn1 first binning.
/// @param bn2 second binning.
///
/// @return \c true if the binnings are not equal, \c false otherwise.
///
inline bool
operator!=( const Binn & bn1, const Binn & bn2){
  return bn1.x != bn2.x || bn1.y != bn2.y;
}



std::string
type_desc (Binn*);

int
_conversion (Binn* _val, const std::string & in);

extern const std::string
BinnOptionDesc;


class BinnProc {
private:
  void * guts;
public:
  BinnProc(const Shape<2> & ish, const Binn & bnn);
  ~BinnProc();
  void operator() (const Map & imap, Map & omap);
};

inline uint binn(uint sz, uint bn) {
  //return bn ? (sz + bn - 1) / bn : 1;
  return bn ? sz/bn : 1 ;
}

inline Shape<2> binn(const Shape<2> & sh, const Binn & bnn) {
  return Shape<2>(binn(sh(0),bnn.y), binn(sh(1),bnn.x) );
}

inline void binn(const Map & imap, Map & omap, const Binn & bnn) {
  BinnProc aa(imap.shape(), bnn);
  BinnProc(imap.shape(), bnn)(imap, omap);
}

inline void binn(Map & iomap, const Binn & bnn) {
  BinnProc(iomap.shape(), bnn)(iomap, iomap);
}







struct Binn3 {
  unsigned int x;      ///< X binning
  unsigned int y;       ///< Y binning
  unsigned int z;       ///< Z binning
  inline Binn3(unsigned int _x=1, unsigned int _y=1, unsigned int _z=1)
    : x(_x), y(_y), z(_z) {}
  inline operator Binn() const {
    return Binn(x,y);
  }
};

inline std::string toString (const Binn3 & bnn) {
  return bnn.x == bnn.y && bnn.x == bnn.z ? toString(bnn.x) : toString ("%ux%ux%u", bnn.x, bnn.y, bnn.z);
}

/// \brief Compare binnings.
///
/// @param bn1 first binning.
/// @param bn2 second binning.
///
/// @return \c true if the binnings are equal, \c false otherwise.
///
inline bool
operator==( const Binn3 & bn1, const Binn3 & bn2){
  return bn1.x == bn2.x && bn1.y == bn2.y && bn1.z == bn2.z;
}

/// \brief Compare binnings.
///
/// @param bn1 first binning.
/// @param bn2 second binning.
///
/// @return \c true if the binnings are not equal, \c false otherwise.
///
inline bool
operator!=( const Binn3 & bn1, const Binn3 & bn2){
  return bn1.x != bn2.x || bn1.y != bn2.y || bn1.z != bn2.z;
}


std::string
type_desc (Binn3*);

int
_conversion (Binn3* _val, const std::string & in);

extern const std::string
Binn3OptionDesc;

/// \brief Apply binning to the array.
///
/// @param inarr Input array.
/// @param outarr Output array.
/// @param binn Binning factor
///
void
binn(const Volume & inarr, Volume & outarr, const Binn3 & bnn);

/// \brief Apply binning to the array.
///
/// @param io_arr Input/output array.
/// @param binn Binning factor
///
void
binn(Volume & io_arr, const Binn3 & bnn);




Shape<2> rotate(const Shape<2> & sh, float angle);

/// \brief Rotate the array.
///
/// @param inarr Input array.
/// @param outarr Output array. Input and output must be different arrays.
/// @param angle Rotation angle.
/// @param bg Values for the pixels in the resulting image not existing in
///        original.
///
void
rotate(const Map & inarr, Map & outarr, float angle, float bg=NAN);

/// \brief Rotate the array.
///
/// @param io_arr Input/output array.
/// @param angle Rotation angle.
/// @param bg Values for the pixels in the resulting image not existing in
///        original.
///
void
rotate(Map & io_arr, float angle, float bg=NAN);




#endif // MATRIX_H
