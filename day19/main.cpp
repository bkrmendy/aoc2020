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

using Letter = std::string;
using Sequence = std::vector<int>;
using Pipe = std::pair<Sequence, Sequence>;
using Rule = std::variant<Letter, Pipe, Sequence>;

std::vector<std::string> cartesian_product(std::vector<std::vector<std::string>>& sets) {
    std::vector<std::string> result{""};
    for (const auto& result_set : sets) {
        std::vector<std::string> intermediate_results{};
        for (const auto& res : result) {
            for (const auto& entry : result_set) {
                intermediate_results.push_back(res);
                intermediate_results.back() += entry;
            }
        }
        result = std::move(intermediate_results);
    }
    return result;
}

std::vector<std::string> matches_for(std::map<int, Rule>& rules, Rule& rule) {
    return std::visit(overloaded {
            [](Letter& letter) {
                return std::vector<std::string>{letter};
            },
            [&rules](Sequence & sequence) {
                std::vector<std::vector<std::string>> all_matches{};
                for (auto rule : sequence) {
                    all_matches.emplace_back(matches_for(rules, rules.at(rule)));
                }
                return cartesian_product(all_matches);
            },
            [&rules](Pipe& pipe) {
                std::vector<std::string> all_matches{};
                auto& [left, right] = pipe;

                auto l_rule = Rule{left};
                auto r_rule = Rule{right};

                std::vector<std::string> left_matches = matches_for(rules, l_rule);
                std::vector<std::string> right_matches = matches_for(rules, r_rule);

                all_matches.insert(all_matches.end(),
                                   std::make_move_iterator(left_matches.begin()),
                                   std::make_move_iterator(left_matches.end()));

                all_matches.insert(all_matches.end(),
                                   std::make_move_iterator(right_matches.begin()),
                                   std::make_move_iterator(right_matches.end()));

                return all_matches;
            }
    }, rule);
}

constexpr auto letter_pattern = ctll::fixed_string{"([0-9]+): '(a|b)'"};
constexpr auto sequence_pattern = ctll::fixed_string{R"(([0-9]+): (([0-9]+\s?)+))"};
constexpr auto pipe_pattern = ctll::fixed_string{R"(([0-9]+): (([0-9]+\s?)+)\| (([0-9]+\s?)+))"};

std::map<int, Rule> read_rules(std::filesystem::path&& path) {
    std::map<int, Rule> rules{};
    for (const auto& line : lines_from_file(path)) {
        if (auto letter = ctre::match<letter_pattern>(line)) {
            auto number = letter.get<1>().to_string();
            auto chr = letter.get<2>().to_string();
            rules.emplace(std::stoi(number), chr);
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

int n_messages_match(std::map<int, Rule>&& rules, std::vector<std::string>& messages) {
    auto possible_messages = matches_for(rules, rules.at(0));

    return std::count_if(messages.begin(),
                         messages.end(),
                         [&possible_messages](std::string& message) {
                              return std::find(possible_messages.begin(),
                                               possible_messages.end(),
                                               message) != possible_messages.end();
                        });
}

int main() {
    auto messages_path = std::filesystem::path{"../input/day19_messages.txt"};
    auto messages = lines_from_file(messages_path);

//    fmt::print("Part one: {}\n",
//               n_messages_match(read_rules(std::filesystem::path{"../input/day19_rules.txt"}),
//                                messages));

    fmt::print("Part two: {}\n",
               n_messages_match(read_rules(std::filesystem::path{"../input/day19_rules_mod.txt"}),
                                messages));
}