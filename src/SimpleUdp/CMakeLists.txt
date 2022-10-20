cmake_minimum_required(VERSION 3.16)

add_executable(simple_udp_receiver.out simple_udp_receiver.c)
target_link_libraries(simple_udp_receiver.out my_library)

add_executable(simple_udp_sender.out simple_udp_sender.c)
target_link_libraries(simple_udp_sender.out my_library)
