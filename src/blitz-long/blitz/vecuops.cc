/***************************************************************************
 * blitz/../vecuops.cc	Expression templates for vectors, unary functions
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
// genvecuops.cpp May  8 2009 16:45:15

#ifndef BZ_VECUOPS_CC
#define BZ_VECUOPS_CC

#ifndef BZ_VECEXPR_H
 #error <blitz/vecuops.cc> must be included via <blitz/vecexpr.h>
#endif // BZ_VECEXPR_H

BZ_NAMESPACE(blitz)

/****************************************************************************
 * abs
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_abs<P_numtype1> > >
abs(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_abs<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_abs<typename P_expr1::T_numtype> > >
abs(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_abs<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_abs<P_numtype1> > >
abs(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_abs<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_abs<ptrdiff_t> > >
abs(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_abs<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_abs<P_numtype1> > >
abs(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_abs<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * acos
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_acos<P_numtype1> > >
acos(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_acos<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_acos<typename P_expr1::T_numtype> > >
acos(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_acos<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_acos<P_numtype1> > >
acos(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_acos<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_acos<ptrdiff_t> > >
acos(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_acos<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_acos<P_numtype1> > >
acos(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_acos<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * acosh
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_acosh<P_numtype1> > >
acosh(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_acosh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_acosh<typename P_expr1::T_numtype> > >
acosh(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_acosh<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_acosh<P_numtype1> > >
acosh(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_acosh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_acosh<ptrdiff_t> > >
acosh(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_acosh<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_acosh<P_numtype1> > >
acosh(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_acosh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * asin
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_asin<P_numtype1> > >
asin(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_asin<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_asin<typename P_expr1::T_numtype> > >
asin(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_asin<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_asin<P_numtype1> > >
asin(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_asin<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_asin<ptrdiff_t> > >
asin(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_asin<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_asin<P_numtype1> > >
asin(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_asin<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * asinh
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_asinh<P_numtype1> > >
asinh(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_asinh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_asinh<typename P_expr1::T_numtype> > >
asinh(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_asinh<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_asinh<P_numtype1> > >
asinh(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_asinh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_asinh<ptrdiff_t> > >
asinh(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_asinh<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_asinh<P_numtype1> > >
asinh(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_asinh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * atan
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_atan<P_numtype1> > >
atan(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_atan<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_atan<typename P_expr1::T_numtype> > >
atan(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_atan<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_atan<P_numtype1> > >
atan(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_atan<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_atan<ptrdiff_t> > >
atan(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_atan<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_atan<P_numtype1> > >
atan(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_atan<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * atan2
 ****************************************************************************/

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_atan2<P_numtype1,P_numtype2> > >
atan2(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_atan2<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_atan2<P_numtype1,typename P_expr2::T_numtype> > >
atan2(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_atan2<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_atan2<P_numtype1,P_numtype2> > >
atan2(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_atan2<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_atan2<P_numtype1,ptrdiff_t> > >
atan2(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_atan2<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<P_numtype1,P_numtype2> > >
atan2(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_atan2<P_numtype1,int> > >
atan2(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_atan2<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_atan2<P_numtype1,float> > >
atan2(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_atan2<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_atan2<P_numtype1,double> > >
atan2(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_atan2<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_atan2<P_numtype1,long double> > >
atan2(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_atan2<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_atan2<P_numtype1,complex<T2> > > >
atan2(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_atan2<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_atan2<P_numtype1,ptrdiff_t> > >
atan2(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_atan2<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_atan2<typename P_expr1::T_numtype,P_numtype2> > >
atan2(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_atan2<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_atan2<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
atan2(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_atan2<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_atan2<typename P_expr1::T_numtype,P_numtype2> > >
atan2(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_atan2<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_atan2<typename P_expr1::T_numtype,ptrdiff_t> > >
atan2(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_atan2<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<typename P_expr1::T_numtype,P_numtype2> > >
atan2(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_atan2<typename P_expr1::T_numtype,int> > >
atan2(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_atan2<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_atan2<typename P_expr1::T_numtype,float> > >
atan2(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_atan2<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_atan2<typename P_expr1::T_numtype,double> > >
atan2(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_atan2<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_atan2<typename P_expr1::T_numtype,long double> > >
atan2(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_atan2<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_atan2<typename P_expr1::T_numtype,complex<T2> > > >
atan2(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_atan2<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_atan2<typename P_expr1::T_numtype,ptrdiff_t> > >
atan2(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_atan2<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_atan2<P_numtype1,P_numtype2> > >
atan2(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_atan2<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_atan2<P_numtype1,typename P_expr2::T_numtype> > >
atan2(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_atan2<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_atan2<P_numtype1,P_numtype2> > >
atan2(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_atan2<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_atan2<P_numtype1,ptrdiff_t> > >
atan2(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_atan2<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<P_numtype1,P_numtype2> > >
atan2(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_atan2<P_numtype1,int> > >
atan2(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_atan2<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_atan2<P_numtype1,float> > >
atan2(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_atan2<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_atan2<P_numtype1,double> > >
atan2(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_atan2<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_atan2<P_numtype1,long double> > >
atan2(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_atan2<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_atan2<P_numtype1,complex<T2> > > >
atan2(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_atan2<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_atan2<P_numtype1,ptrdiff_t> > >
atan2(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_atan2<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_atan2<ptrdiff_t,P_numtype2> > >
atan2(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_atan2<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_atan2<ptrdiff_t,typename P_expr2::T_numtype> > >
atan2(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_atan2<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_atan2<ptrdiff_t,P_numtype2> > >
atan2(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_atan2<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_atan2<ptrdiff_t,ptrdiff_t> > >
atan2(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_atan2<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<ptrdiff_t,P_numtype2> > >
atan2(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_atan2<ptrdiff_t,int> > >
atan2(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_atan2<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_atan2<ptrdiff_t,float> > >
atan2(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_atan2<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_atan2<ptrdiff_t,double> > >
atan2(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_atan2<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_atan2<ptrdiff_t,long double> > >
atan2(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_atan2<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_atan2<ptrdiff_t,complex<T2> > > >
atan2(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_atan2<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_atan2<ptrdiff_t,ptrdiff_t> > >
atan2(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_atan2<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_atan2<P_numtype1,P_numtype2> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_atan2<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_atan2<P_numtype1,typename P_expr2::T_numtype> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_atan2<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_atan2<P_numtype1,P_numtype2> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_atan2<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_atan2<P_numtype1,ptrdiff_t> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_atan2<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<P_numtype1,P_numtype2> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_atan2<P_numtype1,int> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_atan2<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_atan2<P_numtype1,float> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_atan2<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_atan2<P_numtype1,double> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_atan2<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_atan2<P_numtype1,long double> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_atan2<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_atan2<P_numtype1,complex<T2> > > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_atan2<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_atan2<P_numtype1,ptrdiff_t> > >
atan2(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_atan2<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_atan2<int,P_numtype2> > >
atan2(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_atan2<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_atan2<int,typename P_expr2::T_numtype> > >
atan2(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_atan2<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_atan2<int,P_numtype2> > >
atan2(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_atan2<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_atan2<int,ptrdiff_t> > >
atan2(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_atan2<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<int,P_numtype2> > >
atan2(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_atan2<float,P_numtype2> > >
atan2(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_atan2<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_atan2<float,typename P_expr2::T_numtype> > >
atan2(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_atan2<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_atan2<float,P_numtype2> > >
atan2(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_atan2<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_atan2<float,ptrdiff_t> > >
atan2(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_atan2<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<float,P_numtype2> > >
atan2(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_atan2<double,P_numtype2> > >
atan2(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_atan2<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_atan2<double,typename P_expr2::T_numtype> > >
atan2(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_atan2<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_atan2<double,P_numtype2> > >
atan2(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_atan2<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_atan2<double,ptrdiff_t> > >
atan2(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_atan2<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<double,P_numtype2> > >
atan2(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_atan2<long double,P_numtype2> > >
atan2(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_atan2<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_atan2<long double,typename P_expr2::T_numtype> > >
atan2(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_atan2<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_atan2<long double,P_numtype2> > >
atan2(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_atan2<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_atan2<long double,ptrdiff_t> > >
atan2(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_atan2<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<long double,P_numtype2> > >
atan2(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_atan2<complex<T1> ,P_numtype2> > >
atan2(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_atan2<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_atan2<complex<T1> ,typename P_expr2::T_numtype> > >
atan2(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_atan2<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_atan2<complex<T1> ,P_numtype2> > >
atan2(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_atan2<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_atan2<complex<T1> ,ptrdiff_t> > >
atan2(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_atan2<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<complex<T1> ,P_numtype2> > >
atan2(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_atan2<ptrdiff_t,P_numtype2> > >
atan2(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_atan2<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_atan2<ptrdiff_t,typename P_expr2::T_numtype> > >
atan2(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_atan2<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_atan2<ptrdiff_t,P_numtype2> > >
atan2(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_atan2<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_atan2<ptrdiff_t,ptrdiff_t> > >
atan2(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_atan2<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_atan2<ptrdiff_t,P_numtype2> > >
atan2(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_atan2<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


/****************************************************************************
 * atanh
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_atanh<P_numtype1> > >
atanh(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_atanh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_atanh<typename P_expr1::T_numtype> > >
atanh(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_atanh<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_atanh<P_numtype1> > >
atanh(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_atanh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_atanh<ptrdiff_t> > >
atanh(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_atanh<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_atanh<P_numtype1> > >
atanh(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_atanh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * _class
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz__class<P_numtype1> > >
_class(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz__class<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz__class<typename P_expr1::T_numtype> > >
_class(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz__class<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz__class<P_numtype1> > >
_class(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz__class<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz__class<ptrdiff_t> > >
_class(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz__class<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz__class<P_numtype1> > >
_class(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz__class<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * cbrt
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_cbrt<P_numtype1> > >
cbrt(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_cbrt<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_cbrt<typename P_expr1::T_numtype> > >
cbrt(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_cbrt<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_cbrt<P_numtype1> > >
cbrt(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_cbrt<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_cbrt<ptrdiff_t> > >
cbrt(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_cbrt<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_cbrt<P_numtype1> > >
cbrt(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_cbrt<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * ceil
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_ceil<P_numtype1> > >
ceil(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_ceil<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_ceil<typename P_expr1::T_numtype> > >
ceil(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_ceil<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_ceil<P_numtype1> > >
ceil(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_ceil<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_ceil<ptrdiff_t> > >
ceil(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_ceil<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_ceil<P_numtype1> > >
ceil(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_ceil<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * conj
 ****************************************************************************/

#ifdef BZ_HAVE_COMPLEX_FCNS
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_conj<P_numtype1> > >
conj(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_conj<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_conj<typename P_expr1::T_numtype> > >
conj(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_conj<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_conj<P_numtype1> > >
conj(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_conj<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_conj<ptrdiff_t> > >
conj(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_conj<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_conj<P_numtype1> > >
conj(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_conj<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * copysign
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_copysign<P_numtype1,P_numtype2> > >
copysign(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_copysign<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_copysign<P_numtype1,typename P_expr2::T_numtype> > >
copysign(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_copysign<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_copysign<P_numtype1,P_numtype2> > >
copysign(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_copysign<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_copysign<P_numtype1,ptrdiff_t> > >
copysign(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_copysign<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<P_numtype1,P_numtype2> > >
copysign(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_copysign<P_numtype1,int> > >
copysign(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_copysign<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_copysign<P_numtype1,float> > >
copysign(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_copysign<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_copysign<P_numtype1,double> > >
copysign(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_copysign<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_copysign<P_numtype1,long double> > >
copysign(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_copysign<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_copysign<P_numtype1,complex<T2> > > >
copysign(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_copysign<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_copysign<P_numtype1,ptrdiff_t> > >
copysign(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_copysign<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_copysign<typename P_expr1::T_numtype,P_numtype2> > >
copysign(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_copysign<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_copysign<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
copysign(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_copysign<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_copysign<typename P_expr1::T_numtype,P_numtype2> > >
copysign(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_copysign<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_copysign<typename P_expr1::T_numtype,ptrdiff_t> > >
copysign(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_copysign<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<typename P_expr1::T_numtype,P_numtype2> > >
copysign(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_copysign<typename P_expr1::T_numtype,int> > >
copysign(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_copysign<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_copysign<typename P_expr1::T_numtype,float> > >
copysign(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_copysign<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_copysign<typename P_expr1::T_numtype,double> > >
copysign(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_copysign<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_copysign<typename P_expr1::T_numtype,long double> > >
copysign(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_copysign<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_copysign<typename P_expr1::T_numtype,complex<T2> > > >
copysign(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_copysign<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_copysign<typename P_expr1::T_numtype,ptrdiff_t> > >
copysign(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_copysign<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_copysign<P_numtype1,P_numtype2> > >
copysign(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_copysign<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_copysign<P_numtype1,typename P_expr2::T_numtype> > >
copysign(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_copysign<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_copysign<P_numtype1,P_numtype2> > >
copysign(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_copysign<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_copysign<P_numtype1,ptrdiff_t> > >
copysign(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_copysign<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<P_numtype1,P_numtype2> > >
copysign(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_copysign<P_numtype1,int> > >
copysign(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_copysign<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_copysign<P_numtype1,float> > >
copysign(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_copysign<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_copysign<P_numtype1,double> > >
copysign(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_copysign<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_copysign<P_numtype1,long double> > >
copysign(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_copysign<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_copysign<P_numtype1,complex<T2> > > >
copysign(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_copysign<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_copysign<P_numtype1,ptrdiff_t> > >
copysign(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_copysign<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_copysign<ptrdiff_t,P_numtype2> > >
copysign(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_copysign<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_copysign<ptrdiff_t,typename P_expr2::T_numtype> > >
copysign(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_copysign<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_copysign<ptrdiff_t,P_numtype2> > >
copysign(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_copysign<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_copysign<ptrdiff_t,ptrdiff_t> > >
copysign(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_copysign<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<ptrdiff_t,P_numtype2> > >
copysign(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_copysign<ptrdiff_t,int> > >
copysign(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_copysign<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_copysign<ptrdiff_t,float> > >
copysign(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_copysign<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_copysign<ptrdiff_t,double> > >
copysign(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_copysign<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_copysign<ptrdiff_t,long double> > >
copysign(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_copysign<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_copysign<ptrdiff_t,complex<T2> > > >
copysign(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_copysign<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_copysign<ptrdiff_t,ptrdiff_t> > >
copysign(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_copysign<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_copysign<P_numtype1,P_numtype2> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_copysign<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_copysign<P_numtype1,typename P_expr2::T_numtype> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_copysign<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_copysign<P_numtype1,P_numtype2> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_copysign<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_copysign<P_numtype1,ptrdiff_t> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_copysign<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<P_numtype1,P_numtype2> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_copysign<P_numtype1,int> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_copysign<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_copysign<P_numtype1,float> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_copysign<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_copysign<P_numtype1,double> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_copysign<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_copysign<P_numtype1,long double> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_copysign<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_copysign<P_numtype1,complex<T2> > > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_copysign<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_copysign<P_numtype1,ptrdiff_t> > >
copysign(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_copysign<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_copysign<int,P_numtype2> > >
copysign(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_copysign<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_copysign<int,typename P_expr2::T_numtype> > >
copysign(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_copysign<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_copysign<int,P_numtype2> > >
copysign(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_copysign<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_copysign<int,ptrdiff_t> > >
copysign(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_copysign<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<int,P_numtype2> > >
copysign(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_copysign<float,P_numtype2> > >
copysign(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_copysign<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_copysign<float,typename P_expr2::T_numtype> > >
copysign(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_copysign<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_copysign<float,P_numtype2> > >
copysign(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_copysign<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_copysign<float,ptrdiff_t> > >
copysign(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_copysign<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<float,P_numtype2> > >
copysign(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_copysign<double,P_numtype2> > >
copysign(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_copysign<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_copysign<double,typename P_expr2::T_numtype> > >
copysign(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_copysign<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_copysign<double,P_numtype2> > >
copysign(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_copysign<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_copysign<double,ptrdiff_t> > >
copysign(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_copysign<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<double,P_numtype2> > >
copysign(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_copysign<long double,P_numtype2> > >
copysign(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_copysign<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_copysign<long double,typename P_expr2::T_numtype> > >
copysign(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_copysign<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_copysign<long double,P_numtype2> > >
copysign(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_copysign<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_copysign<long double,ptrdiff_t> > >
copysign(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_copysign<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<long double,P_numtype2> > >
copysign(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_copysign<complex<T1> ,P_numtype2> > >
copysign(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_copysign<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_copysign<complex<T1> ,typename P_expr2::T_numtype> > >
copysign(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_copysign<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_copysign<complex<T1> ,P_numtype2> > >
copysign(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_copysign<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_copysign<complex<T1> ,ptrdiff_t> > >
copysign(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_copysign<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<complex<T1> ,P_numtype2> > >
copysign(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_copysign<ptrdiff_t,P_numtype2> > >
copysign(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_copysign<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_copysign<ptrdiff_t,typename P_expr2::T_numtype> > >
copysign(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_copysign<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_copysign<ptrdiff_t,P_numtype2> > >
copysign(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_copysign<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_copysign<ptrdiff_t,ptrdiff_t> > >
copysign(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_copysign<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_copysign<ptrdiff_t,P_numtype2> > >
copysign(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_copysign<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

#endif

/****************************************************************************
 * cos
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_cos<P_numtype1> > >
cos(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_cos<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_cos<typename P_expr1::T_numtype> > >
cos(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_cos<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_cos<P_numtype1> > >
cos(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_cos<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_cos<ptrdiff_t> > >
cos(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_cos<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_cos<P_numtype1> > >
cos(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_cos<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * cosh
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_cosh<P_numtype1> > >
cosh(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_cosh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_cosh<typename P_expr1::T_numtype> > >
cosh(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_cosh<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_cosh<P_numtype1> > >
cosh(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_cosh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_cosh<ptrdiff_t> > >
cosh(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_cosh<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_cosh<P_numtype1> > >
cosh(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_cosh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * drem
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_drem<P_numtype1,P_numtype2> > >
drem(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_drem<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_drem<P_numtype1,typename P_expr2::T_numtype> > >
drem(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_drem<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_drem<P_numtype1,P_numtype2> > >
drem(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_drem<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_drem<P_numtype1,ptrdiff_t> > >
drem(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_drem<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<P_numtype1,P_numtype2> > >
drem(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_drem<P_numtype1,int> > >
drem(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_drem<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_drem<P_numtype1,float> > >
drem(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_drem<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_drem<P_numtype1,double> > >
drem(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_drem<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_drem<P_numtype1,long double> > >
drem(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_drem<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_drem<P_numtype1,complex<T2> > > >
drem(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_drem<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_drem<P_numtype1,ptrdiff_t> > >
drem(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_drem<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_drem<typename P_expr1::T_numtype,P_numtype2> > >
drem(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_drem<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_drem<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
drem(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_drem<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_drem<typename P_expr1::T_numtype,P_numtype2> > >
drem(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_drem<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_drem<typename P_expr1::T_numtype,ptrdiff_t> > >
drem(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_drem<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<typename P_expr1::T_numtype,P_numtype2> > >
drem(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_drem<typename P_expr1::T_numtype,int> > >
drem(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_drem<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_drem<typename P_expr1::T_numtype,float> > >
drem(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_drem<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_drem<typename P_expr1::T_numtype,double> > >
drem(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_drem<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_drem<typename P_expr1::T_numtype,long double> > >
drem(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_drem<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_drem<typename P_expr1::T_numtype,complex<T2> > > >
drem(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_drem<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_drem<typename P_expr1::T_numtype,ptrdiff_t> > >
drem(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_drem<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_drem<P_numtype1,P_numtype2> > >
drem(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_drem<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_drem<P_numtype1,typename P_expr2::T_numtype> > >
drem(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_drem<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_drem<P_numtype1,P_numtype2> > >
drem(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_drem<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_drem<P_numtype1,ptrdiff_t> > >
drem(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_drem<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<P_numtype1,P_numtype2> > >
drem(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_drem<P_numtype1,int> > >
drem(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_drem<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_drem<P_numtype1,float> > >
drem(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_drem<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_drem<P_numtype1,double> > >
drem(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_drem<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_drem<P_numtype1,long double> > >
drem(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_drem<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_drem<P_numtype1,complex<T2> > > >
drem(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_drem<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_drem<P_numtype1,ptrdiff_t> > >
drem(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_drem<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_drem<ptrdiff_t,P_numtype2> > >
drem(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_drem<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_drem<ptrdiff_t,typename P_expr2::T_numtype> > >
drem(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_drem<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_drem<ptrdiff_t,P_numtype2> > >
drem(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_drem<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_drem<ptrdiff_t,ptrdiff_t> > >
drem(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_drem<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<ptrdiff_t,P_numtype2> > >
drem(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_drem<ptrdiff_t,int> > >
drem(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_drem<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_drem<ptrdiff_t,float> > >
drem(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_drem<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_drem<ptrdiff_t,double> > >
drem(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_drem<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_drem<ptrdiff_t,long double> > >
drem(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_drem<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_drem<ptrdiff_t,complex<T2> > > >
drem(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_drem<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_drem<ptrdiff_t,ptrdiff_t> > >
drem(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_drem<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_drem<P_numtype1,P_numtype2> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_drem<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_drem<P_numtype1,typename P_expr2::T_numtype> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_drem<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_drem<P_numtype1,P_numtype2> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_drem<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_drem<P_numtype1,ptrdiff_t> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_drem<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<P_numtype1,P_numtype2> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_drem<P_numtype1,int> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_drem<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_drem<P_numtype1,float> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_drem<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_drem<P_numtype1,double> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_drem<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_drem<P_numtype1,long double> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_drem<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_drem<P_numtype1,complex<T2> > > >
drem(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_drem<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_drem<P_numtype1,ptrdiff_t> > >
drem(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_drem<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_drem<int,P_numtype2> > >
drem(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_drem<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_drem<int,typename P_expr2::T_numtype> > >
drem(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_drem<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_drem<int,P_numtype2> > >
drem(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_drem<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_drem<int,ptrdiff_t> > >
drem(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_drem<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<int,P_numtype2> > >
drem(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_drem<float,P_numtype2> > >
drem(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_drem<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_drem<float,typename P_expr2::T_numtype> > >
drem(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_drem<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_drem<float,P_numtype2> > >
drem(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_drem<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_drem<float,ptrdiff_t> > >
drem(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_drem<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<float,P_numtype2> > >
drem(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_drem<double,P_numtype2> > >
drem(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_drem<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_drem<double,typename P_expr2::T_numtype> > >
drem(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_drem<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_drem<double,P_numtype2> > >
drem(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_drem<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_drem<double,ptrdiff_t> > >
drem(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_drem<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<double,P_numtype2> > >
drem(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_drem<long double,P_numtype2> > >
drem(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_drem<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_drem<long double,typename P_expr2::T_numtype> > >
drem(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_drem<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_drem<long double,P_numtype2> > >
drem(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_drem<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_drem<long double,ptrdiff_t> > >
drem(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_drem<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<long double,P_numtype2> > >
drem(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_drem<complex<T1> ,P_numtype2> > >
drem(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_drem<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_drem<complex<T1> ,typename P_expr2::T_numtype> > >
drem(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_drem<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_drem<complex<T1> ,P_numtype2> > >
drem(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_drem<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_drem<complex<T1> ,ptrdiff_t> > >
drem(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_drem<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<complex<T1> ,P_numtype2> > >
drem(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_drem<ptrdiff_t,P_numtype2> > >
drem(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_drem<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_drem<ptrdiff_t,typename P_expr2::T_numtype> > >
drem(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_drem<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_drem<ptrdiff_t,P_numtype2> > >
drem(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_drem<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_drem<ptrdiff_t,ptrdiff_t> > >
drem(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_drem<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_drem<ptrdiff_t,P_numtype2> > >
drem(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_drem<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

#endif

/****************************************************************************
 * exp
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_exp<P_numtype1> > >
exp(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_exp<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_exp<typename P_expr1::T_numtype> > >
exp(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_exp<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_exp<P_numtype1> > >
exp(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_exp<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_exp<ptrdiff_t> > >
exp(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_exp<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_exp<P_numtype1> > >
exp(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_exp<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * expm1
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_expm1<P_numtype1> > >
expm1(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_expm1<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_expm1<typename P_expr1::T_numtype> > >
expm1(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_expm1<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_expm1<P_numtype1> > >
expm1(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_expm1<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_expm1<ptrdiff_t> > >
expm1(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_expm1<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_expm1<P_numtype1> > >
expm1(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_expm1<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * erf
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_erf<P_numtype1> > >
erf(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_erf<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_erf<typename P_expr1::T_numtype> > >
erf(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_erf<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_erf<P_numtype1> > >
erf(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_erf<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_erf<ptrdiff_t> > >
erf(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_erf<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_erf<P_numtype1> > >
erf(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_erf<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * erfc
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_erfc<P_numtype1> > >
erfc(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_erfc<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_erfc<typename P_expr1::T_numtype> > >
erfc(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_erfc<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_erfc<P_numtype1> > >
erfc(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_erfc<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_erfc<ptrdiff_t> > >
erfc(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_erfc<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_erfc<P_numtype1> > >
erfc(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_erfc<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * fabs
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_abs<P_numtype1> > >
fabs(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_abs<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_abs<typename P_expr1::T_numtype> > >
fabs(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_abs<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_abs<P_numtype1> > >
fabs(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_abs<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_abs<ptrdiff_t> > >
fabs(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_abs<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_abs<P_numtype1> > >
fabs(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_abs<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * floor
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_floor<P_numtype1> > >
floor(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_floor<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_floor<typename P_expr1::T_numtype> > >
floor(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_floor<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_floor<P_numtype1> > >
floor(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_floor<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_floor<ptrdiff_t> > >
floor(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_floor<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_floor<P_numtype1> > >
floor(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_floor<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * fmod
 ****************************************************************************/

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_fmod<P_numtype1,P_numtype2> > >
fmod(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_fmod<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_fmod<P_numtype1,typename P_expr2::T_numtype> > >
fmod(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_fmod<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_fmod<P_numtype1,P_numtype2> > >
fmod(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_fmod<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_fmod<P_numtype1,ptrdiff_t> > >
fmod(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_fmod<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<P_numtype1,P_numtype2> > >
fmod(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_fmod<P_numtype1,int> > >
fmod(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_fmod<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_fmod<P_numtype1,float> > >
fmod(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_fmod<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_fmod<P_numtype1,double> > >
fmod(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_fmod<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_fmod<P_numtype1,long double> > >
fmod(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_fmod<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_fmod<P_numtype1,complex<T2> > > >
fmod(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_fmod<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_fmod<P_numtype1,ptrdiff_t> > >
fmod(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_fmod<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_fmod<typename P_expr1::T_numtype,P_numtype2> > >
fmod(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_fmod<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_fmod<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
fmod(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_fmod<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_fmod<typename P_expr1::T_numtype,P_numtype2> > >
fmod(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_fmod<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_fmod<typename P_expr1::T_numtype,ptrdiff_t> > >
fmod(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_fmod<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<typename P_expr1::T_numtype,P_numtype2> > >
fmod(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_fmod<typename P_expr1::T_numtype,int> > >
fmod(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_fmod<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_fmod<typename P_expr1::T_numtype,float> > >
fmod(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_fmod<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_fmod<typename P_expr1::T_numtype,double> > >
fmod(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_fmod<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_fmod<typename P_expr1::T_numtype,long double> > >
fmod(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_fmod<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_fmod<typename P_expr1::T_numtype,complex<T2> > > >
fmod(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_fmod<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_fmod<typename P_expr1::T_numtype,ptrdiff_t> > >
fmod(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_fmod<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_fmod<P_numtype1,P_numtype2> > >
fmod(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_fmod<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_fmod<P_numtype1,typename P_expr2::T_numtype> > >
fmod(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_fmod<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_fmod<P_numtype1,P_numtype2> > >
fmod(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_fmod<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_fmod<P_numtype1,ptrdiff_t> > >
fmod(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_fmod<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<P_numtype1,P_numtype2> > >
fmod(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_fmod<P_numtype1,int> > >
fmod(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_fmod<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_fmod<P_numtype1,float> > >
fmod(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_fmod<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_fmod<P_numtype1,double> > >
fmod(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_fmod<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_fmod<P_numtype1,long double> > >
fmod(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_fmod<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_fmod<P_numtype1,complex<T2> > > >
fmod(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_fmod<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_fmod<P_numtype1,ptrdiff_t> > >
fmod(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_fmod<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_fmod<ptrdiff_t,P_numtype2> > >
fmod(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_fmod<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_fmod<ptrdiff_t,typename P_expr2::T_numtype> > >
fmod(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_fmod<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_fmod<ptrdiff_t,P_numtype2> > >
fmod(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_fmod<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_fmod<ptrdiff_t,ptrdiff_t> > >
fmod(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_fmod<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<ptrdiff_t,P_numtype2> > >
fmod(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_fmod<ptrdiff_t,int> > >
fmod(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_fmod<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_fmod<ptrdiff_t,float> > >
fmod(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_fmod<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_fmod<ptrdiff_t,double> > >
fmod(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_fmod<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_fmod<ptrdiff_t,long double> > >
fmod(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_fmod<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_fmod<ptrdiff_t,complex<T2> > > >
fmod(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_fmod<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_fmod<ptrdiff_t,ptrdiff_t> > >
fmod(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_fmod<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_fmod<P_numtype1,P_numtype2> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_fmod<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_fmod<P_numtype1,typename P_expr2::T_numtype> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_fmod<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_fmod<P_numtype1,P_numtype2> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_fmod<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_fmod<P_numtype1,ptrdiff_t> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_fmod<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<P_numtype1,P_numtype2> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_fmod<P_numtype1,int> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_fmod<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_fmod<P_numtype1,float> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_fmod<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_fmod<P_numtype1,double> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_fmod<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_fmod<P_numtype1,long double> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_fmod<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_fmod<P_numtype1,complex<T2> > > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_fmod<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_fmod<P_numtype1,ptrdiff_t> > >
fmod(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_fmod<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_fmod<int,P_numtype2> > >
fmod(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_fmod<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_fmod<int,typename P_expr2::T_numtype> > >
fmod(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_fmod<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_fmod<int,P_numtype2> > >
fmod(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_fmod<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_fmod<int,ptrdiff_t> > >
fmod(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_fmod<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<int,P_numtype2> > >
fmod(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_fmod<float,P_numtype2> > >
fmod(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_fmod<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_fmod<float,typename P_expr2::T_numtype> > >
fmod(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_fmod<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_fmod<float,P_numtype2> > >
fmod(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_fmod<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_fmod<float,ptrdiff_t> > >
fmod(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_fmod<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<float,P_numtype2> > >
fmod(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_fmod<double,P_numtype2> > >
fmod(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_fmod<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_fmod<double,typename P_expr2::T_numtype> > >
fmod(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_fmod<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_fmod<double,P_numtype2> > >
fmod(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_fmod<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_fmod<double,ptrdiff_t> > >
fmod(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_fmod<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<double,P_numtype2> > >
fmod(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_fmod<long double,P_numtype2> > >
fmod(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_fmod<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_fmod<long double,typename P_expr2::T_numtype> > >
fmod(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_fmod<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_fmod<long double,P_numtype2> > >
fmod(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_fmod<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_fmod<long double,ptrdiff_t> > >
fmod(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_fmod<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<long double,P_numtype2> > >
fmod(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_fmod<complex<T1> ,P_numtype2> > >
fmod(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_fmod<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_fmod<complex<T1> ,typename P_expr2::T_numtype> > >
fmod(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_fmod<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_fmod<complex<T1> ,P_numtype2> > >
fmod(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_fmod<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_fmod<complex<T1> ,ptrdiff_t> > >
fmod(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_fmod<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<complex<T1> ,P_numtype2> > >
fmod(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_fmod<ptrdiff_t,P_numtype2> > >
fmod(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_fmod<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_fmod<ptrdiff_t,typename P_expr2::T_numtype> > >
fmod(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_fmod<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_fmod<ptrdiff_t,P_numtype2> > >
fmod(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_fmod<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_fmod<ptrdiff_t,ptrdiff_t> > >
fmod(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_fmod<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_fmod<ptrdiff_t,P_numtype2> > >
fmod(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_fmod<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


/****************************************************************************
 * hypot
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_hypot<P_numtype1,P_numtype2> > >
hypot(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_hypot<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_hypot<P_numtype1,typename P_expr2::T_numtype> > >
hypot(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_hypot<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_hypot<P_numtype1,P_numtype2> > >
hypot(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_hypot<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_hypot<P_numtype1,ptrdiff_t> > >
hypot(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_hypot<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<P_numtype1,P_numtype2> > >
hypot(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_hypot<P_numtype1,int> > >
hypot(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_hypot<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_hypot<P_numtype1,float> > >
hypot(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_hypot<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_hypot<P_numtype1,double> > >
hypot(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_hypot<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_hypot<P_numtype1,long double> > >
hypot(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_hypot<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_hypot<P_numtype1,complex<T2> > > >
hypot(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_hypot<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_hypot<P_numtype1,ptrdiff_t> > >
hypot(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_hypot<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_hypot<typename P_expr1::T_numtype,P_numtype2> > >
hypot(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_hypot<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_hypot<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
hypot(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_hypot<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_hypot<typename P_expr1::T_numtype,P_numtype2> > >
hypot(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_hypot<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_hypot<typename P_expr1::T_numtype,ptrdiff_t> > >
hypot(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_hypot<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<typename P_expr1::T_numtype,P_numtype2> > >
hypot(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_hypot<typename P_expr1::T_numtype,int> > >
hypot(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_hypot<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_hypot<typename P_expr1::T_numtype,float> > >
hypot(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_hypot<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_hypot<typename P_expr1::T_numtype,double> > >
hypot(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_hypot<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_hypot<typename P_expr1::T_numtype,long double> > >
hypot(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_hypot<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_hypot<typename P_expr1::T_numtype,complex<T2> > > >
hypot(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_hypot<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_hypot<typename P_expr1::T_numtype,ptrdiff_t> > >
hypot(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_hypot<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_hypot<P_numtype1,P_numtype2> > >
hypot(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_hypot<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_hypot<P_numtype1,typename P_expr2::T_numtype> > >
hypot(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_hypot<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_hypot<P_numtype1,P_numtype2> > >
hypot(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_hypot<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_hypot<P_numtype1,ptrdiff_t> > >
hypot(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_hypot<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<P_numtype1,P_numtype2> > >
hypot(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_hypot<P_numtype1,int> > >
hypot(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_hypot<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_hypot<P_numtype1,float> > >
hypot(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_hypot<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_hypot<P_numtype1,double> > >
hypot(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_hypot<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_hypot<P_numtype1,long double> > >
hypot(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_hypot<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_hypot<P_numtype1,complex<T2> > > >
hypot(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_hypot<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_hypot<P_numtype1,ptrdiff_t> > >
hypot(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_hypot<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_hypot<ptrdiff_t,P_numtype2> > >
hypot(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_hypot<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_hypot<ptrdiff_t,typename P_expr2::T_numtype> > >
hypot(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_hypot<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_hypot<ptrdiff_t,P_numtype2> > >
hypot(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_hypot<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_hypot<ptrdiff_t,ptrdiff_t> > >
hypot(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_hypot<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<ptrdiff_t,P_numtype2> > >
hypot(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_hypot<ptrdiff_t,int> > >
hypot(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_hypot<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_hypot<ptrdiff_t,float> > >
hypot(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_hypot<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_hypot<ptrdiff_t,double> > >
hypot(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_hypot<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_hypot<ptrdiff_t,long double> > >
hypot(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_hypot<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_hypot<ptrdiff_t,complex<T2> > > >
hypot(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_hypot<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_hypot<ptrdiff_t,ptrdiff_t> > >
hypot(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_hypot<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_hypot<P_numtype1,P_numtype2> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_hypot<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_hypot<P_numtype1,typename P_expr2::T_numtype> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_hypot<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_hypot<P_numtype1,P_numtype2> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_hypot<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_hypot<P_numtype1,ptrdiff_t> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_hypot<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<P_numtype1,P_numtype2> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_hypot<P_numtype1,int> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_hypot<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_hypot<P_numtype1,float> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_hypot<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_hypot<P_numtype1,double> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_hypot<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_hypot<P_numtype1,long double> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_hypot<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_hypot<P_numtype1,complex<T2> > > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_hypot<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_hypot<P_numtype1,ptrdiff_t> > >
hypot(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_hypot<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_hypot<int,P_numtype2> > >
hypot(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_hypot<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_hypot<int,typename P_expr2::T_numtype> > >
hypot(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_hypot<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_hypot<int,P_numtype2> > >
hypot(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_hypot<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_hypot<int,ptrdiff_t> > >
hypot(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_hypot<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<int,P_numtype2> > >
hypot(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_hypot<float,P_numtype2> > >
hypot(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_hypot<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_hypot<float,typename P_expr2::T_numtype> > >
hypot(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_hypot<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_hypot<float,P_numtype2> > >
hypot(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_hypot<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_hypot<float,ptrdiff_t> > >
hypot(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_hypot<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<float,P_numtype2> > >
hypot(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_hypot<double,P_numtype2> > >
hypot(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_hypot<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_hypot<double,typename P_expr2::T_numtype> > >
hypot(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_hypot<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_hypot<double,P_numtype2> > >
hypot(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_hypot<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_hypot<double,ptrdiff_t> > >
hypot(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_hypot<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<double,P_numtype2> > >
hypot(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_hypot<long double,P_numtype2> > >
hypot(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_hypot<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_hypot<long double,typename P_expr2::T_numtype> > >
hypot(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_hypot<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_hypot<long double,P_numtype2> > >
hypot(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_hypot<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_hypot<long double,ptrdiff_t> > >
hypot(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_hypot<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<long double,P_numtype2> > >
hypot(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_hypot<complex<T1> ,P_numtype2> > >
hypot(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_hypot<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_hypot<complex<T1> ,typename P_expr2::T_numtype> > >
hypot(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_hypot<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_hypot<complex<T1> ,P_numtype2> > >
hypot(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_hypot<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_hypot<complex<T1> ,ptrdiff_t> > >
hypot(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_hypot<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<complex<T1> ,P_numtype2> > >
hypot(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_hypot<ptrdiff_t,P_numtype2> > >
hypot(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_hypot<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_hypot<ptrdiff_t,typename P_expr2::T_numtype> > >
hypot(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_hypot<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_hypot<ptrdiff_t,P_numtype2> > >
hypot(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_hypot<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_hypot<ptrdiff_t,ptrdiff_t> > >
hypot(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_hypot<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_hypot<ptrdiff_t,P_numtype2> > >
hypot(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_hypot<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

#endif

/****************************************************************************
 * ilogb
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_ilogb<P_numtype1> > >
ilogb(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_ilogb<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_ilogb<typename P_expr1::T_numtype> > >
ilogb(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_ilogb<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_ilogb<P_numtype1> > >
ilogb(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_ilogb<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_ilogb<ptrdiff_t> > >
ilogb(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_ilogb<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_ilogb<P_numtype1> > >
ilogb(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_ilogb<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * blitz_isnan
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_blitz_isnan<P_numtype1> > >
blitz_isnan(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_blitz_isnan<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_blitz_isnan<typename P_expr1::T_numtype> > >
blitz_isnan(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_blitz_isnan<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_blitz_isnan<P_numtype1> > >
blitz_isnan(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_blitz_isnan<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_blitz_isnan<ptrdiff_t> > >
blitz_isnan(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_blitz_isnan<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_blitz_isnan<P_numtype1> > >
blitz_isnan(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_blitz_isnan<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * itrunc
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_itrunc<P_numtype1> > >
itrunc(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_itrunc<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_itrunc<typename P_expr1::T_numtype> > >
itrunc(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_itrunc<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_itrunc<P_numtype1> > >
itrunc(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_itrunc<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_itrunc<ptrdiff_t> > >
itrunc(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_itrunc<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_itrunc<P_numtype1> > >
itrunc(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_itrunc<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * j0
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_j0<P_numtype1> > >
j0(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_j0<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_j0<typename P_expr1::T_numtype> > >
j0(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_j0<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_j0<P_numtype1> > >
j0(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_j0<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_j0<ptrdiff_t> > >
j0(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_j0<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_j0<P_numtype1> > >
j0(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_j0<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * j1
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_j1<P_numtype1> > >
j1(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_j1<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_j1<typename P_expr1::T_numtype> > >
j1(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_j1<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_j1<P_numtype1> > >
j1(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_j1<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_j1<ptrdiff_t> > >
j1(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_j1<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_j1<P_numtype1> > >
j1(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_j1<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * lgamma
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_lgamma<P_numtype1> > >
lgamma(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_lgamma<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_lgamma<typename P_expr1::T_numtype> > >
lgamma(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_lgamma<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_lgamma<P_numtype1> > >
lgamma(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_lgamma<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_lgamma<ptrdiff_t> > >
lgamma(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_lgamma<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_lgamma<P_numtype1> > >
lgamma(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_lgamma<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * log
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_log<P_numtype1> > >
log(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_log<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_log<typename P_expr1::T_numtype> > >
log(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_log<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_log<P_numtype1> > >
log(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_log<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_log<ptrdiff_t> > >
log(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_log<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_log<P_numtype1> > >
log(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_log<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * logb
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_logb<P_numtype1> > >
logb(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_logb<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_logb<typename P_expr1::T_numtype> > >
logb(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_logb<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_logb<P_numtype1> > >
logb(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_logb<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_logb<ptrdiff_t> > >
logb(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_logb<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_logb<P_numtype1> > >
logb(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_logb<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * log1p
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_log1p<P_numtype1> > >
log1p(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_log1p<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_log1p<typename P_expr1::T_numtype> > >
log1p(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_log1p<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_log1p<P_numtype1> > >
log1p(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_log1p<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_log1p<ptrdiff_t> > >
log1p(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_log1p<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_log1p<P_numtype1> > >
log1p(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_log1p<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * log10
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_log10<P_numtype1> > >
log10(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_log10<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_log10<typename P_expr1::T_numtype> > >
log10(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_log10<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_log10<P_numtype1> > >
log10(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_log10<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_log10<ptrdiff_t> > >
log10(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_log10<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_log10<P_numtype1> > >
log10(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_log10<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * nearest
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_nearest<P_numtype1> > >
nearest(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_nearest<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_nearest<typename P_expr1::T_numtype> > >
nearest(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_nearest<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_nearest<P_numtype1> > >
nearest(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_nearest<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_nearest<ptrdiff_t> > >
nearest(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_nearest<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_nearest<P_numtype1> > >
nearest(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_nearest<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * nextafter
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_nextafter<P_numtype1,P_numtype2> > >
nextafter(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_nextafter<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_nextafter<P_numtype1,typename P_expr2::T_numtype> > >
nextafter(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_nextafter<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<P_numtype1,P_numtype2> > >
nextafter(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_nextafter<P_numtype1,ptrdiff_t> > >
nextafter(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_nextafter<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<P_numtype1,P_numtype2> > >
nextafter(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_nextafter<P_numtype1,int> > >
nextafter(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_nextafter<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_nextafter<P_numtype1,float> > >
nextafter(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_nextafter<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_nextafter<P_numtype1,double> > >
nextafter(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_nextafter<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_nextafter<P_numtype1,long double> > >
nextafter(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_nextafter<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_nextafter<P_numtype1,complex<T2> > > >
nextafter(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_nextafter<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_nextafter<P_numtype1,ptrdiff_t> > >
nextafter(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_nextafter<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_nextafter<typename P_expr1::T_numtype,P_numtype2> > >
nextafter(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_nextafter<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_nextafter<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
nextafter(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_nextafter<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<typename P_expr1::T_numtype,P_numtype2> > >
nextafter(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_nextafter<typename P_expr1::T_numtype,ptrdiff_t> > >
nextafter(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_nextafter<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<typename P_expr1::T_numtype,P_numtype2> > >
nextafter(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_nextafter<typename P_expr1::T_numtype,int> > >
nextafter(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_nextafter<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_nextafter<typename P_expr1::T_numtype,float> > >
nextafter(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_nextafter<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_nextafter<typename P_expr1::T_numtype,double> > >
nextafter(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_nextafter<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_nextafter<typename P_expr1::T_numtype,long double> > >
nextafter(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_nextafter<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_nextafter<typename P_expr1::T_numtype,complex<T2> > > >
nextafter(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_nextafter<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_nextafter<typename P_expr1::T_numtype,ptrdiff_t> > >
nextafter(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_nextafter<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_nextafter<P_numtype1,P_numtype2> > >
nextafter(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_nextafter<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_nextafter<P_numtype1,typename P_expr2::T_numtype> > >
nextafter(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_nextafter<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<P_numtype1,P_numtype2> > >
nextafter(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_nextafter<P_numtype1,ptrdiff_t> > >
nextafter(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_nextafter<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<P_numtype1,P_numtype2> > >
nextafter(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_nextafter<P_numtype1,int> > >
nextafter(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_nextafter<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_nextafter<P_numtype1,float> > >
nextafter(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_nextafter<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_nextafter<P_numtype1,double> > >
nextafter(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_nextafter<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_nextafter<P_numtype1,long double> > >
nextafter(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_nextafter<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_nextafter<P_numtype1,complex<T2> > > >
nextafter(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_nextafter<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_nextafter<P_numtype1,ptrdiff_t> > >
nextafter(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_nextafter<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_nextafter<ptrdiff_t,P_numtype2> > >
nextafter(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_nextafter<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_nextafter<ptrdiff_t,typename P_expr2::T_numtype> > >
nextafter(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_nextafter<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<ptrdiff_t,P_numtype2> > >
nextafter(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_nextafter<ptrdiff_t,ptrdiff_t> > >
nextafter(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_nextafter<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<ptrdiff_t,P_numtype2> > >
nextafter(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_nextafter<ptrdiff_t,int> > >
nextafter(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_nextafter<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_nextafter<ptrdiff_t,float> > >
nextafter(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_nextafter<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_nextafter<ptrdiff_t,double> > >
nextafter(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_nextafter<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_nextafter<ptrdiff_t,long double> > >
nextafter(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_nextafter<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_nextafter<ptrdiff_t,complex<T2> > > >
nextafter(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_nextafter<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_nextafter<ptrdiff_t,ptrdiff_t> > >
nextafter(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_nextafter<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_nextafter<P_numtype1,P_numtype2> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_nextafter<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_nextafter<P_numtype1,typename P_expr2::T_numtype> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_nextafter<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<P_numtype1,P_numtype2> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_nextafter<P_numtype1,ptrdiff_t> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_nextafter<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<P_numtype1,P_numtype2> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_nextafter<P_numtype1,int> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_nextafter<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_nextafter<P_numtype1,float> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_nextafter<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_nextafter<P_numtype1,double> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_nextafter<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_nextafter<P_numtype1,long double> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_nextafter<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_nextafter<P_numtype1,complex<T2> > > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_nextafter<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_nextafter<P_numtype1,ptrdiff_t> > >
nextafter(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_nextafter<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_nextafter<int,P_numtype2> > >
nextafter(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_nextafter<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_nextafter<int,typename P_expr2::T_numtype> > >
nextafter(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_nextafter<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<int,P_numtype2> > >
nextafter(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_nextafter<int,ptrdiff_t> > >
nextafter(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_nextafter<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<int,P_numtype2> > >
nextafter(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_nextafter<float,P_numtype2> > >
nextafter(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_nextafter<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_nextafter<float,typename P_expr2::T_numtype> > >
nextafter(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_nextafter<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<float,P_numtype2> > >
nextafter(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_nextafter<float,ptrdiff_t> > >
nextafter(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_nextafter<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<float,P_numtype2> > >
nextafter(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_nextafter<double,P_numtype2> > >
nextafter(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_nextafter<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_nextafter<double,typename P_expr2::T_numtype> > >
nextafter(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_nextafter<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<double,P_numtype2> > >
nextafter(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_nextafter<double,ptrdiff_t> > >
nextafter(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_nextafter<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<double,P_numtype2> > >
nextafter(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_nextafter<long double,P_numtype2> > >
nextafter(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_nextafter<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_nextafter<long double,typename P_expr2::T_numtype> > >
nextafter(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_nextafter<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<long double,P_numtype2> > >
nextafter(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_nextafter<long double,ptrdiff_t> > >
nextafter(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_nextafter<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<long double,P_numtype2> > >
nextafter(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_nextafter<complex<T1> ,P_numtype2> > >
nextafter(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_nextafter<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_nextafter<complex<T1> ,typename P_expr2::T_numtype> > >
nextafter(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_nextafter<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_nextafter<complex<T1> ,P_numtype2> > >
nextafter(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_nextafter<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_nextafter<complex<T1> ,ptrdiff_t> > >
nextafter(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_nextafter<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<complex<T1> ,P_numtype2> > >
nextafter(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_nextafter<ptrdiff_t,P_numtype2> > >
nextafter(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_nextafter<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_nextafter<ptrdiff_t,typename P_expr2::T_numtype> > >
nextafter(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_nextafter<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_nextafter<ptrdiff_t,P_numtype2> > >
nextafter(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_nextafter<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_nextafter<ptrdiff_t,ptrdiff_t> > >
nextafter(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_nextafter<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_nextafter<ptrdiff_t,P_numtype2> > >
nextafter(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_nextafter<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

#endif

/****************************************************************************
 * pow
 ****************************************************************************/

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_pow<P_numtype1,P_numtype2> > >
pow(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_pow<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_pow<P_numtype1,typename P_expr2::T_numtype> > >
pow(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_pow<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_pow<P_numtype1,P_numtype2> > >
pow(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_pow<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_pow<P_numtype1,ptrdiff_t> > >
pow(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_pow<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<P_numtype1,P_numtype2> > >
pow(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_pow<P_numtype1,int> > >
pow(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_pow<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_pow<P_numtype1,float> > >
pow(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_pow<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_pow<P_numtype1,double> > >
pow(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_pow<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_pow<P_numtype1,long double> > >
pow(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_pow<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_pow<P_numtype1,complex<T2> > > >
pow(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_pow<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_pow<P_numtype1,ptrdiff_t> > >
pow(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_pow<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_pow<typename P_expr1::T_numtype,P_numtype2> > >
pow(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_pow<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_pow<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
pow(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_pow<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_pow<typename P_expr1::T_numtype,P_numtype2> > >
pow(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_pow<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_pow<typename P_expr1::T_numtype,ptrdiff_t> > >
pow(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_pow<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<typename P_expr1::T_numtype,P_numtype2> > >
pow(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_pow<typename P_expr1::T_numtype,int> > >
pow(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_pow<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_pow<typename P_expr1::T_numtype,float> > >
pow(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_pow<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_pow<typename P_expr1::T_numtype,double> > >
pow(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_pow<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_pow<typename P_expr1::T_numtype,long double> > >
pow(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_pow<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_pow<typename P_expr1::T_numtype,complex<T2> > > >
pow(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_pow<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_pow<typename P_expr1::T_numtype,ptrdiff_t> > >
pow(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_pow<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_pow<P_numtype1,P_numtype2> > >
pow(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_pow<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_pow<P_numtype1,typename P_expr2::T_numtype> > >
pow(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_pow<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_pow<P_numtype1,P_numtype2> > >
pow(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_pow<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_pow<P_numtype1,ptrdiff_t> > >
pow(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_pow<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<P_numtype1,P_numtype2> > >
pow(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_pow<P_numtype1,int> > >
pow(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_pow<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_pow<P_numtype1,float> > >
pow(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_pow<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_pow<P_numtype1,double> > >
pow(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_pow<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_pow<P_numtype1,long double> > >
pow(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_pow<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_pow<P_numtype1,complex<T2> > > >
pow(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_pow<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_pow<P_numtype1,ptrdiff_t> > >
pow(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_pow<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_pow<ptrdiff_t,P_numtype2> > >
pow(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_pow<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_pow<ptrdiff_t,typename P_expr2::T_numtype> > >
pow(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_pow<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_pow<ptrdiff_t,P_numtype2> > >
pow(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_pow<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_pow<ptrdiff_t,ptrdiff_t> > >
pow(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_pow<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<ptrdiff_t,P_numtype2> > >
pow(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_pow<ptrdiff_t,int> > >
pow(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_pow<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_pow<ptrdiff_t,float> > >
pow(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_pow<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_pow<ptrdiff_t,double> > >
pow(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_pow<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_pow<ptrdiff_t,long double> > >
pow(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_pow<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_pow<ptrdiff_t,complex<T2> > > >
pow(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_pow<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_pow<ptrdiff_t,ptrdiff_t> > >
pow(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_pow<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_pow<P_numtype1,P_numtype2> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_pow<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_pow<P_numtype1,typename P_expr2::T_numtype> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_pow<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_pow<P_numtype1,P_numtype2> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_pow<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_pow<P_numtype1,ptrdiff_t> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_pow<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<P_numtype1,P_numtype2> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_pow<P_numtype1,int> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_pow<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_pow<P_numtype1,float> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_pow<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_pow<P_numtype1,double> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_pow<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_pow<P_numtype1,long double> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_pow<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_pow<P_numtype1,complex<T2> > > >
pow(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_pow<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_pow<P_numtype1,ptrdiff_t> > >
pow(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_pow<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_pow<int,P_numtype2> > >
pow(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_pow<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_pow<int,typename P_expr2::T_numtype> > >
pow(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_pow<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_pow<int,P_numtype2> > >
pow(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_pow<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_pow<int,ptrdiff_t> > >
pow(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_pow<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<int,P_numtype2> > >
pow(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_pow<float,P_numtype2> > >
pow(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_pow<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_pow<float,typename P_expr2::T_numtype> > >
pow(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_pow<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_pow<float,P_numtype2> > >
pow(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_pow<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_pow<float,ptrdiff_t> > >
pow(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_pow<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<float,P_numtype2> > >
pow(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_pow<double,P_numtype2> > >
pow(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_pow<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_pow<double,typename P_expr2::T_numtype> > >
pow(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_pow<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_pow<double,P_numtype2> > >
pow(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_pow<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_pow<double,ptrdiff_t> > >
pow(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_pow<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<double,P_numtype2> > >
pow(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_pow<long double,P_numtype2> > >
pow(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_pow<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_pow<long double,typename P_expr2::T_numtype> > >
pow(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_pow<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_pow<long double,P_numtype2> > >
pow(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_pow<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_pow<long double,ptrdiff_t> > >
pow(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_pow<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<long double,P_numtype2> > >
pow(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_pow<complex<T1> ,P_numtype2> > >
pow(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_pow<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_pow<complex<T1> ,typename P_expr2::T_numtype> > >
pow(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_pow<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_pow<complex<T1> ,P_numtype2> > >
pow(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_pow<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_pow<complex<T1> ,ptrdiff_t> > >
pow(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_pow<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<complex<T1> ,P_numtype2> > >
pow(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_pow<ptrdiff_t,P_numtype2> > >
pow(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_pow<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_pow<ptrdiff_t,typename P_expr2::T_numtype> > >
pow(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_pow<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_pow<ptrdiff_t,P_numtype2> > >
pow(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_pow<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_pow<ptrdiff_t,ptrdiff_t> > >
pow(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_pow<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_pow<ptrdiff_t,P_numtype2> > >
pow(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_pow<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


/****************************************************************************
 * remainder
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_remainder<P_numtype1,P_numtype2> > >
remainder(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_remainder<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_remainder<P_numtype1,typename P_expr2::T_numtype> > >
remainder(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_remainder<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_remainder<P_numtype1,P_numtype2> > >
remainder(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_remainder<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_remainder<P_numtype1,ptrdiff_t> > >
remainder(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_remainder<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<P_numtype1,P_numtype2> > >
remainder(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_remainder<P_numtype1,int> > >
remainder(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_remainder<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_remainder<P_numtype1,float> > >
remainder(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_remainder<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_remainder<P_numtype1,double> > >
remainder(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_remainder<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_remainder<P_numtype1,long double> > >
remainder(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_remainder<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_remainder<P_numtype1,complex<T2> > > >
remainder(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_remainder<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_remainder<P_numtype1,ptrdiff_t> > >
remainder(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_remainder<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_remainder<typename P_expr1::T_numtype,P_numtype2> > >
remainder(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_remainder<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_remainder<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
remainder(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_remainder<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_remainder<typename P_expr1::T_numtype,P_numtype2> > >
remainder(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_remainder<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_remainder<typename P_expr1::T_numtype,ptrdiff_t> > >
remainder(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_remainder<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<typename P_expr1::T_numtype,P_numtype2> > >
remainder(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_remainder<typename P_expr1::T_numtype,int> > >
remainder(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_remainder<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_remainder<typename P_expr1::T_numtype,float> > >
remainder(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_remainder<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_remainder<typename P_expr1::T_numtype,double> > >
remainder(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_remainder<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_remainder<typename P_expr1::T_numtype,long double> > >
remainder(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_remainder<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_remainder<typename P_expr1::T_numtype,complex<T2> > > >
remainder(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_remainder<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_remainder<typename P_expr1::T_numtype,ptrdiff_t> > >
remainder(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_remainder<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_remainder<P_numtype1,P_numtype2> > >
remainder(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_remainder<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_remainder<P_numtype1,typename P_expr2::T_numtype> > >
remainder(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_remainder<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_remainder<P_numtype1,P_numtype2> > >
remainder(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_remainder<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_remainder<P_numtype1,ptrdiff_t> > >
remainder(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_remainder<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<P_numtype1,P_numtype2> > >
remainder(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_remainder<P_numtype1,int> > >
remainder(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_remainder<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_remainder<P_numtype1,float> > >
remainder(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_remainder<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_remainder<P_numtype1,double> > >
remainder(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_remainder<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_remainder<P_numtype1,long double> > >
remainder(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_remainder<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_remainder<P_numtype1,complex<T2> > > >
remainder(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_remainder<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_remainder<P_numtype1,ptrdiff_t> > >
remainder(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_remainder<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_remainder<ptrdiff_t,P_numtype2> > >
remainder(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_remainder<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_remainder<ptrdiff_t,typename P_expr2::T_numtype> > >
remainder(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_remainder<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_remainder<ptrdiff_t,P_numtype2> > >
remainder(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_remainder<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_remainder<ptrdiff_t,ptrdiff_t> > >
remainder(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_remainder<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<ptrdiff_t,P_numtype2> > >
remainder(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_remainder<ptrdiff_t,int> > >
remainder(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_remainder<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_remainder<ptrdiff_t,float> > >
remainder(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_remainder<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_remainder<ptrdiff_t,double> > >
remainder(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_remainder<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_remainder<ptrdiff_t,long double> > >
remainder(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_remainder<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_remainder<ptrdiff_t,complex<T2> > > >
remainder(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_remainder<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_remainder<ptrdiff_t,ptrdiff_t> > >
remainder(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_remainder<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_remainder<P_numtype1,P_numtype2> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_remainder<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_remainder<P_numtype1,typename P_expr2::T_numtype> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_remainder<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_remainder<P_numtype1,P_numtype2> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_remainder<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_remainder<P_numtype1,ptrdiff_t> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_remainder<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<P_numtype1,P_numtype2> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_remainder<P_numtype1,int> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_remainder<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_remainder<P_numtype1,float> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_remainder<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_remainder<P_numtype1,double> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_remainder<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_remainder<P_numtype1,long double> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_remainder<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_remainder<P_numtype1,complex<T2> > > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_remainder<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_remainder<P_numtype1,ptrdiff_t> > >
remainder(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_remainder<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_remainder<int,P_numtype2> > >
remainder(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_remainder<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_remainder<int,typename P_expr2::T_numtype> > >
remainder(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_remainder<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_remainder<int,P_numtype2> > >
remainder(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_remainder<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_remainder<int,ptrdiff_t> > >
remainder(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_remainder<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<int,P_numtype2> > >
remainder(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_remainder<float,P_numtype2> > >
remainder(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_remainder<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_remainder<float,typename P_expr2::T_numtype> > >
remainder(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_remainder<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_remainder<float,P_numtype2> > >
remainder(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_remainder<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_remainder<float,ptrdiff_t> > >
remainder(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_remainder<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<float,P_numtype2> > >
remainder(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_remainder<double,P_numtype2> > >
remainder(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_remainder<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_remainder<double,typename P_expr2::T_numtype> > >
remainder(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_remainder<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_remainder<double,P_numtype2> > >
remainder(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_remainder<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_remainder<double,ptrdiff_t> > >
remainder(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_remainder<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<double,P_numtype2> > >
remainder(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_remainder<long double,P_numtype2> > >
remainder(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_remainder<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_remainder<long double,typename P_expr2::T_numtype> > >
remainder(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_remainder<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_remainder<long double,P_numtype2> > >
remainder(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_remainder<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_remainder<long double,ptrdiff_t> > >
remainder(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_remainder<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<long double,P_numtype2> > >
remainder(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_remainder<complex<T1> ,P_numtype2> > >
remainder(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_remainder<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_remainder<complex<T1> ,typename P_expr2::T_numtype> > >
remainder(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_remainder<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_remainder<complex<T1> ,P_numtype2> > >
remainder(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_remainder<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_remainder<complex<T1> ,ptrdiff_t> > >
remainder(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_remainder<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<complex<T1> ,P_numtype2> > >
remainder(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_remainder<ptrdiff_t,P_numtype2> > >
remainder(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_remainder<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_remainder<ptrdiff_t,typename P_expr2::T_numtype> > >
remainder(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_remainder<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_remainder<ptrdiff_t,P_numtype2> > >
remainder(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_remainder<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_remainder<ptrdiff_t,ptrdiff_t> > >
remainder(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_remainder<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_remainder<ptrdiff_t,P_numtype2> > >
remainder(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_remainder<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

#endif

/****************************************************************************
 * rint
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_rint<P_numtype1> > >
rint(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_rint<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_rint<typename P_expr1::T_numtype> > >
rint(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_rint<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_rint<P_numtype1> > >
rint(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_rint<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_rint<ptrdiff_t> > >
rint(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_rint<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_rint<P_numtype1> > >
rint(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_rint<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * rsqrt
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_rsqrt<P_numtype1> > >
rsqrt(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_rsqrt<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_rsqrt<typename P_expr1::T_numtype> > >
rsqrt(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_rsqrt<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_rsqrt<P_numtype1> > >
rsqrt(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_rsqrt<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_rsqrt<ptrdiff_t> > >
rsqrt(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_rsqrt<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_rsqrt<P_numtype1> > >
rsqrt(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_rsqrt<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * scalb
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_scalb<P_numtype1,P_numtype2> > >
scalb(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_scalb<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_scalb<P_numtype1,typename P_expr2::T_numtype> > >
scalb(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_scalb<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_scalb<P_numtype1,P_numtype2> > >
scalb(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_scalb<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_scalb<P_numtype1,ptrdiff_t> > >
scalb(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_scalb<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<P_numtype1,P_numtype2> > >
scalb(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_scalb<P_numtype1,int> > >
scalb(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_scalb<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_scalb<P_numtype1,float> > >
scalb(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_scalb<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_scalb<P_numtype1,double> > >
scalb(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_scalb<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_scalb<P_numtype1,long double> > >
scalb(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_scalb<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_scalb<P_numtype1,complex<T2> > > >
scalb(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_scalb<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_scalb<P_numtype1,ptrdiff_t> > >
scalb(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_scalb<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_scalb<typename P_expr1::T_numtype,P_numtype2> > >
scalb(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_scalb<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_scalb<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
scalb(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_scalb<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_scalb<typename P_expr1::T_numtype,P_numtype2> > >
scalb(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_scalb<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_scalb<typename P_expr1::T_numtype,ptrdiff_t> > >
scalb(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_scalb<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<typename P_expr1::T_numtype,P_numtype2> > >
scalb(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_scalb<typename P_expr1::T_numtype,int> > >
scalb(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_scalb<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_scalb<typename P_expr1::T_numtype,float> > >
scalb(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_scalb<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_scalb<typename P_expr1::T_numtype,double> > >
scalb(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_scalb<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_scalb<typename P_expr1::T_numtype,long double> > >
scalb(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_scalb<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_scalb<typename P_expr1::T_numtype,complex<T2> > > >
scalb(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_scalb<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_scalb<typename P_expr1::T_numtype,ptrdiff_t> > >
scalb(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_scalb<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_scalb<P_numtype1,P_numtype2> > >
scalb(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_scalb<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_scalb<P_numtype1,typename P_expr2::T_numtype> > >
scalb(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_scalb<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_scalb<P_numtype1,P_numtype2> > >
scalb(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_scalb<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_scalb<P_numtype1,ptrdiff_t> > >
scalb(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_scalb<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<P_numtype1,P_numtype2> > >
scalb(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_scalb<P_numtype1,int> > >
scalb(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_scalb<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_scalb<P_numtype1,float> > >
scalb(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_scalb<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_scalb<P_numtype1,double> > >
scalb(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_scalb<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_scalb<P_numtype1,long double> > >
scalb(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_scalb<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_scalb<P_numtype1,complex<T2> > > >
scalb(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_scalb<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_scalb<P_numtype1,ptrdiff_t> > >
scalb(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_scalb<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_scalb<ptrdiff_t,P_numtype2> > >
scalb(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_scalb<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_scalb<ptrdiff_t,typename P_expr2::T_numtype> > >
scalb(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_scalb<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_scalb<ptrdiff_t,P_numtype2> > >
scalb(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_scalb<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_scalb<ptrdiff_t,ptrdiff_t> > >
scalb(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_scalb<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<ptrdiff_t,P_numtype2> > >
scalb(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_scalb<ptrdiff_t,int> > >
scalb(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_scalb<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_scalb<ptrdiff_t,float> > >
scalb(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_scalb<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_scalb<ptrdiff_t,double> > >
scalb(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_scalb<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_scalb<ptrdiff_t,long double> > >
scalb(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_scalb<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_scalb<ptrdiff_t,complex<T2> > > >
scalb(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_scalb<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_scalb<ptrdiff_t,ptrdiff_t> > >
scalb(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_scalb<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_scalb<P_numtype1,P_numtype2> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_scalb<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_scalb<P_numtype1,typename P_expr2::T_numtype> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_scalb<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_scalb<P_numtype1,P_numtype2> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_scalb<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_scalb<P_numtype1,ptrdiff_t> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_scalb<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<P_numtype1,P_numtype2> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_scalb<P_numtype1,int> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_scalb<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_scalb<P_numtype1,float> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_scalb<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_scalb<P_numtype1,double> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_scalb<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_scalb<P_numtype1,long double> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_scalb<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_scalb<P_numtype1,complex<T2> > > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_scalb<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_scalb<P_numtype1,ptrdiff_t> > >
scalb(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_scalb<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_scalb<int,P_numtype2> > >
scalb(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_scalb<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_scalb<int,typename P_expr2::T_numtype> > >
scalb(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_scalb<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_scalb<int,P_numtype2> > >
scalb(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_scalb<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_scalb<int,ptrdiff_t> > >
scalb(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_scalb<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<int,P_numtype2> > >
scalb(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_scalb<float,P_numtype2> > >
scalb(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_scalb<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_scalb<float,typename P_expr2::T_numtype> > >
scalb(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_scalb<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_scalb<float,P_numtype2> > >
scalb(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_scalb<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_scalb<float,ptrdiff_t> > >
scalb(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_scalb<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<float,P_numtype2> > >
scalb(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_scalb<double,P_numtype2> > >
scalb(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_scalb<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_scalb<double,typename P_expr2::T_numtype> > >
scalb(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_scalb<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_scalb<double,P_numtype2> > >
scalb(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_scalb<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_scalb<double,ptrdiff_t> > >
scalb(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_scalb<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<double,P_numtype2> > >
scalb(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_scalb<long double,P_numtype2> > >
scalb(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_scalb<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_scalb<long double,typename P_expr2::T_numtype> > >
scalb(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_scalb<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_scalb<long double,P_numtype2> > >
scalb(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_scalb<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_scalb<long double,ptrdiff_t> > >
scalb(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_scalb<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<long double,P_numtype2> > >
scalb(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_scalb<complex<T1> ,P_numtype2> > >
scalb(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_scalb<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_scalb<complex<T1> ,typename P_expr2::T_numtype> > >
scalb(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_scalb<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_scalb<complex<T1> ,P_numtype2> > >
scalb(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_scalb<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_scalb<complex<T1> ,ptrdiff_t> > >
scalb(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_scalb<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<complex<T1> ,P_numtype2> > >
scalb(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_scalb<ptrdiff_t,P_numtype2> > >
scalb(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_scalb<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_scalb<ptrdiff_t,typename P_expr2::T_numtype> > >
scalb(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_scalb<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_scalb<ptrdiff_t,P_numtype2> > >
scalb(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_scalb<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_scalb<ptrdiff_t,ptrdiff_t> > >
scalb(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_scalb<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_scalb<ptrdiff_t,P_numtype2> > >
scalb(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_scalb<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

#endif

/****************************************************************************
 * sin
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_sin<P_numtype1> > >
sin(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_sin<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_sin<typename P_expr1::T_numtype> > >
sin(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_sin<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_sin<P_numtype1> > >
sin(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_sin<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_sin<ptrdiff_t> > >
sin(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_sin<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_sin<P_numtype1> > >
sin(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_sin<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * sinh
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_sinh<P_numtype1> > >
sinh(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_sinh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_sinh<typename P_expr1::T_numtype> > >
sinh(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_sinh<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_sinh<P_numtype1> > >
sinh(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_sinh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_sinh<ptrdiff_t> > >
sinh(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_sinh<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_sinh<P_numtype1> > >
sinh(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_sinh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * sqr
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_sqr<P_numtype1> > >
sqr(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_sqr<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_sqr<typename P_expr1::T_numtype> > >
sqr(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_sqr<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_sqr<P_numtype1> > >
sqr(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_sqr<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_sqr<ptrdiff_t> > >
sqr(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_sqr<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_sqr<P_numtype1> > >
sqr(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_sqr<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * sqrt
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_sqrt<P_numtype1> > >
sqrt(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_sqrt<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_sqrt<typename P_expr1::T_numtype> > >
sqrt(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_sqrt<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_sqrt<P_numtype1> > >
sqrt(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_sqrt<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_sqrt<ptrdiff_t> > >
sqrt(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_sqrt<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_sqrt<P_numtype1> > >
sqrt(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_sqrt<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * tan
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_tan<P_numtype1> > >
tan(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_tan<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_tan<typename P_expr1::T_numtype> > >
tan(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_tan<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_tan<P_numtype1> > >
tan(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_tan<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_tan<ptrdiff_t> > >
tan(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_tan<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_tan<P_numtype1> > >
tan(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_tan<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * tanh
 ****************************************************************************/

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_tanh<P_numtype1> > >
tanh(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_tanh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_tanh<typename P_expr1::T_numtype> > >
tanh(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_tanh<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_tanh<P_numtype1> > >
tanh(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_tanh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_tanh<ptrdiff_t> > >
tanh(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_tanh<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_tanh<P_numtype1> > >
tanh(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_tanh<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


/****************************************************************************
 * uitrunc
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_uitrunc<P_numtype1> > >
uitrunc(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_uitrunc<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_uitrunc<typename P_expr1::T_numtype> > >
uitrunc(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_uitrunc<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_uitrunc<P_numtype1> > >
uitrunc(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_uitrunc<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_uitrunc<ptrdiff_t> > >
uitrunc(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_uitrunc<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_uitrunc<P_numtype1> > >
uitrunc(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_uitrunc<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * unordered
 ****************************************************************************/

#ifdef BZ_HAVE_SYSTEM_V_MATH
template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_unordered<P_numtype1,P_numtype2> > >
unordered(const Vector<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_unordered<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_unordered<P_numtype1,typename P_expr2::T_numtype> > >
unordered(const Vector<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_unordered<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_unordered<P_numtype1,P_numtype2> > >
unordered(const Vector<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_unordered<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
    _bz_unordered<P_numtype1,ptrdiff_t> > >
unordered(const Vector<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, Range,
        _bz_unordered<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<P_numtype1,P_numtype2> > >
unordered(const Vector<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_unordered<P_numtype1,int> > >
unordered(const Vector<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_unordered<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_unordered<P_numtype1,float> > >
unordered(const Vector<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_unordered<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_unordered<P_numtype1,double> > >
unordered(const Vector<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_unordered<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_unordered<P_numtype1,long double> > >
unordered(const Vector<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_unordered<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_unordered<P_numtype1,complex<T2> > > >
unordered(const Vector<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_unordered<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_unordered<P_numtype1,ptrdiff_t> > >
unordered(const Vector<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_unordered<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
    _bz_unordered<typename P_expr1::T_numtype,P_numtype2> > >
unordered(_bz_VecExpr<P_expr1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorIterConst<P_numtype2>,
        _bz_unordered<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
    _bz_unordered<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > >
unordered(_bz_VecExpr<P_expr1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExpr<P_expr2>,
        _bz_unordered<typename P_expr1::T_numtype,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
    _bz_unordered<typename P_expr1::T_numtype,P_numtype2> > >
unordered(_bz_VecExpr<P_expr1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, VectorPickIterConst<P_numtype2>,
        _bz_unordered<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
    _bz_unordered<typename P_expr1::T_numtype,ptrdiff_t> > >
unordered(_bz_VecExpr<P_expr1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, Range,
        _bz_unordered<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_expr1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<typename P_expr1::T_numtype,P_numtype2> > >
unordered(_bz_VecExpr<P_expr1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<typename P_expr1::T_numtype,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
    _bz_unordered<typename P_expr1::T_numtype,int> > >
unordered(_bz_VecExpr<P_expr1> d1, int d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<int>,
        _bz_unordered<typename P_expr1::T_numtype,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
    _bz_unordered<typename P_expr1::T_numtype,float> > >
unordered(_bz_VecExpr<P_expr1> d1, float d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<float>,
        _bz_unordered<typename P_expr1::T_numtype,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
    _bz_unordered<typename P_expr1::T_numtype,double> > >
unordered(_bz_VecExpr<P_expr1> d1, double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<double>,
        _bz_unordered<typename P_expr1::T_numtype,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
    _bz_unordered<typename P_expr1::T_numtype,long double> > >
unordered(_bz_VecExpr<P_expr1> d1, long double d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<long double>,
        _bz_unordered<typename P_expr1::T_numtype,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class P_expr1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_unordered<typename P_expr1::T_numtype,complex<T2> > > >
unordered(_bz_VecExpr<P_expr1> d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_unordered<typename P_expr1::T_numtype,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_unordered<typename P_expr1::T_numtype,ptrdiff_t> > >
unordered(_bz_VecExpr<P_expr1> d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<_bz_VecExpr<P_expr1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_unordered<typename P_expr1::T_numtype,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
    _bz_unordered<P_numtype1,P_numtype2> > >
unordered(const VectorPick<P_numtype1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorIterConst<P_numtype2>,
        _bz_unordered<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
    _bz_unordered<P_numtype1,typename P_expr2::T_numtype> > >
unordered(const VectorPick<P_numtype1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExpr<P_expr2>,
        _bz_unordered<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
    _bz_unordered<P_numtype1,P_numtype2> > >
unordered(const VectorPick<P_numtype1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, VectorPickIterConst<P_numtype2>,
        _bz_unordered<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
    _bz_unordered<P_numtype1,ptrdiff_t> > >
unordered(const VectorPick<P_numtype1>& d1, Range d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, Range,
        _bz_unordered<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<P_numtype1,P_numtype2> > >
unordered(const VectorPick<P_numtype1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
    _bz_unordered<P_numtype1,int> > >
unordered(const VectorPick<P_numtype1>& d1, int d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<int>,
        _bz_unordered<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
    _bz_unordered<P_numtype1,float> > >
unordered(const VectorPick<P_numtype1>& d1, float d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<float>,
        _bz_unordered<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
    _bz_unordered<P_numtype1,double> > >
unordered(const VectorPick<P_numtype1>& d1, double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<double>,
        _bz_unordered<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
    _bz_unordered<P_numtype1,long double> > >
unordered(const VectorPick<P_numtype1>& d1, long double d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<long double>,
        _bz_unordered<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_unordered<P_numtype1,complex<T2> > > >
unordered(const VectorPick<P_numtype1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_unordered<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_unordered<P_numtype1,ptrdiff_t> > >
unordered(const VectorPick<P_numtype1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<VectorPickIterConst<P_numtype1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_unordered<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
    _bz_unordered<ptrdiff_t,P_numtype2> > >
unordered(Range d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorIterConst<P_numtype2>,
        _bz_unordered<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
    _bz_unordered<ptrdiff_t,typename P_expr2::T_numtype> > >
unordered(Range d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExpr<P_expr2>,
        _bz_unordered<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
    _bz_unordered<ptrdiff_t,P_numtype2> > >
unordered(Range d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<Range, VectorPickIterConst<P_numtype2>,
        _bz_unordered<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, Range,
    _bz_unordered<ptrdiff_t,ptrdiff_t> > >
unordered(Range d1, Range d2)
{
    typedef _bz_VecExprOp<Range, Range,
        _bz_unordered<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<ptrdiff_t,P_numtype2> > >
unordered(Range d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<Range, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<int>,
    _bz_unordered<ptrdiff_t,int> > >
unordered(Range d1, int d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<int>,
        _bz_unordered<ptrdiff_t,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<int>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<float>,
    _bz_unordered<ptrdiff_t,float> > >
unordered(Range d1, float d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<float>,
        _bz_unordered<ptrdiff_t,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<float>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<double>,
    _bz_unordered<ptrdiff_t,double> > >
unordered(Range d1, double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<double>,
        _bz_unordered<ptrdiff_t,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<double>(d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
    _bz_unordered<ptrdiff_t,long double> > >
unordered(Range d1, long double d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<long double>,
        _bz_unordered<ptrdiff_t,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<long double>(d2)));
}

template<class T2>
inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
    _bz_unordered<ptrdiff_t,complex<T2> > > >
unordered(Range d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<complex<T2> > ,
        _bz_unordered<ptrdiff_t,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<complex<T2> > (d2)));
}


inline
_bz_VecExpr<_bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
    _bz_unordered<ptrdiff_t,ptrdiff_t> > >
unordered(Range d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<Range, _bz_VecExprConstant<ptrdiff_t>,
        _bz_unordered<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1, _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
    _bz_unordered<P_numtype1,P_numtype2> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorIterConst<P_numtype2>,
        _bz_unordered<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
    _bz_unordered<P_numtype1,typename P_expr2::T_numtype> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExpr<P_expr2>,
        _bz_unordered<P_numtype1,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
    _bz_unordered<P_numtype1,P_numtype2> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, VectorPickIterConst<P_numtype2>,
        _bz_unordered<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
    _bz_unordered<P_numtype1,ptrdiff_t> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, Range d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, Range,
        _bz_unordered<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2));
}

template<class P_numtype1, int N_length1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<P_numtype1,P_numtype2> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<P_numtype1,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), d2.beginFast()));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
    _bz_unordered<P_numtype1,int> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, int d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<int>,
        _bz_unordered<P_numtype1,int> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<int>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
    _bz_unordered<P_numtype1,float> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, float d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<float>,
        _bz_unordered<P_numtype1,float> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<float>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
    _bz_unordered<P_numtype1,double> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<double>,
        _bz_unordered<P_numtype1,double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<double>(d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
    _bz_unordered<P_numtype1,long double> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, long double d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<long double>,
        _bz_unordered<P_numtype1,long double> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<long double>(d2)));
}

template<class P_numtype1, int N_length1, class T2>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
    _bz_unordered<P_numtype1,complex<T2> > > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, complex<T2> d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<complex<T2> > ,
        _bz_unordered<P_numtype1,complex<T2> > > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<complex<T2> > (d2)));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
    _bz_unordered<P_numtype1,ptrdiff_t> > >
unordered(const TinyVector<P_numtype1, N_length1>& d1, ptrdiff_t d2)
{
    typedef _bz_VecExprOp<TinyVectorIterConst<P_numtype1, N_length1>, _bz_VecExprConstant<ptrdiff_t>,
        _bz_unordered<P_numtype1,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast(), _bz_VecExprConstant<ptrdiff_t>(d2)));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
    _bz_unordered<int,P_numtype2> > >
unordered(int d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorIterConst<P_numtype2>,
        _bz_unordered<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
    _bz_unordered<int,typename P_expr2::T_numtype> > >
unordered(int d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, _bz_VecExpr<P_expr2>,
        _bz_unordered<int,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
    _bz_unordered<int,P_numtype2> > >
unordered(int d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, VectorPickIterConst<P_numtype2>,
        _bz_unordered<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, Range,
    _bz_unordered<int,ptrdiff_t> > >
unordered(int d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, Range,
        _bz_unordered<int,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<int,P_numtype2> > >
unordered(int d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<int>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<int,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<int>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
    _bz_unordered<float,P_numtype2> > >
unordered(float d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorIterConst<P_numtype2>,
        _bz_unordered<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
    _bz_unordered<float,typename P_expr2::T_numtype> > >
unordered(float d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, _bz_VecExpr<P_expr2>,
        _bz_unordered<float,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
    _bz_unordered<float,P_numtype2> > >
unordered(float d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, VectorPickIterConst<P_numtype2>,
        _bz_unordered<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, Range,
    _bz_unordered<float,ptrdiff_t> > >
unordered(float d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, Range,
        _bz_unordered<float,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<float,P_numtype2> > >
unordered(float d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<float>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<float,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<float>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
    _bz_unordered<double,P_numtype2> > >
unordered(double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorIterConst<P_numtype2>,
        _bz_unordered<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
    _bz_unordered<double,typename P_expr2::T_numtype> > >
unordered(double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, _bz_VecExpr<P_expr2>,
        _bz_unordered<double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
    _bz_unordered<double,P_numtype2> > >
unordered(double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, VectorPickIterConst<P_numtype2>,
        _bz_unordered<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, Range,
    _bz_unordered<double,ptrdiff_t> > >
unordered(double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, Range,
        _bz_unordered<double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<double,P_numtype2> > >
unordered(double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<double>(d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
    _bz_unordered<long double,P_numtype2> > >
unordered(long double d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorIterConst<P_numtype2>,
        _bz_unordered<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
    _bz_unordered<long double,typename P_expr2::T_numtype> > >
unordered(long double d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, _bz_VecExpr<P_expr2>,
        _bz_unordered<long double,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
    _bz_unordered<long double,P_numtype2> > >
unordered(long double d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, VectorPickIterConst<P_numtype2>,
        _bz_unordered<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
    _bz_unordered<long double,ptrdiff_t> > >
unordered(long double d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, Range,
        _bz_unordered<long double,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<long double,P_numtype2> > >
unordered(long double d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<long double>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<long double,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<long double>(d1), d2.beginFast()));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
    _bz_unordered<complex<T1> ,P_numtype2> > >
unordered(complex<T1> d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorIterConst<P_numtype2>,
        _bz_unordered<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1, class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
    _bz_unordered<complex<T1> ,typename P_expr2::T_numtype> > >
unordered(complex<T1> d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , _bz_VecExpr<P_expr2>,
        _bz_unordered<complex<T1> ,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
    _bz_unordered<complex<T1> ,P_numtype2> > >
unordered(complex<T1> d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , VectorPickIterConst<P_numtype2>,
        _bz_unordered<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class T1>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
    _bz_unordered<complex<T1> ,ptrdiff_t> > >
unordered(complex<T1> d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , Range,
        _bz_unordered<complex<T1> ,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2));
}

template<class T1, class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<complex<T1> ,P_numtype2> > >
unordered(complex<T1> d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<complex<T1> > , TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<complex<T1> ,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<complex<T1> > (d1), d2.beginFast()));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
    _bz_unordered<ptrdiff_t,P_numtype2> > >
unordered(ptrdiff_t d1, const Vector<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorIterConst<P_numtype2>,
        _bz_unordered<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

template<class P_expr2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
    _bz_unordered<ptrdiff_t,typename P_expr2::T_numtype> > >
unordered(ptrdiff_t d1, _bz_VecExpr<P_expr2> d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, _bz_VecExpr<P_expr2>,
        _bz_unordered<ptrdiff_t,typename P_expr2::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
    _bz_unordered<ptrdiff_t,P_numtype2> > >
unordered(ptrdiff_t d1, const VectorPick<P_numtype2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, VectorPickIterConst<P_numtype2>,
        _bz_unordered<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
    _bz_unordered<ptrdiff_t,ptrdiff_t> > >
unordered(ptrdiff_t d1, Range d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, Range,
        _bz_unordered<ptrdiff_t,ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2));
}

template<class P_numtype2, int N_length2>
inline
_bz_VecExpr<_bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
    _bz_unordered<ptrdiff_t,P_numtype2> > >
unordered(ptrdiff_t d1, const TinyVector<P_numtype2, N_length2>& d2)
{
    typedef _bz_VecExprOp<_bz_VecExprConstant<ptrdiff_t>, TinyVectorIterConst<P_numtype2, N_length2>,
        _bz_unordered<ptrdiff_t,P_numtype2> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(_bz_VecExprConstant<ptrdiff_t>(d1), d2.beginFast()));
}

#endif

/****************************************************************************
 * y0
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_y0<P_numtype1> > >
y0(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_y0<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_y0<typename P_expr1::T_numtype> > >
y0(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_y0<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_y0<P_numtype1> > >
y0(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_y0<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_y0<ptrdiff_t> > >
y0(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_y0<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_y0<P_numtype1> > >
y0(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_y0<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

/****************************************************************************
 * y1
 ****************************************************************************/

#ifdef BZ_HAVE_IEEE_MATH
template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
    _bz_y1<P_numtype1> > >
y1(const Vector<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorIterConst<P_numtype1>,
        _bz_y1<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

template<class P_expr1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
    _bz_y1<typename P_expr1::T_numtype> > >
y1(_bz_VecExpr<P_expr1> d1)
{
    typedef _bz_VecExprUnaryOp<_bz_VecExpr<P_expr1>,
        _bz_y1<typename P_expr1::T_numtype> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
    _bz_y1<P_numtype1> > >
y1(const VectorPick<P_numtype1>& d1)
{
    typedef _bz_VecExprUnaryOp<VectorPickIterConst<P_numtype1>,
        _bz_y1<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}


inline
_bz_VecExpr<_bz_VecExprUnaryOp<Range,
    _bz_y1<ptrdiff_t> > >
y1(Range d1)
{
    typedef _bz_VecExprUnaryOp<Range,
        _bz_y1<ptrdiff_t> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1));
}

template<class P_numtype1, int N_length1>
inline
_bz_VecExpr<_bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
    _bz_y1<P_numtype1> > >
y1(const TinyVector<P_numtype1, N_length1>& d1)
{
    typedef _bz_VecExprUnaryOp<TinyVectorIterConst<P_numtype1, N_length1>,
        _bz_y1<P_numtype1> > T_expr;

    return _bz_VecExpr<T_expr>(T_expr(d1.beginFast()));
}

#endif

BZ_NAMESPACE_END

#endif
