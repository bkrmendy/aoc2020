//
// Created by Berci on 2020. 12. 10..
//

#include <filesystem>

#include <fmt/core.h>
#include "utils.h"

int is_occupied(int row, int column, std::vector<std::string>& lines) {
    if (row < 0
        || row >= lines.size()
        || column < 0
        || column >= lines.at(row).size()) {
        return 0;
    }
    if (lines.at(row).at(column) == '#') {
        return 1;
    }
    return 0;
}

char occupied(int row, int column, std::vector<std::string>& lines) {
    int neighbors
            = is_occupied(row - 1, column - 1, lines)
              + is_occupied(row - 1, column, lines)
              + is_occupied(row - 1, column + 1, lines)
              + is_occupied(row, column - 1, lines)
              + is_occupied(row, column + 1, lines)
              + is_occupied(row + 1, column - 1, lines)
              + is_occupied(row + 1, column, lines)
              + is_occupied(row + 1, column + 1, lines);

    if (lines.at(row).at(column) == 'L') {
        return neighbors == 0 ? '#' : 'L';
    }
    if (lines.at(row).at(column) == '#') {
        return neighbors >= 4 ? 'L' : '#';
    }
    return '.';
}

int is_seat(int row, int column, int dr, int dc, std::vector<std::string>& lines) {
    int r = row + dr;
    int c = column + dc;
    while (r < lines.size()
            && r >= 0
            && c >= 0
            && c < lines.at(r).size()){
        if (lines.at(r).at(c) == '#') {
            return  1;
        }
        if (lines.at(r).at(c) == 'L') {
            return 0;
        }
        r += dr;
        c += dc;
    }
    return 0;
}

char occupied2(int row, int column, std::vector<std::string>& lines) {
    int neighbors
        = is_seat(row, column, -1, -1, lines)
          + is_seat(row, column, -1, 0, lines)
          + is_seat(row, column, -1, 1, lines)
          + is_seat(row, column, 0, -1, lines)
          + is_seat(row, column, 0, 1, lines)
          + is_seat(row, column, 1, -1, lines)
          + is_seat(row, column, 1, 0, lines)
          + is_seat(row, column, 1, 1, lines);

    if (lines.at(row).at(column) == 'L') {
        return neighbors == 0 ? '#' : 'L';
    }
    if (lines.at(row).at(column) == '#') {
        return neighbors >= 5 ? 'L' : '#';
    }
    return '.';
}

std::vector<std::string> step(std::vector<std::string>& lines) {
    std::vector<std::string> lines2 = lines;
    for (int row = 0; row < lines.size(); row++) {
        for(int column = 0; column < lines.at(row).size(); column++) {
            lines2.at(row).at(column) = occupied(row, column, lines);
        }
    }
    return lines2;
}

std::vector<std::string> step2(std::vector<std::string> lines) {
    std::vector<std::string> lines2 = lines;
    for (int row = 0; row < lines.size(); row++) {
        for(int column = 0; column < lines.at(row).size(); column++) {
            lines2.at(row).at(column) = occupied2(row, column, lines);
        }
    }
    return lines2;
}

int count(std::vector<std::string>& lines) {
    int res = 0;
    for (const auto& row : lines) {
        for (const auto& seat : row) {
            if (seat == '#') {
                res += 1;
            }
        }
    }
    return res;
}

int part_one(std::vector<std::string>& lines) {
    while (true) {
        auto lines2 = step(lines);
        if (lines2 == lines) {
            return count(lines2);
        }
        lines = lines2;
    }
}

int part_two(std::vector<std::string>& lines) {
    while (true) {
        auto lines2 = step2(lines);
        if (lines2 == lines) {
            return count(lines2);
        }
        lines = lines2;
    }
}

int main() {
    auto path = std::filesystem::path{"../input/day11.txt"};

    std::vector<std::string> lines = lines_from_file(path);
    std::vector<std::string> lines2 = lines;

    fmt::print("Part one: {}\n", part_one(lines));
    fmt::print("Part two: {}\n", part_two(lines2));
}