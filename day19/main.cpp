//
// Created by Berci on 2020. 12. 18..
//

#include <map>
#include <vector>
#include <variant>
#include <numeric>
#include <tuple>

#include <ctre.h>
#include <fmt/core.h>

#include "overloaded.h"
#include "utils.h"

using Letter = char;
using Sequence = std::vector<int>;
using Pipe = std::pair<Sequence, Sequence>;
using Rule = std::variant<Letter, Pipe, Sequence>;

using MatchResult = std::pair<std::string::iterator, bool>;

MatchResult matches(std::string::iterator begin,
                    std::string::iterator end,
                    std::map<int, Rule>& rules,
                    Rule& rule) {
    if (begin == end) {
        return MatchResult{begin, false};
    }

    return std::visit(overloaded {
            [&begin](Letter& letter) -> MatchResult {
                if (*begin == letter) {
                    return {std::next(begin), true};
                }
                return {begin, false};
            },
            [&begin, &end, &rules](Sequence& seq) -> MatchResult {
                auto cur = begin;
                for (auto& elem : seq) {
                    auto [it, matched] = matches(cur, end, rules, rules.at(elem));
                    if (not matched) {
                        return MatchResult{begin, false};
                    }
                    cur = it;
                }
                return MatchResult{cur, true};
            },
            [&begin, &end, &rules](Pipe& pipe) -> MatchResult {
                auto l_rule = Rule{pipe.first};
                auto r_rule = Rule{pipe.second};

                auto [it, matched] = matches(begin, end, rules, l_rule);
                if (matched) {
                    return MatchResult{it, true};
                }
                return matches(begin, end, rules, r_rule);
            }
    }, rule);
}

constexpr auto letter_pattern = ctll::fixed_string{R"(([0-9]+): '(a|b)')"};
constexpr auto sequence_pattern = ctll::fixed_string{R"(([0-9]+): (([0-9]+\s?)+))"};
constexpr auto pipe_pattern = ctll::fixed_string{R"(([0-9]+): (([0-9]+\s?)+)\| (([0-9]+\s?)+))"};

std::map<int, Rule> read_rules(std::filesystem::path&& path) {
    std::map<int, Rule> rules{};
    for (const auto& line : lines_from_file(path)) {
        if (auto letter = ctre::match<letter_pattern>(line)) {
            auto number = letter.get<1>().to_string();
            auto chr = letter.get<2>().to_string();
            rules.emplace(std::stoi(number), chr[0]);
        } else if (auto seq = ctre::match<sequence_pattern>(line)) {
            auto number = seq.get<1>().to_string();
            std::vector<int> numbers{};
            for (const auto& n : split(seq.get<2>().to_string(), " ")) {
                numbers.push_back(std::stoi(n));
            }
            rules.emplace(std::stoi(number), numbers);
            
        } else if (auto pipe = ctre::match<pipe_pattern>(line)) {
            auto number = pipe.get<1>().to_string();
            auto first = pipe.get<2>().to_string();
            auto second = pipe.get<4>().to_string();

            std::vector<int> left{};
            for (const auto& n : split(first, " ")) {
                left.push_back(std::stoi(n));
            }

            std::vector<int> right{};
            for (const auto& n : split(second, " ")) {
                right.push_back(std::stoi(n));
            }

            rules.emplace(std::stoi(number), std::pair<Sequence, Sequence>(left, right));
        } else {
            assert(0 && "No rules matched");
        }
    }

    return rules;
}

/**
 * solution due to reddit user u/Zv0n in day 19 solution thread
 */
int part2(std::map<int, Rule>& rules, std::vector<std::string>& messages) {
    int total = 0;

    for (auto& message : messages) {
        auto cur = message.begin();
        auto end = message.end();

        auto c42 = 0;
        auto c31 = 0;

        // match 42s
        while (cur < end) {
            auto [it, matched] = matches(cur, end, rules, rules.at(42));
            cur = it;
            if (matched) {
                c42 += 1;
            } else {
                break;
            }
        }

        // match 31s
        while (cur < end) {
            auto [it, matched] = matches(cur, end, rules, rules.at(31));
            cur = it;
            if (matched) {
                c31 += 1;
            } else {
                break;
            }
        }

        if (1 <= c31
            && 2 <= c42
            && c31 < c42
            && cur == end) {
            total += 1;
        }
    }

    return total;
}

int n_messages_match(std::map<int, Rule>& rules, std::vector<std::string>& messages) {
    return std::count_if(messages.begin(),
                         messages.end(),
                         [&rules](std::string& message) {
                            auto [it, does_match] = matches(message.begin(),
                                                            message.end(),
                                                            rules,
                                                            rules.at(0));

                            bool reached_end = it == message.end();

                            return does_match && reached_end;
        });
}

int main() {
    auto messages_path = std::filesystem::path{"../input/day19/messages.txt"};
    auto messages = lines_from_file(messages_path);
    auto rules = read_rules(std::filesystem::path{"../input/day19/rules.txt"});

    fmt::print("Part one: {}\n", n_messages_match(rules, messages));

    rules.at(8) = Pipe{Sequence{42}, Sequence{42, 8}};
    rules.at(11) = Pipe{Sequence{42, 31}, Sequence{42, 11, 31}};

    fmt::print("Part two: {}\n", part2(rules, messages));
}