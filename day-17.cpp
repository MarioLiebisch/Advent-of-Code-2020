#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

#include <fstream>
#include <set>
#include <numeric>

enum class Cube : bool {
    inactive = false,
    active
};

auto day17part1(const char *file) -> std::size_t {
    std::map<long, std::map<long, std::map<long, Cube>>> world, changes;
    std::ifstream stream(file);

    long y = 0;
    {
        std::string line;
        while (std::getline(stream, line)) {
            for (long x = 0; x < line.size(); ++x) {
                world[0][y][x] = static_cast<Cube>(line[x] == '#');
            }
            y++;
        }
    }

#if 0
    // Print the world
    std::cout << "Before any cycles:\n\n";
    for (const auto& plane : world) {
        std::cout << "z=" << plane.first << "\n";
        for (const auto& line : plane.second) {
            for (const auto& cube : line.second) {
                std::cout << (cube.second == Cube::active ? "#" : ".");
            }
            std::cout << "\n";
        }
        std::cout << "\n\n";
    }
#endif

    long minz = -1;
    long maxz = 1;
    long miny = -1;
    long maxy = world[0].size();
    long minx = -1;
    long maxx = maxy;

    for (std::size_t cycle = 0; cycle < 6; ++cycle) {
        std::cout << "[C " << (cycle + 1) << "] ";
        changes.clear();
        // Simulate this cycle and get the world changes

        for (long z = minz; z <= maxz; ++z) {
            auto& a = world[z];
            for (long y = miny; y <= maxy; ++y) {
                auto& b = a[y];
                for (long x = minx; x <= maxx; ++x) {
                    std::size_t neighbours = 0;
                    for (long tz = z - 1; tz <= z + 1; ++tz) {
                        auto& d = world[tz];
                        for (long ty = y - 1; ty <= y + 1; ++ty) {
                            auto& e = d[ty];
                            for (long tx = x - 1; tx <= x + 1; ++tx) {
                                if (tx == x && ty == y && tz == z) {
                                    continue;
                                }
                                if (e[tx] == Cube::active) {
                                    ++neighbours;
                                }
                            }
                        }
                    }

                    if (b[x] == Cube::active) {
                        if (neighbours != 2 && neighbours != 3) {
                            changes[z][y][x] = Cube::inactive;
                        }
                    }
                    else if (neighbours == 3) {
                        changes[z][y][x] = Cube::active;
                        if (z <= minz) {
                            minz = z - 1;
                        }
                        if (z >= maxz) {
                            maxz = z + 1;
                        }
                        if (y <= miny) {
                            miny = y - 1;
                        }
                        if (y >= maxy) {
                            maxy = y + 1;
                        }
                        if (x <= minx) {
                            minx = x - 1;
                        }
                        if (x >= maxx) {
                            maxx = x + 1;
                        }
                    }
                }
            }
        }

        // Now apply the changes
        for (const auto& plane : changes) {
            auto& wplane = world[plane.first];
            for (const auto& line : plane.second) {
                auto& wline = wplane[line.first];
                for (const auto& cube : line.second) {
                    wline[cube.first] = cube.second;
                }
            }
        }

#if 0
        // Print the world
        std::cout << "After " << (cycle + 1) << " cycles:\n\n";

        for (long z = minz; z <= maxz; ++z) {
            std::cout << "z=" << z << "\n";
            for (long y = miny; y <= maxy; ++y) {
                for (long x = minx; x <= maxx; ++x) {
                    std::cout << (world[z][y][x] == Cube::active ? "#" : ".");
                }
                std::cout << "\n";
            }
            std::cout << "\n\n";
        }
#endif
    }

    std::size_t num_active = 0;
    for (const auto& plane : world) {
        for (const auto& line : plane.second) {
            for (const auto& cube : line.second) {
                if (cube.second == Cube::active) {
                    ++num_active;
                }
            }
        }
    }
    return num_active;
}


auto day17part2(const char *file) -> std::size_t {
    std::map<long, std::map<long, std::map<long, std::map<long, Cube>>>> world, changes;
    std::ifstream stream(file);

    long y = 0;
    {
        std::string line;
        while (std::getline(stream, line)) {
            for (long x = 0; x < line.size(); ++x) {
                world[0][0][y][x] = static_cast<Cube>(line[x] == '#');
            }
            y++;
        }
    }

#if 0
    // Print the world
    std::cout << "Before any cycles:\n\n";
    for (const auto& plane : world) {
        std::cout << "z=" << plane.first << "\n";
        for (const auto& line : plane.second) {
            for (const auto& cube : line.second) {
                std::cout << (cube.second == Cube::active ? "#" : ".");
            }
            std::cout << "\n";
        }
        std::cout << "\n\n";
    }
#endif

    long minw = -1;
    long maxw = -1;
    long minz = -1;
    long maxz = 1;
    long miny = -1;
    long maxy = world[0][0].size();
    long minx = -1;
    long maxx = maxy;

    for (std::size_t cycle = 0; cycle < 6; ++cycle) {
        std::cout << "[C " << (cycle + 1) << "] ";

        changes.clear();
        // Simulate this cycle and get the world changes

        for (long w = minw; w <= maxw; ++w) {
            auto& a = world[w];
            for (long z = minz; z <= maxz; ++z) {
                auto& b = a[z];
                for (long y = miny; y <= maxy; ++y) {
                    auto& c = b[y];
                    for (long x = minx; x <= maxx; ++x) {
                        std::size_t neighbours = 0;
                        for (long tw = w - 1; tw <= w + 1; ++tw) {
                            auto& e = world[tw];
                            for (long tz = z - 1; tz <= z + 1; ++tz) {
                                auto& f = e[tz];
                                for (long ty = y - 1; ty <= y + 1; ++ty) {
                                    auto& g = f[ty];
                                    for (long tx = x - 1; tx <= x + 1; ++tx) {
                                        if (tx == x && ty == y && tz == z && tw == w) {
                                            continue;
                                        }
                                        if (g[tx] == Cube::active) {
                                            ++neighbours;
                                        }
                                    }
                                }
                            }
                        }

                        if (c[x] == Cube::active) {
                            if (neighbours != 2 && neighbours != 3) {
                                changes[w][z][y][x] = Cube::inactive;
                            }
                        }
                        else if (neighbours == 3) {
                            changes[w][z][y][x] = Cube::active;
                            if (w <= minw) {
                                minw = w - 1;
                            }
                            if (w >= maxw) {
                                maxw = w + 1;
                            }
                            if (z <= minz) {
                                minz = z - 1;
                            }
                            if (z >= maxz) {
                                maxz = z + 1;
                            }
                            if (y <= miny) {
                                miny = y - 1;
                            }
                            if (y >= maxy) {
                                maxy = y + 1;
                            }
                            if (x <= minx) {
                                minx = x - 1;
                            }
                            if (x >= maxx) {
                                maxx = x + 1;
                            }
                        }
                    }
                }
            }
        }

        // Now apply the changes
        for (const auto& whatever : changes) {
            auto& wwhatever = world[whatever.first];
            for (const auto& plane : whatever.second) {
                auto& wplane = wwhatever[plane.first];
                for (const auto& line : plane.second) {
                    auto& wline = wplane[line.first];
                    for (const auto& cube : line.second) {
                        wline[cube.first] = cube.second;
                    }
                }
            }
        }

#if 0
        // Print the world
        std::cout << "After " << (cycle + 1) << " cycles:\n\n";

        for (long w = minw; w <= maxw; ++w) {
            for (long z = minz; z <= maxz; ++z) {
                std::cout << "z=" << z << ", w=" << w << "\n";
                for (long y = miny; y <= maxy; ++y) {
                    for (long x = minx; x <= maxx; ++x) {
                        std::cout << (world[w][z][y][x] == Cube::active ? "#" : ".");
                    }
                    std::cout << "\n";
                }
                std::cout << "\n\n";
            }
        }
#endif
    }

    std::size_t num_active = 0;
    for (const auto& whatever : world) {
        for (const auto& plane : whatever.second) {
            for (const auto& line : plane.second) {
                for (const auto& cube : line.second) {
                    if (cube.second == Cube::active) {
                        ++num_active;
                    }
                }
            }
        }
    }
    return num_active;
}

auto main(int argc, char** argv) -> int {
    const std::size_t r1 = day17part1("day-17-sample.txt");
    std::cout << "Sample 1 solution: " << r1 << "\n";
    const std::size_t r2 = day17part2("day-17-sample.txt");
    std::cout << "Sample 2 solution: " << r2 << "\n";

    const std::size_t r3 = day17part1("day-17-input.txt");
    std::cout << "Part 1 solution: " << r3 << "\n";
    const std::size_t r4 = day17part2("day-17-input.txt");
    std::cout << "Part 2 solution: " << r4 << "\n";
    return 0;
}
