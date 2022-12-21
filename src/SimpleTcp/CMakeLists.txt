cmake_minimum_required(VERSION 3.16)

add_executable(simple_tcp_server.out simple_tcp_server.c)
target_link_libraries(simple_tcp_server.out my_library)

add_executable(simple_tcp_client.out simple_tcp_client.c)
target_link_libraries(simple_tcp_client.out my_library)

add_executable(simple_tcp_client_with_nr.out simple_tcp_client_with_nr.c)
target_link_libraries(simple_tcp_client_with_nr.out my_library)
