# To make sure the internal version of blitz is used

include_directories("../blitz-long")

link_directories(${CMAKE_BINARY_DIR}/src/common)

add_executable(ct ct.cpp)
target_link_libraries(ct
  common
  poptmx
  kernel
)

add_executable(ct-vol ct-vol.cpp)
target_link_libraries(ct-vol
  common
  poptmx
  kernel
  experiment
)

add_executable(ct-line ct-line.cpp)
target_link_libraries(ct-line
  common
  poptmx
  kernel
)

add_executable(sino sino.cpp)
target_link_libraries(sino
  common
  poptmx
  experiment
)



add_executable(sino-abs sino-abs.cpp)
target_link_libraries(sino-abs
  common
  poptmx
  experiment
)

add_executable(sino-dei sino-dei.cpp)
target_link_libraries(sino-dei
  common
  poptmx
  experiment
  ldei
)

add_executable(sino-edei sino-edei.cpp)
target_link_libraries(sino-edei
  common
  poptmx
  experiment
  ledei
)

add_executable(sino-ipc sino-ipc.cpp)
target_link_libraries(sino-ipc
  common
  poptmx
  experiment
  lipc
)


add_executable(ff ff.cpp)
target_link_libraries(ff
  common
  poptmx
  experiment
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

add_executable(multipc multipc.cpp)
target_link_libraries(multipc
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

add_executable(ct-abs ct-abs.cpp)
target_link_libraries(ct-abs
  common
  poptmx
  experiment
  kernel
)

add_executable(ct-dei ct-dei.cpp)
target_link_libraries(ct-dei
  common
  poptmx
  experiment
  kernel
  ldei
)

add_executable(ct-edei ct-edei.cpp)
target_link_libraries(ct-edei
  common
  poptmx
  experiment
  kernel
  ledei
)

add_executable(ct-ipc ct-ipc.cpp)
target_link_libraries(ct-ipc
  common
  poptmx
  experiment
  kernel
  lipc
)

add_executable(f2i f2i.cpp)
target_link_libraries(f2i
  common
  poptmx
)

add_executable(simulate-ipc simulate-ipc.cpp)
target_link_libraries(simulate-ipc
  common
  poptmx
  lipc
)

add_executable(trans transform.cpp)
target_link_libraries(trans
  common
  poptmx
)

add_executable(norm norm.cpp)
target_link_libraries(norm
  common
  poptmx
  experiment
)

add_executable(proj projection.cpp)
target_link_libraries(proj
  common
  poptmx
)


# left here for the back compatibility with old imbl4massive scripts - to go away after 2019
add_executable(stitch stitch.cpp)
target_link_libraries(stitch
  common
  poptmx
)




set( all_targets
  multipc ipc dei edei trans norm
  sino sino-abs sino-dei sino-edei sino-ipc
  ct ct-vol ct-abs ct-dei ct-edei ct-ipc
  ct-line ts ff rc2fd f2i proj
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






