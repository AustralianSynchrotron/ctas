#ifndef MATRIX_H
#define MATRIX_H

#include "common.world.h"
#include <algorithm>
#include <list>
#include <numeric>

#define BZ_THREADSAFE
//#define BZ_DEBUG_LOG_REFERENCES
#include "../blitz-long/blitz/array.h"
//#include <blitz/array.h>



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
blitz::TinyVector<Tt,DimT> & copyMost( const blitz::TinyVector<Ts,DimS> & source
                                     ,       blitz::TinyVector<Tt,DimT> & target) {
  for (int dim = 0; dim < DimT && dim < DimS; ++dim)
    target(DimT-1-dim) = source(DimS-1-dim);
  return target;
} ;

template<int DimT, int DimS, class T>
blitz::TinyVector<T,DimT> copyMost(const blitz::TinyVector<T,DimS> & source) {
  blitz::TinyVector<T,DimT> toRet;
  return copyMost(source, toRet);
} ;



template<int Dim>
using ArrayF = blitz::Array<float,Dim>;

/// \brief 1D line with data.
///
/// One dimensional array of the ::float elements.
/// Used for filters, sinogram's lines and so on.
typedef ArrayF<1> Line;

/// \brief 2D Array with data.
///
/// Two dimensional array of the ::float elements.
/// Used for sinograms, input and output images etc.
typedef ArrayF<2> Map;
static const Map defaultMap;

/// \brief 3D Array with data.
///
/// Three dimensional array of the ::float elements.
typedef ArrayF<3> Volume;

static blitz::firstIndex  i0;
static blitz::secondIndex i1;

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





template<int Dim>
using Shape = blitz::TinyVector<ssize_t,Dim>;

//template<int Dim>
//struct Shape
//  : public blitz::TinyVector<ssize_t,Dim>
//{
//  using blitz::TinyVector<ssize_t,Dim>::TinyVector;
//  Shape() : blitz::TinyVector<ssize_t,Dim>(ssize_t(0)) {}
//  Shape(const blitz::TinyVector<ssize_t,Dim> & other) : blitz::TinyVector<ssize_t,Dim>(other) {}
//  Shape(ssize_t lngth) : blitz::TinyVector<ssize_t,1>(lngth) {}
//  Shape(ssize_t hght, ssize_t wdth) : blitz::TinyVector<ssize_t,2>(hght,wdth) {}
//  Shape(ssize_t dpth, ssize_t hght, ssize_t wdth) : blitz::TinyVector<ssize_t,3>(dpth,hght,wdth) {}
//  template <class T>
//  Shape(const blitz::TinyVector<T,Dim> & other) : blitz::TinyVector<ssize_t,Dim>(other) {}
//  template <class T>
//  Shape& operator=(const blitz::TinyVector<T,Dim> & other) {
//    blitz::TinyVector<ssize_t,Dim>::operator=(other);
//    return *this;
//  }
//  // above template does not instantinate automatically to produce below assign operator
//  Shape& operator=(const Shape & other) { return this->operator=<ssize_t>(other); }
//};

template<int Dim>
ssize_t size(const Shape<Dim> sh) {
  return std::accumulate(whole(sh), 1, std::multiplies<ssize_t>());
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
  Crop() : blitz::TinyVector<Segment,Dim>() {}
  Crop(const blitz::TinyVector<Segment,Dim> & other) : blitz::TinyVector<Segment,Dim>(other) {}
  Crop(const std::string & str) ;
  explicit operator bool() const { return std::any_of( whole(*this), [](const Segment & seg){return bool(seg);}); }
  Shape<Dim> apply(const Shape<Dim> & ish) const ;
  ArrayF<Dim> apply(const ArrayF<Dim> & iarr) const ;
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
class Binn : public blitz::TinyVector<ssize_t,Dim> {
  using blitz::TinyVector<ssize_t,Dim>::TinyVector; // negative flips
private:
  static const std::string modname;
  mutable void * guts = 0;
  Binn<Dim> flipped() const;
  void subapply(const ArrayF<Dim> & iarr, ArrayF<Dim> & oarr) const;
  bool flipOnly() const { return std::all_of( whole(*this), [](const ssize_t & bnn){return std::abs(bnn) == 1;}); }
public:
  Binn() : blitz::TinyVector<ssize_t,Dim>(1) {}
  Binn(const blitz::TinyVector<ssize_t,Dim> & other) : blitz::TinyVector<ssize_t,Dim>(other) {  }
  Binn(const std::string & str);
  ~Binn() { if (guts) specialize(); }
  Binn<Dim> & operator=(const Binn & other);
  explicit operator bool() const { return std::any_of( whole(*this), [](const ssize_t & bnn){return bnn != 1;}); }
  void specialize(const Shape<Dim> ish = Shape<Dim>()) const;
  Shape<Dim> apply(const Shape<Dim> & ish) const ;
  const ArrayF<Dim> apply(const ArrayF<Dim> & iarr) const ;
  void apply(const ArrayF<Dim> & inarr, ArrayF<Dim> & outarr) const ;
};


inline ssize_t binnOne(ssize_t sz, ssize_t bnn) {
  return bnn ? (sz + abs(bnn) - 1) / abs(bnn)  :  1 ;
}

template<int Dim>
std::string type_desc (Binn<Dim>*) {
  std::string toRet = "INT";
  if (Dim>1)
    toRet += "[," + toString( blitz::TinyVector<std::string,Dim-1>("INT") ) + "]";
  return toRet;
}

template<int Dim>
int _conversion (Binn<Dim>* _val, const std::string & in) {
  try { *_val = Binn<Dim>(in); }
  catch (...) {return -1;}
  return 1;
}


extern const std::string
BinnOptionDesc;


template<int Dim>
class PointF : public blitz::TinyVector<float,Dim> {
  using blitz::TinyVector<float,Dim>::TinyVector;
public:
  PointF() : blitz::TinyVector<float,Dim>(0.0f) {}
};

template<int Dim>
std::string type_desc (PointF<Dim>*) {
  return toString( blitz::TinyVector<std::string,Dim>("FLOAT") );
}

template<int Dim>
int _conversion (PointF<Dim>* _val, const std::string & in) {
  if (in.empty()) {
    *_val = PointF<Dim>();
    return 1;
  }
  std::deque<std::string> subs = split(in, ",");
  const std::string modname = toString(Dim)+"D-point";
  if ( Dim != subs.size() ) {
    warn(modname, "Could not parse string \""+in+"\". Must contain " + toString(Dim)
                  + " comma-delimited float numbers, while " + toString(subs.size()) + " found.");
    return -1;
  }
  float flt;
  for (uint curD=0; curD<Dim; ++curD) {
    const std::string & curStr = subs.at(curD);
    if ( 1 != sscanf( curStr.c_str(), "%f", &flt ) ) {
      warn(modname, "Could not parse string \""+curStr+"\" as float point value."
                    " for dimension " + toString(curD) + ".");
      return -1;
    }
    // Dim-1-curD here is to reflect the fact that natural x,y,z... in matrix notaition has inverted order
    (*_val)(Dim-1-curD) = flt;
  }
  return 1;
}




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
