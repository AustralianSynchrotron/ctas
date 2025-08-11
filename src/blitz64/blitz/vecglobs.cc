/*
 * $Id: vecglobs.cc,v 1.3 2007/09/06 00:53:17 julianc Exp $
 *
 * Copyright (C) 1997 Todd Veldhuizen <tveldhui@oonumerics.org>
 * All rights reserved.  Please see <blitz/blitz.h> for terms and
 * conditions of use.
 *
 */

#ifndef BZ_VECGLOBS_CC
#define BZ_VECGLOBS_CC

#ifndef BZ_VECGLOBS_H
 #include "vecglobs.h"
#endif

#include "vecaccum.cc"    // accumulate()
#include "vecdelta.cc"    // delta()
#include "vecmin.cc"      // min(), minValue(), minIndex()
#include "vecmax.cc"      // max(), maxValue(), maxIndex()
#include "vecsum.cc"      // sum(), mean()
#include "vecproduct.cc"  // product()
#include "vecdot.cc"      // dot()
#include "vecnorm.cc"     // norm()
#include "vecnorm1.cc"    // norm1()
#include "vecany.cc"      // any()
#include "vecall.cc"      // all()
#include "veccount.cc"    // count()

#endif // BZ_VECGLOBS_CC
