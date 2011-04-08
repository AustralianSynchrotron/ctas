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

