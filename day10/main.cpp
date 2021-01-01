//
// Created by Berci on 2020. 12. 09..
//
#include <set>
#include <map>

#include <fmt/core.h>
#include "aocutils.h"

int part_one(std::vector<int>& entries) {
    int last = 0;
    std::vector<int> diffs;

    for (int entry : entries) {
        diffs.push_back(entry - last);
        last = entry;
    }

    int ones = std::count_if(diffs.begin(), diffs.end(), [](auto e) { return e == 1; });
    int threes = std::count_if(diffs.begin(), diffs.end(), [](auto e) { return e == 3; });

    return ones * threes;
}

long walk(std::map<size_t, long>& memo, std::vector<int>& entries, size_t start, size_t end) {
    if (memo.count(start) > 0) {
        return memo.at(start);
    }

    if (start + 1 == end) {
        return 1;
    }


    long acc = 0;
    size_t next = start + 1;
    while (next != end && entries.at(next) - entries.at(start) <= 3) {
        acc += walk(memo, entries, next, end);
        next += 1;
    }

    memo.emplace(start, acc);
    return acc;
}

long part_two(std::vector<int>& entries) {
    std::map<size_t, long> memo{};
    return walk(memo, entries, 0, entries.size());
}

int main() {
    auto path = std::filesystem::path("../input/day10.txt");

    std::vector<int> entries = entries_from_file<int>(path, [](const std::string& entry) {
       return std::stoi(entry);
    });

    entries.push_back(0);

    std::sort(entries.begin(), entries.end());

    entries.push_back(*entries.rbegin() + 3);

    fmt::print("Part one: {}\n", part_one(entries));
    fmt::print("Part two: {}\n", part_two(entries));
}