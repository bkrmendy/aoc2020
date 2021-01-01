//
// Created by Berci on 2020. 12. 24..
//

#include <filesystem>
#include <vector>
#include <map>

#include <fmt/core.h>
#include "aocutils.h"

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

struct hasher : std::unary_function<Coordinate, size_t> {
    size_t operator()(const Coordinate& c) const {
        return c.first ^ c.second;
    }
};

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

template<typename State>
State next_state(State this_state, size_t active_neighbors) {
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
std::vector<Coordinate> neighbors_of(const Coordinate& coord) {
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

template<typename Coord, typename State>
std::vector<State> neighbors_of(std::unordered_map<Coord, State, hasher>& cubes, Coord coord) {
    auto neighbors = neighbors_of(coord);
    std::vector<State> states{};
    std::transform(neighbors.begin(),
                   neighbors.end(),
                   std::back_inserter(states),
                   [&cubes](const auto& c) { return at_with_default(cubes, c, Side::White); });
    return states;
}

template<typename Coord, typename State>
std::unordered_map<Coord, State, hasher> extend_neighbors(std::unordered_map<Coord, Side, hasher>& cubes,
                                                          State default_state) {
    std::unordered_map<Coordinate, Side, hasher> these_tiles{};
    for (const auto& [coord, _] : cubes) {
        const auto& neighbors = neighbors_of(coord);
        these_tiles.insert(std::make_pair(coord, default_state));
        for (const Coord& neighbor : neighbors) {
            these_tiles.insert(std::make_pair(neighbor, default_state));
        }
    }

    for (const auto& [coord, state] : cubes) {
        these_tiles.at(coord) = state;
    }

    return these_tiles;
}

template<typename Coord, typename State>
std::unordered_map<Coord, State, hasher> step(std::unordered_map<Coord, State, hasher>& tiles) {
    auto these_cubes = tiles;
    for (const auto& [coord, state] : tiles) {
        auto neighbors = neighbors_of(tiles, coord);
        auto actives = std::count_if(neighbors.begin(),
                                     neighbors.end(),
                                     [](auto state) { return state == Side::Black; });

        these_cubes.at(coord) = next_state(state, actives);
    }
    return these_cubes;
}

template<typename Coord, typename State>
int play(std::unordered_map<Coord, State, hasher>& tiles, size_t rounds) {
    for (size_t cycle = 0; cycle < rounds; cycle++) {
        auto extended_tiles = extend_neighbors(tiles, Side::White);
        tiles = step(extended_tiles);
    }

    return std::count_if(tiles.begin(),
                         tiles.end(),
                         [](auto& tile) {
        auto& [_, state] = tile;
        return state == Side::Black;
    });
}

int main() {
    auto path = std::filesystem::path{"../input/day24.txt"};
    auto instructions = parse_instructions(lines_from_file(path));

    auto n_blacks = [](std::unordered_map<Coordinate, Side, hasher>& map) {
        return std::count_if(map.begin(), map.end(), [](auto& entry) {
            auto& [_, side] = entry;
            return side == Side::Black;
        });
    };

    /// part one
    std::unordered_map<Coordinate, Side, hasher> tiles{};
    for (const auto& sequence : instructions) {
        auto coord = Coordinate{0, 0};

        for (auto& instruction : sequence) {
            coord = move(coord, instruction);
        }

        if (!tiles.contains(coord)) {
            tiles.emplace(coord, Side::Black);
        } else {
            auto side = tiles.at(coord);
            tiles.at(coord) = side == Side::White ? Side::Black : Side::White;
        }
    }

    fmt::print("Part one: {}\n", n_blacks(tiles));

    /// part two
    play(tiles, 100);
    fmt::print("Part two: {}\n", n_blacks(tiles));
}