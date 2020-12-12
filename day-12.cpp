#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

auto day12part1(const std::vector<aoc::NavInstruction>& instructions) -> std::size_t {
    aoc::Position ship;
    for (const auto& i : instructions) {
        switch (i.type) {
        case 'N':
            ship.y -= i.value;
            break;
        case 'S':
            ship.y += i.value;
            break;
        case 'E':
            ship.x += i.value;
            break;
        case 'W':
            ship.x -= i.value;
            break;
        case 'L':
            ship.orientation = static_cast<aoc::Position::Orientation>(static_cast<unsigned char>(static_cast<int>(ship.orientation) - i.value / 90) % static_cast<int>(aoc::Position::Orientation::COUNT));
            break;
        case 'R':
            ship.orientation = static_cast<aoc::Position::Orientation>(static_cast<unsigned char>(static_cast<int>(ship.orientation) + i.value / 90) % static_cast<int>(aoc::Position::Orientation::COUNT));
            break;
        case 'F':
            switch (ship.orientation) {
            case aoc::Position::Orientation::NORTH:
                ship.y -= i.value;
                break;
            case aoc::Position::Orientation::SOUTH:
                ship.y += i.value;
                break;
            case aoc::Position::Orientation::EAST:
                ship.x += i.value;
                break;
            case aoc::Position::Orientation::WEST:
                ship.x -= i.value;
                break;
            }
            break;
        }
    }
    return static_cast<std::size_t>(std::abs(ship.x)) + static_cast<std::size_t>(std::abs(ship.y));
}

auto day12part2(const std::vector<aoc::NavInstruction>& instructions) -> std::size_t {
    aoc::Position ship, waypoint;
    waypoint.x = 10;
    waypoint.y = -1;
    for (const auto& i : instructions) {
        switch (i.type) {
        case 'N':
            waypoint.y -= i.value;
            break;
        case 'S':
            waypoint.y += i.value;
            break;
        case 'E':
            waypoint.x += i.value;
            break;
        case 'W':
            waypoint.x -= i.value;
            break;
        case 'L':
            for (int j = 0; j < i.value / 90; ++j) {
                aoc::Position tmp = waypoint;
                waypoint.x = tmp.y;
                waypoint.y = -tmp.x;
            }
            break;
        case 'R':
            for (int j = 0; j < i.value / 90; ++j) {
                aoc::Position tmp = waypoint;
                waypoint.x = -tmp.y;
                waypoint.y = tmp.x;
            }
            break;
        case 'F':
            ship.x += waypoint.x * i.value;
            ship.y += waypoint.y * i.value;
            break;
        }
    }
    return static_cast<std::size_t>(std::abs(ship.x)) + static_cast<std::size_t>(std::abs(ship.y));
}

auto main(int argc, char** argv) -> int {
    std::vector<aoc::NavInstruction> instructions;

    aoc::readFromFile<aoc::NavInstruction>("day-12-sample.txt", std::back_inserter(instructions));
    std::cout << "Sample 1 solution: " << day12part1(instructions) << "\n";
    std::cout << "Sample 2 solution: " << day12part2(instructions) << "\n";

    instructions.clear();
    aoc::readFromFile<aoc::NavInstruction>("day-12-input.txt", std::back_inserter(instructions));
    std::cout << "Part 1 solution: " << day12part1(instructions) << "\n";
    std::cout << "Part 2 solution: " << day12part2(instructions) << "\n";

    return 0;
}
