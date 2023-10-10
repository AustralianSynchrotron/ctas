
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

FIND_PACKAGE(ImageMagick 6.4.2  REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS Magick++ REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS MagickCore REQUIRED)
include_directories("${ImageMagick_INCLUDE_DIRS}")

FIND_PACKAGE(TIFF REQUIRED)
include_directories("${TIFF_INCLUDE_DIRS}")

FIND_PACKAGE(HDF5 REQUIRED COMPONENTS C)
include_directories("${HDF5_INCLUDE_DIRS}")

FIND_PACKAGE(FFTW COMPONENTS FLOAT_LIB REQUIRED)
include_directories("${FFTW_INCLUDE_DIRS}")

FIND_PACKAGE(GSL REQUIRED)
include_directories("${GSL_INCLUDE_DIRS}")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


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
  ${HDF5_LIBRARIES}
  ${ImageMagick_LIBRARIES}
  ${OpenCL_LIBRARIES}
  TIFF::TIFF
  Threads::Threads
  flatfield
)


add_library(flatfield SHARED flatfield.h flatfield.cpp)
target_clsrc(flatfield ff.cl)


add_library(lct SHARED ct.h lct.cpp)
target_clsrc(lct ct.cl)
target_link_libraries(lct
  Threads::Threads
  FFTW::Float
  GSL::gsl
  GSL::gslcblas
)


add_library(lipc SHARED ipc.h lipc.cpp)
target_clsrc(lipc ipc.cl)
target_link_libraries(lipc
  FFTW::Float
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
