//
// Created by Berci on 2020. 12. 02..
//

#include <filesystem>
#include <numeric>
#include <fmt/core.h>

#include "utils.h"

struct Rule {
    int lo;
    int hi;
    char letter;
    std::string password;
};

int main() {
    auto path = std::filesystem::path{"../input/day2.txt"};

    std::vector<Rule> passwords = entries_from_file<Rule>(path, [](const std::string& entry) {
       std::string hi_lo_sep = "-";
       std::string char_sep = " ";
       std::string password_sep = ": ";

       auto lo = std::stoi(entry.substr(0, entry.find(hi_lo_sep)));
       auto hi = std::stoi(entry.substr(entry.find(hi_lo_sep) + 1, entry.find(char_sep)));
       auto letter = entry.substr(entry.find(char_sep) + 1, entry.find(password_sep))[0];
       auto password = entry.substr(entry.find(password_sep) + 2, entry.size());

       return Rule {
           .lo = lo,
           .hi = hi,
           .letter = letter,
           .password = password
       };
    });

    int n_correct_passwords_part_one
        = std::reduce(
                passwords.begin(),
                passwords.end(),
                0,
                [](const int n_correct, Rule& rule) {
                    size_t n_letter = 0;
                    for (char letter : rule.password) {
                        if (letter == rule.letter) {
                            n_letter += 1;
                        }
                    }

                    if (n_letter >= rule.lo && n_letter <= rule.hi) {
                        return  n_correct + 1;
                    }
                    return n_correct;
                });

    fmt::print("Part one: {}\n", n_correct_passwords_part_one);

    int n_correct_passwords_part_two
        = std::reduce(
                passwords.begin(),
                passwords.end(),
                0,
                [](const int n_correct, const Rule& rule) {
                    bool is_correct =
                            (rule.password[rule.lo - 1] == rule.letter && rule.password[rule.hi - 1] != rule.letter)
                            || (rule.password[rule.lo - 1] != rule.letter && rule.password[rule.hi - 1] == rule.letter);

                    return is_correct ? n_correct + 1 : n_correct;
                });

    fmt::print("Part one: {}\n", n_correct_passwords_part_two);
}