//
// Created by Berci on 2020. 12. 23..
//

#include <vector>
#include <list>
#include <fmt/core.h>
#include <fmt/ranges.h>

std::list<size_t>::iterator next_with_wrap(std::list<size_t>& list, std::list<size_t>::iterator it) {
    it = std::next(it);
    if (it == list.end()) {
        return list.begin();
    }
    return it;
}

/**
 * Original solution included for completeness
 */
[[maybe_unused]]
void play(std::list<size_t>& cups, size_t moves) {
    int current = *cups.begin();
    auto cur = cups.begin();
    int max = *std::max_element(cups.begin(), cups.end());
    int min = *std::min_element(cups.begin(), cups.end());

    for (size_t round = 0; round < moves; round++) {
        if (round % 1000 == 0) {
            fmt::print("Round {}\n", round);
        }
        std::list<size_t> this_range{};

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
 * Solution inspired by reddit:
 * Array where index is cup value and value at index is next in said array
 */
std::vector<size_t> play2(std::vector<size_t> cups, size_t moves) {
    size_t min = 1;
    size_t max = cups.size() - 1;
    for (size_t round = 0; round < moves; round++) {
        // remove next 3
        size_t current = cups.at(0);
        size_t nxt = cups.at(current);
        size_t nxt2 = cups.at(nxt);
        size_t nxt3 = cups.at(nxt2);

        // select destination
        size_t destination = current;
        do {
            if (destination == min) {
                destination = max;
            } else {
                destination -= 1;
            }
        }
        while (destination == nxt || destination == nxt2 || destination == nxt3);

        // insert immediately clockwise
        cups.at(current) = cups.at(nxt3);
        size_t destination_next = cups.at(destination);
        cups.at(destination) = nxt;
        cups.at(nxt3) = destination_next;

        // select new current
        cups.at(0) = cups.at(current);
    }
    return cups;
}

std::string part_one_sol(std::vector<size_t>&& cups) {
    std::string res;
    size_t current = cups.at(1);
    while (current != 1) {
        res += current + '0';
        current = cups.at(current);
    }
    return res;
}

int main() {
    // original: 583976241      c  1  2  3  4  5  6  7  8  9
    std::vector<size_t> cups = {5, 5, 4, 9, 1, 8, 2, 6, 3, 7};

    // example: 389125467     c  1  2  3  4  5  6  7  8  9
    std::vector<size_t> ex = {3, 2, 5, 8, 6, 4, 7, 3, 9, 1};

    fmt::print("Part one: {}\n", part_one_sol(play2(ex, 100)));

    std::vector<size_t> cups_ext = cups;
    cups_ext.at(1) = 10;
    for (size_t i = 10; i < 1'000'000; i++) {
        cups_ext.push_back(i + 1);
    }
    cups_ext.push_back(5);
    assert(cups_ext.size() == 1000001);

    auto z = play2(cups_ext, 10'000'000);

    uint64_t first = z.at(1);
    uint64_t second = z.at(first);

    fmt::print("Part two: {} * {} = {}\n", first, second, first * second);
}