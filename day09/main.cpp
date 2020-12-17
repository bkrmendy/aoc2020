//
// Created by Berci on 2020. 12. 08..
//

#include <filesystem>

#include <fmt/core.h>
#include <set>
#include "utils.h"

long part_one(std::vector<long>& entries) {
    size_t preamble_length = 25;

    bool found = false;
    for (size_t i = preamble_length; i < entries.size(); i++) {
        for (size_t k = i - preamble_length; k < i; k++) {
            for (size_t l = i - preamble_length; l < i && l != k; l++) {
                found |= (entries.at(i) == entries.at(l) + entries.at(k));
            }
        }
        if (!found) {
            return entries.at(i);

        } else {
            found = false;
        }
    }
    return -1;
}

int main() {
    auto path = std::filesystem::path("../input/day9.txt");

    std::vector<long> entries = entries_from_file<long>(path, [](const std::string& line) {
       return std::stol(line);
    });

    long n = part_one(entries);
    fmt::print("Part one: {}\n", n);

    for (size_t i = 0; i < entries.size() - 1; i++) {
        std::set<long> nums{};

        nums.insert(entries.at(i));
        long acc = entries.at(i);

        for (size_t k = i + 1; k < entries.size() && acc <= n; k++) {
            if (acc == n && nums.size() > 1) {
                long min = *nums.begin();
                long max = *nums.rbegin();
                fmt::print("Part two: {}\n", min + max);
            }

            nums.insert(entries.at(k));
            acc += entries.at(k);
        }
    }
}
