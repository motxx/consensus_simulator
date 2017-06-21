set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(simulator simulator.cc)
target_link_libraries(simulator
  sumeragi
  optional
  gflags
  )