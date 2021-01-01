//
// Created by Berci on 2020. 12. 17..
//

#include <unordered_map>
#include <tuple>
#include <vector>
#include <algorithm>

#include <fmt/core.h>
#include "aocutils.h"

using Crd3 = std::tuple<int, int, int>;
using Crd4 = std::tuple<int, int, int, int>;

struct crd3_hash : public std::unary_function<Crd3, std::size_t> {
    std::size_t operator()(const Crd3& k) const
    {
        auto& [x, y, z] = k;
        return x ^ y ^ z;
    }
};

struct crd4_hash : public std::unary_function<Crd4, std::size_t> {
    std::size_t operator()(const Crd4& k) const
    {
        auto& [x, y, z, w] = k;
        return x ^ y ^ z ^ w;
    }
};

enum class Status {
    Active,
    Inactive
};

template<typename State>
State next_state(State this_state, size_t active_neighbors) {
    if (this_state == Status::Active && (active_neighbors == 2 || active_neighbors == 3)) {
        return Status::Active;
    }
    if (this_state == Status::Inactive && active_neighbors == 3) {
        return Status::Active;
    }
    return Status::Inactive;
}

template<typename T>
std::vector<T> sub_cube(T c);

template<>
std::vector<Crd4> sub_cube(Crd4 c) {
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
std::vector<Crd3> sub_cube(Crd3 c) {
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

template<typename Coord, typename State, typename Hash>
std::vector<Status> neighbors_of(std::unordered_map<Coord, State, Hash>& cubes, Coord coord) {
    auto neighbors = sub_cube(coord);
    std::vector<Status> states{};
    std::transform(neighbors.begin(),
                   neighbors.end(),
                   std::back_inserter(states),
                   [&cubes](const auto& c) { return at_with_default(cubes, c, Status::Inactive); });
    return states;
}

template<typename Coord, typename State, typename Hash>
std::unordered_map<Coord, State, Hash> extend_neighbors(std::unordered_map<Coord, State, Hash>& cubes,
                                                        State default_state) {
    std::unordered_map<Coord, State, Hash> these_tiles{};
    for (const auto& [coord, _] : cubes) {
        auto neighbors = sub_cube(coord);
        these_tiles.insert(std::make_pair(coord, default_state));
        for (auto& neighbor : neighbors) {
            these_tiles.insert(std::make_pair(neighbor, default_state));
        }
    }

    for (const auto& [coord, state] : cubes) {
        these_tiles.at(coord) = state;
    }

    return these_tiles;
}

template<typename T, typename State, typename Hash>
std::unordered_map<T, State, Hash> step(std::unordered_map<T, State, Hash>& cubes) {
    auto these_cubes = cubes;
    for (const auto& [coord, state] : cubes) {
        auto neighbors = neighbors_of(cubes, coord);
        auto actives = std::count_if(neighbors.begin(),
                                     neighbors.end(),
                                     [](auto state) { return state == Status::Active; });

        these_cubes.at(coord) = next_state(state, actives);
    }
    return these_cubes;
}

template <typename Coord, typename S, typename Hash>
int play(std::unordered_map<Coord, S, Hash>& cubes, size_t rounds) {
    for (size_t cycle = 0; cycle < rounds; cycle++) {
        auto extended_tiles = extend_neighbors(cubes, Status::Inactive);
        cubes = step(extended_tiles);
    }

    return std::count_if(cubes.begin(),
                         cubes.end(),
                         [](auto cube) { return cube.second == Status::Active; });
}

int main() {
    auto path = std::filesystem::path{"../input/day17.txt"};
    auto lines = lines_from_file(path);

    std::unordered_map<Crd3, Status, crd3_hash> cubes3;

    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines.at(r).size(); c++) {
            auto state = lines.at(r).at(c) == '.' ? Status::Inactive : Status::Active;
            cubes3.insert({std::make_tuple(c, r, 0), state});
        }
    }

    std::unordered_map<Crd4, Status, crd4_hash> cubes4;

    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines.at(r).size(); c++) {
            auto state = lines.at(r).at(c) == '.' ? Status::Inactive : Status::Active;
            cubes4.insert({std::make_tuple(c, r, 0, 0), state});
        }
    }

    fmt::print("Part one: {}\n", play(cubes3, 6));
    fmt::print("Part two: {}\n", play(cubes4, 6));

}