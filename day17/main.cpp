//
// Created by Berci on 2020. 12. 17..
//

#include <map>
#include <tuple>
#include <set>
#include <vector>
#include <algorithm>

#include <fmt/core.h>
#include "utils.h"

struct Coord {
    int x;
    int y;
    int z;
};
bool operator<(const Coord& left, const Coord& right) {
    if (left.x < right.x) return left.x < right.x;
    if (left.y < right.y) return left.y < right.y;
    return left.z < right.z;

}

bool operator==(const Coord& left, const Coord& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

bool operator!=(Coord& left, Coord& right) {
    return !(left == right);
}

using Crd3 = std::tuple<int, int, int>;
using Crd4 = std::tuple<int, int, int, int>;

enum class State {
    Active,
    Inactive
};

template <typename T>
State neighbor_with_default(std::map<T, State>& cubes, T c, State def) {
    if (cubes.contains(c)) {
        return cubes.at(c);
    }
    return def;
}

State next_state(State this_state, size_t active_neighbors) {
    if (this_state == State::Active && (active_neighbors == 2 || active_neighbors == 3)) {
        return State::Active;
    }
    if (this_state == State::Inactive && active_neighbors == 3) {
        return State::Active;
    }
    return State::Inactive;
}

template<typename T>
std::vector<T> sub_cube(std::map<T, State>& cubes, T c);

template<>
std::vector<Crd4> sub_cube(std::map<Crd4, State>& cubes, Crd4 c) {
    int x = std::get<0>(c);
    int y = std::get<1>(c);
    int z = std::get<2>(c);
    int w = std::get<3>(c);
    std::vector<Crd4> neighbors{};
    for (int zz = z -1; zz <= z + 1; zz++) {
        for (int yy = y -1; yy <= y + 1; yy++) {
            for (int xx = x -1; xx <= x + 1; xx++) {
                for (int ww = w - 1; ww <= w + 1; ww++) {
                    auto n = std::make_tuple(xx, yy, zz, ww);
                    if (n != c) {
                        neighbors.push_back(n);
                    }
                }
            }
        }
    }
    assert(neighbors.size() == 80);
    return neighbors;
}

template<>
std::vector<Crd3> sub_cube(std::map<Crd3, State>& cubes, Crd3 c) {
    int x = std::get<0>(c);
    int y = std::get<1>(c);
    int z = std::get<2>(c);
    std::vector<Crd3> neighbors{};
    for (int zz = z -1; zz <= z + 1; zz++) {
        for (int yy = y -1; yy <= y + 1; yy++) {
            for (int xx = x -1; xx <= x + 1; xx++) {
                auto n = std::make_tuple(xx, yy, zz);
                if (n != c) {
                    neighbors.push_back(n);
                }
            }
        }
    }
    assert(neighbors.size() == 26);
    return neighbors;
}

template<typename T>
std::vector<State> neighbors_of(std::map<T, State>& cubes, T c) {
    auto neighbors = sub_cube(cubes, c);
    std::vector<State> states{};
    std::transform(neighbors.begin(),
                   neighbors.end(),
                   std::back_inserter(states),
                   [&cubes](const auto& c) { return neighbor_with_default(cubes, c, State::Inactive); });
    return states;
}

template<typename T>
std::map<T, State> extend_neighbors(std::map<T, State>& cubes) {
    std::map<T, State> these_cubes{};
    for (const auto& entry : cubes) {
        auto coord = entry.first;
        auto s_cube = sub_cube(cubes, coord);
        these_cubes.insert(std::make_pair(coord, State::Inactive));
        for (auto& cube : s_cube) {
            these_cubes.insert(std::make_pair(cube, State::Inactive));
        }
    }

    for (const auto& entry : cubes) {
        auto coord = entry.first;
        auto state = entry.second;
        these_cubes.at(coord) = state;
    }

    return these_cubes;
}

template<typename T>
std::map<T, State> step(std::map<T, State>& cubes) {
    std::map<T, State> these_cubes = cubes;
    for (const auto& entry : cubes) {
        auto coord = entry.first;
        auto state = entry.second;
        auto neighbors = neighbors_of(cubes, coord);
        auto actives = std::count_if(neighbors.begin(),
                                     neighbors.end(),
                                     [](auto state) { return state == State::Active; });

        these_cubes.at(coord) = next_state(state, actives);
    }
    return these_cubes;
}

int part_one(std::map<Crd3, State>& cubes) {
    for (size_t cycle = 0; cycle < 6; cycle++) {
        std::map<Crd3, State> e_cubes = extend_neighbors(cubes);
        cubes = step(e_cubes);
    }

    return std::count_if(cubes.begin(),
                         cubes.end(),
                         [](auto cube) { return cube.second == State::Active; });
}

int part_two(std::map<Crd4, State>& cubes) {
    for (size_t cycle = 0; cycle < 6; cycle++) {
        std::map<Crd4, State> e_cubes = extend_neighbors(cubes);
        cubes = step(e_cubes);
    }

    return std::count_if(cubes.begin(),
                         cubes.end(),
                         [](auto cube) { return cube.second == State::Active; });
}

int main() {
    auto path = std::filesystem::path{"../input/day17.txt"};
    auto lines = lines_from_file(path);

    std::map<Crd3, State> cubes3;

    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines.at(r).size(); c++) {
            auto state = lines.at(r).at(c) == '.' ? State::Inactive : State::Active;
            cubes3.insert({std::make_tuple(c, r, 0), state});
        }
    }

    std::map<Crd4, State> cubes4;

    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines.at(r).size(); c++) {
            auto state = lines.at(r).at(c) == '.' ? State::Inactive : State::Active;
            cubes4.insert({std::make_tuple(c, r, 0, 0), state});
        }
    }

    fmt::print("Part one: {}\n", part_one(cubes3));
    fmt::print("Part two: {}\n", part_two(cubes4));

}