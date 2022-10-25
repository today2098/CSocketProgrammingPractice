cmake_minimum_required(VERSION 3.16)

add_executable(mcast_ft_receiver.out mcast_ft_receiver.c)
target_link_libraries(mcast_ft_receiver.out my_library)

add_executable(mcast_ft_sender.out mcast_ft_sender.c)
target_link_libraries(mcast_ft_sender.out my_library)
