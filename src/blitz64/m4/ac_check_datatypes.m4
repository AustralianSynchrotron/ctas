AC_DEFUN([AC_CHECK_DATATYPES],[

AC_MSG_NOTICE([

Checking sizes of needed data types

])

AC_CHECK_HEADERS(stddef.h)

includes="#ifdef HAVE_STDDEF_H
          #include <stddef.h>
          #endif
         "

AC_COMPILE_CHECK_SIZEOF(int)
AC_COMPILE_CHECK_SIZEOF(ssize_t)
AC_COMPILE_CHECK_SIZEOF(ptrdiff_t, $includes)

])