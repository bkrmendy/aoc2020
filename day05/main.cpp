//
// Created by Berci on 2020. 12. 04..
//

#include <filesystem>
#include <vector>
#include <set>
#include <numeric>

#include <fmt/core.h>
#include "utils.h"

int partition_rows(size_t rows_min, size_t rows_max, std::string&& bsp) {
    if (bsp.size() == 1) {
        if (bsp[0] == 'F') {
            return rows_min;
        }
        return rows_max;
    }

    assert(rows_min < rows_max);
    assert(bsp.size() <= 7);

    if (bsp[0] == 'F') {
        return partition_rows(rows_min,
                              rows_min + (rows_max - rows_min) / 2,
                              bsp.substr(1));
    } else if (bsp[0] == 'B') {
        return partition_rows(rows_min + (rows_max - rows_min) / 2 + 1,
                              rows_max,
                              bsp.substr(1));
    }
    assert(false);
}

int partition_columns(size_t columns_min, size_t columns_max, std::string&& bsp) {
    if (bsp.size() == 1) {
        if (bsp[0] == 'L') {
            return columns_min;
        }
        return columns_max;
    }

    assert(columns_min < columns_max);
    assert(bsp.size() <= 3);

    if (bsp[0] == 'L') {
        return partition_columns(columns_min,
                                 columns_min + (columns_max - columns_min) / 2,
                                 bsp.substr(1));
    } else if (bsp[0] == 'R') {
        return partition_columns(columns_min + (columns_max - columns_min) / 2 + 1,
                                 columns_max,
                                 bsp.substr(1));
    }

    assert(false);
}

int seat_id(int row, int column) {
    return row * 8 + column;
}

void part_one(const std::vector<std::string>& lines) {
    const size_t rows = 127;
    const size_t columns = 7;

    int max_id = 0;

    std::for_each(lines.begin(), lines.end(), [&max_id, &rows, &columns](const std::string& line) {
        auto this_id = seat_id(
                partition_rows(0, rows, line.substr(0, 7)),
                partition_columns(0, columns, line.substr(7, line.size()))
        );
        max_id = std::max(max_id, this_id);
    });

    fmt::print("Part one: {}\n", max_id);
}

int main() {
    auto path = std::filesystem::path("../input/day05.txt");
    auto lines = lines_from_file(path);

    part_one(lines);

    const size_t rows = 127;
    const size_t columns = 7;

    std::vector<int> ids = entries_from_file<int>(path, [&rows, &columns](const std::string& line) {
        return seat_id(
                partition_rows(0, rows, line.substr(0, 7)),
                partition_columns(0, columns, line.substr(7, line.size()))
        );
    });

    std::sort(ids.begin(), ids.end());

    std::set<int> seats{};

    for (const auto& seat : ids) {
        seats.emplace(seat);
    }

    for (const auto& seat : seats) {
        if (!seats.contains(seat + 1) && seats.contains(seat + 2)) {
            fmt::print("Part one: {}\n", seat + 1);
        }
    }

    return 0;
}
