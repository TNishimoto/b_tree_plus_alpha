cmake_minimum_required(VERSION 3.21)
project(examples CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

INCLUDE_DIRECTORIES(../modules)
add_executable(spsi_example.out spsi_example.cpp)
target_link_libraries(spsi_example.out)

add_executable(dynamic_bit_example.out dynamic_bit_example.cpp)
target_link_libraries(dynamic_bit_example.out)



#add_executable(performance.out main/performance_main.cpp ${SOURCES})
#target_link_libraries(performance.out)

#target_link_libraries(test.out fn)

