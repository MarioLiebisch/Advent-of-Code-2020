#include <iostream>
#include <vector>
#include <algorithm>

#include "utility.h"

std::vector<long> expenses;

const long TARGET_NUMBER = 2020;

auto day1part1(const std::vector<long> &expenses) -> long {
    for (const auto& a : expenses) {
        for (const auto& b : expenses) {
            if (a == b) {
                continue;
            }
            if (TARGET_NUMBER == a + b) {
                return a * b;
            }
        }
    }
    return 0;
}

auto day1part2(const std::vector<long>& expenses) -> long {
    for (const auto& a : expenses) {
        for (const auto& b : expenses) {
            for (const auto& c : expenses) {
                if (a == b || a == c || b == c) {
                    continue;
                }
                if (TARGET_NUMBER == a + b + c) {
                    return a * b * c;
                }
            }
        }
    }
    return 0;
}

auto main(int argc, char **argv) -> int {
    aoc::readFromFile<long>("day-01-sample.txt", std::back_inserter(expenses));
    std::cout << "Sample 1 solution: " << day1part1(expenses) << "\n";
    std::cout << "Sample 2 solution: " << day1part2(expenses) << "\n";

    expenses.clear();
    aoc::readFromFile<long>("day-01-input.txt", std::back_inserter(expenses));
    std::cout << "Part 1 solution: " << day1part1(expenses) << "\n";
    std::cout << "Part 2 solution: " << day1part2(expenses) << "\n";

    return 0;
}
