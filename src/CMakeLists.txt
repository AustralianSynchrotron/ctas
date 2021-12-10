

if( NOT DEFINED no_opencl )

  FIND_PACKAGE (OpenCL)
  if (OPENCL_FOUND)
    add_definitions(-DOPENCL_FOUND)
    add_definitions(-DCL_TARGET_OPENCL_VERSION=220)
    set(OPENCL_LL "OpenCL")
    find_package(clFFT REQUIRED)
    set(CLFFT_LL "clFFT")
    else()
    message(WARNING "No OpenCL implementation found. Building for CPU only.")
  endif()
  
endif()

macro (target_clsrc trg clf)
if (OPENCL_FOUND)
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
endif()
endmacro()



add_subdirectory(blitz-long)
add_subdirectory(common)
add_subdirectory(executables)

CONFIGURE_FILE(ctas.in ctas @ONLY)
install(PROGRAMS ${CMAKE_BINARY_DIR}/src/ctas DESTINATION bin)


install(FILES "ctas.1" DESTINATION "${MAN_DESTINATION}")

add_custom_target("ctas_man_html"
  COMMAND man2html -r "ctas.1" | sed -e "s:../man1/::g" > "${CMAKE_HOME_DIRECTORY}/doc/site/userdoc/ctas.1.html"
)
add_dependencies(doc "ctas_man_html")

