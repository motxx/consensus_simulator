add_library(leader leader.cc)
target_link_libraries(leader
  network_simulator
  optional
  )

add_library(validator validator.cc)
target_link_libraries(validator
  network_simulator
  optional
  )

add_library(validator_death validator_death.cc)
target_link_libraries(validator_death optional)

add_library(proxy_tail proxy_tail.cc)
target_link_libraries(proxy_tail
  network_simulator
  optional
  )

add_library(proxy_tail_death proxy_tail_death.cc)
target_link_libraries(proxy_tail_death optional)

add_library(member member.cc)
target_link_libraries(member
  network_simulator
  optional
  )
