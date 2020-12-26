//
// Created by Berci on 2020. 12. 15..
//

#include <string>
#include <numeric>
#include <unordered_set>
#include <map>

#include <ctre.h>
#include <fmt/core.h>

#include "utils.h"

class Constraint {
    int u1;
    int u2;
    int h1;
    int h2;
public:
    Constraint(int u1, int u2, int h1, int h2) : u1(u1), u2(u2), h1(h1), h2(h2) {}
    bool matches(const int number) const {
        return (u1 <= number && number <= u2)
                || (h1 <= number && number <= h2);
    }
};

constexpr auto pattern = ctll::fixed_string{"([a-z: ]*)([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)"};

std::vector<Constraint> read_constrains(std::vector<std::string>&& lines) {
    std::vector<Constraint> constraints;
    for (const auto& line : lines) {
        if (auto m = ctre::match<pattern>(line)) {
            auto u1 = std::stoi(m.get<2>().to_string());
            auto u2 = std::stoi(m.get<3>().to_string());
            auto h1 = std::stoi(m.get<4>().to_string());
            auto h2 = std::stoi(m.get<5>().to_string());
            constraints.emplace_back(u1, u2, h1, h2);
        }
    }

    return constraints;
}

std::vector<int> read_ticket(std::string& line) {
    auto nums = split(line, ",");
    std::vector<int> vals{};
    std::transform(nums.begin(), nums.end(), std::back_inserter(vals), [](auto n) { return std::stoi(n); });
    return vals;
}

std::vector<std::vector<int>> read_tickets(std::vector<std::string>&& lines) {
    std::vector<std::vector<int>> tickets{};
    for (auto& line : lines) {
        tickets.emplace_back(read_ticket(line));
    }
    return tickets;
}

std::pair<int, std::vector<std::vector<int>>> valid_tickets(std::vector<std::vector<int>>& tickets,
                                            std::vector<Constraint> constraints) {
    std::vector<std::vector<int>> valids;
    int total = 0;
    for (auto & ticket : tickets) {
        bool ok = true;
        for (int value : ticket) {
            bool this_ok = std::any_of(constraints.begin(),
                                  constraints.end(),
                                  [&value](Constraint c) { return c.matches(value); });
            ok = ok && this_ok;
            if (not this_ok) {
                total += value;
            }
        }
        if (ok) {
            valids.push_back(ticket);
        }
    }
    return {total, valids};
}

uint64_t part_two(std::vector<std::vector<int>>& tickets, std::vector<Constraint> constraints) {
    std::string your_ticket = "83,137,101,73,67,61,103,131,151,127,113,107,109,89,71,139,167,97,59,53";
    auto this_ticket = read_ticket(your_ticket);

    std::unordered_set<size_t> matchers{};
    for (size_t ic = 0; ic < constraints.size(); ic++) {
        matchers.insert(ic);
    }

    std::map<size_t, size_t> constraint_to_pos{};

    while (!matchers.empty()) {
        for (size_t column = 0; column < this_ticket.size(); column++) {
            std::unordered_set<size_t> these_matchers = matchers;
            for (auto &ticket : tickets) {
                std::unordered_set<size_t> m{};
                for (auto c : these_matchers) {
                    if (!constraints.at(c).matches(ticket.at(column))) {
                        m.insert(c);
                    }
                }
                these_matchers = std::move(m);
            }
            if (these_matchers.size() == 1) {
                matchers.erase(*these_matchers.begin());
                constraint_to_pos.emplace(*these_matchers.begin(), column);
            }
        }
    }

    uint64_t res = 1;
    res *= this_ticket.at(constraint_to_pos.at(0));
    res *= this_ticket.at(constraint_to_pos.at(1));
    res *= this_ticket.at(constraint_to_pos.at(2));
    res *= this_ticket.at(constraint_to_pos.at(3));
    res *= this_ticket.at(constraint_to_pos.at(4));
    res *= this_ticket.at(constraint_to_pos.at(5));

    return res;
}

int main() {
    auto nearby_path = std::filesystem::path{"../input/day16_nearby.txt"};
    auto constraints_path = std::filesystem::path{"../input/day16_constraints.txt"};

    std::vector<Constraint> constraints = read_constrains(lines_from_file(constraints_path));
    std::vector<std::vector<int>> tickets = read_tickets(lines_from_file(nearby_path));

    auto [total, valids] = valid_tickets(tickets, constraints);

    fmt::print("Part one: {}\n", total);
    fmt::print("Part two: {}\n", part_two(valids, constraints));
}