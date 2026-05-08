cmake_minimum_required(VERSION 3.21)
project(examples CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

INCLUDE_DIRECTORIES(../modules)

add_executable(dynamic_prefix_sum_example dynamic_prefix_sum_example.cpp)
target_link_libraries(dynamic_prefix_sum_example)

add_executable(dynamic_bit_example dynamic_bit_example.cpp)
target_link_libraries(dynamic_bit_example)

add_executable(dynamic_sequence_64_example dynamic_sequence_64_example.cpp)
target_link_libraries(dynamic_sequence_64_example)

add_executable(dynamic_wavelet_tree_example dynamic_wavelet_tree_example.cpp)
target_link_libraries(dynamic_wavelet_tree_example)

add_executable(dynamic_permutation_example dynamic_permutation_example.cpp)
target_link_libraries(dynamic_permutation_example)




#add_executable(performance main/performance_main.cpp ${SOURCES})
#target_link_libraries(performance)

#target_link_libraries(test fn)

