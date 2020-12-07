#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

#include "utility.h"

const char TREE = '#';

const std::unordered_map<std::string, bool(*)(const std::string&)> validators = {
    {"byr", [](const std::string& input) {
        const std::size_t y = std::stoul(input);
        return y >= 1920 && y <= 2002;
    }},
    {"iyr", [](const std::string& input) {
        const std::size_t y = std::stoul(input);
        return y >= 2010 && y <= 2020;
    }},
    {"eyr", [](const std::string& input) {
        const std::size_t y = std::stoul(input);
        return y >= 2020 && y <= 2030;
    }},
    {"hgt", [](const std::string& input) {
        const std::size_t l = input.length();
        const std::size_t s = std::stoul(input);
        if (l < 3) {
            return false;
        }
        if (input[l - 2] == 'c' && input[l - 1] == 'm') {
            return s >= 150 && s <= 193;
        }
        else if (input[l - 2] == 'i' && input[l - 1] == 'n') {
            return s >= 59 && s <= 76;
        }
        return false;
    }},
    {"hcl", [](const std::string& input) {
        if (input[0] != '#' || input.length() != 7) {
            return false;
        }
        return input.find_first_not_of("0123456789abcdef", 1) == std::string::npos;
    }},
    {"ecl", [](const std::string& input) {
        const std::set<std::string> colors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
        return colors.find(input) != colors.end();
    }},
    {"pid", [](const std::string& input) {
        return input.length() == 9 && input.find_first_not_of("0123456789") == std::string::npos;
    }}
};

auto day04part1(const std::vector<aoc::Passport> &pps) -> std::size_t {
    std::size_t valid = 0;
    for (const auto& pp : pps) {
        bool this_valid = true;
        for (const auto& v : validators) {
            if (pp.find(v.first) == pp.end()) {
                this_valid = false;
                break;
            }
        }
        if (this_valid) {
            ++valid;
        }
    }
    return valid;
}

auto day04part2(const std::vector<aoc::Passport>& pps) -> std::size_t {
    std::size_t valid = 0;
    for (const auto& pp : pps) {
        bool this_valid = true;
        for (const auto& v : validators) {
            const auto& kv = pp.find(v.first);
            if (kv == pp.end() || !v.second(kv->second)) {
                this_valid = false;
                break;
            }
        }
        if (this_valid) {
            ++valid;
        }
    }
    return valid;
}

auto main(int argc, char **argv) -> int {
    std::vector<aoc::Passport> passports;

    aoc::readFromFile("day-04-sample.txt", std::back_inserter(passports));
    std::cout << "Sample 1 solution: " << day04part1(passports) << "\n";
    std::cout << "Sample 2 solution: " << day04part2(passports) << "\n";

    passports.clear();
    aoc::readFromFile("day-04-input.txt", std::back_inserter(passports));
    std::cout << "Part 1 solution: " << day04part1(passports) << "\n";
    std::cout << "Part 2 solution: " << day04part2(passports) << "\n";

    return 0;
}
