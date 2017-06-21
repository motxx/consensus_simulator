include(ExternalProject)

# Set default ExternalProject root directory
set(EP_PREFIX "${PROJECT_SOURCE_DIR}/third_party")
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})

##########################
#         gtest          #
##########################
# testing is an option. Look at the main CMakeLists.txt for details.
ExternalProject_Add(google_test
  GIT_REPOSITORY "https://github.com/google/googletest.git"
  CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
  -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
  -Dgtest_force_shared_crt=ON
  -Dgtest_disable_pthreads=OFF
  INSTALL_COMMAND "" # remove install step
  UPDATE_COMMAND "" # remove update step
  TEST_COMMAND "" # remove test step
  )
ExternalProject_Get_Property(google_test source_dir binary_dir)
set(gtest_SOURCE_DIR ${source_dir})
set(gtest_BINARY_DIR ${binary_dir})

add_library(gtest STATIC IMPORTED)
file(MAKE_DIRECTORY ${gtest_SOURCE_DIR}/googletest/include)

set_target_properties(gtest
  PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR}/googletest/include
  IMPORTED_LINK_INTERFACE_LIBRARIES "pthread;${gtest_BINARY_DIR}/googlemock/gtest/libgtest_main.a"
  IMPORTED_LOCATION ${gtest_BINARY_DIR}/googlemock/gtest/libgtest.a
  )

add_library(gmock STATIC IMPORTED)
file(MAKE_DIRECTORY ${gtest_SOURCE_DIR}/googlemock/include)

set_target_properties(gmock
  PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR}/googlemock/include
  IMPORTED_LINK_INTERFACE_LIBRARIES "pthread;${gtest_BINARY_DIR}/googlemock/libgmock_main.a"
  IMPORTED_LOCATION ${gtest_BINARY_DIR}/googlemock/libgmock.a
  )

add_dependencies(gtest google_test)
add_dependencies(gmock google_test)

######################
#       gflags       #
######################
externalproject_add(gflags_gflags
  URL "https://github.com/gflags/gflags/archive/v2.2.0.tar.gz"
  INSTALL_COMMAND "" # remove install step
  TEST_COMMAND "" # remove test step
  UPDATE_COMMAND "" # remove update step
  )
externalproject_get_property(gflags_gflags binary_dir)
set(gflags_INCLUDE_DIR ${binary_dir}/include)
set(gflags_LIBRARY ${binary_dir}/lib/libgflags.a)

add_library(gflags STATIC IMPORTED)
file(MAKE_DIRECTORY ${gflags_INCLUDE_DIR})

set_target_properties(gflags PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${gflags_INCLUDE_DIR}
  IMPORTED_LOCATION ${gflags_LIBRARY}
  IMPORTED_LINK_INTERFACE_LIBRARIES "pthread"
  )
add_dependencies(gflags gflags_gflags)

#############################
#         optional          #
#############################
ExternalProject_Add(martinmoene_optional
  GIT_REPOSITORY "https://github.com/martinmoene/optional-lite"
  GIT_TAG "v2.0.0"
  CONFIGURE_COMMAND "" # remove configure step
  BUILD_COMMAND "" # remove build step
  INSTALL_COMMAND "" # remove install step
  TEST_COMMAND "" # remove test step
  UPDATE_COMMAND "" # remove update step
  )
ExternalProject_Get_Property(martinmoene_optional source_dir)
set(optional_INCLUDE_DIRS ${source_dir}/include)
file(MAKE_DIRECTORY ${optional_INCLUDE_DIRS})

add_library(optional INTERFACE IMPORTED)
set_target_properties(optional PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${optional_INCLUDE_DIRS}
  )

add_dependencies(optional martinmoene_optional)


#[[
####################
#      snappy      #
####################
EXTERNALPROJECT_ADD(
  snappy_proj
  GIT_REPOSITORY https://github.com/google/snappy.git
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ./autogen.sh && ./configure
  SOURCE_DIR ${snappy_build}
  BUILD_COMMAND make -j${PROCESSOR_COUNT_VAL}
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND ""
)

SET(SNAPPY_INCLUDE_DIR ${snappy_build}/include)
SET(SNAPPY_LIB_DIR ${snappy_build})

ADD_LIBRARY(snappy STATIC IMPORTED)
SET_PROPERTY(TARGET snappy PROPERTY IMPORTED_LOCATION ${snappy_build}/.libs/${CMAKE_STATIC_LIBRARY_PREFIX}snappy${CMAKE_STATIC_LIBRARY_SUFFIX})

ADD_DEPENDENCIES(snappy snappy_proj)

############################
#         leveldb          #
############################
ExternalProject_Add(google_leveldb
  GIT_REPOSITORY    "https://github.com/google/leveldb.git"
  BUILD_IN_SOURCE   1
  BUILD_COMMAND     $(MAKE) -j1 OPT=-fPIC
  CONFIGURE_COMMAND "" # remove configure step
  INSTALL_COMMAND   "" # remove install step
  TEST_COMMAND      "" # remove test step
  UPDATE_COMMAND    "" # remove update step
  )
ExternalProject_Get_Property(google_leveldb source_dir)
set(leveldb_SOURCE_DIR "${source_dir}")

add_library(leveldb STATIC IMPORTED)
file(MAKE_DIRECTORY ${leveldb_SOURCE_DIR}/include)
file(MAKE_DIRECTORY ${leveldb_SOURCE_DIR}/out-shared)
file(MAKE_DIRECTORY ${leveldb_SOURCE_DIR}/out-static)
set_target_properties(leveldb PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${leveldb_SOURCE_DIR}/include
  IMPORTED_LINK_INTERFACE_LIBRARIES "snappy"
  IMPORTED_LOCATION ${leveldb_SOURCE_DIR}/out-static/libleveldb.a
  )
add_dependencies(leveldb google_leveldb)

INCLUDE(ExternalLeveldb)
INCLUDE_DIRECTORIES(${LEVELDB_INCLUDE_DIR})
LINK_DIRECTORIES(${LEVELDB_LIB_DIR})
]]