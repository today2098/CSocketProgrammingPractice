cmake_minimum_required(VERSION 3.16)

add_executable(udp_echo_server.out udp_echo_server.c)
target_link_libraries(udp_echo_server.out my_library)

add_executable(udp_echo_client.out udp_echo_client.c)
target_link_libraries(udp_echo_client.out my_library)
