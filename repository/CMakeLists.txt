add_library(storage storage.cc)
target_link_libraries(storage
  #leveldb
  boost_serialization
  pthread
  optional
  )
