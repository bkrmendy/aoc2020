//
// Created by Berci on 2020. 12. 11..
//

#include <filesystem>
#include <cmath>

#include <fmt/core.h>
#include "aocutils.h"

struct Move {
    char direction;
    int amount;

    Move(char direction, int amount) : direction(direction), amount(amount) {}
};

struct Heading {
    int dx;
    int dy;

    Heading(int dx, int dy) : dx(dx), dy(dy) {}

    static Heading north() {
        return Heading{0, 1};
    }

    static Heading south() {
        return Heading{0, -1};
    }

    static Heading east() {
        return Heading{1, 0};
    }

    static Heading west() {
        return Heading{-1, 0};
    }
};

bool operator==(const Heading& left, const Heading& right) {
    return left.dx == right.dx && left.dy == right.dy;
}

Heading direction_to_heading(Heading current, char direction) {
    if (direction == 'F') {
        return current;
    }
    if (direction == 'N') {
        return Heading::north();
    }
    if (direction == 'S') {
        return Heading::south();
    }
    if (direction == 'W') {
        return Heading::west();
    }
    if (direction == 'E') {
        return Heading::east();
    }
    if (direction == 'L') {
        return Heading{-current.dy, current.dx};
    }
    if (direction == 'R') {
        return Heading{current.dy, -current.dx};
    }
    assert(0 && "Unknown heading");
}

int part_one(std::vector<Move>& moves) {
    Heading heading = Heading::east();
    int offset_x = 0;
    int offset_y = 0;

    for (const auto& move : moves) {
        if (move.direction == 'F') {
            offset_x += heading.dx * move.amount;
            offset_y += heading.dy * move.amount;
        }
        else if (move.direction == 'R' || move.direction == 'L') {
            int amount = move.amount;
            while (amount > 0) {
                heading = direction_to_heading(heading, move.direction);
                amount -= 90;
            }
        } else {
            Heading this_heading = direction_to_heading(heading, move.direction);
            offset_x += this_heading.dx * move.amount;
            offset_y += this_heading.dy * move.amount;
        }
    }

    return std::abs(offset_x) + std::abs(offset_y);
}

int main() {
    auto path = std::filesystem::path{"../input/day12.txt"};

    std::vector<Move> moves = entries_from_file<Move>(path, [](const std::string& entry) {
       return Move(entry.at(0), std::stoi(entry.substr(1)));
    });

    int offset_x = 0;
    int offset_y = 0;

    int waypt_x = 10;
    int waypt_y = 1;

    for (const auto& move : moves) {
        if (move.direction == 'F') {
            offset_x += waypt_x * move.amount;
            offset_y += waypt_y * move.amount;
        } else if (move.direction == 'L' || move.direction == 'R') {
            Heading wpt{waypt_x, waypt_y};
            int amount = move.amount;
            while (amount > 0) {
                wpt = direction_to_heading(wpt, move.direction);
                amount -= 90;
            }
            waypt_x = wpt.dx;
            waypt_y = wpt.dy;
        } else {
            Heading this_heading = direction_to_heading(Heading::east(), move.direction);
            waypt_x += this_heading.dx * move.amount;
            waypt_y += this_heading.dy * move.amount;
        }
    }

    int part_two = std::abs(offset_x) + std::abs(offset_y);

    fmt::print("Part one: {}\n", part_one(moves));
    fmt::print("Part two: {}\n", part_two);
}