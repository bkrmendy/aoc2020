cmake_minimum_required(VERSION 3.17)
project(aoc2020)

set(CMAKE_CXX_STANDARD 20)

# -----
# Tools
# -----

add_subdirectory(thirdparty/lexy)

add_library(aocutils
        aocutils/utils.h
        aocutils/utils.cpp)

# ----------------
# Daily challenges
# ----------------

add_executable(dayone day1/main.cpp)
include_directories(dayone aocutils thirdparty/lexy/include)
target_link_libraries(dayone aocutils)

add_executable(daytwo day2/main.cpp)
include_directories(daytwo aocutils thirdparty/lexy/include)
target_link_libraries(daytwo aocutils)
