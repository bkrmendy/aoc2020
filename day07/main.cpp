//
// Created by Berci on 2020. 12. 06..
//

#include <filesystem>
#include <map>
#include <set>
#include <utility>
#include <numeric>

#include <fmt/core.h>
#include "../aocutils/aocutils.h"

struct Entry {
    int quantity;
    std::string color;

    Entry(int quantity, std::string color) : quantity(quantity), color(std::move(color)) {}
};

bool bag_can_contain(std::string& color,
                     std::vector<Entry>& contained,
                     std::map<std::string, std::vector<Entry>>& bags) {
    for (const auto& entry : contained) {
        if (entry.color == color) {
            return true;
        }
        if (bag_can_contain(color, bags.at(entry.color), bags)) {
            return true;
        }
    }

    return false;
}

int bag_contains_n(std::vector<Entry>& entries, std::map<std::string, std::vector<Entry>>& bags) {
    return std::reduce(entries.begin(),
                       entries.end(),
                       0,
                       [&bags](int acc, Entry& entry) {
        int contained = bag_contains_n(bags.at(entry.color), bags);
       return acc + entry.quantity + entry.quantity * contained;
    });
}

int main() {
    auto path = std::filesystem::path("../input/day07.txt");
    auto lines = lines_from_file(path);

    std::map<std::string, std::vector<Entry>> entries{};

    for (const auto& line : lines) {
        auto parts = split(line, " bags contain ");

        if (parts.at(1).find("no other") != std::string::npos) {
            entries.emplace(parts.at(0), std::vector<Entry>{});
        } else {
            const auto contained = split(parts.at(1).substr(0, parts.at(1).size() - 1), ", ");

            std::vector<Entry> colors_amounts{};

            for (const auto& entry : contained) {
                int amount = std::stoi(entry.substr(0, 1));
                std::string color = entry.substr(2, entry.find(" bag") - 2);
                colors_amounts.emplace_back(amount, color);
            }

            entries.emplace(parts.at(0), colors_amounts);
        }
    }

    std::string target = "shiny gold";

    std::set<std::string> bags;
    for (auto& [bag, contained] : entries) {
        if (bag_can_contain(target, contained, entries)) {
            bags.insert(bag);
        }
    }

    fmt::print("Part one: {}\n", bags.size());

    int shiny_contains = 0;

    for (auto& [bag, bags] : entries) {
        if (bag == target) {
            shiny_contains += bag_contains_n(bags, entries);
        }
    }

    fmt::print("Part one: {}\n", shiny_contains);

    return 0;
}