#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

std::vector<aoc::PasswordData> passwords;

const long TARGET_NUMBER = 2020;

auto day02part1(const aoc::PasswordData &password) -> unsigned int {
    std::unordered_map<char, std::size_t> counts;
    for (const char &c : password.password) {
        ++counts[c];
    }
    for (const auto& rule : password.rules) {
        const char &c = rule.character;
        if (counts[c] < rule.minimum || counts[c] > rule.maximum) {
            return 0;
        }
    }
    return 1;
}

auto day02part2(const aoc::PasswordData& password) -> unsigned int {
    for (const auto& rule : password.rules) {
        const char& c1 = password.password[rule.minimum - 1];
        const char& c2 = password.password[rule.maximum - 1];
        if ((c1 != rule.character && c2 != rule.character) || (c1 == c2)) {
            return false;
        }
    }
    return true;
}

auto main(int argc, char **argv) -> int {
    std::size_t valid = 0;
    aoc::readFromFile<aoc::PasswordData>("day-02-sample.txt", std::back_inserter(passwords));
    for (const auto& pwd : passwords) {
        valid += day02part1(pwd);
    }
    std::cout << "Sample 1 solution: " << valid << "\n";
    valid = 0;
    for (const auto& pwd : passwords) {
        valid += day02part2(pwd);
    }
    std::cout << "Sample 2 solution: " << valid << "\n";


    valid = 0;
    passwords.clear();
    aoc::readFromFile<aoc::PasswordData>("day-02-input.txt", std::back_inserter(passwords));
    for (const auto& pwd : passwords) {
        valid += day02part1(pwd);
    }
    std::cout << "Part 1 solution: " << valid << "\n";
    valid = 0;
    for (const auto& pwd : passwords) {
        valid += day02part2(pwd);
    }
    std::cout << "Part 2 solution: " << valid << "\n";

    passwords.clear();
    // aoc::readFromFile<aoc::PasswordData>("day-02-input.txt", std::back_inserter(passwords));

    return 0;
}
