//
// Created by Berci on 2020. 12. 15..
//

#include <map>
#include <tuple>

#include <fmt/core.h>

int main() {
    //       n               last before last
    std::map<int, std::tuple<int, int>> input = {
//            {0, {1, -1}},
//            {3, {2, -1}},
//            {6, {3, -1}}
            {2, {1, -1}},
            {1, {2, -1}},
            {10, {3, -1}},
            {11, {4, -1}},
            {0, {5, -1}},
            {6, {6, -1}},
    };

    int last = 6;

    int turn = 7;
    while (turn <= 30000000) {
        std::tuple<int, int> g = input.at(last);
        if (std::get<1>(g) < 0) {
            last = 0;
        } else {
            last = std::get<0>(g) - std::get<1>(g);
        }
        if (input.count(last) == 0) {
            input.emplace(last, std::tuple<int, int>(turn, -1));
        } else {
            auto h = input.at(last);
            input.at(last) = std::tuple<int, int>(turn, std::get<0>(h));
        }

        turn += 1;
    }

    fmt::print("Part one: {}", last);


}