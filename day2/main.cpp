//
// Created by Berci on 2020. 12. 02..
//

#include <filesystem>
#include <numeric>
#include <iostream>

#include "utils.h"
#include "lexy/dsl.hpp"
#include "lexy/input/file.hpp"
#include "lexy/parse.hpp"

struct Rule {
    int lo;
    int hi;
    char letter;
    std::string password;
};

namespace grammar {
    namespace dsl = lexy::dsl;
    struct bound {
        static constexpr auto rule = dsl::integer<int>(dsl::digits<>);
        static constexpr auto value = lexy::as_integer<int>;
    };

    struct letter {
        static constexpr auto rule = dsl::ascii::character;
        static constexpr auto value = lexy::callback<char>([](char c) { return c; });
    };

    struct password {
        static constexpr auto rule = dsl::capture(dsl::while_(dsl::code_point));
        static constexpr auto value = lexy::as_string<std::string>;
    };

    struct line {
        static constexpr auto rule
            = dsl::p<bound>
            + dsl::lit_c<'-'>
            + dsl::p<bound>
            + dsl::lit_c<' '>
            + dsl::p<letter>
            + dsl::lit_c<':'>
            + dsl::lit_c<' '>
            + dsl::p<password>;

        static constexpr auto value = lexy::construct<Rule>;
    };

    struct lines {
        static constexpr auto rule = dsl::list(dsl::p<line>, dsl::sep(dsl::newline));
        static constexpr auto list = lexy::as_list<std::vector<Rule>>;
    };
}


int main() {
    auto path = std::filesystem::path{"../input/day2.txt"};

    auto file = lexy::read_file(path.c_str());
    auto lines = lexy::parse<grammar::lines>(file.value(), lexy::callback([](const auto& context, const auto& error) {}));

    std::vector<Rule> passwords = lines.value();

//    std::vector<Rule> passwords = entries_from_file<Rule>(path, [](const std::string& entry) {
//       std::string hi_lo_sep = "-";
//       std::string char_sep = " ";
//       std::string password_sep = ": ";
//
//       auto lo = std::stoi(entry.substr(0, entry.find(hi_lo_sep)));
//       auto hi = std::stoi(entry.substr(entry.find(hi_lo_sep) + 1, entry.find(char_sep)));
//       auto letter = entry.substr(entry.find(char_sep) + 1, entry.find(password_sep))[0];
//       auto password = entry.substr(entry.find(password_sep) + 2, entry.size());
//
//       return Rule {
//           .lo = lo,
//           .hi = hi,
//           .letter = letter,
//           .password = password
//       };
//    });

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

    std::cout << "Part one: " << n_correct_passwords_part_one << std::endl;

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

    std::cout << "Part two: " << n_correct_passwords_part_two << std::endl;
}