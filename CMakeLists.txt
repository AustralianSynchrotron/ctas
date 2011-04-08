cmake_minimum_required (VERSION 2.6)
project (ctas)
set(VERSION 0.5.2)

include(CheckIncludeFile)
include(CheckLibraryExists)



set(LIBEXEC_DESTINATION "libexec/${PROJECT_NAME}")
set(LIB_DESTINATION "lib/${PROJECT_NAME}")
set(MAN_DESTINATION "man/man1")




get_target_property(DOC_TARGET doc TYPE)
if(NOT DOC_TARGET)
  add_custom_target(doc)
endif()

# Doxygen documentation
find_package(Doxygen)
if ( ${DOXYGEN_FOUND} MATCHES "YES")

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                 @ONLY)

  add_custom_target(doxygen
    COMMAND echo "  -- Running doxygen. Output will be redirected to \"\\\"${CMAKE_CURRENT_BINARY_DIR}/doxygen.log\\\"\"."
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile &> doxygen.log)
  add_dependencies(doc doxygen)

endif()


add_subdirectory(src)






