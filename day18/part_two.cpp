//
// Created by Berci on 2020. 12. 18..
//

#include <string>
#include <queue>
#include <stack>
#include <numeric>

#include <fmt/core.h>
#include "utils.h"

/// Based on
/// https://en.wikipedia.org/wiki/Shunting-yard_algorithm
/// https://stackoverflow.com/questions/40328938/how-to-evaluate-reverse-polish-notation-using-stacks
/// TIL

bool is_operator(char c) {
    return c == '*' || c == '+';
}

bool is_number(char c) {
    return '0' <= c && c <= '9';
}

long to_number(char c) {
    return c - '0';
}

bool is_open_paren(char c) {
    return c == '(';
}

bool is_close_paren(char c) {
    return c == ')';
}

std::queue<char> shunt(std::string& line) {
    std::queue<char> output;
    std::stack<char> operator_stack;

    for (char c : line) {
        if (is_number(c)) {
            output.push(c);
        } else if (is_operator(c)) {
            while (!operator_stack.empty()
                    // advanced math
                    && c == '*' && operator_stack.top() == '+'
                    && !is_open_paren(operator_stack.top())) {
                output.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(c);
        } else if (is_open_paren(c)) {
            operator_stack.push(c);
        } else if (is_close_paren(c)) {
            while (!is_open_paren(operator_stack.top())) {
                output.push(operator_stack.top());
                operator_stack.pop();
            }
            if (is_open_paren(operator_stack.top())) {
                operator_stack.pop();
            }
        }
    }

    while (!operator_stack.empty()) {
        output.push(operator_stack.top());
        operator_stack.pop();
    }

    return output;
}

long rpn_eval(std::queue<char>&& out_queue) {
    std::stack<long> s;

    while (!out_queue.empty()) {
        char t = out_queue.front();
        out_queue.pop();
        if (is_number(t)) {
            s.push(to_number(t));
        } else {
            long left = s.top();
            s.pop();
            long right = s.top();
            s.pop();

            if (t == '*') {
                s.push(left * right);
            } else if (t == '+') {
                s.push(left + right);
            } else {
                assert(0 && "Unknown operator");
            }
        }
    }

    return s.top();
}

int main() {
    auto path = std::filesystem::path("../input/day18.txt");
    auto lines = lines_from_file(path);

    long sum = std::reduce(lines.begin(),
                           lines.end(),
                           0l,
                           [](long acc, std::string& line) {
                               return acc + rpn_eval(shunt(line));
    });

    fmt::print("Part two: {}\n", sum);

}