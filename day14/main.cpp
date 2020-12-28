//
// Created by Berci on 2020. 12. 13..
//

#include <map>
#include <numeric>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <ctre.h>
#include "utils.h"

int64_t masked_value(std::string& mask, int64_t value) {
    for (size_t i = 0; i < 36; i++) {
        int64_t one = 1;
        auto c = mask.at(35-i);
        if (c == '1') {
            value |= one << i;
        } else if (c == '0') {
            value &= ~(one << i);
        }
    }
    return value;
}

std::vector<int64_t> masked_address(std::string& mask, int64_t address) {
    int64_t one = 1;
    std::vector<size_t> xs{};

    for (size_t i = 0; i < 36; i++) {
        auto c = mask.at(35-i);
        if (c == '1') {
            address |= one << i;
        } else if (c == '0') {
            // unchanged
        } else {
            xs.push_back(i);
        }
    }

    std::vector<int64_t> addresses{};

    for(int64_t i = 0; i < pow(2, xs.size()); i++) {
        int64_t this_address = address;
        for(size_t bit_idx = 0; bit_idx < xs.size(); bit_idx++) {
            int64_t bit = (i >> bit_idx) & 1;
            if (bit == 1) {
                this_address |= one << xs.at(bit_idx);
            } else if (bit == 0) {
                this_address &= ~(one << xs.at(bit_idx));
            } else {
                assert(0 && "bit should be 0 or 1!");
            }
        }
        addresses.push_back(this_address);
    }

    return addresses;
}

constexpr auto mask_pattern = ctll::fixed_string{"mask = ([1X0]+)"};
constexpr auto mem_pattern = ctll::fixed_string{"mem.([0-9]+). = ([0-9]+)"};

int64_t part_one(std::vector<std::string>& lines) {
    std::string mask;
    std::map<int64_t, int64_t> memory{};

    for (const auto& instruction : lines) {
        if (auto m = ctre::match<mask_pattern>(instruction)) {
            mask = m.get<1>();
        } else if (auto mem = ctre::match<mem_pattern>(instruction)) {
            auto loc = mem.get<1>().to_string();
            auto value = mem.get<2>().to_string();
            int64_t  lloc = std::stoull(loc);
            int64_t vvalue = std::stoull(value);
            memory[lloc] = masked_value(mask, vvalue);
        }
    }

    return std::reduce(memory.begin(),
                       memory.end(),
                       INT64_C(0),
                       [](int64_t acc, auto pair) {
                          return acc + pair.second;
    });
}

int64_t part_two(std::vector<std::string>& lines) {
    std::string mask;
    std::map<int64_t, int64_t> memory{};

    for (const auto& instruction : lines) {
        if (auto m = ctre::match<mask_pattern>(instruction)) {
            mask = m.get<1>();
        } else if (auto mem = ctre::match<mem_pattern>(instruction)) {
            auto loc = mem.get<1>().to_string();
            auto value = mem.get<2>().to_string();
            int64_t lloc = std::stoull(loc);
            int64_t vvalue = std::stoull(value);

            for (int64_t address : masked_address(mask, lloc)) {
                memory[address] = vvalue;
            }
        }
    }

    return std::reduce(memory.begin(),
                       memory.end(),
                       INT64_C(0),
                       [](int64_t acc, std::pair<int64_t, int64_t> pair) {
                           return acc + pair.second;
                       });
}

int main() {
    auto path = std::filesystem::path{"../input/day14.txt"};
    auto lines = lines_from_file(path);

    fmt::print("Part one: {}\n", part_one(lines));
    fmt::print("Part two: {}\n", part_two(lines));
}