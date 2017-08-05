add_library(storage_with_sqlite storage_with_sqlite.cc)
target_link_libraries(storage_with_sqlite
  sqlitecpp
  expected
  optional
  )
