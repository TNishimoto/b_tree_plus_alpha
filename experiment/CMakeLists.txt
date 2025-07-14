cmake_minimum_required(VERSION 3.21)
project(drfmi CXX)

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

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message("Release Mode")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()


INCLUDE_DIRECTORIES(modules/DYNAMIC/include)
INCLUDE_DIRECTORIES(modules/bit_vector)

INCLUDE_DIRECTORIES(../include)
INCLUDE_DIRECTORIES(../modules)


# For DYNAMIC
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# hopscotch_map
ExternalProject_Add(hopscotch_map
  GIT_REPOSITORY "https://github.com/Tessil/hopscotch-map.git"
  BUILD_IN_SOURCE TRUE
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR} # TODO ADD static build flag
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
  BUILD_COMMAND ""
  CONFIGURE_COMMAND "")
ExternalProject_Get_property(hopscotch_map INSTALL_DIR)
set(hopscotch_map_INCLUDE "${INSTALL_DIR}/src/hopscotch_map/include/")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${INSTALL_DIR}/src/hopscotch_map/include)

include_directories(${CMAKE_BINARY_DIR}/modules/DYNAMIC/hopscotch_map-prefix/src/hopscotch_map/include)


#FILE(GLOB SOURCES lib/*.cpp)

FILE(GLOB SOURCES lib/dynamic_rank_select/*.cpp)


add_executable(prefix_sum.out main/prefix_sum_main.cpp)
target_link_libraries(prefix_sum.out)

add_executable(bit_rank_select.out main/bit_rank_select_main.cpp)
target_link_libraries(bit_rank_select.out)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
    target_compile_options(bit_rank_select.out PRIVATE -mbmi2)
else()
    message(STATUS "Not a 64-bit architecture.")
endif()

add_executable(rank_select.out main/rank_select_main.cpp)
target_link_libraries(rank_select.out)

add_executable(permutation.out main/permutation_main.cpp)
target_link_libraries(permutation.out)
