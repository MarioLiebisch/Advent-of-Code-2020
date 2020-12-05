#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

aoc::MapData map;

const char TREE = '#';

auto day03part1(const aoc::MapData &map, signed int x_step = 3, signed int y_step = 1) -> std::size_t {
    std::size_t trees = 0;
    std::size_t x = 0, y = 0;
    const std::size_t width = map.at(0).size();
    while (y < map.size()) {
        if (TREE == map.at(y).at(x)) {
            ++trees;
        }
        y += y_step;
        x = (x + x_step) % width;
    }
    return trees;
}

auto day03part2(const aoc::MapData& map) -> std::size_t {
    return day03part1(map, 1, 1) * day03part1(map, 3, 1) * day03part1(map, 5, 1) * day03part1(map, 7, 1) * day03part1(map, 1, 2);
}

auto main(int argc, char **argv) -> int {
    aoc::readFromFile("day-03-sample.txt", map);
    std::cout << "Sample 1 solution: " << day03part1(map) << "\n";
    std::cout << "Sample 2 solution: " << day03part2(map) << "\n";

    map.clear();
    aoc::readFromFile("day-03-input.txt", map);
    std::cout << "Part 1 solution: " << day03part1(map) << "\n";
    std::cout << "Part 2 solution: " << day03part2(map) << "\n";

    return 0;
}
