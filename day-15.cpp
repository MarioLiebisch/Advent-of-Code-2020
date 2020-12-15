#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

#include <fstream>
#include <set>
#include <numeric>

auto day15part1(const char *file) -> std::size_t {
    std::ifstream stream(file);
    std::string line;
    std::getline(stream, line);
    std::map<std::size_t, std::size_t> memory;
    bool preroll = true;
    std::size_t diff = 0;
    std::size_t num;
    for (std::size_t turn = 0; turn < 2020; ++turn) {
        // Still reading off the paper?
        if (preroll) {
            std::size_t o;
            num = std::stoull(line, &o);
            if (o != line.length()) {
                line = line.substr(o + 1);
            }
            else {
                line.clear();
                preroll = false;
            }
        }
        // No, so new number is the previous difference (or 0)
        else {
            num = diff;
        }
        // Look for a previous appearance
        auto& a = memory.find(num);
        // Never seen
        if (a == memory.end()) {
            diff = 0;
            memory.insert(std::make_pair(num, turn));
        }
        // Found!
        else {
            diff = turn - a->second;
            a->second = turn;
        }
    }
    return num;
}


auto day15part2(const char *file) -> std::size_t {
    std::ifstream stream(file);
    std::string line;
    std::getline(stream, line);
    std::map<std::size_t, std::size_t> memory;
    bool preroll = true;
    std::size_t diff = 0;
    std::size_t num;
    // There's probably a smarter way to do this fast, but... let's just wait (got work to do)...
    for (std::size_t turn = 0; turn < 30000000; ++turn) {
        // Still reading off the paper?
        if (preroll) {
            std::size_t o;
            num = std::stoull(line, &o);
            if (o != line.length()) {
                line = line.substr(o + 1);
            }
            else {
                line.clear();
                preroll = false;
            }
        }
        // No, so new number is the previous difference (or 0)
        else {
            num = diff;
        }
        // Look for a previous appearance
        auto& a = memory.find(num);
        // Never seen
        if (a == memory.end()) {
            diff = 0;
            memory.insert(std::make_pair(num, turn));
        }
        // Found!
        else {
            diff = turn - a->second;
            a->second = turn;
        }
    }
    return num;
}

auto main(int argc, char** argv) -> int {

    std::cout << "Sample 1 solution: " << day15part1("day-15-sample.txt") << "\n";
    std::cout << "Sample 2 solution: " << day15part2("day-15-sample.txt") << "\n";

    std::cout << "Part 1 solution: " << day15part1("day-15-input.txt") << "\n";
    std::cout << "Part 2 solution: " << day15part2("day-15-input.txt") << "\n";

    return 0;
}
