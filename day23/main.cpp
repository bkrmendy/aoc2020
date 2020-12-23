//
// Created by Berci on 2020. 12. 23..
//

#include <list>
#include <fmt/core.h>
#include <fmt/ranges.h>

std::list<int>::iterator next_with_wrap(std::list<int>& list, std::list<int>::iterator it) {
    it = std::next(it);
    if (it == list.end()) {
        return list.begin();
    }
    return it;
}

void play(std::list<int>& cups, size_t moves) {
    int current = *cups.begin();
    auto cur = cups.begin();
    int max = *std::max_element(cups.begin(), cups.end());
    int min = *std::min_element(cups.begin(), cups.end());

    for (size_t round = 0; round < moves; round++) {
        if (round % 1000 == 0) {
            fmt::print("Round {}\n", round);
        }
        std::list<int> this_range{};

        // remove next 3
        auto cur1 = next_with_wrap(cups, cur);
        this_range.push_back(*cur1);
        auto cur2 = next_with_wrap(cups, cur1);
        this_range.push_back(*cur2);
        auto cur3 = next_with_wrap(cups, cur2);
        this_range.push_back(*cur3);

        cups.erase(cur1);
        cups.erase(cur2);
        cups.erase(cur3);

        // select destination
        int destination = current;

        if (destination == min) {
            destination = max;
        } else {
            destination = current - 1;
        }

        while (std::find(this_range.begin(), this_range.end(), destination) != this_range.end()) {
            if (destination == min) {
                destination = max;
            } else {
                destination = destination - 1;
            }
        }

        auto dest = std::find(cups.begin(), cups.end(), destination);
        dest = next_with_wrap(cups, dest);

        // insert immediately clockwise
        for (auto &picked : this_range) {
            cups.insert(dest, picked);
        }

        // select new current
        cur = next_with_wrap(cups, cur);
        current = *cur;
    }
}

/**
 *
 * TODO: array where index is cup value and value is next pointer in said array
 */

int main() {
    std::list<int> cups = {5, 8, 3, 9, 7, 6, 2, 4, 1};
    play(cups, 100);
    // 24987653
    fmt::print("Part one: {}\n", cups);

    std::list<int> cups_ext = {3,8,9,1,2,5,4,6,7};
    for (int n = 10; n < 1'000'000; n++) {
        cups_ext.push_back(n);
    }

    play(cups_ext, 10'000'000);

    auto one = std::find(cups_ext.begin(), cups_ext.end(), 1);
    one = next_with_wrap(cups_ext, one);
    int first = *one;
    one = next_with_wrap(cups_ext, one);
    int second = *one;

    fmt::print("Part two: {}\n", first * second);
}