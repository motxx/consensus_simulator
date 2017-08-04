set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_library(network_simulator network_simulator.cc)
target_link_libraries(network_simulator
  server
  optional
  )

add_executable(simulator simulator.cc)
target_link_libraries(simulator
  sumeragi
  optional
  gflags
  )
