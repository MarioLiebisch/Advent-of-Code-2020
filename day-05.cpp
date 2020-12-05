#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

#include "utility.h"

auto day05part1(const std::vector<aoc::BoardingPass>& passes) -> unsigned int {
    unsigned int highest = 0;
    for (const auto& pass : passes) {
        if (pass.id > highest) {
            highest = pass.id;
        }
    }
    return highest;
}

auto day05part2(std::vector<aoc::BoardingPass>& passes) -> std::size_t {
    std::size_t last = 0;

    // Sort them all by ID
    std::sort(passes.begin(), passes.end());

    // Skip empty non-existant seats by assuming the last id is that of the first one
    last = passes[0].id;
    for (const auto& pass : passes) {
        // Did we skip one? My seat!
        if (last + 1 < pass.id) {
            return last + 1;
        }
        last = pass.id;
    }
    // Shouldn't happen
    return 0;
}

auto main(int argc, char **argv) -> int {
    std::vector<aoc::BoardingPass> passes;
    aoc::BoardingPass s1test = aoc::convert<aoc::BoardingPass>("FBFBBFFRLR");
    std::cout << "Sample 1 solution: row " << s1test.row << ", col " << s1test.col << "\n";

    aoc::readFromFile<aoc::BoardingPass>("day-05-input.txt", std::back_inserter(passes));
    std::cout << "Part 1 solution: " << day05part1(passes) << "\n";
    std::cout << "Part 2 solution: " << day05part2(passes) << "\n";

    return 0;
}
