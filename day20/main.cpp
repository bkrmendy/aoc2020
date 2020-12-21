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
    size_t top;
    size_t right;
    size_t bottom;
    size_t left;

    const std::vector<std::string> rows;

    Block(const size_t number,
          std::vector<std::string> lines)
        : number(number)
        , rows(std::move(lines)) {
        auto hasher = std::hash<std::string>{};
        this->top = hasher(rows[1]);
        this->bottom = hasher(rows[rows.size() - 1]);

        std::string first_row{};
        std::string last_row{};

        for (size_t i = 1; i < rows.size(); i++) {
            auto line = rows.at(i);
            first_row += rows.at(0);
            last_row += rows.at(line.size() - 1);
        }

        this->left = hasher(first_row);
        this->right = hasher(last_row);
    }
};

std::vector<Block> read_blocks(std::filesystem::path from) {
    auto blocks = blocks_from_file(from, "\n\n");

    std::vector<Block> res{};
    for (const auto& block : blocks) {
        auto lines = split(block, "\n");
        size_t number = std::stoul(lines[0].substr(lines[0].find(' ')));

        // add original orientation
        res.emplace_back(number, lines);

        // add upside down version
        auto upside_down = reverse(lines);
        res.emplace_back(number, upside_down);

        // add flipped version
        std::vector<std::string> flipped{};
        std::transform(lines.begin(),
                       lines.end(),
                       std::back_inserter(flipped),
                       [](std::string& row) { return reverse(row); });
        auto flipped_lines = flipped;
        res.emplace_back(number, flipped);

        // add upside-down flipped version
        res.emplace_back(number, reverse(flipped_lines));
    }

    assert(res.size() == blocks.size() * 4);

    return res;
}

int main() {
    auto path = std::filesystem::path("../input/day20.txt");
    auto blocks = read_blocks(path);
    
    std::map<size_t, std::vector<size_t>> edge_to_block;

    for (const auto& block : blocks) {
        for (const auto& id : { block.right, block.left, block.bottom, block.top }) {
            if (edge_to_block.contains(id)) {
                edge_to_block.emplace(id, std::vector<size_t>{});
            }
            edge_to_block.at(id).push_back(block.number);
        }
    }



    fmt::print("Part one: {}\n", 22);
    fmt::print("Part two: {}\n", 33);
}
