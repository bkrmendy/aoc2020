#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <numeric>
#include <utils.h>

void part_one(std::vector<int>& numbers) {
    for (const auto& left : numbers) {
        for (const auto &right : numbers) {
            if (left + right == 2020) {
                std::cout << "Part one: " << std::endl;
                std::cout << left * right << std::endl;
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
                    std::cout << "Part two: " << std::endl;
                    std::cout << left * middle * right << std::endl;
                    return;
                }
            }
        }
    }
}

int main() {
    auto path = std::filesystem::path("../input/day1.txt");
    std::ifstream input{path.c_str()};

    std::vector<int> numbers;

    auto lines = lines_from_file(path);

    std::transform(lines.begin(), lines.end(), std::back_inserter(numbers), [](const auto& str) {
        return std::stoi(str);
    });

    part_one(numbers);
    part_two(numbers);

    return 0;
}
