#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

aoc::MapData map;

const char TILE_EMPTY = 'L';
const char TILE_OCCUPIED = '#';
const char TILE_FLOOR = '.';

auto day11part1(const aoc::MapData& map) -> std::size_t {
    auto work(map);
    const int h = static_cast<int>(map.size());
    const int w = static_cast<int>(map.at(0).size());

    aoc::MapData changes;
    do {
        changes.clear();
        for (int y = 0; y < h; ++y) {
            const auto& row = work.at(y);
            for (int x = 0; x < w; ++x) {
                const char c = row.at(x);

                if (TILE_FLOOR == c) {
                    continue;
                }

                std::size_t count = 0;
                for (int ty = std::max(0, y - 1); ty < h && ty < y + 2; ++ty) {
                    const auto& crow = work.at(ty);
                    for (int tx = std::max(0, x - 1); tx < w && tx < x + 2; ++tx) {
                        // Don't count the current position
                        if (ty == y && tx == x) {
                            continue;
                        }
                        if (TILE_OCCUPIED == crow.at(tx)) {
                            ++count;
                        }
                    }
                }

                if (TILE_EMPTY == c && 0 == count) {
                    changes[y][x] = TILE_OCCUPIED;
                }
                else if (TILE_OCCUPIED == c && 4 <= count) {
                    changes[y][x] = TILE_EMPTY;
                }
            }
        }
        for (const auto& crow : changes) {
            auto& row = work.at(crow.first);
            for (const auto& t : crow.second) {
                row[t.first] = t.second;
            }
        }

        // Debug drawing; slow for obvious reasons!
        /* std::cout << '\n';
        for (int y = 0; y < h; ++y) {
            const auto& row = work.at(y);
            for (const auto &c : row) {
                std::cout << c.second;
            }
            std::cout << '\n';
        }
        std::cout << '\n'; */
    } while (!changes.empty());

    std::size_t occupied = 0;
    for (int y = 0; y < h; ++y) {
        const auto& row = work.at(y);
        for (int x = 0; x < w; ++x) {
            const char c = row.at(x);
            if (TILE_OCCUPIED == c) {
                ++occupied;
            }
        }
    }

    return occupied;
}

auto day11part2(const aoc::MapData& map) -> std::size_t {
    auto work(map);
    const int h = static_cast<int>(map.size());
    const int w = static_cast<int>(map.at(0).size());

    aoc::MapData changes;
    do {
        changes.clear();
        for (int y = 0; y < h; ++y) {
            const auto& row = work.at(y);
            auto& nrow = work.at(y);
            for (int x = 0; x < w; ++x) {
                const char c = row.at(x);

                if (TILE_FLOOR == c) {
                    continue;
                }

                std::size_t count = 0;

                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (0 == dy && 0 == dx) {
                            continue;
                        }

                        char seen = 0;
                        for (int ty = y + dy, tx = x + dx; ty >= 0 && ty < h && tx >= 0 && tx < w; ty += dy, tx += dx) {
                            const char c = work.at(ty).at(tx);
                            if (TILE_EMPTY == c) {
                                break;
                            }
                            if (TILE_OCCUPIED == c) {
                                ++count;
                                break;
                            }
                        }
                    }
                }

                if (TILE_EMPTY == c && 0 == count) {
                    changes[y][x] = TILE_OCCUPIED;
                }
                else if (TILE_OCCUPIED == c && 5 <= count) {
                    changes[y][x] = TILE_EMPTY;
                }
            }
        }
        for (const auto& crow : changes) {
            auto& row = work.at(crow.first);
            for (const auto& t : crow.second) {
                row[t.first] = t.second;
            }
        }

        // Debug drawing; slow for obvious reasons!
        /* std::cout << '\n';
        for (int y = 0; y < h; ++y) {
            const auto& row = work.at(y);
            for (const auto &c : row) {
                std::cout << c.second;
            }
            std::cout << '\n';
        }
        std::cout << '\n'; */
    } while (!changes.empty());

    std::size_t occupied = 0;
    for (int y = 0; y < h; ++y) {
        const auto& row = work.at(y);
        for (int x = 0; x < w; ++x) {
            const char c = row.at(x);
            if (TILE_OCCUPIED == c) {
                ++occupied;
            }
        }
    }

    return occupied;
}

auto main(int argc, char** argv) -> int {
    aoc::readFromFile("day-11-sample.txt", map);
    std::cout << "Sample 1 solution: " << day11part1(map) << "\n";
    std::cout << "Sample 2 solution: " << day11part2(map) << "\n";

    map.clear();
    aoc::readFromFile("day-11-input.txt", map);
    std::cout << "Part 1 solution: " << day11part1(map) << "\n";
    std::cout << "Part 2 solution: " << day11part2(map) << "\n";

    return 0;
}
