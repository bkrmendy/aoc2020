//
// Created by Berci on 2020. 12. 06..
//

#include <filesystem>
#include <set>
#include <numeric>

#include <fmt/core.h>
#include "utils.h"

int main() {
    auto path = std::filesystem::path("../input/day06.txt");

    auto lines = lines_from_file(path);

    std::vector<std::set<char>> groups{};

    std::vector<std::set<char>> persons{};

    for(const auto& line : lines) {
        if (line.empty()) {
            auto intersect = intersection(persons);
            groups.push_back(intersect);
            persons = std::vector<std::set<char>>{};
        } else {
            std::set<char> this_person{};
            for (const char chr : line) {
                this_person.insert(chr);
            }
            persons.push_back(this_person);
        }
    }

    groups.push_back(intersection(persons));

    int n_answers
        = std::reduce(
                groups.begin(),
                groups.end(),
                0,
                [](int acc, const std::set<char>& group) {
                    return acc + group.size();
                });

    fmt::print("Part one: {}\n", n_answers);
}