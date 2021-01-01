//
// Created by Berci on 2020. 12. 07..
//

#include <filesystem>
#include <exception>

#include <fmt/core.h>
#include "aocutils.h"

enum class OpCode {
    Nop,
    Jmp,
    Acc
};

class InfiniteLoopException : public std::exception {
    const char * what() const noexcept override {
        return "Infinite loop encountered!";
    }
};

class AllModificationsDone : public std::exception {
    const char * what() const noexcept override {
        return "Cannot modify program further!";
    }
};

struct Instr {
    int argument;
    OpCode opCode;
    bool seen;

    Instr(const OpCode &opCode, int argument)
        : opCode(opCode)
        , argument(argument)
        , seen{false}
        {}
};

class Machine {
    size_t iptr_;
    int accumulator_;

    size_t nop_position_;
    size_t jmp_position_;

    std::vector<Instr> original_instructions_;
    std::vector<Instr> instructions_;
public:
    Machine(std::vector<Instr> instructions)
        : iptr_{0}
        , accumulator_{0}
        , nop_position_{0}
        , jmp_position_{0}
        , original_instructions_{std::move(instructions)}
        , instructions_{original_instructions_}
        {}

    void run() {
        while (true) {
            if (iptr_ >= instructions_.size()) {
                return;
            }

            auto& this_instr = instructions_.at(iptr_);
            if (this_instr.seen) {
                throw InfiniteLoopException();
            }

            this_instr.seen = true;
            auto arg = this_instr.argument;

            switch (this_instr.opCode) {
                case OpCode::Acc:
                    this->accumulator_ += arg;
                    this->iptr_ += 1;
                    break;
                case OpCode::Jmp:
                    this->iptr_ += arg;
                    break;
                case OpCode::Nop:
                    this->iptr_ += 1;
            }
        }
    }

    int accumulator() const {
        return accumulator_;
    }

    void reset() {
        instructions_ = original_instructions_;
        iptr_ = 0;
        accumulator_ = 0;
    }

    void tweak() {
        while (nop_position_ < instructions_.size()) {
            if (instructions_.at(nop_position_).opCode == OpCode::Nop) {
                instructions_.at(nop_position_).opCode = OpCode::Jmp;
                nop_position_++;
                return;
            } else {
                nop_position_++;
            }
        }

        while (jmp_position_ < instructions_.size()) {
            if (instructions_.at(jmp_position_).opCode == OpCode::Jmp) {
                instructions_.at(jmp_position_).opCode = OpCode::Nop;
                jmp_position_++;
                return;
            } else {
                jmp_position_++;
            }
        }

        throw AllModificationsDone{};
    }
};

int part_one(std::vector<Instr>& instructions) {
    Machine machine{instructions};

    try {
        machine.run();
    } catch (InfiniteLoopException& ex) {
        return machine.accumulator();
    }
    throw InfiniteLoopException();
}

int part_two(std::vector<Instr>& instructions) {
    Machine machine{instructions};
    while (true) {
        try {
            machine.run();
            return machine.accumulator();
        } catch (InfiniteLoopException& ex) {
            machine.reset();
            machine.tweak();
        }
    }
}

int main() {
    auto path = std::filesystem::path("../input/day08.txt");
    std::vector<Instr> instructions = entries_from_file<Instr>(path, [](const std::string& line) {
        auto parts = split(line, " ");
        auto argument = std::stoi(parts.at(1));
        if (parts.at(0) == "acc") {
            return Instr{OpCode::Acc, argument};
        }
        if (parts.at(0) == "jmp") {
            return Instr{OpCode::Jmp, argument};
        }
        if (parts.at(0) == "nop") {
            return Instr{OpCode::Nop, argument};
        }

        assert(0 && "Unknown instruction");
    });

    fmt::print("Part one: {}\n", part_one(instructions));

    fmt::print("Part two: {}\n", part_two(instructions));
}