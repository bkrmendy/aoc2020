//
// Created by Berci on 2020. 12. 10..
//

#include <filesystem>

#include <fmt/core.h>
#include "utils.h"

int main() {
    auto path = std::filesystem::path{"../input/day10.txt"};

    fmt::print("Part one: {}\n", 1337);
    fmt::print("Part two: {}\n", 8888);
}