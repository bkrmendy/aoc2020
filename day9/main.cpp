//
// Created by Berci on 2020. 12. 08..
//

#include <filesystem>

#include <fmt/core.h>
#include "util.h"

int main() {
    auto path = std::filesystem::path("../input/day9.txt");

    auto part_one = 1;
    auto part_two = 2;

    fmt::print("Part one: {}\n", part_one);
    fmt::print("Part one: {}\n", part_two);
}
