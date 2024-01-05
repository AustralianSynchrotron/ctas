cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0045 NEW)

project(ctas)
set(VERSION 0.7.0)

set(CMAKE_MODULE_PATH ${ctas_SOURCE_DIR}/cmake)
set(CMAKE_C_STANDARD "17")
set(CMAKE_CXX_STANDARD "17")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffast-math -pthread")
set(CMAKE_CXX_FLAGS "$ENV{CPPFLAGS} ${CMAKE_CXX_FLAGS} -ffast-math -pthread")

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# dist target
if(NOT TARGET dist)
  add_custom_target(dist
    COMMAND git archive --prefix=${PROJECT_NAME}.${VERSION}/ HEAD | bzip2 >
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.${VERSION}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

include(CheckIncludeFile)
include(CheckLibraryExists)

set(LIBEXEC_DESTINATION "libexec/${PROJECT_NAME}")
set(LIB_DESTINATION "lib/${PROJECT_NAME}")
set(INC_DESTINATION "include/${PROJECT_NAME}")
set(MAN_DESTINATION "man/man1")
add_custom_target(doc)

# Doxygen documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(
    doxygen
    COMMAND
      echo
      "  -- Running doxygen. Output will be redirected to \"\\\"${CMAKE_CURRENT_BINARY_DIR}/doxygen.log\\\"\"."
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile &> doxygen.log)
endif()

# Just a workaroun to show cl files in Qt creator
file(GLOB_RECURSE OpenCLfiles "*.cl")
add_custom_target(OpenCLsource SOURCES ${OpenCLfiles})

add_subdirectory(src)
