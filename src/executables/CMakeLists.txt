
link_directories(${CMAKE_BINARY_DIR}/src/common)

if(POLICY CMP0146)
  cmake_policy(SET CMP0146 OLD)
endif()
#find_package(OpenCV CONFIG)
#if(POLICY CMP0146)
#  cmake_policy(SET CMP0146 NEW)
#endif()
#if (OpenCV_FOUND)
#  include_directories( ${OpenCV_INCLUDE_DIRS} )
#  add_definitions(-DOPENCV_FOUND)
#endif()

find_library(POPTL poptmx)
find_path(POPTH poptmx.h REQUIRED)


add_executable(ct ct.cpp)
target_link_libraries(ct
  common
  lct
  ${POPTL}
)

add_executable(ring ring.cpp)
target_link_libraries(ring
  common
  lct
  ${POPTL}
)

add_executable(ax axis.cpp)
target_link_libraries(ax
  common
  lct
  ${POPTL}
)

add_executable(rc2fd rc2fd.cpp)
target_link_libraries(rc2fd
  common
  ledei
  ${POPTL}
)

add_executable(ipc ipc.cpp)
target_link_libraries(ipc
  common
  lipc
  ${POPTL}
)

add_executable(dei dei.cpp)
target_link_libraries(dei
  common
  ldei
  ${POPTL}
)

add_executable(edei edei.cpp)
target_link_libraries(edei
  common
  ledei
  ${POPTL}
)

add_executable(ts ts.cpp)
target_link_libraries(ts
  common
  lct
  ${POPTL}
)


add_executable(simulate-ipc simulate-ipc.cpp)
target_link_libraries(simulate-ipc
  common
  lipc
  ${POPTL}
)

add_executable(norm norm.cpp)
target_link_libraries(norm
  common
  ${POPTL}
)

add_executable(proj proj.cpp)
target_opencl(proj projection.cl)
target_link_libraries(proj
  common
  ${OpenCV_LIBS}
  ${POPTL}
)

add_executable(mosaic mosaic.cpp)
target_opencl(mosaic projection.cl)
target_link_libraries(mosaic
  common
  ${OpenCV_LIBS}
  ${POPTL}
)

add_executable(v2v vol2vol.cpp)
target_opencl(v2v ../common/matrix.cl)
target_link_libraries(v2v
  common
  ${POPTL}
)

add_executable(test test.cpp)
target_link_libraries(test
  common
  ${POPTL}
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






