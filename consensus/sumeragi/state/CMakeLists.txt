add_library(leader leader.cc)
target_link_libraries(leader optional)

add_library(validator validator.cc)
target_link_libraries(validator optional)

add_library(validator_death validator_death.cc)
target_link_libraries(validator_death optional)

add_library(proxy_tail proxy_tail.cc)
target_link_libraries(proxy_tail optional)

add_library(proxy_tail_death proxy_tail_death.cc)
target_link_libraries(proxy_tail_death optional)

add_library(member member.cc)
target_link_libraries(member optional)
