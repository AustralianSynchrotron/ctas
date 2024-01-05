
link_directories(${CMAKE_BINARY_DIR}/src/common)

add_executable(ct ct.cpp)
target_link_libraries(ct
  common
  poptmx
  lct
)

add_executable(ring ring.cpp)
target_link_libraries(ring
  common
  poptmx
  lct
)

add_executable(ax axis.cpp)
target_link_libraries(ax
  common
  poptmx
  lct
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
  lct
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
target_opencl(proj projection.cl)
target_link_libraries(proj
  common
  poptmx
  ${OpenCV_LIBS}
)

add_executable(mosaic mosaic.cpp)
target_opencl(mosaic projection.cl)
target_link_libraries(mosaic
  common
  poptmx
  ${OpenCV_LIBS}
)



add_executable(v2v vol2vol.cpp)
target_opencl(v2v ../common/matrix.cl)
target_link_libraries(v2v
  common
  poptmx
  flatfield
)


add_executable(test test.cpp)
target_link_libraries(test
  common
  poptmx
)


set( all_targets
  ipc dei edei norm ax ct ts rc2fd proj mosaic v2v ring
)


install(TARGETS ${all_targets}
  RUNTIME DESTINATION ${LIBEXEC_DESTINATION}
)


foreach(targ ${all_targets})

    set(manout "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${targ}.1")
    add_custom_command( TARGET "${targ}" POST_BUILD
      COMMAND LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/src/common "./${targ}" -very-long-key-phrase-to-output-man-pages >> ${manout}
    )
    install(FILES "${manout}" DESTINATION "${MAN_DESTINATION}")


    set(htmlout "${CMAKE_HOME_DIRECTORY}/doc/site/userdoc/${PROJECT_NAME}-${targ}.1.html")
    add_custom_target("${targ}_man_html"
      COMMAND man2html -r ${manout} | sed -e "s:../man1/::g" > "${htmlout}"
    )
    add_dependencies(doc "${targ}_man_html")

endforeach(targ)






