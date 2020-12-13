#include <fmt/core.h>
#include "utils.h"
#include "chinese_remainder.h"

long long first_timestamp_larger_than(long long timestamp, int bus_id) {
    long long frac = timestamp / bus_id;
    long long next_timestamp = (frac + 1) * bus_id;
    return next_timestamp;
}

long long part_one(long long timestamp, std::vector<std::string>& buses) {
    auto this_bus = 0;
    auto min_time = 100000000000000;
    for (const auto& bus : buses) {
        if (bus != "x") {
            auto id = std::stoi(bus);
            long long wait_time = first_timestamp_larger_than(timestamp, id) - timestamp;
            if (wait_time < min_time) {
                min_time = wait_time;
                this_bus = id;
            }
        }
    }
    return min_time * this_bus;
}

int main() {
    auto path = std::filesystem::path("../input/day13.txt");
    auto lines = lines_from_file(path);

    long long timestamp = std::stoi(lines.at(0));
    std::vector<std::string> buses = split(lines.at(1), ",");

    std::vector<int> bus_ids{};

    for (const auto& bus : buses) {
        if (bus == "x") {
            bus_ids.push_back(0);
        } else {
            bus_ids.push_back(std::stoi(bus));
        }
    }



    fmt::print("Part one: {}\n", part_one(timestamp, buses));
    fmt::print("Part two: solved with python");
}