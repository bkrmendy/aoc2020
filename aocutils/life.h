//
// Created by Berci on 2021. 01. 01..
//

#ifndef AOC2020_LIFE_H
#define AOC2020_LIFE_H

#include <unordered_set>
#include <future>
#include <vector>

template <typename Coord>
std::unordered_set<Coord> neighbors_of(const Coord& coord);

template <typename State>
State next_state(const State& this_state, size_t n_active_neighbors);

template <typename State>
State make_active_state();

template <typename State>
State make_inactive_state();

template <typename Coord, typename State>
class Life {
public:
    using Container = std::unordered_set<Coord>;

private:
    State get_next_state(const Container& state,
                         const Coord& coord, State current_state) const {
        Container neighbors = neighbors_of(coord);
        size_t n_actives = std::count_if(neighbors.begin(),
                                         neighbors.end(),
                                         [&state](const Coord& neighbor) { return state.contains(neighbor); });
        return next_state(current_state, n_actives);
    };

public:
    Container step(const Container& state) {
        Container new_state{};
        auto active_state = make_active_state<State>();
        auto inactive_state = make_inactive_state<State>();

        for (const Coord& coord : state) {
            if (get_next_state(state, coord, active_state) == active_state) {
                new_state.insert(coord);
            }

            for (const Coord& crd : neighbors_of(coord)) {
                auto this_state = state.contains(crd) ? active_state : inactive_state;
                if (get_next_state(state, crd, this_state) == active_state) {
                    new_state.insert(crd);
                }
            }
        }

        return new_state;
    }
};



#endif //AOC2020_LIFE_H
