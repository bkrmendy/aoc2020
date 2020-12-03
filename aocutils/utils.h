//
// Created by Berci on 2020. 12. 01..
//

#ifndef AOC2020_UTILS_H
#define AOC2020_UTILS_H

#include <vector>
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

#endif //AOC2020_UTILS_H