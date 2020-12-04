//
// Created by Berci on 2020. 12. 04..
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <numeric>
#include <utils.h>

int main() {
    auto path = std::filesystem::path("../input/day5.txt");

    std::vector<int> numbers = entries_from_file<int>(path, [](const auto& line) {
        return std::stoi(line);
    });

    return 0;
}
