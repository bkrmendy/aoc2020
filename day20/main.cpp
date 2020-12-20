//
// Created by Berci on 2020. 12. 20..
//

#include <vector>
#include <cmath>
#include <utility>
#include <map>

#include <fmt/core.h>
#include "utils.h"

struct Block {
    size_t number;
    const size_t top;
    const size_t right;
    const size_t bottom;
    const size_t left;

    Block(const size_t number,
          const size_t top,
          const size_t right,
          const size_t bottom,
          const size_t left)
        : number(number)
        , top(top)
        , right(right)
        , bottom(bottom)
        , left(left
    ) {}

    // rotate right
    Block rotate() const {
        return Block{number, left, top, right, bottom};
    }

    Block flip_vertical() const {
        return Block{number, bottom, right, top, left};
    }

    Block flip_horizontal() const {
        return Block{number, top, left, bottom, right};
    }
};



std::vector<Block> read_blocks(std::filesystem::path from) {
    auto blocks = blocks_from_file(from, "\n\n");

    std::vector<Block> res{};
    for (const auto& block : blocks) {
        auto lines = split(block, "\n");
        size_t number = std::stoul(lines[0].substr(lines[0].find(" ")));
        auto hasher = std::hash<std::string>{};
        auto top = hasher(lines[1]);
        auto bottom = hasher(lines[lines.size() - 1]);

        std::string first_row{};
        std::string last_row{};
        for (size_t i = 1; i < lines.size(); i++) {
            auto line = lines.at(i);
            first_row += line.at(0);
            last_row += line.at(line.size() - 1);
        }

        auto first = hasher(first_row);
        auto last = hasher(last_row);

        res.emplace_back(number, top, last, bottom, first);
    }

    return res;
}

int main() {
    auto path = std::filesystem::path("../input/day20.txt");
    auto blocks = read_blocks(path);

    auto side_length = sqrt(blocks.size());


    std::map<std::pair<size_t, size_t>, Block> grid{};
    for (size_t row = 0; row < side_length; row++) {
        for (size_t column = 0; column < side_length; column++) {
            grid.emplace(std::make_pair<int, int>(row, column), blocks.at(row * side_length + column));
        }
    }

    fmt::print("Part one: {}\n", 22);
    fmt::print("Part two: {}\n", 33);
}
