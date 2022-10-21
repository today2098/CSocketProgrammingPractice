cmake_minimum_required(VERSION 3.16)

add_executable(mcast_receiver.out mcast_receiver.c)
target_link_libraries(mcast_receiver.out my_library)

add_executable(mcast_receiver_oldapi.out mcast_receiver_oldapi.c)
target_link_libraries(mcast_receiver_oldapi.out my_library)

add_executable(mcast_sender.out mcast_sender.c)
target_link_libraries(mcast_sender.out my_library)
