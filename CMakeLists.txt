cmake_minimum_required(VERSION 3.17)
project(aoc2020)

set(CMAKE_CXX_STANDARD 20)

add_library(aocutils aocutils/utils.h aocutils/utils.cpp day2/main.cpp)

add_executable(dayone day1/main.cpp)
include_directories(dayone aocutils)
target_link_libraries(dayone aocutils)

add_executable(daytwo day2/main.cpp)
include_directories(daytwo aocutils)
target_link_libraries(daytwo aocutils)
