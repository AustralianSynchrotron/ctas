set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(ImageMagick 6.4.2 REQUIRED
  COMPONENTS Magick++ MagickCore)
include_directories("${ImageMagick_INCLUDE_DIRS}")

find_package(TIFF REQUIRED)
include_directories("${TIFF_INCLUDE_DIRS}")

find_package(HDF5 REQUIRED COMPONENTS C)
include_directories("${HDF5_INCLUDE_DIRS}")

find_package(FFTW REQUIRED COMPONENTS FLOAT_LIB)
include_directories("${FFTW_INCLUDE_DIRS}")

find_package(GSL REQUIRED)
include_directories("${GSL_INCLUDE_DIRS}")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


function(target_headers target)
  set_target_properties(${target} PROPERTIES PUBLIC_HEADER "${ARGN}")
  target_sources(${target} PRIVATE "${ARGN}")
endfunction()


add_library(poptmx SHARED poptmx.cpp)

add_library(common SHARED
  common.world.cpp
  matrix.world.cpp
  parallel.world.cpp
  physical.world.cpp
  external.world.cpp
)
target_headers(common
  ctas.h
  common.world.h
  matrix.world.h
  parallel.world.h
  physical.world.h
  external.world.h)
target_opencl(common matrix.cl)
target_link_libraries(common
  ${HDF5_LIBRARIES}
  ${OpenCL_LIBRARIES}
  ImageMagick::Magick++
  TIFF::TIFF
  Threads::Threads
  flatfield
)

add_library(flatfield SHARED flatfield.cpp)
target_headers(flatfield flatfield.h)
target_opencl(flatfield ff.cl)

add_library(lct SHARED lct.cpp)
target_headers(lct ct.h)
target_opencl(lct ct.cl)
target_link_libraries(lct
  Threads::Threads
  FFTW::Float
  GSL::gsl
  GSL::gslcblas)

add_library(lipc SHARED lipc.cpp)
target_headers(lipc ipc.h)
target_opencl(lipc ipc.cl)
target_link_libraries(lipc
  FFTW::Float
  ${CLFFT_LL})

add_library(ldei SHARED ldei.cpp)
target_headers(ldei dei.h)

add_library(ledei SHARED ledei.cpp)

install(
  TARGETS poptmx
          common
          lct
          lipc
          ldei
          ledei
          flatfield
  LIBRARY DESTINATION ${LIB_DESTINATION}
  PUBLIC_HEADER DESTINATION ${INC_DESTINATION}
)

file(GLOB_RECURSE Headers "*.h")
add_custom_target(.headers SOURCES ${Headers})
#file(GLOB_RECURSE Sources "*.cpp")
#add_custom_target(.sources SOURCES ${Sources})
