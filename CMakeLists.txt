cmake_minimum_required(VERSION 3.17)
project(aoc2020)

set(CMAKE_CXX_STANDARD 20)

# -----
# Tools
# -----

add_library(aocutils aocutils/utils.h aocutils/utils.cpp)

# ----------------
# Daily challenges
# ----------------

add_executable(dayone day1/main.cpp)
include_directories(dayone aocutils)
target_link_libraries(dayone aocutils)

add_executable(daytwo day2/main.cpp)
include_directories(daytwo aocutils)
target_link_libraries(daytwo aocutils)

add_executable(daythree day3/main.cpp)
include_directories(daythree aocutils)
target_link_libraries(daythree aocutils)

add_executable(dayfour day4/main.cpp)
include_directories(dayfour aocutils)
target_link_libraries(dayfour aocutils)

add_executable(dayfive day5/main.cpp)
include_directories(dayfive aocutils)
target_link_libraries(dayfive aocutils)
