cmake_minimum_required(VERSION 3.16)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(multi_tcp_echo_server.out multi_tcp_echo_server.c)
target_link_libraries(multi_tcp_echo_server.out my_library Threads::Threads)

add_executable(multi_tcp_echo_client.out multi_tcp_echo_client.c)
target_link_libraries(multi_tcp_echo_client.out my_library)
