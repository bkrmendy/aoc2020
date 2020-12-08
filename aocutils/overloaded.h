//
// Created by Berci on 2020. 12. 08..
//

#ifndef AOC2020_OVERLOADED_H
#define AOC2020_OVERLOADED_H

/// https://en.cppreference.com/w/cpp/utility/variant/visit

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

#endif //AOC2020_OVERLOADED_H
