//
// Created by Berci on 2020. 12. 17..
//

#include <utility>
#include <vector>
#include <numeric>
#include <memory>

#include "utils.h"
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

using Step = std::pair<size_t, std::shared_ptr<Expression>>;

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
        return Step(from + 1, std::make_shared<Number>(to_number(this_token)));
    }

    auto [to, expr] = evaluate(tokens, from + 1, closing_paren_pos(tokens, from));

    return Step(to + 1, expr);
}

Step evaluate(std::string& tokens, size_t from, size_t to) {
    auto [to_e, expr] = evaluate_expr(tokens, from);

    size_t this_pos = to_e;
    std::shared_ptr<Expression> l_expr = expr;

    while (this_pos < to) {
        auto op = get_operator(tokens, this_pos);
        auto [to_r, r_expr] = evaluate_expr(tokens, this_pos + 1);

        l_expr = std::make_shared<Compound>(op, l_expr, r_expr);

        this_pos = to_r;
    }
    return Step(this_pos, l_expr);
}

std::shared_ptr<Expression> balance(const std::shared_ptr<Expression>& e) {
    if (auto n = std::dynamic_pointer_cast<Number>(e)) {
        return n;
    }

    if (auto c = std::dynamic_pointer_cast<Compound>(e)) {
        if (c->op() == Operator::Add) {
            if (std::dynamic_pointer_cast<Number>(c->left())
                && std::dynamic_pointer_cast<Number>(c->right())) {
                return c;
            }

            if (auto left = std::dynamic_pointer_cast<Compound>(c->left())) {
                if (left->op() == Operator::Multiply) {
                    c = rotate_right(c, left);
                }
            }

            if (auto right = std::dynamic_pointer_cast<Compound>(c->right())) {
                if (right->op() == Operator::Multiply) {
                    c = rotate_left(c, right);
                }
            }
        }

        return std::make_shared<Compound>(
                c->op(),
                balance(c->left()),
                balance(c->right())
        );
    }
    assert(0 && "Unknown expression");
}

long part_one(std::vector<std::string>& lines) {
    return std::reduce(lines.begin(),
                       lines.end(),
                       0l,
                       [](auto acc, auto line) {
        auto [to, res] = evaluate(line, 0, line.size());
        assert(to == line.size());
        return acc + res->evaluate();
    });
}

long part_two(std::vector<std::string>& lines) {
    return std::reduce(lines.begin(),
                       lines.end(),
                       0l,
                       [](auto acc, auto line) {
       auto[to, res] = evaluate(line, 0, line.size());
       assert(to == line.size());
       return acc + balance(res)->evaluate();
   });
}

int main() {
    auto path = std::filesystem::path("../input/day18.txt");
    auto lines = lines_from_file(path);
    assert(lines.empty() == false);

    fmt::print("Part one: {}\n", part_one(lines));
    fmt::print("Part one: {}\n", part_two(lines));
}