/* Select the compiler-specific config.h header file */
#ifndef BZCONFIG_H
#define BZCONFIG_H

#if defined(__APPLE__)
/* IBM xlc compiler for Darwin */
#include "apple/bzconfig.h"

#elif defined(__INTEL_COMPILER)
/* Intel icc compiler */
#include "intel/bzconfig.h"

#elif defined(_MSC_VER)
/* Microsoft VS.NET compiler */
#include "ms/bzconfig.h"

#elif defined(__xlC__)
/* IBM xlC compiler */
#include "ibm/bzconfig.h"

#elif defined(__DECCXX)
/* Compaq cxx compiler */
#include "compaq/bzconfig.h"

#elif defined(__HP_aCC)
/* HP aCC compiler */
#include "hp/bzconfig.h"

#elif defined(_SGI_COMPILER_VERSION)
/* SGI CC compiler */
#include "sgi/bzconfig.h"

#elif defined(__SUNPRO_CC)
/* SunPRO CC compiler */
#include "sun/bzconfig.h"

#elif defined(__PATHCC__)
/* Pathscale pathCC compiler */
#include "pathscale/bzconfig.h"

#elif defined(__GNUC__)
/* GNU gcc compiler */
#include "gnu/bzconfig.h"

#elif defined(__PGI)
/* PGI pgCC compiler */
#include "pgi/bzconfig.h"

#elif defined(__KCC)
/* KAI KCC compiler */
#include "kai/bzconfig.h"

#elif defined(__FUJITSU)
/* Fujitsu FCC compiler */
#include "fujitsu/bzconfig.h"

/* Add other compilers here */

#else
#error Unknown compiler
#endif

#endif /* BZCONFIG_H */
