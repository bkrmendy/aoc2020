//
// Created by Berci on 2020. 12. 01..
//

#include <fstream>
#include "./utils.h"

std::vector<std::string> lines_from_file(std::filesystem::path& path) {
    std::vector<std::string> lines{};
    std::ifstream input{path};

    std::string buffer;
    while (std::getline(input, buffer)) {
        lines.emplace_back(buffer);
    }

    return lines;
}

