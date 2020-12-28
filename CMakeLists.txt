cmake_minimum_required(VERSION 3.17)
project(aoc2020)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# -----
# Tools
# -----

add_subdirectory(aocutils/fmt)

add_library(aocutils
        ${utils_src}
        aocutils/utils.h
        aocutils/utils.cpp

        aocutils/overloaded.h

        aocutils/ctre.h)

target_link_libraries(aocutils fmt)

# Macro for daily challenge executable
function(Day day)
    add_executable(day${day} day${day}/main.cpp)
    include_directories(day${day} aocutils)
    target_link_libraries(day${day} aocutils)
endfunction()

# ----------------
# Daily challenges
# ----------------

Day(01)
Day(02)
Day(03)
Day(04)
Day(05)
Day(06)
Day(07)
Day(08)
Day(09)
Day(10)
Day(11)
Day(12)
Day(13)
Day(14)
Day(15)
Day(16)
Day(17)
Day(18)
Day(19)
Day(21)
Day(22)
Day(23)
Day(24)
Day(25)

# Day 18 part 2
add_executable(day18b day18/part_two.cpp)
include_directories(day18b aocutils)
target_link_libraries(day18b aocutils)


