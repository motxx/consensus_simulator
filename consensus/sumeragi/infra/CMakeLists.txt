add_library(client client_with_simulate.cc)
target_link_libraries(client optional)

add_library(server server_with_simulate.cc)
target_link_libraries(server optional)
