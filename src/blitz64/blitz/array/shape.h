/***************************************************************************
 * blitz/array/shape.h        
 *
 * Copyright (C) 1997-2001 Todd Veldhuizen <tveldhui@oonumerics.org>
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
 * Suggestions:          blitz-dev@oonumerics.org
 * Bugs:                 blitz-bugs@oonumerics.org
 *
 * For more information, please see the Blitz++ Home Page:
 *    http://oonumerics.org/blitz/
 *
 ****************************************************************************/
#ifndef BZ_ARRAYSHAPE_H
#define BZ_ARRAYSHAPE_H

#ifndef BZ_ARRAY_H
 #error <blitz/array/shape.h> must be included via <blitz/array.h>
#endif

BZ_NAMESPACE(blitz)

/*
 * These routines make it easier to create shape parameters on
 * the fly: instead of having to write
 *
 * A.resize(TinyVector<int,4>(8,8,8,12));
 *
 * you can just say
 *
 * A.resize(shape(8,8,8,12));
 *
 */
inline TinyVector<ssize_t,1> shape(ssize_t n1)
{ return TinyVector<ssize_t,1>(n1); }

inline TinyVector<ssize_t,2> shape(ssize_t n1, ssize_t n2)
{ return TinyVector<ssize_t,2>(n1,n2); }

inline TinyVector<ssize_t,3> shape(
    ssize_t n1, ssize_t n2, ssize_t n3)
{ return TinyVector<ssize_t,3>(n1,n2,n3); }

inline TinyVector<ssize_t,4> shape(
    ssize_t n1, ssize_t n2, ssize_t n3, ssize_t n4)
{ return TinyVector<ssize_t,4>(n1,n2,n3,n4); }

inline TinyVector<ssize_t,5> shape(
    ssize_t n1, ssize_t n2, ssize_t n3, ssize_t n4,
    ssize_t n5)
{ return TinyVector<ssize_t,5>(n1,n2,n3,n4,n5); }

inline TinyVector<ssize_t,6> shape(
    ssize_t n1, ssize_t n2, ssize_t n3, ssize_t n4,
    ssize_t n5, ssize_t n6)
{ return TinyVector<ssize_t,6>(n1,n2,n3,n4,n5,n6); }

inline TinyVector<ssize_t,7> shape(
    ssize_t n1, ssize_t n2, ssize_t n3, ssize_t n4,
    ssize_t n5, ssize_t n6, ssize_t n7)
{ return TinyVector<ssize_t,7>(n1,n2,n3,n4,n5,n6,n7); }

inline TinyVector<ssize_t,8> shape(
    ssize_t n1, ssize_t n2, ssize_t n3, ssize_t n4,
    ssize_t n5, ssize_t n6, ssize_t n7, ssize_t n8)
{ return TinyVector<ssize_t,8>(n1,n2,n3,n4,n5,n6,n7,n8); }

inline TinyVector<ssize_t,9> shape(
    ssize_t n1, ssize_t n2, ssize_t n3, ssize_t n4,
    ssize_t n5, ssize_t n6, ssize_t n7, ssize_t n8,
    ssize_t n9)
{ return TinyVector<ssize_t,9>(n1,n2,n3,n4,n5,n6,n7,n8,n9); }

inline TinyVector<ssize_t,10> shape(
    ssize_t n1, ssize_t n2, ssize_t n3, ssize_t n4,
    ssize_t n5, ssize_t n6, ssize_t n7, ssize_t n8,
    ssize_t n9, ssize_t n10)
{ return TinyVector<ssize_t,10>(n1,n2,n3,n4,n5,n6,n7,n8,n9,n10); }

inline TinyVector<ssize_t,11> shape(
    ssize_t n1, ssize_t n2, ssize_t n3, ssize_t n4,
    ssize_t n5, ssize_t n6, ssize_t n7, ssize_t n8,
    ssize_t n9, ssize_t n10, ssize_t n11)
{ return TinyVector<ssize_t,11>(n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11); }

BZ_NAMESPACE_END

#endif // BZ_ARRAYSHAPE_H

