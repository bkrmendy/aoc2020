//
// Created by Berci on 2020. 12. 03..
//

#include <map>
#include <set>
#include <numeric>

#include <fmt/core.h>
#include "utils.h"

std::vector<std::map<std::string, std::string>> batches(std::vector<std::string>& lines) {
    std::vector<std::map<std::string, std::string>> passports{};

    std::map<std::string, std::string> this_passport{};

    for (const auto& line : lines) {
        if (line.empty()) {
            passports.push_back(this_passport);
            this_passport = std::map<std::string, std::string>{};
        } else {
            auto entries = split(line, " ");
            for (const auto& entry : entries) {
                auto key = entry.substr(0, entry.find(':'));
                auto value = entry.substr(entry.find(':') + 1, entry.size());
                this_passport.emplace(key, value);
            }
        }
    }

    passports.push_back(this_passport);

    return passports;
}

int main() {
    auto path = std::filesystem::path{"../input/day04.txt"};
    auto lines = lines_from_file(path);

    auto passports = batches(lines);

    auto n_correct
        = std::count_if(
                passports.begin(),
                passports.end(),
                [](const std::map<std::string, std::string>& entry) {
                   if (entry.count("byr") == 0
                          || entry.count("iyr") == 0
                          || entry.count("eyr") == 0
                          || entry.count("hgt") == 0
                          || entry.count("hcl") == 0
                          || entry.count("ecl") == 0
                          || entry.count("pid") == 0) {
                       return false;
                   }

                   int byr = std::stoi(entry.find("byr")->second);
                   if (byr < 1920 || byr > 2002) { return false; }

                   auto iyr = std::stoi(entry.find("iyr")->second);
                   if (iyr < 2010 || iyr > 2020) { return false; }

                    auto eyr = std::stoi(entry.find("eyr")->second);
                    if (eyr < 2020 || eyr > 2030) { return false; }

                    std::string hgt_key = entry.find("hgt")->second;
                    auto hgt = std::stoi(entry.find("hgt")->second);
                    if (hgt_key.find("cm") != std::string::npos) {
                        if (hgt < 150 || hgt > 193) { return false; }
                    } else if (hgt_key.find("in") != std::string::npos) {
                        if (hgt < 59 || hgt > 76) { return false; }
                    } else {
                        return false;
                    }

                    std::string hcl = entry.find("hcl")->second;
                    if (hcl.size() != 7) { return false; }
                    if (hcl[0] != '#') { return false; }

                    for (size_t i = 0; i < 7; i++) {
                        if (!((hcl[i] >= 'a' && hcl[i] <= 'f')
                                || (hcl[i] >= '0' && hcl[i] <= '9')
                                || hcl[i] == '#')) {
                            return false;
                        }
                    }

                    std::set<std::string> colors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
                    auto ecl = entry.find("ecl")->second;
                    if (!colors.contains(ecl)) { return false; }

                    return entry.find("pid")->second.size() == 9;
                });

    fmt::print("Part one: {}\n", n_correct);
}

