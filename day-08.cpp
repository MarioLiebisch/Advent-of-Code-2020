#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

#include "utility.h"

std::set<std::size_t> visited;
std::vector<aoc::Instruction> instructions;
std::size_t last_jump = -1;
signed int last_jump_acc = 0;
bool infinite = false;
std::set<std::size_t> nops, jmps;

auto day08part1(const std::vector<aoc::Instruction>& instructions) -> std::size_t {
    aoc::VirtualMachine vm(instructions);
    visited.clear();
    vm.setPreCallback([](std::size_t& ip, signed int& accumulator) -> bool {
        if (visited.find(ip) != visited.end()) {
            return false; // Halt execution
        }
        visited.insert(ip);
        return true; // Continue execution
        });
    return vm();
}

auto day08part2(const std::vector<aoc::Instruction>& instructions) -> std::size_t {
    {
        aoc::VirtualMachine vm(instructions);
        visited.clear();
        // Let's first execute the program and track all no-ops and jumps along the way
        vm.setPreCallback([](std::size_t& ip, signed int& accumulator) -> bool {
            if (visited.find(ip) != visited.end()) {
                return false; // Halt execution
            }
            visited.insert(ip);
            // Track jumps
            if (::instructions[ip].op == "jmp") {
                jmps.insert(ip);
            }
            // Track no-ops
            else if (::instructions[ip].op == "nop") {
                nops.insert(ip);
            }
            return true; // Continue execution
            });
        vm();
    }

    // Try to change any no-op to jump:
    for (const auto& nip : nops) {
        std::vector<aoc::Instruction> temp(instructions);
        temp[nip].op = "jmp";
        aoc::VirtualMachine vm(temp);
        infinite = false;
        visited.clear();
        vm.setPreCallback([](std::size_t& ip, signed int& accumulator) -> bool {
            if (visited.find(ip) != visited.end()) {
                infinite = true;
                return false; // Halt execution
            }
            visited.insert(ip);
            return true; // Continue execution
            });
        const signed int ret = vm();
        if (!infinite) { // Solution!
            return ret;
        }
    }

    // Try to change any jump to no-op:
    for (const auto& jip : jmps) {
        std::vector<aoc::Instruction> temp(instructions);
        temp[jip].op = "nop";
        aoc::VirtualMachine vm(temp);
        infinite = false;
        visited.clear();
        vm.setPreCallback([](std::size_t& ip, signed int& accumulator) -> bool {
            if (visited.find(ip) != visited.end()) {
                infinite = true;
                return false; // Halt execution
            }
            visited.insert(ip);
            return true; // Continue execution
            });
        const signed int ret = vm();
        if (!infinite) { // Solution!
            return ret;
        }
    }
    // Shouldn't ever happen.
    return 0;
}

auto main(int argc, char **argv) -> int {
    aoc::readFromFile<aoc::Instruction>("day-08-sample.txt", std::back_inserter(instructions));
    std::cout << "Part 1 sample: " << day08part1(instructions) << "\n";
    std::cout << "Part 2 sample: " << day08part2(instructions) << "\n";

    instructions.clear();
    aoc::readFromFile<aoc::Instruction>("day-08-input.txt", std::back_inserter(instructions));
    std::cout << "Part 1 solution: " << day08part1(instructions) << "\n";
    std::cout << "Part 2 solution: " << day08part2(instructions) << "\n";

    return 0;
}
