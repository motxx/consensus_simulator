
SET(PREFIX_SNAPPY ${PROJECT_SOURCE_DIR}/third_party)
ExternalProject_Add(google_snappy
  PREFIX ${PREFIX_SNAPPY}
  GIT_REPOSITORY https://github.com/google/snappy/
  BUILD_COMMAND mkdir ${PREFIX_SNAPPY}/src/google_snappy/build; cd ${PREFIX_SNAPPY}/src/google_snappy/build && cmake ../ && make
  INSTALL_COMMAND make install
  INSTALL_DIR ${CMAKE_BINARY_DIR}/third_party
  # Update command has to be set to "", otherwise CMake will refetch
  # & rebuild every time.
  UPDATE_COMMAND ""
  )

ExternalProject_Get_Property(google_snappy install_dir)
INCLUDE_DIRECTORIES (${install_dir}/include)

ADD_LIBRARY(snappy STATIC IMPORTED)
SET_PROPERTY(TARGET snappy PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/third_party/snappy)
ADD_DEPENDENCIES(snappy google_snappy)
