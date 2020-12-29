//
// Created by Berci on 2020. 12. 20..
//

#include <utility>
#include <vector>
#include <cmath>
#include <map>
#include <optional>

#include <fmt/core.h>
#include "utils.h"

/*
 * Solution ported from https://github.com/encse/adventofcode/blob/master/2020/Day20/Solution.cs
 * (stealing with taste)
 */

class Block {
    uint64_t number_;
    size_t size_;
    std::vector<std::string> image_;

    // the big idea
    size_t orientation = 0;

    std::string slice(size_t row, size_t column, size_t row_delta, size_t column_delta) {
        std::string result;
        for (size_t i = 0; i < size_; i++) {
            result += this->at(row, column);
            row += row_delta;
            column += column_delta;
        }
        return result;
    }

public:
    Block(uint64_t number, std::vector<std::string> image)
        : number_(number)
        , image_(std::move(image))
        , size_{image.size()}
        {}

    void changeOrientation() {
        orientation += 1;
    }

    char at(size_t row, size_t column) {
        std::pair<size_t, size_t> coords{row, column};

        for (size_t i = 0; i < orientation % 4; i++) {
            auto [rrow, ccolumn] = coords;
            coords = { ccolumn, size_ - 1 - rrow};
        }

        if (orientation % 8 >= 4) {
            auto [rrow, ccolumn] = coords;
            coords = {rrow, size_ - 1 - ccolumn};
        }

        auto [r, c] = coords;
        return image_.at(r).at(c);
    }

    std::string row(size_t r) {
        return slice(r, 0, 0, 1);
    }

    std::string column(size_t c) {
        return slice(0, c, 1, 0);
    }

    std::string top() {
        return row(0);
    }

    std::string bottom() {
        return row(size_ - 1);
    }

    std::string left() {
        return column(0);
    }

    std::string right() {
        return column(size_ - 1);
    }

    uint64_t number() const {
        return number_;
    }

    const std::vector<std::string>& image() const {
        return image_;
    }

    size_t size() const {
        return size_;
    }

};

bool operator==(const Block& left, const Block& right) {
    return left.number() == right.number();
}

std::vector<std::vector<Block>> assemble(std::vector<Block>& blocks) {
    auto pairs = std::map<std::string, std::vector<Block>>{};

    for (auto& block : blocks) {
        for (size_t i = 0; i < 8; i++) {
            auto pattern = block.top();
            if (not pairs.contains(pattern)) {
                pairs.emplace(pattern, std::vector<Block>{});
            }
            pairs.at(pattern).push_back(block);
            block.changeOrientation();
        }
    }

    auto place = [&pairs, &blocks](std::optional<Block>& above, std::optional<Block>& left) -> Block {
        auto is_edge = [&pairs](std::string&& pattern) -> bool { return pairs.at(pattern).size() == 1; };

        auto neighbor_of = [&pairs](Block& block, std::string&& pattern) {
            auto candidates = pairs.at(pattern);
            if (candidates.at(0) == block) {
                return candidates.at(1);
            }
            return candidates.at(0);
        };

        if (above == std::nullopt && left == std::nullopt) {
            for (auto& block : blocks) {
                for (size_t i = 0; i < 8; i++) {
                    if (is_edge(block.top()) && is_edge(block.left())) {
                        return block;
                    }
                    block.changeOrientation();
                }
            }
        } else {
            auto tile = above.has_value() ? neighbor_of(above.value(), above.value().bottom()) : neighbor_of(left.value(), left.value().right());
            while (true) {
                auto topMatch = !above.has_value() ? is_edge(tile.top()) : tile.top() == above->bottom();
                auto leftMatch = !left.has_value() ? is_edge(tile.left()) : tile.left() == left->right();

                if (topMatch && leftMatch) {
                    return tile;
                }
                tile.changeOrientation();
            }

        }
        assert(0 && "A fitting block should be found");
    };

    auto size = sqrt(blocks.size());
    auto result = std::vector<std::vector<Block>>{};

    for (size_t row = 0; row < size; row++) {
        auto this_row = std::vector<Block>{};
        for (size_t column = 0; column < size; column++) {
            std::optional<Block> above = std::nullopt;
            if (row > 0) {
                above = result.at(row - 1).at(column);
            }
            std::optional<Block> left = std::nullopt;
            if (column > 0) {
                left = this_row.at(column - 1);
            }
            this_row.push_back(place(above, left));
        }
        result.push_back(std::move(this_row));
    }

    return result;
}

std::vector<Block> read_blocks(std::filesystem::path from) {
    auto blocks = blocks_from_file(from, "\n\n");

    std::vector<Block> res{};
    for (const auto& block : blocks) {
        auto lines = split(block, "\n");
        size_t number = std::stoul(lines[0].substr(lines[0].find(' ')));

        std::vector<std::string> image{};
        for (size_t i = 1; i < lines.size(); i++) {
            image.push_back(lines.at(i));
        }

        res.emplace_back(number, image);
    }
    return res;
}

Block merge(std::vector<std::vector<Block>>& blocks) {
    auto image = std::vector<std::string>{};
    auto size = blocks.at(0).at(0).size();
    for (size_t row = 0; row < blocks.size(); row++) {
        for (size_t i = 1; i < size - 1; i++) {
            std::string this_row;
            for (size_t column = 0; column < blocks.size(); column++) {
                this_row += blocks.at(row).at(column).row(i).substr(1, size - 2);
            }
            image.push_back(std::move(this_row));
        }
    }
    assert(image.size() == blocks.size() * (blocks.at(0).at(0).size() - 2));
    assert(image.at(0).size() == blocks.size() * (blocks.at(0).at(0).size() - 2));
    return Block(0xE4C5E, image);
}

int match_count(Block& image, std::vector<std::string>& monster) {
    int result = 0;

    size_t monster_length = monster.at(0).size();
    size_t monster_height = monster.size();

    for (size_t row = 0; row < image.image().size() - monster_height; row++) {
        for(size_t column = 0; column < image.image().at(0).size() - monster_length; column++) {
            auto match = [&image, &monster, &row, &column]() -> bool {
                for (size_t c = 0; c < monster.at(0).size(); c++) {
                    for (size_t r = 0; r < monster.size(); r++) {
                        if (image.at(row + r, column + c) != '#' && monster.at(r).at(c) == '#') {
                            return false;
                        }
                    }
                }
                return true;
            };
            if (match()) {
                result += 1;
            }
        }
    }

    return result;
}

int main() {
    auto path = std::filesystem::path("../input/day20.txt");
    auto blocks = read_blocks(path);

    auto size = sqrt(blocks.size());

    auto image = assemble(blocks);

    auto topleft = image.at(0).at(0).number();
    auto topright = image.at(0).at(size - 1).number();
    auto bottomleft = image.at(size - 1).at(0).number();
    auto bottomright = image.at(size - 1).at(size - 1).number();

    fmt::print("Part one: {}\n", topleft * topright * bottomleft * bottomright);

    auto merged = merge(image);
    auto monster = std::vector<std::string> {
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   "
    };
    int monster_hash_count = 15;

    int hash_count = 0;
    for (auto& row : merged.image()) {
        hash_count += std::count_if(row.begin(), row.end(), [](char c) { return c == '#'; });
    }

    for (size_t i = 0; i < 8; i++) {
        int n_monsters = match_count(merged, monster);
        if (n_monsters > 0) {
            fmt::print("Part two: {}\n", hash_count - n_monsters * monster_hash_count);
            return EXIT_SUCCESS;
        }
        merged.changeOrientation();
    }
    assert(0 && "There have to be some monsters monsters around!");
}
