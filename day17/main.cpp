//
// Created by Berci on 2020. 12. 17..
//

#include <tuple>
#include <algorithm>

#include <fmt/core.h>
#include "aocutils.h"
#include "life.h"

using Crd3 = std::tuple<int, int, int>;
using Crd4 = std::tuple<int, int, int, int>;

namespace std {
    template <>
    struct hash<Crd3> {
        size_t operator()(const Crd3& coordinate) const {
            auto [x, y, z] = coordinate;
            return x ^ y ^ z;
        }
    };

    template <>
    struct hash<Crd4> {
        size_t operator()(const Crd4& coordinate) const {
            auto [x, y, z, w] = coordinate;
            return x ^ y ^ z ^ w;
        }
    };
}

enum class Status {
    Active,
    Inactive
};

using CubeLife = Life<Crd3, Status>;
using HyperCubeLife = Life<Crd4, Status>;

template <>
Status make_active_state() { return Status::Active; }

template <>
Status make_inactive_state() { return Status::Inactive; }

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

CubeLife::Container neighbors_of(const Crd3& coord) {
    int x = std::get<0>(coord);
    int y = std::get<1>(coord);
    int z = std::get<2>(coord);
    std::unordered_set<Crd3> neighbors{};
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

HyperCubeLife::Container neighbors_of(const Crd4& coord) {
    int x = std::get<0>(coord);
    int y = std::get<1>(coord);
    int z = std::get<2>(coord);
    int w = std::get<3>(coord);
    std::unordered_set<Crd4> neighbors{};
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

//template <typename Coord>
//size_t play(typename Life<Coord, Status>::Container& coords, size_t rounds) {
//    auto life = Life<Coord, Status>{};
//    for (size_t round = 0; round < rounds; round++) {
//        auto coords2 = life.step(coords);
//        coords = std::move(coords2);
//    }
//    return coords.size();
//}

int main() {
    auto path = std::filesystem::path{"../input/day17.txt"};
    auto lines = lines_from_file(path);

    CubeLife::Container cubes3;

    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines.at(r).size(); c++) {
            if (lines.at(r).at(c) == '#') {
                cubes3.emplace(r, c, 0);
            }
        }
    }

    auto cube_life = CubeLife{};
    for (size_t round = 0; round < 6; round++) {
        cubes3 = cube_life.step(cubes3);
    }
    fmt::print("Part one: {}\n", cubes3.size());

//    HyperCubeLife::Container cubes4;
//
//    for (int r = 0; r < lines.size(); r++) {
//        for (int c = 0; c < lines.at(r).size(); c++) {
//            if (lines.at(r).at(c) == '#') {
//                cubes4.emplace(r, c, 0, 0);
//            }
//        }
//    }
//
//    auto life = HyperCubeLife{};
//    for (size_t round = 0; round < 6; round++) {
//        cubes4 = life.step(cubes4);
//    }
//    fmt::print("Part two: {}\n", cubes4.size());
}