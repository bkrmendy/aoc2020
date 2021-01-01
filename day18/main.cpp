//
// Created by Berci on 2020. 12. 17..
//

#include <utility>
#include <vector>
#include <numeric>
#include <memory>

#include "aocutils.h"
#include <fmt/core.h>

long to_number(long i) {
    assert('0' <= i && i <= '9');
    return i - '0';
}

bool is_number(char c) {
    return '0' <= c && c <= '9';
}

enum class Operator {
    Add,
    Multiply
};

using Step = std::pair<size_t, long>;

size_t closing_paren_pos(std::string& tokens, size_t from) {
    size_t parens = 0;
    size_t to = from + 1;
    while (true) {
        char this_token = tokens.at(to);
        if (this_token == ')') {
            if (parens == 0) {
                return to;
            }
            parens -= 1;
        }
        if (this_token == '(') {
            parens += 1;
        }
        to += 1;
    }
}

Operator get_operator(std::string& tokens, size_t at) {
    if (tokens.at(at) == '*') {
        return Operator::Multiply;
    } else if (tokens.at(at) == '+') {
        return Operator::Add;
    }
    assert(0 && "Unknown operator");
}

Step evaluate(std::string& tokens, size_t from, size_t to);

Step evaluate_expr(std::string& tokens, size_t from) {
    char this_token = tokens.at(from);
    if (is_number(this_token)) {
        return Step(from + 1, to_number(this_token));
    }

    auto [to, expr] = evaluate(tokens, from + 1, closing_paren_pos(tokens, from));

    return Step(to + 1, expr);
}

Step evaluate(std::string& tokens, size_t from, size_t to) {
    auto [to_e, expr] = evaluate_expr(tokens, from);

    size_t this_pos = to_e;
    long l_value = expr;

    while (this_pos < to) {
        auto op = get_operator(tokens, this_pos);
        auto [to_r, r_expr] = evaluate_expr(tokens, this_pos + 1);

        if (op == Operator::Add) {
            l_value = l_value + r_expr;
        } else if (op == Operator::Multiply) {
            l_value = l_value * r_expr;
        } else {
            assert(0 && "Unknown operator");
        }

        this_pos = to_r;
    }
    return Step(this_pos, l_value);
}

long part_one(std::vector<std::string>& lines) {
    return std::reduce(lines.begin(),
                       lines.end(),
                       0l,
                       [](auto acc, auto line) {
        auto [to, res] = evaluate(line, 0, line.size());
        assert(to == line.size());
        return acc + res;
    });
}

int main() {
    auto path = std::filesystem::path("../input/day18.txt");
    auto lines = lines_from_file(path);
    assert(lines.empty() == false);

    fmt::print("Part one: {}\n", part_one(lines));
}