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

typedef ssize_t MyIndexType;

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
inline TinyVector<MyIndexType,1> shape(MyIndexType n1)
{ return TinyVector<MyIndexType,1>(n1); }

inline TinyVector<MyIndexType,2> shape(MyIndexType n1, MyIndexType n2)
{ return TinyVector<MyIndexType,2>(n1,n2); }

inline TinyVector<MyIndexType,3> shape(
    MyIndexType n1, MyIndexType n2, MyIndexType n3)
{ return TinyVector<MyIndexType,3>(n1,n2,n3); }

inline TinyVector<MyIndexType,4> shape(
    MyIndexType n1, MyIndexType n2, MyIndexType n3, MyIndexType n4)
{ return TinyVector<MyIndexType,4>(n1,n2,n3,n4); }

inline TinyVector<MyIndexType,5> shape(
    MyIndexType n1, MyIndexType n2, MyIndexType n3, MyIndexType n4,
    MyIndexType n5)
{ return TinyVector<MyIndexType,5>(n1,n2,n3,n4,n5); }

inline TinyVector<MyIndexType,6> shape(
    MyIndexType n1, MyIndexType n2, MyIndexType n3, MyIndexType n4,
    MyIndexType n5, MyIndexType n6)
{ return TinyVector<MyIndexType,6>(n1,n2,n3,n4,n5,n6); }

inline TinyVector<MyIndexType,7> shape(
    MyIndexType n1, MyIndexType n2, MyIndexType n3, MyIndexType n4,
    MyIndexType n5, MyIndexType n6, MyIndexType n7)
{ return TinyVector<MyIndexType,7>(n1,n2,n3,n4,n5,n6,n7); }

inline TinyVector<MyIndexType,8> shape(
    MyIndexType n1, MyIndexType n2, MyIndexType n3, MyIndexType n4,
    MyIndexType n5, MyIndexType n6, MyIndexType n7, MyIndexType n8)
{ return TinyVector<MyIndexType,8>(n1,n2,n3,n4,n5,n6,n7,n8); }

inline TinyVector<MyIndexType,9> shape(
    MyIndexType n1, MyIndexType n2, MyIndexType n3, MyIndexType n4,
    MyIndexType n5, MyIndexType n6, MyIndexType n7, MyIndexType n8,
    MyIndexType n9)
{ return TinyVector<MyIndexType,9>(n1,n2,n3,n4,n5,n6,n7,n8,n9); }

inline TinyVector<MyIndexType,10> shape(
    MyIndexType n1, MyIndexType n2, MyIndexType n3, MyIndexType n4,
    MyIndexType n5, MyIndexType n6, MyIndexType n7, MyIndexType n8,
    MyIndexType n9, MyIndexType n10)
{ return TinyVector<MyIndexType,10>(n1,n2,n3,n4,n5,n6,n7,n8,n9,n10); }

inline TinyVector<MyIndexType,11> shape(
    MyIndexType n1, MyIndexType n2, MyIndexType n3, MyIndexType n4,
    MyIndexType n5, MyIndexType n6, MyIndexType n7, MyIndexType n8,
    MyIndexType n9, MyIndexType n10, MyIndexType n11)
{ return TinyVector<MyIndexType,11>(n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11); }

BZ_NAMESPACE_END

#endif // BZ_ARRAYSHAPE_H

