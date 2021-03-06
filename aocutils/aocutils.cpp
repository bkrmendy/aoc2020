//
// Created by Berci on 2020. 12. 01..
//

#include <fstream>
#include <sstream>
#include "./aocutils.h"

std::vector<std::string> lines_from_file(std::filesystem::path& path) {
    std::vector<std::string> lines{};
    std::ifstream input{path};

    std::string buffer;
    while (std::getline(input, buffer)) {
        lines.emplace_back(buffer);
    }

    return lines;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = str.find (delimiter, pos_start)) != std::string::npos) {
        token = str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (!token.empty()) {
            res.push_back(token);
        }
    }

    token = str.substr(pos_start);
    if (!token.empty()) {
        res.push_back (token);
    }

    return res;
}

std::vector<std::string> blocks_from_file(std::filesystem::path& path, std::string split_on) {
    std::vector<std::string> lines{};
    std::ifstream input{path};
    std::stringstream buffer;

    buffer << input.rdbuf();

    auto s = std::move(buffer.str());

    return split(s, split_on);
}



