# To make sure the internal version of blitz is used
include_directories("../blitz-long")

# check ImageMagick
FIND_PACKAGE(ImageMagick REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS Magick++ REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS MagickCore REQUIRED)
# check it's version
find_program(MAGICKCONFIG, Magick++-config)
if( "${MAGICKCONFIG}" MATCHES "MAGICKCONFIG-NOTFOUND" )
  message(FATAL_ERROR "Magick++-config executable not found.")
endif()
execute_process(
  COMMAND Magick++-config --version
  COMMAND cut -d\  -f1
  COMMAND sed -e s:\\.:\;:g
  OUTPUT_VARIABLE IMV
)
list(GET IMV 0 IMV_MAJOR)
list(GET IMV 1 IMV_MINOR)
list(GET IMV 2 IMV_PATCH)
if (  # version < 6.4.2
  ( ${IMV_MAJOR} LESS 6 )
  OR ( ( ${IMV_MAJOR} EQUAL 6 )
    AND ( ( ${IMV_MINOR} LESS 4 )
      OR ( ( ${IMV_MINOR} EQUAL 4 ) AND ( ${IMV_PATCH} LESS 2 ) )
      )
    )
  )
  message("   ImageMagick version ... < 6.4.2")
  set(MAGICKLIB_NAMESPACE true) # for src/config.h
else()
  message("   ImageMagick version ... >= 6.4.2")
endif()
# check HDRI support in ImageMagick
execute_process(
  COMMAND Magick++-config --version
  COMMAND grep HDRI --quiet
  RESULT_VARIABLE IMHDRI
)
if ( ${IMHDRI} EQUAL 0 )
  message(FATAL_ERROR "Install ImageMagick library with _NO_ HDRI supprt.")
endif()

include_directories("${ImageMagick_INCLUDE_DIRS}")


FIND_PACKAGE(TIFF REQUIRED)

find_library(HAVE_FFTW fftw3)
find_library(HAVE_FFTWF fftw3f)
if( "${HAVE_FFTWF}" MATCHES "HAVE_FFTWF-NOTFOUND" )
  message(FATAL_ERROR "FFTW library with float point support not found.")
endif()
check_include_file(fftw3.h HAVE_FFTW3F_H)
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
check_include_file(gsl/gsl_sf_bessel.h HAVE_GSL_SF_BESSEL_H)
if( "${HAVE_GSL_SF_BESSEL_H}" MATCHES "HAVE_GSL_SF_BESSEL_H-NOTFOUND" )
  message(FATAL_ERROR "Could not find \"gsl/gsl_sf_bessel.h\" header which must be a part of the GSL library.")
endif()

find_library(HAVE_FREEIMAGE freeimage)
if( "${HAVE_FREEIMAGE}" MATCHES "HAVE_FREEIMAGE-NOTFOUND" )
  message(FATAL_ERROR "FreeImage library not found.")
endif()
check_include_file(FreeImage.h HAVE_FREEIMAGE_H)
if( "${HAVE_FREEIMAGE_H}" MATCHES "HAVE_FREEIMAGE_H-NOTFOUND" )
  message(FATAL_ERROR "Development part of the FreeImage library not found (FreeImage.h).")
endif()


CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/src/common/config.h.in ${CMAKE_SOURCE_DIR}/src/common/config.h)


add_library(poptmx SHARED poptmx.cpp)

add_library(common SHARED common.cpp)
target_link_libraries(common
  Magick++
  MagickCore
  freeimage
  tiff
  ${OPENCL_LL}
  config++
)

add_library(kernel SHARED kernel.cpp)
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
