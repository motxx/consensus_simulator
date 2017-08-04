addtest(leader_test leader_test.cc)
target_link_libraries(leader_test
  client
  leader
  validator
  member
  gtest
  )