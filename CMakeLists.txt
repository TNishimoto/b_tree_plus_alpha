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

set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Og -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG -DTIME_DEBUG")


#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")


if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message("Release Mode")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Set PROJECT_IS_TOP_LEVEL to ON if if this is the top level project (not
# if this is included as a subproject of something else). Note that this is
# handled automatically for CMake >= 3.21.
if (CMAKE_VERSION VERSION_LESS 3.21)
    if ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
        message("SET PROJECT_IS_TOP_LEVEL ON IN ${PROJECT_NAME}")
        set (PROJECT_IS_TOP_LEVEL ON)
    else()
        message("SET PROJECT_IS_TOP_LEVEL OFF IN ${PROJECT_NAME}")
    set (PROJECT_IS_TOP_LEVEL OFF)    
    endif()
endif ()


#LINK_DIRECTORIES(~/lib)
#include_directories(~/include)
INCLUDE_DIRECTORIES(modules)

#FILE(GLOB SOURCES lib/*.cpp)
#FILE(GLOB SOURCES lib/dynamic_rank_select/*.cpp modules/libdivsufsort/sa.cpp)

#add_library(fn STATIC )
#target_link_libraries(fn)

add_executable(permutation_test.out main/test/permutation_test_main.cpp ${SOURCES})
target_link_libraries(permutation_test.out)

add_executable(spsi_test.out main/test/spsi_test_main.cpp ${SOURCES})
target_link_libraries(spsi_test.out)


add_executable(bit_test.out main/test/bit_test_main.cpp ${SOURCES})
target_link_libraries(bit_test.out)



add_executable(wavelet_tree_test.out main/test/wavelet_tree_test_main.cpp ${SOURCES})
target_link_libraries(wavelet_tree_test.out)

add_executable(sequence_test.out main/test/sequence_test_main.cpp ${SOURCES})
target_link_libraries(sequence_test.out)


#add_executable(performance.out main/performance_main.cpp ${SOURCES})
#target_link_libraries(performance.out)

#target_link_libraries(test.out fn)

