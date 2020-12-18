//
// Created by Berci on 2020. 12. 18..
//

#include <fmt/core.h>
#include "utils.h"

int main() {
    auto path = std::filesystem::path{"../input/day17.txt"};
    auto lines = lines_from_file(path);

    fmt::print("Part one: {}\n", 1);
    fmt::print("Part two: {}\n", 2);
}