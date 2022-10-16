# To make sure the internal version of blitz is used

include_directories("../blitz-long")

link_directories(${CMAKE_BINARY_DIR}/src/common)

add_executable(ct ct.cpp)
target_link_libraries(ct
  common
  poptmx
  kernel
)


add_executable(rc2fd rc2fd.cpp)
target_link_libraries(rc2fd
  common
  poptmx
  ledei
)

add_executable(ipc ipc.cpp)
target_link_libraries(ipc
  common
  poptmx
  lipc
)

add_executable(dei dei.cpp)
target_link_libraries(dei
  common
  poptmx
  ldei
)

add_executable(edei edei.cpp)
target_link_libraries(edei
  common
  poptmx
  ledei
)

add_executable(ts ts.cpp)
target_link_libraries(ts
  common
  poptmx
  kernel
)


add_executable(simulate-ipc simulate-ipc.cpp)
target_link_libraries(simulate-ipc
  common
  poptmx
  lipc
)

add_executable(norm norm.cpp)
target_link_libraries(norm
  common
  poptmx
)

add_executable(proj proj.cpp)
target_clsrc(proj proj.cl)
target_link_libraries(proj
  common
  poptmx
  flatfield
)


add_executable(prct prct.cpp)
target_clsrc(prct proj.cl)
target_link_libraries(prct
  common
  poptmx
  flatfield
  lipc
  kernel
)



add_executable(v2v vol2vol.cpp)
target_clsrc(v2v ../common/binn.cl)
target_link_libraries(v2v
  common
  poptmx
)


add_executable(bctppl bctppl.cpp)
target_clsrc(bctppl formframe.cl)
target_link_libraries(bctppl
  common
  poptmx
  kernel
  flatfield
  lipc
)






#add_executable(test test.cpp)
#target_link_libraries(test
#  common
#  poptmx
#)


set( all_targets
  ipc dei edei norm ct ts rc2fd proj v2v bctppl
)


install(TARGETS ${all_targets}
  RUNTIME DESTINATION ${LIBEXEC_DESTINATION}
)


#foreach(targ ${all_targets})
#
#    set(manout "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${targ}.1")
#    add_custom_command( TARGET "${targ}" POST_BUILD
#      COMMAND LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/src/common "./${targ}" -very-long-key-phrase-to-output-man-pages >> ${manout}
#    )
#    install(FILES "${manout}" DESTINATION "${MAN_DESTINATION}")
#
#
#    set(htmlout "${CMAKE_HOME_DIRECTORY}/doc/site/userdoc/${PROJECT_NAME}-${targ}.1.html")
#    add_custom_target("${targ}_man_html"
#      COMMAND man2html -r ${manout} | sed -e "s:../man1/::g" > "${htmlout}"
#    )
#    add_dependencies(doc "${targ}_man_html")
#
#endforeach(targ)






