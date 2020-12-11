cmake_minimum_required(VERSION 3.17)
project(aoc2020)

set(CMAKE_CXX_STANDARD 20)

# -----
# Tools
# -----

add_subdirectory(aocutils/fmt)

add_library(aocutils
        ${utils_src}
        aocutils/utils.h
        aocutils/utils.cpp
        aocutils/overloaded.h)

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

Day(1)
Day(2)
Day(3)
Day(4)
Day(5)
Day(6)
Day(7)
Day(8)
Day(9)
Day(10)
Day(11)
Day(12)


