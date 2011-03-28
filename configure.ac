AC_PREREQ(2.59)

AC_INIT(ctas, 0.5.1)


AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([foreign 1.9 dist-bzip2 nostdinc no-installinfo])

AC_PROG_CXX
AC_LANG_CPLUSPLUS
#AC_CXX_FLAGS_PRESET
AM_PROG_LIBTOOL

AC_CONFIG_SUBDIRS([src/blitz-long])

AC_CHECK_LIB([fftw3], [main], , [AC_MSG_ERROR([Install 'fftw3' library])] )
AC_CHECK_LIB([fftw3f], [main], , [AC_MSG_ERROR([Install 'fftw3' library with float support])])
AC_CHECK_LIB([gslcblas], [main], , [AC_MSG_ERROR([Install 'gslcblas' library])])
AC_CHECK_LIB([gsl], [main], , [AC_MSG_ERROR([Install 'gsl' library])])
#AC_CHECK_LIB([gomp], [main], , [AC_MSG_ERROR([Reinstall gcc with OpenMP support])])
AC_CHECK_LIB([freeimage], [main], , [AC_MSG_ERROR([Install 'FreeImage' library])] )
AC_CHECK_LIB([tiff], [main], , [AC_MSG_ERROR([Install 'libtiff' library])] )



AC_CHECK_HEADER([fftw3.h], , [AC_MSG_ERROR([Install development part of the 'fftw3' library])])
AC_CHECK_HEADER([gsl/gsl_sf_bessel.h], , [AC_MSG_ERROR([Install development part of the 'gsl' library])])
#AC_CHECK_HEADER([omp.h], , [AC_MSG_ERROR([Reinstall gcc with OpenMP support])])

AC_CHECK_HEADER([tiffio.h], , [AC_MSG_ERROR([Install development part of the 'libtiff' library])])
AC_CHECK_HEADER([FreeImage.h], , [AC_MSG_ERROR([Install development part of the 'FreeImage'.])])



############# IMAGEMAGICK #################################
AC_CHECK_PROG(MAGICK,Magick++-config,yes,no)
if test "$MAGICK" = "no"; then
   AC_MSG_ERROR([Install 'ImageMagick' library])
fi

# Check for version
AC_MSG_CHECKING([ImageMagick version])
IFS='.' read a b c <<< "$(Magick++-config --version | cut -d' ' -f1)"
if (( a<6 || ((a==6 && ((b<4 || (( b==4 && c<2 )) )) )) ))  ;  then
   AC_MSG_RESULT([Old])
   AC_DEFINE([MAGICKLIB_NAMESPACE], [1], [MagickLib namespace is used in older ImageMagick < 6.4.2]) 
else
   AC_MSG_RESULT([New])
fi


MAGICK_LDFLAGS='`Magick++-config --ldflags --libs`'
MAGICK_INCFLAGS='`Magick++-config --cppflags`'
AC_MSG_CHECKING([HDRI support in ImageMagick])
if [ Magick++-config --version | grep HDRI --quiet ] ; then
   AC_MSG_RESULT([bad])
   AC_MSG_ERROR([Install 'ImageMagick' library with _NO_ hdri supprt])
fi
AC_MSG_RESULT([good])

AC_SUBST(MAGICK_LDFLAGS)
AC_SUBST(MAGICK_INCFLAGS)
##########################################################



AC_SUBST(PKGLIBEXECDIR, "$libexecdir/$PACKAGE")


# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([sqrt])
AC_CHECK_FUNC([sin],,[AC_CHECK_LIB(m,sin)])

# autoconf replacement of bzconfig
# AC_CHECK_CXX_FEATURES

# Check data type sizes
# AC_CHECK_DATATYPES

# Summarize variables
# AC_ENV


# AX_CREATE_PKGCONFIG_INFO

AC_OUTPUT(
		Makefile
		src/ctas
		src/Makefile
		src/executables/Makefile
		src/common/Makefile
	)

chmod +x src/ctas

#AC_INFO
