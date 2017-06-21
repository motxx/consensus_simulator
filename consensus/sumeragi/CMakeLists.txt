add_subdirectory(infra)
add_subdirectory(state)
#add_subdirectory(peer_service)

# sumeragi
add_library(sumeragi sumeragi.cc)
target_link_libraries(sumeragi
  leader
  validator
  validator_death
  proxy_tail
  proxy_tail_death
  member
  server
  client
  optional
  )