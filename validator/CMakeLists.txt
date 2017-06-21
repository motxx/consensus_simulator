add_library(stateful_validator stateful_validator.cc)
target_link_libraries(stateful_validator
  storage
  )
