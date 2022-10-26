cmake_minimum_required(VERSION 3.16)

add_executable(simple_ft_receiver.out simple_ft_receiver.c)
target_link_libraries(simple_ft_receiver.out my_library)

add_executable(simple_ft_sender.out simple_ft_sender.c)
target_link_libraries(simple_ft_sender.out my_library)
