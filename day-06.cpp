#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

#include "utility.h"

auto day06(const std::vector<aoc::CustomsDeclaration>& declarations) -> std::size_t {
    std::size_t count = 0;
    for (const auto& d : declarations) {
        count += d.size();
    }
    return count;
}

auto main(int argc, char **argv) -> int {
    std::vector<aoc::CustomsDeclaration> declarations;
    aoc::readFromFile("day-06-sample.txt", std::back_inserter(declarations), false);
    std::cout << "Part 1 sample: " << day06(declarations) << "\n";
    declarations.clear();
    aoc::readFromFile("day-06-sample.txt", std::back_inserter(declarations), true);
    std::cout << "Part 2 sample: " << day06(declarations) << "\n";

    declarations.clear();
    aoc::readFromFile("day-06-input.txt", std::back_inserter(declarations), false);
    std::cout << "Part 1 solution: " << day06(declarations) << "\n";
    declarations.clear();
    aoc::readFromFile("day-06-input.txt", std::back_inserter(declarations), true);
    std::cout << "Part 2 solution: " << day06(declarations) << "\n";

    return 0;
}
