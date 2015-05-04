# To make sure the internal version of blitz is used
include_directories("../blitz-long")


# check ImageMagick
FIND_PACKAGE(ImageMagick 6.4.2 REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS Magick++ REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS MagickCore REQUIRED)

include_directories("${ImageMagick_INCLUDE_DIRS}")

FIND_PACKAGE(TIFF REQUIRED)

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

if (OPENCL_FOUND)
  set(LIMITCL "limit.cl")
  set(LIMITCLSRC "${CMAKE_CURRENT_SOURCE_DIR}/${LIMITCL}")
  set(LIMITCLINC "${CMAKE_BINARY_DIR}/${LIMITCL}.includeme")
  add_custom_command(
    OUTPUT ${LIMITCLINC}
    COMMAND cat ${LIMITCLSRC} | xxd -i > ${LIMITCLINC}
    DEPENDS ${LIMITCLSRC}
  )
endif()

add_library(common SHARED common.cpp ${LIMITCLINC})
target_link_libraries(common 
  Magick++
  MagickCore
  tiff
  ${OPENCL_LL}
)

if (OPENCL_FOUND)
  set(CTCL "ct.cl")
  set(CTCLSRC "${CMAKE_CURRENT_SOURCE_DIR}/${CTCL}")
  set(CTCLINC "${CMAKE_BINARY_DIR}/${CTCL}.includeme")
  add_custom_command(
    OUTPUT ${CTCLINC}
    COMMAND cat ${CTCLSRC} | xxd -i > ${CTCLINC}
    DEPENDS ${CTCLSRC}
  )
endif()

add_library(kernel SHARED kernel.cpp ${CTCLINC})
target_link_libraries(kernel
  fftw3f
  gsl
  gslcblas
  pthread
)

add_library(lipc SHARED lipc.cpp)
target_link_libraries(lipc
  fftw3f
)

add_library(ldei SHARED ldei.cpp)
add_library(ledei SHARED ledei.cpp)


if (OPENCL_FOUND)

  set(FFCL "ff.cl")
  set(FFCLSRC "${CMAKE_CURRENT_SOURCE_DIR}/${FFCL}")
  set(FFCLINC "${CMAKE_BINARY_DIR}/${FFCL}.includeme")
  add_custom_command(
    OUTPUT ${FFCLINC}
    COMMAND cat ${FFCLSRC} | xxd -i > ${FFCLINC}
    DEPENDS ${FFCLSRC}
  )

endif()

add_library(experiment SHARED experiment.cpp ${FFCLINC})


install(TARGETS
  poptmx common kernel lipc ldei ledei experiment
  LIBRARY DESTINATION ${LIB_DESTINATION}
)
