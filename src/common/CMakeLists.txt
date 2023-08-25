# To make sure the internal version of blitz is used
include_directories("../blitz-long")


# check ImageMagick
FIND_PACKAGE(ImageMagick 6.4.2  REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS Magick++ REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS MagickCore REQUIRED)
include_directories("${ImageMagick_INCLUDE_DIRS}")
FIND_PACKAGE(TIFF REQUIRED)
FIND_PACKAGE(HDF5 REQUIRED COMPONENTS C)
include_directories("${HDF5_INCLUDE_DIRS}")
#message("HDFPAR = ${HDF5_IS_PARALLEL}")



find_library(HAVE_FFTW fftw3)
find_library(HAVE_FFTWF fftw3f)
if( "${HAVE_FFTWF}" MATCHES "HAVE_FFTWF-NOTFOUND" )
  message(FATAL_ERROR "FFTW library with float point support not found.")
endif()
check_include_file(fftw3.h HAVE_FFTW3F_H ${CMAKE_CXX_FLAGS})
if( "${HAVE_FFTWF3F_H}" MATCHES "HAVE_FFTWF3F_H-NOTFOUND" )
  message(FATAL_ERROR "Development part of the FFTW library not found (fftw3f.h).")
endif()

find_library(HAVE_GSL gsl)
if( "${HAVE_GSL}" MATCHES "HAVE_GSL-NOTFOUND" )
  message(FATAL_ERROR "GSL library not found.")
endif()
find_library(HAVE_GSLCBLAS gslcblas)
if( "${HAVE_GSLCBLAS}" MATCHES "HAVE_GSLCBLAS-NOTFOUND" )
  message(FATAL_ERROR "GSLCBLAS library not found.")
endif()
check_include_file(gsl/gsl_sf_bessel.h HAVE_GSL_SF_BESSEL_H ${CMAKE_CXX_FLAGS})
if( "${HAVE_GSL_SF_BESSEL_H}" MATCHES "HAVE_GSL_SF_BESSEL_H-NOTFOUND" )
  message(FATAL_ERROR "Could not find \"gsl/gsl_sf_bessel.h\" header which must be a part of the GSL library.")
endif()


CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/src/common/config.h.in ${CMAKE_SOURCE_DIR}/src/common/config.h)

add_library(poptmx SHARED poptmx.cpp)

add_library(common SHARED
  ctas.h
  common.world.h common.world.cpp
  matrix.world.h matrix.world.cpp
  parallel.world.h parallel.world.cpp
  physical.world.h physical.world.cpp
  external.world.h external.world.cpp )
target_clsrc(common matrix.cl)
target_link_libraries(common
  tiff
  ${HDF5_LIBRARIES}
  ${ImageMagick_LIBRARIES}
  ${OpenCL_LIBRARIES}
  flatfield
)


add_library(flatfield SHARED flatfield.h flatfield.cpp)
target_clsrc(flatfield ff.cl)


add_library(lct SHARED ct.h lct.cpp)
target_clsrc(lct ct.cl)
target_link_libraries(lct
  fftw3f
  gsl
  gslcblas
  pthread
)


add_library(lipc SHARED ipc.h lipc.cpp)
target_clsrc(lipc ipc.cl)
target_link_libraries(lipc
  fftw3f
  ${CLFFT_LL}
)


add_library(ldei SHARED dei.h ldei.cpp)
add_library(ledei SHARED ledei.cpp)

install(TARGETS
  poptmx common lct lipc ldei ledei flatfield
  LIBRARY DESTINATION ${LIB_DESTINATION}
)

FILE(GLOB_RECURSE Headers "*.h")
add_custom_target(.headers SOURCES ${Headers})
FILE(GLOB_RECURSE Sources "*.cpp")
add_custom_target(.sources SOURCES ${Sources})
