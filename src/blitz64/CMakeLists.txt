include(ExternalProject)

ExternalProject_Add(blitz64
#  PREFIX .
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BUILD_IN_SOURCE 1
  LOG_CONFIGURE 1
  CONFIGURE_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/configure"
  BUILD_COMMAND "continue"
  INSTALL_COMMAND "continue"
)

FILE(GLOB_RECURSE Headers "*.h")
add_custom_target(.blitzheaders SOURCES ${Headers})
FILE(GLOB_RECURSE Sources "*.cc" "*.cpp")
add_custom_target(.blitzsources SOURCES ${Sources})

