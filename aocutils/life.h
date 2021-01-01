//
// Created by Berci on 2021. 01. 01..
//

#ifndef AOC2020_LIFE_H
#define AOC2020_LIFE_H

#include <set>

template <typename Coord>
std::set<Coord> neighbors_of(const Coord& coord);

template <typename State>
State next_state(const State& this_state, size_t n_active_neighbors);

template <typename State>
State make_active_state();

template <typename State>
State make_inactive_state();

template <typename Coord, typename State>
class Life {
    State get_next_state(const std::set<Coord>& state, const Coord& coord, State current_state) {
        std::set<Coord> neighbors = neighbors_of(coord);
        size_t n_actives = std::count_if(neighbors.begin(),
                                         neighbors.end(),
                                         [&state](const Coord& neighbor) { return state.contains(neighbor); });
        return next_state(current_state, n_actives);
    };

public:
    std::set<Coord> step(const std::set<Coord>& state) {
        std::set<Coord> new_state{};
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
