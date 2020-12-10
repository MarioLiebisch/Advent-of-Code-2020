#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "utility.h"


auto day10part1(const std::vector<std::size_t>& joltages) -> std::size_t {
    std::size_t previous = 0;
    std::unordered_map<std::size_t, std::size_t> jumps;
    for (const auto& j : joltages) {
        ++jumps[j - previous];
        previous = j;
    }
    return jumps[1] * jumps[3];
}

auto travel(const std::unordered_map<std::size_t, std::set<std::size_t>>& routes, std::size_t location, std::size_t target, std::unordered_map<std::size_t, std::size_t>& counts) -> std::size_t {
    if (location == target) {
        return 1;
    }
    const auto& a = counts.find(location);
    if (a != counts.end()) {
        return a->second;
    }
    else {
        std::size_t count = 0;
        for (const auto& r : routes.at(location)) {
            count += travel(routes, r, target, counts);
        }
        counts.insert(std::make_pair(location, count));
        return count;
    }
}

auto day10part2(const std::vector<std::size_t>& joltages) -> std::size_t {
    std::unordered_map<std::size_t, std::set<std::size_t>> routes;
    const std::size_t l = joltages.size();
    for (std::size_t i = 0; i < l; ++i) {
        // Only consider next 3 (can't be more jumps)
        for (std::size_t j = 1; j < 4 && i + j < l; ++j) {
            const std::size_t j1 = joltages[i];
            const std::size_t j2 = joltages[i + j];
            const std::size_t jump = j2 - j1;
            // Only allow valid jumps
            if (jump <= 3) {
                // Save this "route"
                routes[j1].insert(j2);
            }
        }
    }

    std::unordered_map<std::size_t, std::size_t> counts;
    travel(routes, joltages.front(), joltages.back(), counts);

    return counts[joltages.front()];
}

auto main(int argc, char **argv) -> int {
    std::vector<std::size_t> joltages;

    aoc::readFromFile<std::size_t>("day-10-sample.txt", std::back_inserter(joltages));
    joltages.push_back(0); // Outlet
    std::sort(joltages.begin(), joltages.end());
    joltages.push_back(joltages.back() + 3); // Built-in
    std::cout << "Part 1 sample: " << day10part1(joltages) << "\n";
    std::cout << "Part 2 sample: " << day10part2(joltages) << "\n";

    joltages.clear();
    aoc::readFromFile<std::size_t>("day-10-input.txt", std::back_inserter(joltages));
    joltages.push_back(0); // Outlet
    std::sort(joltages.begin(), joltages.end());
    joltages.push_back(joltages.back() + 3); // Built-in
    std::cout << "Part 1 solution: " << day10part1(joltages) << "\n";
    std::cout << "Part 2 solution: " << day10part2(joltages) << "\n";

    return 0;
}
