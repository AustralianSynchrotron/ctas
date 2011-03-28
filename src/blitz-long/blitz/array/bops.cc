/***************************************************************************
 * blitz/../array/bops.cc	Array expression templates (2 operands)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Suggestions:          blitz-suggest@cybervision.com
 * Bugs:                 blitz-bugs@cybervision.com
 *
 * For more information, please see the Blitz++ Home Page:
 *    http://seurat.uwaterloo.ca/blitz/
 *
 ***************************************************************************
 *
 */ 

// Generated source file.  Do not edit. 
// genarrbops.cpp May  8 2009 16:45:13

#ifndef BZ_ARRAYBOPS_CC
#define BZ_ARRAYBOPS_CC

#ifndef BZ_ARRAYEXPR_H
 #error <blitz/array/bops.cc> must be included after <blitz/arrayexpr.h>
#endif

BZ_NAMESPACE(blitz)

/****************************************************************************
 * Addition Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> + Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Add<T_numtype1, T_numtype2 > > >
operator+(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Add<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> + _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      Add<T_numtype1, typename P_expr2::T_numtype > > >
operator+(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      Add<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> + IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      Add<T_numtype1, ptrdiff_t > > >
operator+(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      Add<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> + int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      Add<T_numtype1, int > > >
operator+(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      Add<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> + float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      Add<T_numtype1, float > > >
operator+(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      Add<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> + double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      Add<T_numtype1, double > > >
operator+(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      Add<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> + long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      Add<T_numtype1, long double > > >
operator+(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      Add<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> + complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Add<T_numtype1, complex<T2>  > > >
operator+(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Add<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> + ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Add<T_numtype1, ptrdiff_t > > >
operator+(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Add<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> + Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Add<typename P_expr1::T_numtype, T_numtype2 > > >
operator+(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Add<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> + _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      Add<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator+(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      Add<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> + IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      Add<typename P_expr1::T_numtype, ptrdiff_t > > >
operator+(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      Add<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> + int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      Add<typename P_expr1::T_numtype, int > > >
operator+(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      Add<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> + float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      Add<typename P_expr1::T_numtype, float > > >
operator+(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      Add<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> + double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      Add<typename P_expr1::T_numtype, double > > >
operator+(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      Add<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> + long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      Add<typename P_expr1::T_numtype, long double > > >
operator+(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      Add<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> + complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Add<typename P_expr1::T_numtype, complex<T2>  > > >
operator+(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Add<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> + ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Add<typename P_expr1::T_numtype, ptrdiff_t > > >
operator+(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Add<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> + Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Add<ptrdiff_t, T_numtype2 > > >
operator+(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Add<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> + _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      Add<ptrdiff_t, typename P_expr2::T_numtype > > >
operator+(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      Add<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> + IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      Add<ptrdiff_t, ptrdiff_t > > >
operator+(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      Add<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> + int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      Add<ptrdiff_t, int > > >
operator+(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      Add<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> + float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      Add<ptrdiff_t, float > > >
operator+(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      Add<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> + double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      Add<ptrdiff_t, double > > >
operator+(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      Add<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> + long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      Add<ptrdiff_t, long double > > >
operator+(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      Add<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> + complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Add<ptrdiff_t, complex<T2>  > > >
operator+(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Add<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> + ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Add<ptrdiff_t, ptrdiff_t > > >
operator+(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Add<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int + Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Add<int, T_numtype2 > > >
operator+(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Add<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int + _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      Add<int, typename P_expr2::T_numtype > > >
operator+(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      Add<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int + IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      Add<int, ptrdiff_t > > >
operator+(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      Add<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float + Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Add<float, T_numtype2 > > >
operator+(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Add<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float + _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      Add<float, typename P_expr2::T_numtype > > >
operator+(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      Add<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float + IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      Add<float, ptrdiff_t > > >
operator+(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      Add<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double + Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Add<double, T_numtype2 > > >
operator+(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Add<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double + _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      Add<double, typename P_expr2::T_numtype > > >
operator+(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      Add<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double + IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      Add<double, ptrdiff_t > > >
operator+(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      Add<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double + Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Add<long double, T_numtype2 > > >
operator+(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Add<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double + _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      Add<long double, typename P_expr2::T_numtype > > >
operator+(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      Add<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double + IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      Add<long double, ptrdiff_t > > >
operator+(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      Add<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> + Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      Add<complex<T1> , T_numtype2 > > >
operator+(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      Add<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> + _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      Add<complex<T1> , typename P_expr2::T_numtype > > >
operator+(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      Add<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> + IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      Add<complex<T1> , ptrdiff_t > > >
operator+(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      Add<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t + Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Add<ptrdiff_t, T_numtype2 > > >
operator+(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Add<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t + _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      Add<ptrdiff_t, typename P_expr2::T_numtype > > >
operator+(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      Add<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t + IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      Add<ptrdiff_t, ptrdiff_t > > >
operator+(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      Add<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Subtraction Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> - Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Subtract<T_numtype1, T_numtype2 > > >
operator-(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Subtract<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> - _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      Subtract<T_numtype1, typename P_expr2::T_numtype > > >
operator-(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      Subtract<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> - IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      Subtract<T_numtype1, ptrdiff_t > > >
operator-(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      Subtract<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> - int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      Subtract<T_numtype1, int > > >
operator-(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      Subtract<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> - float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      Subtract<T_numtype1, float > > >
operator-(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      Subtract<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> - double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      Subtract<T_numtype1, double > > >
operator-(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      Subtract<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> - long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      Subtract<T_numtype1, long double > > >
operator-(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      Subtract<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> - complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Subtract<T_numtype1, complex<T2>  > > >
operator-(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Subtract<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> - ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Subtract<T_numtype1, ptrdiff_t > > >
operator-(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Subtract<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> - Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Subtract<typename P_expr1::T_numtype, T_numtype2 > > >
operator-(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Subtract<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> - _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      Subtract<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator-(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      Subtract<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> - IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      Subtract<typename P_expr1::T_numtype, ptrdiff_t > > >
operator-(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      Subtract<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> - int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      Subtract<typename P_expr1::T_numtype, int > > >
operator-(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      Subtract<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> - float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      Subtract<typename P_expr1::T_numtype, float > > >
operator-(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      Subtract<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> - double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      Subtract<typename P_expr1::T_numtype, double > > >
operator-(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      Subtract<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> - long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      Subtract<typename P_expr1::T_numtype, long double > > >
operator-(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      Subtract<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> - complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Subtract<typename P_expr1::T_numtype, complex<T2>  > > >
operator-(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Subtract<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> - ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Subtract<typename P_expr1::T_numtype, ptrdiff_t > > >
operator-(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Subtract<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> - Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Subtract<ptrdiff_t, T_numtype2 > > >
operator-(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Subtract<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> - _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      Subtract<ptrdiff_t, typename P_expr2::T_numtype > > >
operator-(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      Subtract<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> - IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      Subtract<ptrdiff_t, ptrdiff_t > > >
operator-(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      Subtract<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> - int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      Subtract<ptrdiff_t, int > > >
operator-(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      Subtract<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> - float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      Subtract<ptrdiff_t, float > > >
operator-(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      Subtract<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> - double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      Subtract<ptrdiff_t, double > > >
operator-(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      Subtract<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> - long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      Subtract<ptrdiff_t, long double > > >
operator-(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      Subtract<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> - complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Subtract<ptrdiff_t, complex<T2>  > > >
operator-(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Subtract<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> - ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Subtract<ptrdiff_t, ptrdiff_t > > >
operator-(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Subtract<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int - Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Subtract<int, T_numtype2 > > >
operator-(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Subtract<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int - _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      Subtract<int, typename P_expr2::T_numtype > > >
operator-(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      Subtract<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int - IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      Subtract<int, ptrdiff_t > > >
operator-(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      Subtract<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float - Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Subtract<float, T_numtype2 > > >
operator-(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Subtract<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float - _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      Subtract<float, typename P_expr2::T_numtype > > >
operator-(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      Subtract<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float - IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      Subtract<float, ptrdiff_t > > >
operator-(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      Subtract<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double - Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Subtract<double, T_numtype2 > > >
operator-(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Subtract<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double - _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      Subtract<double, typename P_expr2::T_numtype > > >
operator-(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      Subtract<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double - IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      Subtract<double, ptrdiff_t > > >
operator-(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      Subtract<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double - Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Subtract<long double, T_numtype2 > > >
operator-(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Subtract<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double - _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      Subtract<long double, typename P_expr2::T_numtype > > >
operator-(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      Subtract<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double - IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      Subtract<long double, ptrdiff_t > > >
operator-(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      Subtract<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> - Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      Subtract<complex<T1> , T_numtype2 > > >
operator-(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      Subtract<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> - _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      Subtract<complex<T1> , typename P_expr2::T_numtype > > >
operator-(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      Subtract<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> - IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      Subtract<complex<T1> , ptrdiff_t > > >
operator-(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      Subtract<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t - Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Subtract<ptrdiff_t, T_numtype2 > > >
operator-(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Subtract<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t - _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      Subtract<ptrdiff_t, typename P_expr2::T_numtype > > >
operator-(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      Subtract<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t - IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      Subtract<ptrdiff_t, ptrdiff_t > > >
operator-(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      Subtract<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Multiplication Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> * Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Multiply<T_numtype1, T_numtype2 > > >
operator*(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Multiply<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> * _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      Multiply<T_numtype1, typename P_expr2::T_numtype > > >
operator*(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      Multiply<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> * IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      Multiply<T_numtype1, ptrdiff_t > > >
operator*(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      Multiply<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> * int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      Multiply<T_numtype1, int > > >
operator*(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      Multiply<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> * float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      Multiply<T_numtype1, float > > >
operator*(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      Multiply<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> * double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      Multiply<T_numtype1, double > > >
operator*(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      Multiply<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> * long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      Multiply<T_numtype1, long double > > >
operator*(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      Multiply<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> * complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Multiply<T_numtype1, complex<T2>  > > >
operator*(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Multiply<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> * ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Multiply<T_numtype1, ptrdiff_t > > >
operator*(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Multiply<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> * Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Multiply<typename P_expr1::T_numtype, T_numtype2 > > >
operator*(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Multiply<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> * _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      Multiply<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator*(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      Multiply<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> * IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      Multiply<typename P_expr1::T_numtype, ptrdiff_t > > >
operator*(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      Multiply<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> * int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      Multiply<typename P_expr1::T_numtype, int > > >
operator*(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      Multiply<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> * float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      Multiply<typename P_expr1::T_numtype, float > > >
operator*(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      Multiply<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> * double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      Multiply<typename P_expr1::T_numtype, double > > >
operator*(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      Multiply<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> * long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      Multiply<typename P_expr1::T_numtype, long double > > >
operator*(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      Multiply<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> * complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Multiply<typename P_expr1::T_numtype, complex<T2>  > > >
operator*(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Multiply<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> * ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Multiply<typename P_expr1::T_numtype, ptrdiff_t > > >
operator*(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Multiply<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> * Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Multiply<ptrdiff_t, T_numtype2 > > >
operator*(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Multiply<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> * _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      Multiply<ptrdiff_t, typename P_expr2::T_numtype > > >
operator*(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      Multiply<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> * IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      Multiply<ptrdiff_t, ptrdiff_t > > >
operator*(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      Multiply<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> * int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      Multiply<ptrdiff_t, int > > >
operator*(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      Multiply<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> * float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      Multiply<ptrdiff_t, float > > >
operator*(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      Multiply<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> * double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      Multiply<ptrdiff_t, double > > >
operator*(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      Multiply<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> * long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      Multiply<ptrdiff_t, long double > > >
operator*(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      Multiply<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> * complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Multiply<ptrdiff_t, complex<T2>  > > >
operator*(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Multiply<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> * ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Multiply<ptrdiff_t, ptrdiff_t > > >
operator*(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Multiply<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int * Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Multiply<int, T_numtype2 > > >
operator*(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Multiply<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int * _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      Multiply<int, typename P_expr2::T_numtype > > >
operator*(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      Multiply<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int * IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      Multiply<int, ptrdiff_t > > >
operator*(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      Multiply<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float * Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Multiply<float, T_numtype2 > > >
operator*(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Multiply<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float * _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      Multiply<float, typename P_expr2::T_numtype > > >
operator*(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      Multiply<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float * IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      Multiply<float, ptrdiff_t > > >
operator*(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      Multiply<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double * Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Multiply<double, T_numtype2 > > >
operator*(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Multiply<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double * _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      Multiply<double, typename P_expr2::T_numtype > > >
operator*(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      Multiply<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double * IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      Multiply<double, ptrdiff_t > > >
operator*(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      Multiply<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double * Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Multiply<long double, T_numtype2 > > >
operator*(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Multiply<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double * _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      Multiply<long double, typename P_expr2::T_numtype > > >
operator*(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      Multiply<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double * IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      Multiply<long double, ptrdiff_t > > >
operator*(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      Multiply<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> * Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      Multiply<complex<T1> , T_numtype2 > > >
operator*(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      Multiply<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> * _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      Multiply<complex<T1> , typename P_expr2::T_numtype > > >
operator*(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      Multiply<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> * IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      Multiply<complex<T1> , ptrdiff_t > > >
operator*(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      Multiply<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t * Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Multiply<ptrdiff_t, T_numtype2 > > >
operator*(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Multiply<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t * _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      Multiply<ptrdiff_t, typename P_expr2::T_numtype > > >
operator*(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      Multiply<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t * IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      Multiply<ptrdiff_t, ptrdiff_t > > >
operator*(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      Multiply<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Division Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> / Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Divide<T_numtype1, T_numtype2 > > >
operator/(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Divide<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> / _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      Divide<T_numtype1, typename P_expr2::T_numtype > > >
operator/(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      Divide<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> / IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      Divide<T_numtype1, ptrdiff_t > > >
operator/(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      Divide<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> / int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      Divide<T_numtype1, int > > >
operator/(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      Divide<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> / float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      Divide<T_numtype1, float > > >
operator/(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      Divide<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> / double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      Divide<T_numtype1, double > > >
operator/(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      Divide<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> / long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      Divide<T_numtype1, long double > > >
operator/(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      Divide<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> / complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Divide<T_numtype1, complex<T2>  > > >
operator/(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Divide<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> / ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Divide<T_numtype1, ptrdiff_t > > >
operator/(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Divide<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> / Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Divide<typename P_expr1::T_numtype, T_numtype2 > > >
operator/(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Divide<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> / _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      Divide<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator/(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      Divide<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> / IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      Divide<typename P_expr1::T_numtype, ptrdiff_t > > >
operator/(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      Divide<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> / int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      Divide<typename P_expr1::T_numtype, int > > >
operator/(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      Divide<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> / float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      Divide<typename P_expr1::T_numtype, float > > >
operator/(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      Divide<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> / double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      Divide<typename P_expr1::T_numtype, double > > >
operator/(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      Divide<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> / long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      Divide<typename P_expr1::T_numtype, long double > > >
operator/(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      Divide<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> / complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Divide<typename P_expr1::T_numtype, complex<T2>  > > >
operator/(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Divide<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> / ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Divide<typename P_expr1::T_numtype, ptrdiff_t > > >
operator/(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Divide<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> / Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Divide<ptrdiff_t, T_numtype2 > > >
operator/(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Divide<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> / _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      Divide<ptrdiff_t, typename P_expr2::T_numtype > > >
operator/(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      Divide<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> / IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      Divide<ptrdiff_t, ptrdiff_t > > >
operator/(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      Divide<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> / int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      Divide<ptrdiff_t, int > > >
operator/(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      Divide<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> / float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      Divide<ptrdiff_t, float > > >
operator/(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      Divide<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> / double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      Divide<ptrdiff_t, double > > >
operator/(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      Divide<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> / long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      Divide<ptrdiff_t, long double > > >
operator/(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      Divide<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> / complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Divide<ptrdiff_t, complex<T2>  > > >
operator/(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Divide<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> / ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Divide<ptrdiff_t, ptrdiff_t > > >
operator/(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Divide<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int / Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Divide<int, T_numtype2 > > >
operator/(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Divide<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int / _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      Divide<int, typename P_expr2::T_numtype > > >
operator/(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      Divide<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int / IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      Divide<int, ptrdiff_t > > >
operator/(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      Divide<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float / Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Divide<float, T_numtype2 > > >
operator/(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Divide<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float / _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      Divide<float, typename P_expr2::T_numtype > > >
operator/(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      Divide<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float / IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      Divide<float, ptrdiff_t > > >
operator/(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      Divide<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double / Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Divide<double, T_numtype2 > > >
operator/(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Divide<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double / _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      Divide<double, typename P_expr2::T_numtype > > >
operator/(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      Divide<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double / IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      Divide<double, ptrdiff_t > > >
operator/(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      Divide<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double / Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Divide<long double, T_numtype2 > > >
operator/(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Divide<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double / _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      Divide<long double, typename P_expr2::T_numtype > > >
operator/(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      Divide<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double / IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      Divide<long double, ptrdiff_t > > >
operator/(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      Divide<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> / Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      Divide<complex<T1> , T_numtype2 > > >
operator/(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      Divide<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> / _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      Divide<complex<T1> , typename P_expr2::T_numtype > > >
operator/(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      Divide<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> / IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      Divide<complex<T1> , ptrdiff_t > > >
operator/(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      Divide<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t / Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Divide<ptrdiff_t, T_numtype2 > > >
operator/(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Divide<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t / _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      Divide<ptrdiff_t, typename P_expr2::T_numtype > > >
operator/(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      Divide<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t / IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      Divide<ptrdiff_t, ptrdiff_t > > >
operator/(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      Divide<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Modulus Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> % Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Modulo<T_numtype1, T_numtype2 > > >
operator%(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Modulo<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> % _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      Modulo<T_numtype1, typename P_expr2::T_numtype > > >
operator%(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      Modulo<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> % IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      Modulo<T_numtype1, ptrdiff_t > > >
operator%(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      Modulo<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> % int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      Modulo<T_numtype1, int > > >
operator%(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      Modulo<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> % ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Modulo<T_numtype1, ptrdiff_t > > >
operator%(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Modulo<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> % Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Modulo<typename P_expr1::T_numtype, T_numtype2 > > >
operator%(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Modulo<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> % _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      Modulo<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator%(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      Modulo<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> % IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      Modulo<typename P_expr1::T_numtype, ptrdiff_t > > >
operator%(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      Modulo<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> % int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      Modulo<typename P_expr1::T_numtype, int > > >
operator%(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      Modulo<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> % ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Modulo<typename P_expr1::T_numtype, ptrdiff_t > > >
operator%(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Modulo<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> % Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Modulo<ptrdiff_t, T_numtype2 > > >
operator%(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Modulo<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> % _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      Modulo<ptrdiff_t, typename P_expr2::T_numtype > > >
operator%(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      Modulo<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> % IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      Modulo<ptrdiff_t, ptrdiff_t > > >
operator%(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      Modulo<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> % int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      Modulo<ptrdiff_t, int > > >
operator%(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      Modulo<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> % ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Modulo<ptrdiff_t, ptrdiff_t > > >
operator%(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Modulo<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int % Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Modulo<int, T_numtype2 > > >
operator%(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Modulo<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int % _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      Modulo<int, typename P_expr2::T_numtype > > >
operator%(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      Modulo<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int % IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      Modulo<int, ptrdiff_t > > >
operator%(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      Modulo<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t % Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Modulo<ptrdiff_t, T_numtype2 > > >
operator%(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Modulo<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t % _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      Modulo<ptrdiff_t, typename P_expr2::T_numtype > > >
operator%(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      Modulo<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t % IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      Modulo<ptrdiff_t, ptrdiff_t > > >
operator%(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      Modulo<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Greater-than Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> > Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Greater<T_numtype1, T_numtype2 > > >
operator>(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Greater<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> > _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      Greater<T_numtype1, typename P_expr2::T_numtype > > >
operator>(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      Greater<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> > IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      Greater<T_numtype1, ptrdiff_t > > >
operator>(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      Greater<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> > int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      Greater<T_numtype1, int > > >
operator>(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      Greater<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> > float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      Greater<T_numtype1, float > > >
operator>(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      Greater<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> > double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      Greater<T_numtype1, double > > >
operator>(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      Greater<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> > long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      Greater<T_numtype1, long double > > >
operator>(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      Greater<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> > complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Greater<T_numtype1, complex<T2>  > > >
operator>(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Greater<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> > ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Greater<T_numtype1, ptrdiff_t > > >
operator>(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Greater<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> > Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Greater<typename P_expr1::T_numtype, T_numtype2 > > >
operator>(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Greater<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> > _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      Greater<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator>(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      Greater<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> > IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      Greater<typename P_expr1::T_numtype, ptrdiff_t > > >
operator>(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      Greater<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> > int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      Greater<typename P_expr1::T_numtype, int > > >
operator>(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      Greater<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> > float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      Greater<typename P_expr1::T_numtype, float > > >
operator>(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      Greater<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> > double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      Greater<typename P_expr1::T_numtype, double > > >
operator>(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      Greater<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> > long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      Greater<typename P_expr1::T_numtype, long double > > >
operator>(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      Greater<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> > complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Greater<typename P_expr1::T_numtype, complex<T2>  > > >
operator>(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Greater<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> > ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Greater<typename P_expr1::T_numtype, ptrdiff_t > > >
operator>(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Greater<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> > Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Greater<ptrdiff_t, T_numtype2 > > >
operator>(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Greater<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> > _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      Greater<ptrdiff_t, typename P_expr2::T_numtype > > >
operator>(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      Greater<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> > IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      Greater<ptrdiff_t, ptrdiff_t > > >
operator>(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      Greater<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> > int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      Greater<ptrdiff_t, int > > >
operator>(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      Greater<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> > float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      Greater<ptrdiff_t, float > > >
operator>(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      Greater<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> > double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      Greater<ptrdiff_t, double > > >
operator>(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      Greater<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> > long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      Greater<ptrdiff_t, long double > > >
operator>(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      Greater<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> > complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Greater<ptrdiff_t, complex<T2>  > > >
operator>(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Greater<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> > ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Greater<ptrdiff_t, ptrdiff_t > > >
operator>(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Greater<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int > Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Greater<int, T_numtype2 > > >
operator>(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Greater<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int > _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      Greater<int, typename P_expr2::T_numtype > > >
operator>(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      Greater<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int > IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      Greater<int, ptrdiff_t > > >
operator>(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      Greater<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float > Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Greater<float, T_numtype2 > > >
operator>(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Greater<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float > _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      Greater<float, typename P_expr2::T_numtype > > >
operator>(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      Greater<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float > IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      Greater<float, ptrdiff_t > > >
operator>(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      Greater<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double > Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Greater<double, T_numtype2 > > >
operator>(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Greater<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double > _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      Greater<double, typename P_expr2::T_numtype > > >
operator>(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      Greater<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double > IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      Greater<double, ptrdiff_t > > >
operator>(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      Greater<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double > Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Greater<long double, T_numtype2 > > >
operator>(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Greater<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double > _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      Greater<long double, typename P_expr2::T_numtype > > >
operator>(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      Greater<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double > IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      Greater<long double, ptrdiff_t > > >
operator>(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      Greater<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> > Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      Greater<complex<T1> , T_numtype2 > > >
operator>(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      Greater<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> > _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      Greater<complex<T1> , typename P_expr2::T_numtype > > >
operator>(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      Greater<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> > IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      Greater<complex<T1> , ptrdiff_t > > >
operator>(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      Greater<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t > Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Greater<ptrdiff_t, T_numtype2 > > >
operator>(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Greater<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t > _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      Greater<ptrdiff_t, typename P_expr2::T_numtype > > >
operator>(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      Greater<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t > IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      Greater<ptrdiff_t, ptrdiff_t > > >
operator>(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      Greater<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Less-than Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> < Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Less<T_numtype1, T_numtype2 > > >
operator<(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Less<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> < _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      Less<T_numtype1, typename P_expr2::T_numtype > > >
operator<(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      Less<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> < IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      Less<T_numtype1, ptrdiff_t > > >
operator<(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      Less<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> < int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      Less<T_numtype1, int > > >
operator<(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      Less<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> < float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      Less<T_numtype1, float > > >
operator<(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      Less<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> < double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      Less<T_numtype1, double > > >
operator<(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      Less<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> < long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      Less<T_numtype1, long double > > >
operator<(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      Less<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> < complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Less<T_numtype1, complex<T2>  > > >
operator<(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Less<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> < ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Less<T_numtype1, ptrdiff_t > > >
operator<(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Less<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> < Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Less<typename P_expr1::T_numtype, T_numtype2 > > >
operator<(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Less<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> < _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      Less<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator<(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      Less<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> < IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      Less<typename P_expr1::T_numtype, ptrdiff_t > > >
operator<(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      Less<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> < int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      Less<typename P_expr1::T_numtype, int > > >
operator<(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      Less<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> < float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      Less<typename P_expr1::T_numtype, float > > >
operator<(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      Less<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> < double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      Less<typename P_expr1::T_numtype, double > > >
operator<(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      Less<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> < long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      Less<typename P_expr1::T_numtype, long double > > >
operator<(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      Less<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> < complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Less<typename P_expr1::T_numtype, complex<T2>  > > >
operator<(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Less<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> < ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Less<typename P_expr1::T_numtype, ptrdiff_t > > >
operator<(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Less<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> < Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Less<ptrdiff_t, T_numtype2 > > >
operator<(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Less<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> < _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      Less<ptrdiff_t, typename P_expr2::T_numtype > > >
operator<(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      Less<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> < IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      Less<ptrdiff_t, ptrdiff_t > > >
operator<(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      Less<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> < int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      Less<ptrdiff_t, int > > >
operator<(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      Less<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> < float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      Less<ptrdiff_t, float > > >
operator<(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      Less<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> < double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      Less<ptrdiff_t, double > > >
operator<(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      Less<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> < long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      Less<ptrdiff_t, long double > > >
operator<(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      Less<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> < complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Less<ptrdiff_t, complex<T2>  > > >
operator<(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Less<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> < ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Less<ptrdiff_t, ptrdiff_t > > >
operator<(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Less<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int < Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Less<int, T_numtype2 > > >
operator<(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Less<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int < _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      Less<int, typename P_expr2::T_numtype > > >
operator<(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      Less<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int < IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      Less<int, ptrdiff_t > > >
operator<(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      Less<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float < Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Less<float, T_numtype2 > > >
operator<(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Less<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float < _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      Less<float, typename P_expr2::T_numtype > > >
operator<(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      Less<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float < IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      Less<float, ptrdiff_t > > >
operator<(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      Less<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double < Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Less<double, T_numtype2 > > >
operator<(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Less<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double < _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      Less<double, typename P_expr2::T_numtype > > >
operator<(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      Less<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double < IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      Less<double, ptrdiff_t > > >
operator<(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      Less<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double < Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Less<long double, T_numtype2 > > >
operator<(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Less<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double < _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      Less<long double, typename P_expr2::T_numtype > > >
operator<(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      Less<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double < IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      Less<long double, ptrdiff_t > > >
operator<(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      Less<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> < Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      Less<complex<T1> , T_numtype2 > > >
operator<(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      Less<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> < _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      Less<complex<T1> , typename P_expr2::T_numtype > > >
operator<(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      Less<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> < IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      Less<complex<T1> , ptrdiff_t > > >
operator<(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      Less<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t < Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Less<ptrdiff_t, T_numtype2 > > >
operator<(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Less<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t < _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      Less<ptrdiff_t, typename P_expr2::T_numtype > > >
operator<(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      Less<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t < IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      Less<ptrdiff_t, ptrdiff_t > > >
operator<(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      Less<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Greater or equal (>=) operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> >= Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      GreaterOrEqual<T_numtype1, T_numtype2 > > >
operator>=(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      GreaterOrEqual<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> >= _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      GreaterOrEqual<T_numtype1, typename P_expr2::T_numtype > > >
operator>=(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      GreaterOrEqual<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> >= IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      GreaterOrEqual<T_numtype1, ptrdiff_t > > >
operator>=(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      GreaterOrEqual<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> >= int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      GreaterOrEqual<T_numtype1, int > > >
operator>=(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      GreaterOrEqual<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> >= float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      GreaterOrEqual<T_numtype1, float > > >
operator>=(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      GreaterOrEqual<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> >= double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      GreaterOrEqual<T_numtype1, double > > >
operator>=(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      GreaterOrEqual<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> >= long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      GreaterOrEqual<T_numtype1, long double > > >
operator>=(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      GreaterOrEqual<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> >= complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      GreaterOrEqual<T_numtype1, complex<T2>  > > >
operator>=(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      GreaterOrEqual<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> >= ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      GreaterOrEqual<T_numtype1, ptrdiff_t > > >
operator>=(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      GreaterOrEqual<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> >= Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      GreaterOrEqual<typename P_expr1::T_numtype, T_numtype2 > > >
operator>=(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      GreaterOrEqual<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> >= _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      GreaterOrEqual<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator>=(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      GreaterOrEqual<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> >= IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      GreaterOrEqual<typename P_expr1::T_numtype, ptrdiff_t > > >
operator>=(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      GreaterOrEqual<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> >= int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      GreaterOrEqual<typename P_expr1::T_numtype, int > > >
operator>=(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      GreaterOrEqual<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> >= float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      GreaterOrEqual<typename P_expr1::T_numtype, float > > >
operator>=(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      GreaterOrEqual<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> >= double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      GreaterOrEqual<typename P_expr1::T_numtype, double > > >
operator>=(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      GreaterOrEqual<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> >= long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      GreaterOrEqual<typename P_expr1::T_numtype, long double > > >
operator>=(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      GreaterOrEqual<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> >= complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      GreaterOrEqual<typename P_expr1::T_numtype, complex<T2>  > > >
operator>=(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      GreaterOrEqual<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> >= ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      GreaterOrEqual<typename P_expr1::T_numtype, ptrdiff_t > > >
operator>=(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      GreaterOrEqual<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> >= Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      GreaterOrEqual<ptrdiff_t, T_numtype2 > > >
operator>=(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      GreaterOrEqual<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> >= _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      GreaterOrEqual<ptrdiff_t, typename P_expr2::T_numtype > > >
operator>=(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      GreaterOrEqual<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> >= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      GreaterOrEqual<ptrdiff_t, ptrdiff_t > > >
operator>=(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      GreaterOrEqual<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> >= int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      GreaterOrEqual<ptrdiff_t, int > > >
operator>=(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      GreaterOrEqual<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> >= float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      GreaterOrEqual<ptrdiff_t, float > > >
operator>=(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      GreaterOrEqual<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> >= double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      GreaterOrEqual<ptrdiff_t, double > > >
operator>=(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      GreaterOrEqual<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> >= long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      GreaterOrEqual<ptrdiff_t, long double > > >
operator>=(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      GreaterOrEqual<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> >= complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      GreaterOrEqual<ptrdiff_t, complex<T2>  > > >
operator>=(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      GreaterOrEqual<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> >= ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      GreaterOrEqual<ptrdiff_t, ptrdiff_t > > >
operator>=(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      GreaterOrEqual<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int >= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      GreaterOrEqual<int, T_numtype2 > > >
operator>=(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      GreaterOrEqual<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int >= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      GreaterOrEqual<int, typename P_expr2::T_numtype > > >
operator>=(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      GreaterOrEqual<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int >= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      GreaterOrEqual<int, ptrdiff_t > > >
operator>=(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      GreaterOrEqual<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float >= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      GreaterOrEqual<float, T_numtype2 > > >
operator>=(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      GreaterOrEqual<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float >= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      GreaterOrEqual<float, typename P_expr2::T_numtype > > >
operator>=(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      GreaterOrEqual<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float >= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      GreaterOrEqual<float, ptrdiff_t > > >
operator>=(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      GreaterOrEqual<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double >= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      GreaterOrEqual<double, T_numtype2 > > >
operator>=(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      GreaterOrEqual<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double >= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      GreaterOrEqual<double, typename P_expr2::T_numtype > > >
operator>=(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      GreaterOrEqual<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double >= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      GreaterOrEqual<double, ptrdiff_t > > >
operator>=(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      GreaterOrEqual<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double >= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      GreaterOrEqual<long double, T_numtype2 > > >
operator>=(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      GreaterOrEqual<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double >= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      GreaterOrEqual<long double, typename P_expr2::T_numtype > > >
operator>=(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      GreaterOrEqual<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double >= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      GreaterOrEqual<long double, ptrdiff_t > > >
operator>=(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      GreaterOrEqual<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> >= Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      GreaterOrEqual<complex<T1> , T_numtype2 > > >
operator>=(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      GreaterOrEqual<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> >= _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      GreaterOrEqual<complex<T1> , typename P_expr2::T_numtype > > >
operator>=(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      GreaterOrEqual<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> >= IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      GreaterOrEqual<complex<T1> , ptrdiff_t > > >
operator>=(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      GreaterOrEqual<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t >= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      GreaterOrEqual<ptrdiff_t, T_numtype2 > > >
operator>=(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      GreaterOrEqual<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t >= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      GreaterOrEqual<ptrdiff_t, typename P_expr2::T_numtype > > >
operator>=(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      GreaterOrEqual<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t >= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      GreaterOrEqual<ptrdiff_t, ptrdiff_t > > >
operator>=(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      GreaterOrEqual<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Less or equal (<=) operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> <= Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LessOrEqual<T_numtype1, T_numtype2 > > >
operator<=(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LessOrEqual<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> <= _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      LessOrEqual<T_numtype1, typename P_expr2::T_numtype > > >
operator<=(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      LessOrEqual<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> <= IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      LessOrEqual<T_numtype1, ptrdiff_t > > >
operator<=(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      LessOrEqual<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> <= int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      LessOrEqual<T_numtype1, int > > >
operator<=(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      LessOrEqual<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> <= float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      LessOrEqual<T_numtype1, float > > >
operator<=(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      LessOrEqual<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> <= double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      LessOrEqual<T_numtype1, double > > >
operator<=(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      LessOrEqual<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> <= long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      LessOrEqual<T_numtype1, long double > > >
operator<=(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      LessOrEqual<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> <= complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      LessOrEqual<T_numtype1, complex<T2>  > > >
operator<=(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      LessOrEqual<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> <= ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      LessOrEqual<T_numtype1, ptrdiff_t > > >
operator<=(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      LessOrEqual<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> <= Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LessOrEqual<typename P_expr1::T_numtype, T_numtype2 > > >
operator<=(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LessOrEqual<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> <= _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      LessOrEqual<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator<=(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      LessOrEqual<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> <= IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      LessOrEqual<typename P_expr1::T_numtype, ptrdiff_t > > >
operator<=(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      LessOrEqual<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> <= int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      LessOrEqual<typename P_expr1::T_numtype, int > > >
operator<=(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      LessOrEqual<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> <= float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      LessOrEqual<typename P_expr1::T_numtype, float > > >
operator<=(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      LessOrEqual<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> <= double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      LessOrEqual<typename P_expr1::T_numtype, double > > >
operator<=(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      LessOrEqual<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> <= long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      LessOrEqual<typename P_expr1::T_numtype, long double > > >
operator<=(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      LessOrEqual<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> <= complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      LessOrEqual<typename P_expr1::T_numtype, complex<T2>  > > >
operator<=(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      LessOrEqual<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> <= ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      LessOrEqual<typename P_expr1::T_numtype, ptrdiff_t > > >
operator<=(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      LessOrEqual<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> <= Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LessOrEqual<ptrdiff_t, T_numtype2 > > >
operator<=(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LessOrEqual<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> <= _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      LessOrEqual<ptrdiff_t, typename P_expr2::T_numtype > > >
operator<=(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      LessOrEqual<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> <= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      LessOrEqual<ptrdiff_t, ptrdiff_t > > >
operator<=(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      LessOrEqual<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> <= int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      LessOrEqual<ptrdiff_t, int > > >
operator<=(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      LessOrEqual<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> <= float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      LessOrEqual<ptrdiff_t, float > > >
operator<=(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      LessOrEqual<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> <= double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      LessOrEqual<ptrdiff_t, double > > >
operator<=(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      LessOrEqual<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> <= long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      LessOrEqual<ptrdiff_t, long double > > >
operator<=(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      LessOrEqual<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> <= complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      LessOrEqual<ptrdiff_t, complex<T2>  > > >
operator<=(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      LessOrEqual<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> <= ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      LessOrEqual<ptrdiff_t, ptrdiff_t > > >
operator<=(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      LessOrEqual<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int <= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LessOrEqual<int, T_numtype2 > > >
operator<=(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LessOrEqual<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int <= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      LessOrEqual<int, typename P_expr2::T_numtype > > >
operator<=(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      LessOrEqual<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int <= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      LessOrEqual<int, ptrdiff_t > > >
operator<=(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      LessOrEqual<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float <= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LessOrEqual<float, T_numtype2 > > >
operator<=(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LessOrEqual<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float <= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      LessOrEqual<float, typename P_expr2::T_numtype > > >
operator<=(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      LessOrEqual<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float <= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      LessOrEqual<float, ptrdiff_t > > >
operator<=(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      LessOrEqual<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double <= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LessOrEqual<double, T_numtype2 > > >
operator<=(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LessOrEqual<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double <= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      LessOrEqual<double, typename P_expr2::T_numtype > > >
operator<=(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      LessOrEqual<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double <= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      LessOrEqual<double, ptrdiff_t > > >
operator<=(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      LessOrEqual<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double <= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LessOrEqual<long double, T_numtype2 > > >
operator<=(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LessOrEqual<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double <= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      LessOrEqual<long double, typename P_expr2::T_numtype > > >
operator<=(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      LessOrEqual<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double <= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      LessOrEqual<long double, ptrdiff_t > > >
operator<=(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      LessOrEqual<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> <= Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      LessOrEqual<complex<T1> , T_numtype2 > > >
operator<=(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      LessOrEqual<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> <= _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      LessOrEqual<complex<T1> , typename P_expr2::T_numtype > > >
operator<=(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      LessOrEqual<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> <= IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      LessOrEqual<complex<T1> , ptrdiff_t > > >
operator<=(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      LessOrEqual<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t <= Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LessOrEqual<ptrdiff_t, T_numtype2 > > >
operator<=(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LessOrEqual<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t <= _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      LessOrEqual<ptrdiff_t, typename P_expr2::T_numtype > > >
operator<=(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      LessOrEqual<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t <= IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      LessOrEqual<ptrdiff_t, ptrdiff_t > > >
operator<=(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      LessOrEqual<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Equality operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> == Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Equal<T_numtype1, T_numtype2 > > >
operator==(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Equal<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> == _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      Equal<T_numtype1, typename P_expr2::T_numtype > > >
operator==(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      Equal<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> == IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      Equal<T_numtype1, ptrdiff_t > > >
operator==(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      Equal<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> == int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      Equal<T_numtype1, int > > >
operator==(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      Equal<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> == float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      Equal<T_numtype1, float > > >
operator==(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      Equal<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> == double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      Equal<T_numtype1, double > > >
operator==(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      Equal<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> == long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      Equal<T_numtype1, long double > > >
operator==(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      Equal<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> == complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Equal<T_numtype1, complex<T2>  > > >
operator==(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Equal<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> == ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Equal<T_numtype1, ptrdiff_t > > >
operator==(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Equal<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> == Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Equal<typename P_expr1::T_numtype, T_numtype2 > > >
operator==(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Equal<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> == _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      Equal<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator==(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      Equal<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> == IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      Equal<typename P_expr1::T_numtype, ptrdiff_t > > >
operator==(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      Equal<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> == int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      Equal<typename P_expr1::T_numtype, int > > >
operator==(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      Equal<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> == float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      Equal<typename P_expr1::T_numtype, float > > >
operator==(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      Equal<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> == double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      Equal<typename P_expr1::T_numtype, double > > >
operator==(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      Equal<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> == long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      Equal<typename P_expr1::T_numtype, long double > > >
operator==(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      Equal<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> == complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Equal<typename P_expr1::T_numtype, complex<T2>  > > >
operator==(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Equal<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> == ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Equal<typename P_expr1::T_numtype, ptrdiff_t > > >
operator==(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Equal<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> == Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Equal<ptrdiff_t, T_numtype2 > > >
operator==(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Equal<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> == _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      Equal<ptrdiff_t, typename P_expr2::T_numtype > > >
operator==(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      Equal<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> == IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      Equal<ptrdiff_t, ptrdiff_t > > >
operator==(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      Equal<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> == int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      Equal<ptrdiff_t, int > > >
operator==(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      Equal<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> == float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      Equal<ptrdiff_t, float > > >
operator==(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      Equal<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> == double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      Equal<ptrdiff_t, double > > >
operator==(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      Equal<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> == long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      Equal<ptrdiff_t, long double > > >
operator==(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      Equal<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> == complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      Equal<ptrdiff_t, complex<T2>  > > >
operator==(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      Equal<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> == ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      Equal<ptrdiff_t, ptrdiff_t > > >
operator==(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      Equal<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int == Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Equal<int, T_numtype2 > > >
operator==(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Equal<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int == _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      Equal<int, typename P_expr2::T_numtype > > >
operator==(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      Equal<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int == IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      Equal<int, ptrdiff_t > > >
operator==(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      Equal<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float == Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Equal<float, T_numtype2 > > >
operator==(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Equal<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float == _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      Equal<float, typename P_expr2::T_numtype > > >
operator==(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      Equal<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float == IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      Equal<float, ptrdiff_t > > >
operator==(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      Equal<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double == Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Equal<double, T_numtype2 > > >
operator==(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Equal<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double == _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      Equal<double, typename P_expr2::T_numtype > > >
operator==(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      Equal<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double == IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      Equal<double, ptrdiff_t > > >
operator==(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      Equal<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double == Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Equal<long double, T_numtype2 > > >
operator==(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Equal<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double == _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      Equal<long double, typename P_expr2::T_numtype > > >
operator==(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      Equal<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double == IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      Equal<long double, ptrdiff_t > > >
operator==(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      Equal<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> == Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      Equal<complex<T1> , T_numtype2 > > >
operator==(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      Equal<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> == _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      Equal<complex<T1> , typename P_expr2::T_numtype > > >
operator==(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      Equal<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> == IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      Equal<complex<T1> , ptrdiff_t > > >
operator==(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      Equal<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t == Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      Equal<ptrdiff_t, T_numtype2 > > >
operator==(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      Equal<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t == _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      Equal<ptrdiff_t, typename P_expr2::T_numtype > > >
operator==(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      Equal<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t == IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      Equal<ptrdiff_t, ptrdiff_t > > >
operator==(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      Equal<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Not-equal operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> != Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      NotEqual<T_numtype1, T_numtype2 > > >
operator!=(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      NotEqual<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> != _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      NotEqual<T_numtype1, typename P_expr2::T_numtype > > >
operator!=(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      NotEqual<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> != IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      NotEqual<T_numtype1, ptrdiff_t > > >
operator!=(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      NotEqual<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> != int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      NotEqual<T_numtype1, int > > >
operator!=(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      NotEqual<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> != float
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>,
      NotEqual<T_numtype1, float > > >
operator!=(const Array<T_numtype1, N_rank1>& d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<float>, 
      NotEqual<T_numtype1, float> >
      (d1.begin(), 
      _bz_ArrayExprConstant<float>(d2));
}

// Array<T_numtype1, N_rank1> != double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>,
      NotEqual<T_numtype1, double > > >
operator!=(const Array<T_numtype1, N_rank1>& d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<double>, 
      NotEqual<T_numtype1, double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<double>(d2));
}

// Array<T_numtype1, N_rank1> != long double
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>,
      NotEqual<T_numtype1, long double > > >
operator!=(const Array<T_numtype1, N_rank1>& d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<long double>, 
      NotEqual<T_numtype1, long double> >
      (d1.begin(), 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// Array<T_numtype1, N_rank1> != complex<T2>
template<class T_numtype1, int N_rank1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      NotEqual<T_numtype1, complex<T2>  > > >
operator!=(const Array<T_numtype1, N_rank1>& d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      NotEqual<T_numtype1, complex<T2> > >
      (d1.begin(), 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// Array<T_numtype1, N_rank1> != ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      NotEqual<T_numtype1, ptrdiff_t > > >
operator!=(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      NotEqual<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> != Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      NotEqual<typename P_expr1::T_numtype, T_numtype2 > > >
operator!=(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      NotEqual<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> != _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      NotEqual<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator!=(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      NotEqual<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> != IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      NotEqual<typename P_expr1::T_numtype, ptrdiff_t > > >
operator!=(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      NotEqual<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> != int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      NotEqual<typename P_expr1::T_numtype, int > > >
operator!=(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      NotEqual<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> != float
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>,
      NotEqual<typename P_expr1::T_numtype, float > > >
operator!=(_bz_ArrayExpr<P_expr1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<float>, 
      NotEqual<typename P_expr1::T_numtype, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// _bz_ArrayExpr<P_expr1> != double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>,
      NotEqual<typename P_expr1::T_numtype, double > > >
operator!=(_bz_ArrayExpr<P_expr1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<double>, 
      NotEqual<typename P_expr1::T_numtype, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// _bz_ArrayExpr<P_expr1> != long double
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>,
      NotEqual<typename P_expr1::T_numtype, long double > > >
operator!=(_bz_ArrayExpr<P_expr1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<long double>, 
      NotEqual<typename P_expr1::T_numtype, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// _bz_ArrayExpr<P_expr1> != complex<T2>
template<class P_expr1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      NotEqual<typename P_expr1::T_numtype, complex<T2>  > > >
operator!=(_bz_ArrayExpr<P_expr1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      NotEqual<typename P_expr1::T_numtype, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// _bz_ArrayExpr<P_expr1> != ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      NotEqual<typename P_expr1::T_numtype, ptrdiff_t > > >
operator!=(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      NotEqual<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> != Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      NotEqual<ptrdiff_t, T_numtype2 > > >
operator!=(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      NotEqual<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> != _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      NotEqual<ptrdiff_t, typename P_expr2::T_numtype > > >
operator!=(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      NotEqual<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> != IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      NotEqual<ptrdiff_t, ptrdiff_t > > >
operator!=(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      NotEqual<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> != int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      NotEqual<ptrdiff_t, int > > >
operator!=(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      NotEqual<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> != float
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>,
      NotEqual<ptrdiff_t, float > > >
operator!=(IndexPlaceholder<N_index1> d1, 
      float d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<float>, 
      NotEqual<ptrdiff_t, float> >
      (d1, 
      _bz_ArrayExprConstant<float>(d2));
}

// IndexPlaceholder<N_index1> != double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>,
      NotEqual<ptrdiff_t, double > > >
operator!=(IndexPlaceholder<N_index1> d1, 
      double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<double>, 
      NotEqual<ptrdiff_t, double> >
      (d1, 
      _bz_ArrayExprConstant<double>(d2));
}

// IndexPlaceholder<N_index1> != long double
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>,
      NotEqual<ptrdiff_t, long double > > >
operator!=(IndexPlaceholder<N_index1> d1, 
      long double d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<long double>, 
      NotEqual<ptrdiff_t, long double> >
      (d1, 
      _bz_ArrayExprConstant<long double>(d2));
}

#ifdef BZ_HAVE_COMPLEX
// IndexPlaceholder<N_index1> != complex<T2>
template<ptrdiff_t N_index1, class T2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > ,
      NotEqual<ptrdiff_t, complex<T2>  > > >
operator!=(IndexPlaceholder<N_index1> d1, 
      complex<T2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<complex<T2> > , 
      NotEqual<ptrdiff_t, complex<T2> > >
      (d1, 
      _bz_ArrayExprConstant<complex<T2> > (d2));
}
#endif // BZ_HAVE_COMPLEX

// IndexPlaceholder<N_index1> != ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      NotEqual<ptrdiff_t, ptrdiff_t > > >
operator!=(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      NotEqual<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int != Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      NotEqual<int, T_numtype2 > > >
operator!=(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      NotEqual<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int != _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      NotEqual<int, typename P_expr2::T_numtype > > >
operator!=(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      NotEqual<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int != IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      NotEqual<int, ptrdiff_t > > >
operator!=(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      NotEqual<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// float != Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>,
      NotEqual<float, T_numtype2 > > >
operator!=(float d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      NotEqual<float, T_numtype2> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2.begin());
}

// float != _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>,
      NotEqual<float, typename P_expr2::T_numtype > > >
operator!=(float d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      _bz_ArrayExpr<P_expr2>, 
      NotEqual<float, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// float != IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>,
      NotEqual<float, ptrdiff_t > > >
operator!=(float d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<float>, 
      IndexPlaceholder<N_index2>, 
      NotEqual<float, ptrdiff_t> >
      (_bz_ArrayExprConstant<float>(d1), 
      d2);
}

// double != Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      NotEqual<double, T_numtype2 > > >
operator!=(double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      NotEqual<double, T_numtype2> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2.begin());
}

// double != _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>,
      NotEqual<double, typename P_expr2::T_numtype > > >
operator!=(double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      _bz_ArrayExpr<P_expr2>, 
      NotEqual<double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// double != IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>,
      NotEqual<double, ptrdiff_t > > >
operator!=(double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<double>, 
      IndexPlaceholder<N_index2>, 
      NotEqual<double, ptrdiff_t> >
      (_bz_ArrayExprConstant<double>(d1), 
      d2);
}

// long double != Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>,
      NotEqual<long double, T_numtype2 > > >
operator!=(long double d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      NotEqual<long double, T_numtype2> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2.begin());
}

// long double != _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>,
      NotEqual<long double, typename P_expr2::T_numtype > > >
operator!=(long double d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      _bz_ArrayExpr<P_expr2>, 
      NotEqual<long double, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

// long double != IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>,
      NotEqual<long double, ptrdiff_t > > >
operator!=(long double d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<long double>, 
      IndexPlaceholder<N_index2>, 
      NotEqual<long double, ptrdiff_t> >
      (_bz_ArrayExprConstant<long double>(d1), 
      d2);
}

#ifdef BZ_HAVE_COMPLEX
// complex<T1> != Array<T_numtype2, N_rank2>
template<class T1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>,
      NotEqual<complex<T1> , T_numtype2 > > >
operator!=(complex<T1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      ArrayIterator<T_numtype2, N_rank2>, 
      NotEqual<complex<T1> , T_numtype2> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2.begin());
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> != _bz_ArrayExpr<P_expr2>
template<class T1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>,
      NotEqual<complex<T1> , typename P_expr2::T_numtype > > >
operator!=(complex<T1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      _bz_ArrayExpr<P_expr2>, 
      NotEqual<complex<T1> , typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

#ifdef BZ_HAVE_COMPLEX
// complex<T1> != IndexPlaceholder<N_index2>
template<class T1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>,
      NotEqual<complex<T1> , ptrdiff_t > > >
operator!=(complex<T1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<complex<T1> > , 
      IndexPlaceholder<N_index2>, 
      NotEqual<complex<T1> , ptrdiff_t> >
      (_bz_ArrayExprConstant<complex<T1> > (d1), 
      d2);
}
#endif // BZ_HAVE_COMPLEX

// ptrdiff_t != Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      NotEqual<ptrdiff_t, T_numtype2 > > >
operator!=(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      NotEqual<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t != _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      NotEqual<ptrdiff_t, typename P_expr2::T_numtype > > >
operator!=(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      NotEqual<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t != IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      NotEqual<ptrdiff_t, ptrdiff_t > > >
operator!=(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      NotEqual<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Logical AND operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> && Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalAnd<T_numtype1, T_numtype2 > > >
operator&&(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalAnd<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> && _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalAnd<T_numtype1, typename P_expr2::T_numtype > > >
operator&&(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalAnd<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> && IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      LogicalAnd<T_numtype1, ptrdiff_t > > >
operator&&(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      LogicalAnd<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> && int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      LogicalAnd<T_numtype1, int > > >
operator&&(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      LogicalAnd<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> && ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      LogicalAnd<T_numtype1, ptrdiff_t > > >
operator&&(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      LogicalAnd<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> && Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalAnd<typename P_expr1::T_numtype, T_numtype2 > > >
operator&&(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalAnd<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> && _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalAnd<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator&&(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalAnd<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> && IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      LogicalAnd<typename P_expr1::T_numtype, ptrdiff_t > > >
operator&&(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      LogicalAnd<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> && int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      LogicalAnd<typename P_expr1::T_numtype, int > > >
operator&&(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      LogicalAnd<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> && ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      LogicalAnd<typename P_expr1::T_numtype, ptrdiff_t > > >
operator&&(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      LogicalAnd<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> && Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalAnd<ptrdiff_t, T_numtype2 > > >
operator&&(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalAnd<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> && _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalAnd<ptrdiff_t, typename P_expr2::T_numtype > > >
operator&&(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalAnd<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> && IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      LogicalAnd<ptrdiff_t, ptrdiff_t > > >
operator&&(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      LogicalAnd<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> && int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      LogicalAnd<ptrdiff_t, int > > >
operator&&(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      LogicalAnd<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> && ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      LogicalAnd<ptrdiff_t, ptrdiff_t > > >
operator&&(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      LogicalAnd<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int && Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalAnd<int, T_numtype2 > > >
operator&&(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalAnd<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int && _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalAnd<int, typename P_expr2::T_numtype > > >
operator&&(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalAnd<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int && IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      LogicalAnd<int, ptrdiff_t > > >
operator&&(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      LogicalAnd<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t && Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalAnd<ptrdiff_t, T_numtype2 > > >
operator&&(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalAnd<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t && _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalAnd<ptrdiff_t, typename P_expr2::T_numtype > > >
operator&&(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalAnd<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t && IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      LogicalAnd<ptrdiff_t, ptrdiff_t > > >
operator&&(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      LogicalAnd<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Logical OR operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> || Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalOr<T_numtype1, T_numtype2 > > >
operator||(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalOr<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> || _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalOr<T_numtype1, typename P_expr2::T_numtype > > >
operator||(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalOr<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> || IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      LogicalOr<T_numtype1, ptrdiff_t > > >
operator||(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      LogicalOr<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> || int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      LogicalOr<T_numtype1, int > > >
operator||(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      LogicalOr<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> || ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      LogicalOr<T_numtype1, ptrdiff_t > > >
operator||(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      LogicalOr<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> || Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalOr<typename P_expr1::T_numtype, T_numtype2 > > >
operator||(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalOr<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> || _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalOr<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator||(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalOr<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> || IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      LogicalOr<typename P_expr1::T_numtype, ptrdiff_t > > >
operator||(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      LogicalOr<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> || int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      LogicalOr<typename P_expr1::T_numtype, int > > >
operator||(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      LogicalOr<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> || ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      LogicalOr<typename P_expr1::T_numtype, ptrdiff_t > > >
operator||(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      LogicalOr<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> || Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalOr<ptrdiff_t, T_numtype2 > > >
operator||(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalOr<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> || _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalOr<ptrdiff_t, typename P_expr2::T_numtype > > >
operator||(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalOr<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> || IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      LogicalOr<ptrdiff_t, ptrdiff_t > > >
operator||(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      LogicalOr<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> || int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      LogicalOr<ptrdiff_t, int > > >
operator||(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      LogicalOr<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> || ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      LogicalOr<ptrdiff_t, ptrdiff_t > > >
operator||(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      LogicalOr<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int || Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalOr<int, T_numtype2 > > >
operator||(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalOr<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int || _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalOr<int, typename P_expr2::T_numtype > > >
operator||(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalOr<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int || IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      LogicalOr<int, ptrdiff_t > > >
operator||(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      LogicalOr<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t || Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      LogicalOr<ptrdiff_t, T_numtype2 > > >
operator||(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      LogicalOr<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t || _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      LogicalOr<ptrdiff_t, typename P_expr2::T_numtype > > >
operator||(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      LogicalOr<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t || IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      LogicalOr<ptrdiff_t, ptrdiff_t > > >
operator||(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      LogicalOr<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Bitwise XOR Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> ^ Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseXor<T_numtype1, T_numtype2 > > >
operator^(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseXor<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> ^ _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseXor<T_numtype1, typename P_expr2::T_numtype > > >
operator^(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseXor<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> ^ IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      BitwiseXor<T_numtype1, ptrdiff_t > > >
operator^(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      BitwiseXor<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> ^ int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      BitwiseXor<T_numtype1, int > > >
operator^(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      BitwiseXor<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> ^ ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      BitwiseXor<T_numtype1, ptrdiff_t > > >
operator^(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      BitwiseXor<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> ^ Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseXor<typename P_expr1::T_numtype, T_numtype2 > > >
operator^(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseXor<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> ^ _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseXor<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator^(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseXor<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> ^ IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      BitwiseXor<typename P_expr1::T_numtype, ptrdiff_t > > >
operator^(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      BitwiseXor<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> ^ int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      BitwiseXor<typename P_expr1::T_numtype, int > > >
operator^(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      BitwiseXor<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> ^ ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      BitwiseXor<typename P_expr1::T_numtype, ptrdiff_t > > >
operator^(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      BitwiseXor<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> ^ Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseXor<ptrdiff_t, T_numtype2 > > >
operator^(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseXor<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> ^ _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseXor<ptrdiff_t, typename P_expr2::T_numtype > > >
operator^(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseXor<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> ^ IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      BitwiseXor<ptrdiff_t, ptrdiff_t > > >
operator^(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      BitwiseXor<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> ^ int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      BitwiseXor<ptrdiff_t, int > > >
operator^(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      BitwiseXor<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> ^ ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      BitwiseXor<ptrdiff_t, ptrdiff_t > > >
operator^(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      BitwiseXor<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int ^ Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseXor<int, T_numtype2 > > >
operator^(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseXor<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int ^ _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseXor<int, typename P_expr2::T_numtype > > >
operator^(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseXor<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int ^ IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      BitwiseXor<int, ptrdiff_t > > >
operator^(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      BitwiseXor<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t ^ Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseXor<ptrdiff_t, T_numtype2 > > >
operator^(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseXor<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t ^ _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseXor<ptrdiff_t, typename P_expr2::T_numtype > > >
operator^(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseXor<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t ^ IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      BitwiseXor<ptrdiff_t, ptrdiff_t > > >
operator^(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      BitwiseXor<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Bitwise And Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> & Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseAnd<T_numtype1, T_numtype2 > > >
operator&(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseAnd<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> & _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseAnd<T_numtype1, typename P_expr2::T_numtype > > >
operator&(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseAnd<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> & IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      BitwiseAnd<T_numtype1, ptrdiff_t > > >
operator&(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      BitwiseAnd<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> & int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      BitwiseAnd<T_numtype1, int > > >
operator&(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      BitwiseAnd<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> & ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      BitwiseAnd<T_numtype1, ptrdiff_t > > >
operator&(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      BitwiseAnd<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> & Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseAnd<typename P_expr1::T_numtype, T_numtype2 > > >
operator&(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseAnd<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> & _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseAnd<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator&(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseAnd<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> & IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      BitwiseAnd<typename P_expr1::T_numtype, ptrdiff_t > > >
operator&(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      BitwiseAnd<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> & int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      BitwiseAnd<typename P_expr1::T_numtype, int > > >
operator&(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      BitwiseAnd<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> & ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      BitwiseAnd<typename P_expr1::T_numtype, ptrdiff_t > > >
operator&(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      BitwiseAnd<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> & Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseAnd<ptrdiff_t, T_numtype2 > > >
operator&(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseAnd<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> & _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseAnd<ptrdiff_t, typename P_expr2::T_numtype > > >
operator&(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseAnd<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> & IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      BitwiseAnd<ptrdiff_t, ptrdiff_t > > >
operator&(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      BitwiseAnd<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> & int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      BitwiseAnd<ptrdiff_t, int > > >
operator&(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      BitwiseAnd<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> & ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      BitwiseAnd<ptrdiff_t, ptrdiff_t > > >
operator&(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      BitwiseAnd<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int & Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseAnd<int, T_numtype2 > > >
operator&(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseAnd<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int & _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseAnd<int, typename P_expr2::T_numtype > > >
operator&(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseAnd<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int & IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      BitwiseAnd<int, ptrdiff_t > > >
operator&(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      BitwiseAnd<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t & Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseAnd<ptrdiff_t, T_numtype2 > > >
operator&(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseAnd<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t & _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseAnd<ptrdiff_t, typename P_expr2::T_numtype > > >
operator&(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseAnd<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t & IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      BitwiseAnd<ptrdiff_t, ptrdiff_t > > >
operator&(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      BitwiseAnd<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Bitwise Or Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> | Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseOr<T_numtype1, T_numtype2 > > >
operator|(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseOr<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> | _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseOr<T_numtype1, typename P_expr2::T_numtype > > >
operator|(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseOr<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> | IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      BitwiseOr<T_numtype1, ptrdiff_t > > >
operator|(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      BitwiseOr<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> | int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      BitwiseOr<T_numtype1, int > > >
operator|(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      BitwiseOr<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> | ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      BitwiseOr<T_numtype1, ptrdiff_t > > >
operator|(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      BitwiseOr<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> | Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseOr<typename P_expr1::T_numtype, T_numtype2 > > >
operator|(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseOr<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> | _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseOr<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator|(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseOr<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> | IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      BitwiseOr<typename P_expr1::T_numtype, ptrdiff_t > > >
operator|(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      BitwiseOr<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> | int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      BitwiseOr<typename P_expr1::T_numtype, int > > >
operator|(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      BitwiseOr<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> | ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      BitwiseOr<typename P_expr1::T_numtype, ptrdiff_t > > >
operator|(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      BitwiseOr<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> | Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseOr<ptrdiff_t, T_numtype2 > > >
operator|(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseOr<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> | _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseOr<ptrdiff_t, typename P_expr2::T_numtype > > >
operator|(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseOr<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> | IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      BitwiseOr<ptrdiff_t, ptrdiff_t > > >
operator|(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      BitwiseOr<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> | int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      BitwiseOr<ptrdiff_t, int > > >
operator|(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      BitwiseOr<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> | ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      BitwiseOr<ptrdiff_t, ptrdiff_t > > >
operator|(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      BitwiseOr<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int | Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseOr<int, T_numtype2 > > >
operator|(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseOr<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int | _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseOr<int, typename P_expr2::T_numtype > > >
operator|(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseOr<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int | IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      BitwiseOr<int, ptrdiff_t > > >
operator|(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      BitwiseOr<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t | Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      BitwiseOr<ptrdiff_t, T_numtype2 > > >
operator|(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      BitwiseOr<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t | _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      BitwiseOr<ptrdiff_t, typename P_expr2::T_numtype > > >
operator|(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      BitwiseOr<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t | IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      BitwiseOr<ptrdiff_t, ptrdiff_t > > >
operator|(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      BitwiseOr<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Shift right Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> >> Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftRight<T_numtype1, T_numtype2 > > >
operator>>(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftRight<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> >> _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftRight<T_numtype1, typename P_expr2::T_numtype > > >
operator>>(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftRight<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> >> IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      ShiftRight<T_numtype1, ptrdiff_t > > >
operator>>(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      ShiftRight<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> >> int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      ShiftRight<T_numtype1, int > > >
operator>>(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      ShiftRight<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> >> ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      ShiftRight<T_numtype1, ptrdiff_t > > >
operator>>(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      ShiftRight<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> >> Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftRight<typename P_expr1::T_numtype, T_numtype2 > > >
operator>>(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftRight<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> >> _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftRight<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator>>(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftRight<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> >> IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      ShiftRight<typename P_expr1::T_numtype, ptrdiff_t > > >
operator>>(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      ShiftRight<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> >> int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      ShiftRight<typename P_expr1::T_numtype, int > > >
operator>>(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      ShiftRight<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> >> ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      ShiftRight<typename P_expr1::T_numtype, ptrdiff_t > > >
operator>>(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      ShiftRight<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> >> Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftRight<ptrdiff_t, T_numtype2 > > >
operator>>(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftRight<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> >> _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftRight<ptrdiff_t, typename P_expr2::T_numtype > > >
operator>>(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftRight<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> >> IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      ShiftRight<ptrdiff_t, ptrdiff_t > > >
operator>>(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      ShiftRight<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> >> int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      ShiftRight<ptrdiff_t, int > > >
operator>>(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      ShiftRight<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> >> ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      ShiftRight<ptrdiff_t, ptrdiff_t > > >
operator>>(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      ShiftRight<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int >> Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftRight<int, T_numtype2 > > >
operator>>(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftRight<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int >> _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftRight<int, typename P_expr2::T_numtype > > >
operator>>(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftRight<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int >> IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      ShiftRight<int, ptrdiff_t > > >
operator>>(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      ShiftRight<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t >> Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftRight<ptrdiff_t, T_numtype2 > > >
operator>>(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftRight<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t >> _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftRight<ptrdiff_t, typename P_expr2::T_numtype > > >
operator>>(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftRight<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t >> IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      ShiftRight<ptrdiff_t, ptrdiff_t > > >
operator>>(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      ShiftRight<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Shift left Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> << Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftLeft<T_numtype1, T_numtype2 > > >
operator<<(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftLeft<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> << _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftLeft<T_numtype1, typename P_expr2::T_numtype > > >
operator<<(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftLeft<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> << IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      ShiftLeft<T_numtype1, ptrdiff_t > > >
operator<<(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      ShiftLeft<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> << int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      ShiftLeft<T_numtype1, int > > >
operator<<(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      ShiftLeft<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> << ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      ShiftLeft<T_numtype1, ptrdiff_t > > >
operator<<(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      ShiftLeft<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> << Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftLeft<typename P_expr1::T_numtype, T_numtype2 > > >
operator<<(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftLeft<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> << _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftLeft<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
operator<<(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftLeft<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> << IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      ShiftLeft<typename P_expr1::T_numtype, ptrdiff_t > > >
operator<<(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      ShiftLeft<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> << int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      ShiftLeft<typename P_expr1::T_numtype, int > > >
operator<<(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      ShiftLeft<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> << ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      ShiftLeft<typename P_expr1::T_numtype, ptrdiff_t > > >
operator<<(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      ShiftLeft<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> << Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftLeft<ptrdiff_t, T_numtype2 > > >
operator<<(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftLeft<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> << _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftLeft<ptrdiff_t, typename P_expr2::T_numtype > > >
operator<<(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftLeft<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> << IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      ShiftLeft<ptrdiff_t, ptrdiff_t > > >
operator<<(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      ShiftLeft<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> << int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      ShiftLeft<ptrdiff_t, int > > >
operator<<(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      ShiftLeft<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> << ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      ShiftLeft<ptrdiff_t, ptrdiff_t > > >
operator<<(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      ShiftLeft<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int << Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftLeft<int, T_numtype2 > > >
operator<<(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftLeft<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int << _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftLeft<int, typename P_expr2::T_numtype > > >
operator<<(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftLeft<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int << IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      ShiftLeft<int, ptrdiff_t > > >
operator<<(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      ShiftLeft<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t << Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      ShiftLeft<ptrdiff_t, T_numtype2 > > >
operator<<(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      ShiftLeft<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t << _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      ShiftLeft<ptrdiff_t, typename P_expr2::T_numtype > > >
operator<<(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      ShiftLeft<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t << IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      ShiftLeft<ptrdiff_t, ptrdiff_t > > >
operator<<(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      ShiftLeft<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Minimum Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> min Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Min<T_numtype1, T_numtype2 > > >
(min)(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Min<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> min _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Min<T_numtype1, typename P_expr2::T_numtype > > >
(min)(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Min<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> min IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      _bz_Min<T_numtype1, ptrdiff_t > > >
(min)(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      _bz_Min<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> min int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      _bz_Min<T_numtype1, int > > >
(min)(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      _bz_Min<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> min ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      _bz_Min<T_numtype1, ptrdiff_t > > >
(min)(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_Min<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> min Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Min<typename P_expr1::T_numtype, T_numtype2 > > >
(min)(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Min<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> min _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Min<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
(min)(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Min<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> min IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      _bz_Min<typename P_expr1::T_numtype, ptrdiff_t > > >
(min)(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      _bz_Min<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> min int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      _bz_Min<typename P_expr1::T_numtype, int > > >
(min)(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      _bz_Min<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> min ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      _bz_Min<typename P_expr1::T_numtype, ptrdiff_t > > >
(min)(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_Min<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> min Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Min<ptrdiff_t, T_numtype2 > > >
(min)(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Min<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> min _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Min<ptrdiff_t, typename P_expr2::T_numtype > > >
(min)(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Min<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> min IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      _bz_Min<ptrdiff_t, ptrdiff_t > > >
(min)(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      _bz_Min<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> min int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      _bz_Min<ptrdiff_t, int > > >
(min)(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      _bz_Min<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> min ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      _bz_Min<ptrdiff_t, ptrdiff_t > > >
(min)(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_Min<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int min Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Min<int, T_numtype2 > > >
(min)(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Min<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int min _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Min<int, typename P_expr2::T_numtype > > >
(min)(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Min<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int min IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      _bz_Min<int, ptrdiff_t > > >
(min)(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      _bz_Min<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t min Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Min<ptrdiff_t, T_numtype2 > > >
(min)(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Min<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t min _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Min<ptrdiff_t, typename P_expr2::T_numtype > > >
(min)(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Min<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t min IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      _bz_Min<ptrdiff_t, ptrdiff_t > > >
(min)(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      _bz_Min<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
/****************************************************************************
 * Maximum Operators
 ****************************************************************************/

// Array<T_numtype1, N_rank1> max Array<T_numtype2, N_rank2>
template<class T_numtype1, int N_rank1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Max<T_numtype1, T_numtype2 > > >
(max)(const Array<T_numtype1, N_rank1>& d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Max<T_numtype1, T_numtype2> >
      (d1.begin(), 
      d2.begin());
}

// Array<T_numtype1, N_rank1> max _bz_ArrayExpr<P_expr2>
template<class T_numtype1, int N_rank1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Max<T_numtype1, typename P_expr2::T_numtype > > >
(max)(const Array<T_numtype1, N_rank1>& d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Max<T_numtype1, typename P_expr2::T_numtype> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> max IndexPlaceholder<N_index2>
template<class T_numtype1, int N_rank1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>,
      _bz_Max<T_numtype1, ptrdiff_t > > >
(max)(const Array<T_numtype1, N_rank1>& d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      IndexPlaceholder<N_index2>, 
      _bz_Max<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      d2);
}

// Array<T_numtype1, N_rank1> max int
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>,
      _bz_Max<T_numtype1, int > > >
(max)(const Array<T_numtype1, N_rank1>& d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<int>, 
      _bz_Max<T_numtype1, int> >
      (d1.begin(), 
      _bz_ArrayExprConstant<int>(d2));
}

// Array<T_numtype1, N_rank1> max ptrdiff_t
template<class T_numtype1, int N_rank1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      _bz_Max<T_numtype1, ptrdiff_t > > >
(max)(const Array<T_numtype1, N_rank1>& d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<ArrayIterator<T_numtype1, N_rank1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_Max<T_numtype1, ptrdiff_t> >
      (d1.begin(), 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// _bz_ArrayExpr<P_expr1> max Array<T_numtype2, N_rank2>
template<class P_expr1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Max<typename P_expr1::T_numtype, T_numtype2 > > >
(max)(_bz_ArrayExpr<P_expr1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Max<typename P_expr1::T_numtype, T_numtype2> >
      (d1, 
      d2.begin());
}

// _bz_ArrayExpr<P_expr1> max _bz_ArrayExpr<P_expr2>
template<class P_expr1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Max<typename P_expr1::T_numtype, typename P_expr2::T_numtype > > >
(max)(_bz_ArrayExpr<P_expr1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Max<typename P_expr1::T_numtype, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> max IndexPlaceholder<N_index2>
template<class P_expr1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>,
      _bz_Max<typename P_expr1::T_numtype, ptrdiff_t > > >
(max)(_bz_ArrayExpr<P_expr1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      IndexPlaceholder<N_index2>, 
      _bz_Max<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      d2);
}

// _bz_ArrayExpr<P_expr1> max int
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>,
      _bz_Max<typename P_expr1::T_numtype, int > > >
(max)(_bz_ArrayExpr<P_expr1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<int>, 
      _bz_Max<typename P_expr1::T_numtype, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// _bz_ArrayExpr<P_expr1> max ptrdiff_t
template<class P_expr1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      _bz_Max<typename P_expr1::T_numtype, ptrdiff_t > > >
(max)(_bz_ArrayExpr<P_expr1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExpr<P_expr1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_Max<typename P_expr1::T_numtype, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// IndexPlaceholder<N_index1> max Array<T_numtype2, N_rank2>
template<ptrdiff_t N_index1, class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Max<ptrdiff_t, T_numtype2 > > >
(max)(IndexPlaceholder<N_index1> d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Max<ptrdiff_t, T_numtype2> >
      (d1, 
      d2.begin());
}

// IndexPlaceholder<N_index1> max _bz_ArrayExpr<P_expr2>
template<ptrdiff_t N_index1, class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Max<ptrdiff_t, typename P_expr2::T_numtype > > >
(max)(IndexPlaceholder<N_index1> d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Max<ptrdiff_t, typename P_expr2::T_numtype> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> max IndexPlaceholder<N_index2>
template<ptrdiff_t N_index1, ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>,
      _bz_Max<ptrdiff_t, ptrdiff_t > > >
(max)(IndexPlaceholder<N_index1> d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      IndexPlaceholder<N_index2>, 
      _bz_Max<ptrdiff_t, ptrdiff_t> >
      (d1, 
      d2);
}

// IndexPlaceholder<N_index1> max int
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>,
      _bz_Max<ptrdiff_t, int > > >
(max)(IndexPlaceholder<N_index1> d1, 
      int d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<int>, 
      _bz_Max<ptrdiff_t, int> >
      (d1, 
      _bz_ArrayExprConstant<int>(d2));
}

// IndexPlaceholder<N_index1> max ptrdiff_t
template<ptrdiff_t N_index1>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>,
      _bz_Max<ptrdiff_t, ptrdiff_t > > >
(max)(IndexPlaceholder<N_index1> d1, 
      ptrdiff_t d2)
{
    return _bz_ArrayExprBinaryOp<IndexPlaceholder<N_index1>, 
      _bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_Max<ptrdiff_t, ptrdiff_t> >
      (d1, 
      _bz_ArrayExprConstant<ptrdiff_t>(d2));
}

// int max Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Max<int, T_numtype2 > > >
(max)(int d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Max<int, T_numtype2> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2.begin());
}

// int max _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Max<int, typename P_expr2::T_numtype > > >
(max)(int d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Max<int, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// int max IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>,
      _bz_Max<int, ptrdiff_t > > >
(max)(int d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<int>, 
      IndexPlaceholder<N_index2>, 
      _bz_Max<int, ptrdiff_t> >
      (_bz_ArrayExprConstant<int>(d1), 
      d2);
}

// ptrdiff_t max Array<T_numtype2, N_rank2>
template<class T_numtype2, int N_rank2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>,
      _bz_Max<ptrdiff_t, T_numtype2 > > >
(max)(ptrdiff_t d1, 
      const Array<T_numtype2, N_rank2>& d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      ArrayIterator<T_numtype2, N_rank2>, 
      _bz_Max<ptrdiff_t, T_numtype2> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2.begin());
}

// ptrdiff_t max _bz_ArrayExpr<P_expr2>
template<class P_expr2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>,
      _bz_Max<ptrdiff_t, typename P_expr2::T_numtype > > >
(max)(ptrdiff_t d1, 
      _bz_ArrayExpr<P_expr2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      _bz_ArrayExpr<P_expr2>, 
      _bz_Max<ptrdiff_t, typename P_expr2::T_numtype> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}

// ptrdiff_t max IndexPlaceholder<N_index2>
template<ptrdiff_t N_index2>
inline
_bz_ArrayExpr<_bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>,
      _bz_Max<ptrdiff_t, ptrdiff_t > > >
(max)(ptrdiff_t d1, 
      IndexPlaceholder<N_index2> d2)
{
    return _bz_ArrayExprBinaryOp<_bz_ArrayExprConstant<ptrdiff_t>, 
      IndexPlaceholder<N_index2>, 
      _bz_Max<ptrdiff_t, ptrdiff_t> >
      (_bz_ArrayExprConstant<ptrdiff_t>(d1), 
      d2);
}
BZ_NAMESPACE_END

#endif
