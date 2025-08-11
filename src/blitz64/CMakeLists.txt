include(ExternalProject)


set(blitz_DESTDIR ${CMAKE_CURRENT_BINARY_DIR})
set(blitz_include ${blitz_DESTDIR}/${CMAKE_INSTALL_PREFIX}/include PARENT_SCOPE)

ExternalProject_Add(blitz64
#  PREFIX .
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BUILD_IN_SOURCE 1
  LOG_CONFIGURE 1
  CONFIGURE_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/configure"
                    "-q"
                    "--prefix=${CMAKE_INSTALL_PREFIX}"
  BUILD_COMMAND ${MAKE}
  INSTALL_COMMAND make DESTDIR=${blitz_DESTDIR} install >> /dev/null
)

install(
    DIRECTORY ${blitz_DESTDIR}/${CMAKE_INSTALL_PREFIX}/include/blitz
    DESTINATION "${CMAKE_INSTALL_PREFIX}/${INC_DESTINATION}"
    #USE_SOURCE_PERMISSIONS # Remain permissions (rwx) for installed files
)

FILE(GLOB_RECURSE Headers "*.h")
add_custom_target(.blitzheaders SOURCES ${Headers})
FILE(GLOB_RECURSE Sources "*.cc" "*.cpp")
add_custom_target(.blitzsources SOURCES ${Sources})

