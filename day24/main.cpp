//
// Created by Berci on 2020. 12. 24..
//

#include <filesystem>

#include <fmt/core.h>
#include "aocutils.h"
#include "life.h"

enum Direction {
    East,       // e
    SouthEast,  // se
    SouthWest,  // sw
    West,       // w
    NorthWest,  // nw
    NorthEast   // ne
};

enum Side {
    Black,
    White
};

using Coordinate = std::pair<int, int>;

namespace std {
    template <>
    struct hash<Coordinate> {
        size_t operator()(Coordinate const &coordinate) const {
            auto[x, y] = coordinate;
            return x ^ y;
        }
    };
}

std::vector<std::vector<Direction>> parse_instructions(std::vector<std::string>&& lines) {
    std::vector<std::vector<Direction>> instructions;

    for (const auto& line : lines) {
        size_t offset = 0;
        std::vector<Direction> directions;
        while (offset < line.size()) {
            if (line.at(offset) == 'e') {
                directions.push_back(Direction::East);
            } else if (line.at(offset) == 'w') {
                directions.push_back(Direction::West);
            } else if (line.at(offset) == 's' && line.at(offset + 1) == 'e') {
                offset += 1;
                directions.push_back(Direction::SouthEast);
            } else if (line.at(offset) == 's' && line.at(offset + 1) == 'w') {
                offset += 1;
                directions.push_back(Direction::SouthWest);
            } else if (line.at(offset) == 'n' && line.at(offset + 1) == 'e') {
                offset += 1;
                directions.push_back(Direction::NorthEast);
            } else if (line.at(offset) == 'n' && line.at(offset + 1) == 'w') {
                offset += 1;
                directions.push_back(Direction::NorthWest);
            } else {
                assert(0 && "Unknown direction");
            }
            offset += 1;
        }
        instructions.push_back(std::move(directions));
    }

    return instructions;
}

/// https://www.redblobgames.com/grids/hexagons/#coordinates
/// “odd-r” horizontal layout
Coordinate move(Coordinate from, Direction direction) {
    auto [row, column] = from;
    switch (direction) {
        case East:
            return {row, column + 1};
        case West:
            return {row, column - 1};
        case NorthWest:
            if (row % 2 == 0) {
                return {row - 1, column - 1};
            } else {
                return {row - 1, column};
            }
        case SouthEast:
            if (row % 2 == 0) {
                return {row + 1, column};
            } else {
                return {row + 1, column + 1};
            }
        case NorthEast:
            if (row % 2 == 0) {
                return {row - 1, column};
            } else {
                return {row - 1, column + 1};
            }
        case SouthWest:
            if (row % 2 == 0) {
                return {row + 1, column - 1};
            } else {
                return {row + 1, column};
            }
    }
    assert(0 && "Unknown direction");
}

using HexLife = Life<Coordinate, Side>;

template <>
Side make_active_state() { return Side::Black; }

template <>
Side make_inactive_state() { return Side::White; }

template<>
Side next_state(const Side& this_state, size_t active_neighbors) {
    if (this_state == Side::Black
        && (active_neighbors == 0 || active_neighbors > 2)) {
        return Side::White;
    } else if (this_state == Side::White && active_neighbors == 2) {
        return Side::Black;
    } else {
        return this_state;
    }
}

/// https://www.redblobgames.com/grids/hexagons/#neighbors-offset
/// "odd-r"
template <>
HexLife::Container neighbors_of(const Coordinate& coord) {
    auto [row, column] = coord;
    if (row % 2 == 0) {
        return {
            {row, column + 1},          // e
            {row, column - 1},          // w
            {row - 1, column - 1},  // nw
            {row + 1, column},          // se
            {row - 1, column},          // ne
            {row + 1, column - 1}   // sw
        };
    } else {
        return {
                {row, column + 1},          // e
                {row, column - 1},          // w
                {row - 1, column},          // nw
                {row + 1, column + 1},  // se
                {row - 1, column + 1},  // ne
                {row + 1, column}           // sw
        };
    }
}

size_t play(HexLife::Container& tiles, size_t rounds) {
    auto life = Life<Coordinate, Side>();
    for (size_t round = 0; round < rounds; round++) {
        tiles = life.step(tiles);
    }
    return tiles.size();
}

int main() {
    auto path = std::filesystem::path{"../input/day24.txt"};
    auto instructions = parse_instructions(lines_from_file(path));

    /// part one
    HexLife::Container tiles{};
    for (const auto& sequence : instructions) {
        auto coord = Coordinate{0, 0};

        for (auto& instruction : sequence) {
            coord = move(coord, instruction);
        }

        if (!tiles.contains(coord)) {
            tiles.insert(coord);
        } else {
            tiles.erase(coord);
        }
    }

    fmt::print("Part one: {}\n", tiles.size());
    fmt::print("Part two: {}\n", play(tiles, 100));
}