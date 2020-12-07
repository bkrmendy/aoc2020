//
// Created by Berci on 2020. 12. 02..
//

#include <fmt/core.h>
#include "utils.h"

int traverse(const std::vector<std::string>& map, size_t step_right, size_t step_down) {
    const char tree = '#';

    size_t offset = 0;
    size_t trees = 0;

    for (size_t i = 0; i < map.size(); i += step_down) {
        const auto& row = map.at(i);
        if (row[(offset * step_right) % row.size()] == tree) {
            trees = trees + 1;
        }
        offset = offset + 1;
    }

    return trees;
}

int main() {
    auto path = std::filesystem::path("../input/day3.txt");
    auto lines = lines_from_file(path);

    auto trees = traverse(lines, 3, 1);

    fmt::print("Part one: {}\n", trees);

    auto total_trees
        = traverse(lines, 1, 1)
        * traverse(lines, 3, 1)
        * traverse(lines, 5, 1)
        * traverse(lines, 7, 1)
        * traverse(lines, 1, 2);

    fmt::print("Part one: {}\n", total_trees);
}