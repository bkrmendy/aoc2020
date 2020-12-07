#include <filesystem>
#include <fstream>
#include <vector>
#include <numeric>

#include "utils.h"
#include <fmt/core.h>

void part_one(std::vector<int>& numbers) {
    for (const auto& left : numbers) {
        for (const auto &right : numbers) {
            if (left + right == 2020) {
                fmt::print("Part one: {}\n", left * right);
                return;
            }
        }
    }
}

void part_two(std::vector<int>& numbers) {
    for (const auto& left : numbers) {
        for (const auto &middle : numbers) {
            for (const auto &right : numbers) {
                if (left + middle + right == 2020) {
                    fmt::print("Part two: {}\n", left * middle * right);
                    return;
                }
            }
        }
    }
}

int main() {
    auto path = std::filesystem::path("../input/day1.txt");
    std::ifstream input{path.c_str()};

    std::vector<int> numbers = entries_from_file<int>(path, [](const auto& line) {
        return std::stoi(line);
    });

    part_one(numbers);
    part_two(numbers);

    return 0;
}
