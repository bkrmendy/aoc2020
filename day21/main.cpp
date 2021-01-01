//
// Created by Berci on 2020. 12. 21..
//

#include <set>
#include <map>

#include <fmt/core.h>
#include "aocutils.h"

int main() {
    auto path = std::filesystem::path{"../input/day21.txt"};
    auto lines = lines_from_file(path);

    std::set<std::string> ingredients;
    std::map<std::string, std::set<std::string>> allergen_to_ingredient;
    std::vector<std::vector<std::string>> recipes;

    for (auto& line : lines) {
        auto part = line.substr(0, line.size() - 1);
        auto ins = split(line.substr(0, line.find('(')), " ");
        std::string delim = "(contains";
        auto g = part.substr(line.find(delim) + delim.size() + 1);
        auto all = split(g, ", ");

        std::vector<std::string> this_recipe;

        for (const auto& ingredient : ins) {
            ingredients.insert(ingredient);
            this_recipe.push_back(ingredient);
        }
        recipes.push_back(std::move(this_recipe));

        auto ins_set = std::set<std::string>{ins.begin(), ins.end()};

        for (const auto& allergen : all) {
            if (!allergen_to_ingredient.contains(allergen)) {
                allergen_to_ingredient.emplace(allergen, ins_set);
            }
            auto ingrs = allergen_to_ingredient.at(allergen);
            auto h = std::vector<std::set<std::string>>{ingrs, ins_set};
            allergen_to_ingredient[allergen] = intersection(h);
        }
    }

    for (auto& [_, ingrs] : allergen_to_ingredient) {
        ingredients = difference(ingredients, ingrs);
    }

    int count = 0;
    for (auto& recipe : recipes) {
        count += std::count_if(recipe.begin(),
                               recipe.end(),
                               [&ingredients](auto& ingr) { return ingredients.contains(ingr); });
    }

    fmt::print("Part one: {}\n", count);

    /*
     * Low tech solution:
     * 
     * diary:
     *  kllgt
     * eggs:
     *  jrnqx
     * fish:
     *  ljvx
     * nuts:
     *  zxstb
     * peanuts:
     *  gnbxs
     * sesame:
     *  mhtc
     * soy:
     *  hfdxb
     * wheat:
     *  hbfnkq
     *
     * Canonical List:
     *  kllgt,jrnqx,ljvx,zxstb,gnbxs,mhtc,hfdxb,hbfnkq
     */

    fmt::print("Part two: {}\n", "kllgt,jrnqx,ljvx,zxstb,gnbxs,mhtc,hfdxb,hbfnkq");


}