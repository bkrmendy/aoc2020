//
// Created by Berci on 2020. 12. 06..
//

#include <filesystem>
#include <set>
#include <numeric>
#include <iostream>

#include "utils.h"


std::set<char> intersection(std::vector<std::set<char>>& sets) {
    if (sets.empty()) {
        return std::set<char>{};
    }

    if (sets.size() == 1) {
        return sets.at(0);
    };

    std::set<char> intersect = sets.at(0);
    for (const auto& set : sets) {
        std::set<char> intersect2{};
        std::set_intersection(set.begin(), set.end(),
                              intersect.begin(), intersect.end(),
                              std::inserter(intersect2, intersect2.begin()));
        intersect = intersect2;
    }

    return intersect;
}

int main() {
    auto path = std::filesystem::path("../input/day6.txt");

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

    std::cout << "Part two: " << n_answers << std::endl;
}