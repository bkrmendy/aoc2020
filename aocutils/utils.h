//
// Created by Berci on 2020. 12. 01..
//

#ifndef AOC2020_UTILS_H
#define AOC2020_UTILS_H

#include <vector>
#include <set>
#include <string>
#include <filesystem>
#include <functional>
#include <string>

std::vector<std::string> lines_from_file(std::filesystem::path&);

template<typename T>
std::vector<T> entries_from_file(std::filesystem::path& path, std::function<T(const std::string&)> make_entry) {
    auto lines = lines_from_file(path);
    std::vector<T> entries{};

    std::transform(lines.begin(), lines.end(), std::back_inserter(entries), make_entry);

    return entries;
}

template<typename T>
std::set<T> intersection(std::vector<std::set<T>>& sets) {
    if (sets.empty()) {
        return std::set<T>{};
    }

    if (sets.size() == 1) {
        return sets.at(0);
    };

    std::set<T> intersect = sets.at(0);
    for (const auto& set : sets) {
        std::set<T> intersect2{};
        std::set_intersection(set.begin(), set.end(),
                              intersect.begin(), intersect.end(),
                              std::inserter(intersect2, intersect2.begin()));
        intersect = intersect2;
    }

    return intersect;
}

template<typename T>
std::set<T> difference(std::set<T>& left, std::set<T>& right) {
    std::set<T> res;
    std::set_difference(left.begin(), left.end(),
                        right.begin(), right.end(),
                        std::inserter(res, res.begin()));
    return res;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter);

#endif //AOC2020_UTILS_H
