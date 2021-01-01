//
// Created by Berci on 2020. 12. 17..
//

#include <unordered_map>
#include <tuple>
#include <vector>
#include <algorithm>

#include <fmt/core.h>
#include "aocutils.h"
#include "life.h"

using Crd3 = std::tuple<int, int, int>;
using Crd4 = std::tuple<int, int, int, int>;

enum class Status {
    Active,
    Inactive
};

std::set<Crd4> sub_cube(Crd4 c) {
    int x = std::get<0>(c);
    int y = std::get<1>(c);
    int z = std::get<2>(c);
    int w = std::get<3>(c);
    std::set<Crd4> neighbors{};
    for (int zz = z -1; zz <= z + 1; zz++) {
        for (int yy = y -1; yy <= y + 1; yy++) {
            for (int xx = x -1; xx <= x + 1; xx++) {
                for (int ww = w - 1; ww <= w + 1; ww++) {
                    if (!(x == xx && y == yy && z == zz && w == ww)) {
                        neighbors.emplace(xx, yy, zz, ww);
                    }
                }
            }
        }
    }
    assert(neighbors.size() == 80);
    return neighbors;
}

std::set<Crd3> sub_cube(Crd3 c) {
    int x = std::get<0>(c);
    int y = std::get<1>(c);
    int z = std::get<2>(c);
    std::set<Crd3> neighbors{};
    for (int zz = z -1; zz <= z + 1; zz++) {
        for (int yy = y -1; yy <= y + 1; yy++) {
            for (int xx = x -1; xx <= x + 1; xx++) {
                if (!(x == xx && y == yy && z == zz)) {
                    neighbors.emplace(xx, yy, zz);
                }
            }
        }
    }
    assert(neighbors.size() == 26);
    return neighbors;
}

template <typename Coord>
std::set<Coord> neighbors_of(const Coord& coord) {
    return sub_cube(coord);
}

template <>
Status next_state(const Status& this_state, size_t n_active_neighbors) {
    if (this_state == Status::Active
        && (n_active_neighbors == 2 || n_active_neighbors == 3)) {
        return Status::Active;
    }
    if (this_state == Status::Inactive && n_active_neighbors == 3) {
        return Status::Active;
    }
    return Status::Inactive;
}

template <>
Status make_active_state() {
    return Status::Active;
}

template <>
Status make_inactive_state() {
    return Status::Inactive;
}

template <typename Coord>
size_t play(std::set<Coord>& coords, size_t rounds) {
    auto life = Life<Coord, Status>();
    for (size_t round = 0; round < rounds; round++) {
        auto coords2 = life.step(coords);
        coords = std::move(coords2);
    }
    return coords.size();
}

int main() {
    auto path = std::filesystem::path{"../input/day17.txt"};
    auto lines = lines_from_file(path);

    std::set<Crd3> cubes3;

    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines.at(r).size(); c++) {
            if (lines.at(r).at(c) == '#') {
                cubes3.emplace(r, c, 0);
            }
        }
    }

    std::set<Crd4> cubes4;

    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines.at(r).size(); c++) {
            if (lines.at(r).at(c) == '#') {
                cubes4.emplace(r, c, 0, 0);
            }
        }
    }

    fmt::print("Part one: {}\n", play(cubes3, 6));
    fmt::print("Part two: {}\n", play(cubes4, 6));
}