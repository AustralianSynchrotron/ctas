
FIND_PACKAGE (OpenCL REQUIRED)
include_directories("${OpenCL_INCLUDE_DIRS}")
add_definitions(-DOPENCL_FOUND)
add_definitions(-DCL_TARGET_OPENCL_VERSION=220)
set(OPENCL_LL "OpenCL")

find_package(clFFT REQUIRED)
include_directories("${CLFFT_INCLUDE_DIRS}")
set(CLFFT_LL "clFFT")

if(POLICY CMP0146)
  cmake_policy(SET CMP0146 OLD)
endif()
find_package(OpenCV CONFIG)
if(POLICY CMP0146)
  cmake_policy(SET CMP0146 NEW)
endif()
if (OpenCV_FOUND)
  include_directories( ${OpenCV_INCLUDE_DIRS} )
  add_definitions(-DOPENCV_FOUND)
endif()

macro (target_opencl trg clf)
  set( CLSRC "${CMAKE_CURRENT_SOURCE_DIR}/${clf}" )
  set( CLINC "${CMAKE_CURRENT_BINARY_DIR}/${clf}.includeme" )
  add_custom_command(
    OUTPUT ${CLINC}
    COMMAND cat ${CLSRC} | xxd -i > ${CLINC}
    DEPENDS ${CLSRC}
  )
  target_include_directories(${trg} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_sources( ${trg} PRIVATE ${CLINC} )
  unset(CLSRC)
  unset(CLINC)
endmacro()

add_subdirectory(blitz64)
include_directories("blitz64")
add_subdirectory(common)
add_subdirectory(executables)

CONFIGURE_FILE(ctas.in ctas @ONLY)
install(PROGRAMS ${CMAKE_BINARY_DIR}/src/ctas DESTINATION bin)
add_custom_target(.ctas SOURCES ctas.in)


install(FILES "ctas.1" DESTINATION "${MAN_DESTINATION}")

add_custom_target("ctas_man_html"
  COMMAND man2html -r "ctas.1" | sed -e "s:../man1/::g" > "${CMAKE_HOME_DIRECTORY}/doc/site/userdoc/ctas.1.html"
)
add_dependencies(doc "ctas_man_html")

