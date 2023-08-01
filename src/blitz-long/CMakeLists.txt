include(ExternalProject)

ExternalProject_Add(blitz-long
#  PREFIX .
  SOURCE_DIR .
  BUILD_IN_SOURCE 1
  LOG_CONFIGURE 1
  CONFIGURE_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/configure"
  BUILD_COMMAND "continue"
  INSTALL_COMMAND "continue"
)
