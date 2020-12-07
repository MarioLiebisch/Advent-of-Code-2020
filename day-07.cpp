#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

#include "utility.h"

auto day07part1(const std::vector<aoc::PackagingRule>& rules) -> std::size_t {
    // Build a list of what might be where
    std::unordered_map<std::string, std::set<std::string>> contained;

    for (const auto& r : rules) {
        for (const auto& c : r.second) {
            // c.first = child/content
            // r.first = parent/container
            contained[c.first].insert(r.first);
        }
    }

    // Get all containers directly containing the bag
    std::set<std::string> res = contained["shiny gold bag"];

    // Now keep looking for new possible containers until we find no more
    std::size_t c;
    do {
        c = res.size();
        for (const auto& b : res) {
            res.insert(contained[b].begin(), contained[b].end());
        }
    } while (c != res.size());

    return res.size();
}

auto day07part2(const std::vector<aoc::PackagingRule>& rules) -> std::size_t {
    std::unordered_map<std::string, std::size_t> unprocessed;
    std::unordered_map<std::string, std::size_t> next;
    // Starting at -1 since we don't count the shiny gold bag!
    std::size_t count = -1;
    // One shiny gold bag to be processed
    unprocessed.insert(std::make_pair("shiny gold bag", 1));

    while (unprocessed.size()) {
        next.clear();
        for (const auto& r : rules) {
            const auto& a = unprocessed.find(r.first);
            // Is this bag type unprocessed?
            if (a != unprocessed.end()) {
                // How many times?
                const auto& n = a->second;
                count += n;
                // Get all contents and add them for next round
                for (const auto& c : r.second) {
                    next[c.first] += c.second * n;
                }
                // unprocessed.erase(a);
            }
        }
        unprocessed = next;
    }
    
    return count;
}

auto main(int argc, char **argv) -> int {
    std::vector<aoc::PackagingRule> rules;
    aoc::readFromFile<aoc::PackagingRule>("day-07-sample.txt", std::back_inserter(rules));
    std::cout << "Part 1 sample: " << day07part1(rules) << "\n";
    rules.clear();
    aoc::readFromFile<aoc::PackagingRule>("day-07-sample.txt", std::back_inserter(rules));
    std::cout << "Part 2 sample: " << day07part2(rules) << "\n";

    rules.clear();
    aoc::readFromFile<aoc::PackagingRule>("day-07-input.txt", std::back_inserter(rules));
    std::cout << "Part 1 solution: " << day07part1(rules) << "\n";
    rules.clear();
    aoc::readFromFile<aoc::PackagingRule>("day-07-input.txt", std::back_inserter(rules));
    std::cout << "Part 2 solution: " << day07part2(rules) << "\n";

    return 0;
}
