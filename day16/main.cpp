//
// Created by Berci on 2020. 12. 15..
//

#include <string>
#include <numeric>
#include <set>
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

std::string your_ticket = "83,137,101,73,67,61,103,131,151,127,113,107,109,89,71,139,167,97,59,53";

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

int part_one(std::filesystem::path& nearby_path, std::filesystem::path& constraints_path) {
    std::vector<Constraint> constraints = read_constrains(lines_from_file(constraints_path));
    std::vector<std::vector<int>> tickets = read_tickets(lines_from_file(nearby_path));

    int total = 0;
    for (size_t i = 0; i < tickets.size(); i++) {
        for (size_t j = 0; j < tickets.at(i).size(); j++) {
            auto value = tickets.at(i).at(j);
            bool ok = std::any_of(constraints.begin(),
                                  constraints.end(),
                                  [&value](Constraint c) { return c.matches(value); });
            if (not ok) {
                total += value;
            }
        }
    }

    return total;
}

int part_two(std::filesystem::path& nearby_path, std::filesystem::path& constraints_path) {
    std::vector<Constraint> constraints = read_constrains(lines_from_file(constraints_path));
    std::vector<std::vector<int>> tickets = read_tickets(lines_from_file(nearby_path));
    auto this_ticket = read_ticket(your_ticket);

    std::set<size_t> matchers{};
    for (size_t c = 0; c < constraints.size(); c++) {
        matchers.insert(c);
    }

    std::map<size_t, size_t> constraint_to_pos{};

    for(size_t i = 0; i < this_ticket.size(); i++) {
        std::set<size_t> these_matchers = matchers;
        for (size_t j = 0; j < tickets.size(); j++) {
            std::set<size_t> m{};
            for (auto c : these_matchers) {
                if (constraints.at(c).matches(tickets.at(j).at(i))) {
                    m.insert(c);
                }
            }
            these_matchers = m;
        }
        assert(these_matchers.size() == 1);
        constraint_to_pos.emplace(*these_matchers.begin(), i);
    }

    auto g = 0;

}

int main() {
    auto path = std::filesystem::path{"../input/day16_nearby.txt"};
    auto constraints_path = std::filesystem::path{"../input/day16_constraints.txt"};

    fmt::print("Part one: {}\n", part_one(path, constraints_path));
    fmt::print("Part two: {}\n", part_two(path, constraints_path));
}