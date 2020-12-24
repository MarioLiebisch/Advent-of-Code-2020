#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <unordered_map>

/*

   Hex Grid Layout (skewed grid)

   \   / \   / \   / \   / \   /
    \ /   \ /   \ /   \ /   \ /
     |     |     |     |     |
     |-1,-1| 0,-1| 1,-1| 2,-1|
     |     |     |     |     |
    / \   / \   / \   / \   / \
   /   \ /   \ /   \ /   \ /   \
  |     |     |     |     |     |
  |-2,0 |-1,0 | 0,0 | 1,0 | 2,0 |
  |     |     |     |     |     |
   \   / \   / \   / \   / \   /
    \ /   \ /   \ /   \ /   \ /
     |     |     |     |     |
     |-2,1 |-1,1 | 0,1 | 1,1 |
     |     |     |     |     |
    / \   / \   / \   / \   / \
   /   \ /   \ /   \ /   \ /   \

*/

using Tile = bool;
auto day24part1(const char* file, const bool debug = false) -> std::size_t {
    std::string tmp;
    std::ifstream stream(file);

    std::unordered_map<long, std::unordered_map<long, Tile>> tiles;

    while (std::getline(stream, tmp)) {
        long x = 0, y = 0;
        while (!tmp.empty()) {
            const char c = tmp[0];
            tmp.erase(tmp.begin());
            switch (c) {
            case 'e': // one to the right
                ++x;
                break;
            case 'w': // one to the left
                --x;
                break;
            case 's': // down and left/right
            {
                ++y;
                const char c = tmp[0];
                tmp.erase(tmp.begin());
                switch (c) {
                case 'e':
                    // Do nothing
                    break;
                case 'w':
                    --x;
                    break;
                default:
                    // Shouldn't happen
                    assert(false);
                    break;
                }
                break;
            }
            case 'n': // up and left/right
            {
                --y;
                const char c = tmp[0];
                tmp.erase(tmp.begin());
                switch (c) {
                case 'e':
                    ++x;
                    break;
                case 'w':
                    // Do nothing
                    break;
                default:
                    // Shouldn't happen
                    assert(false);
                    break;
                }
                break;
            }
            default:
                // Shouldn't happen
                assert(false);
                break;
            }
        }
        // Tile identified, flip it:
        tiles[x][y] = !tiles[x][y];
    }

    std::size_t count = 0;
    for (const auto& col : tiles) {
        for (const auto& t : col.second) {
            if (t.second) {
                ++count;
            }
        }
    }

    return count;
}

auto day24part2(const char* file, const bool debug = false) -> std::size_t {
    std::string tmp;
    std::ifstream stream(file);

    std::unordered_map<long, std::unordered_map<long, Tile>> tiles, changes;

    while (std::getline(stream, tmp)) {
        long x = 0, y = 0;
        while (!tmp.empty()) {
            const char c = tmp[0];
            tmp.erase(tmp.begin());
            switch (c) {
            case 'e': // one to the right
                ++x;
                break;
            case 'w': // one to the left
                --x;
                break;
            case 's': // one to the left/right and down
            {
                ++y;
                const char c = tmp[0];
                tmp.erase(tmp.begin());
                switch (c) {
                case 'e':
                    // Do nothing
                    break;
                case 'w':
                    --x;
                    break;
                default:
                    // Shouldn't happen
                    assert(false);
                    break;
                }
                break;
            }
            case 'n': // one to the left/right only
            {
                --y;
                const char c = tmp[0];
                tmp.erase(tmp.begin());
                switch (c) {
                case 'e':
                    ++x;
                    break;
                case 'w':
                    // Do nothing
                    break;
                default:
                    // Shouldn't happen
                    assert(false);
                    break;
                }
                break;
            }
            default:
                // Shouldn't happen
                assert(false);
                break;
            }
        }
        // Tile identified, flip it:
        tiles[x][y] = !tiles[x][y];
    }

    for (std::size_t d = 1; d <= 100; ++d) {
        changes.clear();
        long minx = 0, maxx = 0, miny = 0, maxy = 0;

        // Find the smallest/biggest coordinates of black tiles
        for (const auto& col : tiles) {
            for (const auto& t : col.second) {
                // Only consider black tiles
                if (t.second) {
                    if (col.first < minx) {
                        minx = col.first;
                    }
                    if (col.first > maxx) {
                        maxx = col.first;
                    }
                    if (t.first < miny) {
                        miny = t.first;
                    }
                    if (t.first > maxy) {
                        maxy = t.first;
                    }
                }
            }
        }

        // Determine all changes
        for (long x = minx - 1; x <= maxx + 1; ++x) {
            for (long y = miny - 1; y <= maxy + 1; ++y) {
                std::size_t neighbours = 0;
                // Look for neighbours
                for (long dx = -1; dx <= 1; ++dx) {
                    for (long dy = -1; dy <= 1; ++dy) {
                        // Don't count offsets -1/-1, +1/+2, and 0/0
                        // since they're not adjacent (or the current tile)
                        if ((dx != dy) && tiles[x + dx][y + dy]) {
                            ++neighbours;
                        }
                    }
                }
                // See if anything changes
                if (tiles[x][y]) {
                    changes[x][y] = (neighbours > 0 && neighbours < 3);
                }
                else {
                    changes[x][y] = neighbours == 2;
                }
            }
        }

        // Apply the changes
        for (const auto& col : changes) {
            for (const auto& t : col.second) {
                tiles[col.first][t.first] = t.second;
            }
        }

        if (debug) {
            std::size_t count = 0;
            for (const auto& col : tiles) {
                for (const auto& t : col.second) {
                    if (t.second) {
                        ++count;
                    }
                }
            }
            std::cout << "Day " << d << ": " << count << "\n";
        }
    }

    std::size_t count = 0;
    for (const auto& col : tiles) {
        for (const auto& t : col.second) {
            if (t.second) {
                ++count;
            }
        }
    }

    return count;
}

auto main(int argc, char** argv) -> int {
    const std::size_t r1 = day24part1("day-24-sample.txt", true);
    std::cout << "Sample 1 solution: " << r1 << "\n";
    const std::size_t r2 = day24part2("day-24-sample.txt");
    std::cout << "Sample 2 solution: " << r2 << "\n";

    const std::size_t r3 = day24part1("day-24-input.txt");
    std::cout << "Part 1 solution: " << r3 << "\n";
    const std::size_t r4 = day24part2("day-24-input.txt");
    std::cout << "Part 2 solution: " << r4 << "\n";
    return 0;
}
