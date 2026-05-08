cmake_minimum_required(VERSION 3.21)
project(b_tree_plus_alpha CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CPP_STATIC)
set(CMAKE_CXX_FLAGS "-m64  -D_INT128 -Wall -Wextra -static")
else(CPP_STATIC)
set(CMAKE_CXX_FLAGS "-m64  -D_INT128 -Wall -Wextra")
endif(CPP_STATIC)

set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Og -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG -DTIME_DEBUG")


#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")


if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message("Release Mode")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

INCLUDE_DIRECTORIES(../modules)


add_executable(permutation_test permutation_test_main.cpp)
target_link_libraries(permutation_test)

add_executable(prefix_sum_test prefix_sum_test_main.cpp)
target_link_libraries(prefix_sum_test)

add_executable(bit_test bit_test_main.cpp)
target_link_libraries(bit_test)

add_executable(wavelet_tree_test wavelet_tree_test_main.cpp)
target_link_libraries(wavelet_tree_test)

add_executable(sequence_test sequence_test_main.cpp)
target_link_libraries(sequence_test)

add_executable(range_search_test range_search_test_main.cpp)
target_link_libraries(range_search_test)


