//
// Created by Berci on 2020. 12. 22..
//

#include <deque>
#include <set>
#include <tuple>

#include <fmt/core.h>
#include "utils.h"

enum class Player {
    PlayerOne,
    PlayerTwo
};

std::deque<int> read_deck(std::filesystem::path&& path) {
    std::vector<int> deck
        = entries_from_file<int>(path,[](const std::string& line) { return std::stoi(line); });
    auto q = std::deque<int>();
    std::move(deck.begin(), deck.end(), std::back_inserter(q));
    return q;
}

long score(std::deque<int>& deck) {
    long res = 0;
    long coeff = 1;

    while (!deck.empty()) {
        res += coeff * deck.back();
        deck.pop_back();
        coeff += 1;
    }

    return res;
}

std::deque<int> copy_deck(int top, std::deque<int>& deck) {
    std::deque<int> copy{};
    for (size_t i = 0; i < top; i++) {
        copy.push_back(deck.at(i));
    }
    return copy;
}

std::pair<Player, long> play_recursive(std::deque<int>& player_1_deck, std::deque<int>& player_2_deck) {
    std::set<std::pair<std::deque<int>, std::deque<int>>> prev_decks;

    while (!player_1_deck.empty() && !player_2_deck.empty()) {
        if (prev_decks.contains({player_1_deck, player_2_deck})) {
            return {Player::PlayerOne, score(player_1_deck)};
        }
        prev_decks.insert({player_1_deck, player_2_deck});

        auto p1_top = player_1_deck.front();
        player_1_deck.pop_front();
        auto p2_top = player_2_deck.front();
        player_2_deck.pop_front();

        Player winner;
        if (p1_top <= player_1_deck.size() && p2_top <= player_2_deck.size()) {
            std::deque<int> p1_copy = copy_deck(p1_top, player_1_deck);
            std::deque<int> p2_copy = copy_deck(p2_top, player_2_deck);

            const auto& [winning_player, _] = play_recursive(p1_copy, p2_copy);
            winner = winning_player;
        } else {
            winner = p1_top > p2_top ? Player::PlayerOne : Player::PlayerTwo;
        }

        if (winner == Player::PlayerOne) {
            player_1_deck.push_back(p1_top);
            player_1_deck.push_back(p2_top);
        } else if (winner == Player::PlayerTwo) {
            player_2_deck.push_back(p2_top);
            player_2_deck.push_back(p1_top);
        } else {
            assert(0 && "Unrecognized player");
        }
    }

    if (!player_1_deck.empty()) {
        return {Player::PlayerOne, score(player_1_deck)};
    } else {
        return {Player::PlayerTwo, score(player_2_deck)};
    }
}

long part_one(std::deque<int> player_1_deck, std::deque<int> player_2_deck) {
    while (!player_1_deck.empty() && !player_2_deck.empty()) {
        auto p1_top = player_1_deck.front();
        player_1_deck.pop_front();
        auto p2_top = player_2_deck.front();
        player_2_deck.pop_front();

        if (p1_top > p2_top) {
            player_1_deck.push_back(p1_top);
            player_1_deck.push_back(p2_top);
        } else {
            player_2_deck.push_back(p2_top);
            player_2_deck.push_back(p1_top);
        }
    }

    if (!player_1_deck.empty()) {
        return score(player_1_deck);
    } else {
        return score(player_2_deck);
    }
}

long part_two(std::deque<int> player_1_deck, std::deque<int> player_2_deck) {
    const auto& [_, winning_score] = play_recursive(player_1_deck, player_2_deck);
    return winning_score;
}

int main() {
    auto player_1_deck = read_deck(std::filesystem::path{"../input/day22_1.txt"});
    auto player_2_deck = read_deck(std::filesystem::path{"../input/day22_2.txt"});

    fmt::print("Part one: {}\n", part_one(player_1_deck, player_2_deck));
    fmt::print("Part two: {}\n", part_two(player_1_deck, player_2_deck));
}