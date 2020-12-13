#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

#include <fstream>
#include <set>
#include <numeric>

auto day13part1(const char *file) -> std::size_t {
    std::set<std::size_t> lines;

    std::ifstream sfile(file);
    std::string tmp;
    std::getline(sfile, tmp);
    const std::size_t start = std::stol(tmp);
    std::getline(sfile, tmp);
    do {
        if (tmp[0] == 'x' || tmp[0] == ',') {
            tmp = tmp.substr(1);
        }
        else {
            std::size_t o;
            const std::size_t i = std::stol(tmp, &o);
            tmp = tmp.substr(o);
            lines.insert(i);
        }
    } while (!tmp.empty());

    std::size_t earliest_time = -1, earliest_id = 0;

    for (const auto& l : lines) {
        const std::size_t departure = static_cast<std::size_t>(std::ceil(start / static_cast<double>(l)) * l);
        if (departure < earliest_time) {
            earliest_time = departure;
            earliest_id = l;
        }
    }

    return (earliest_time - start) * earliest_id;
}


auto day13part2(const char *file) -> std::size_t {
    std::vector<std::size_t> lines;

    std::ifstream sfile(file);
    std::string tmp;
    std::getline(sfile, tmp);
    std::getline(sfile, tmp);
    do {
        if (tmp[0] == 'x') {
            lines.push_back(0);
            tmp = tmp.substr(1);
        }
        else if (tmp[0] == ',') {
            tmp = tmp.substr(1);
        }
        else {
            std::size_t o;
            const std::size_t i = std::stol(tmp, &o);
            tmp = tmp.substr(o);
            lines.push_back(i);
        }
    } while (!tmp.empty());

    std::size_t n = 1;
    const std::size_t l = lines.size();
    std::size_t time = 0;
    std::size_t interval = lines[0];
    for (int i = 1; i < l; ++i) {
        // Skip unused lines/positions
        if (lines[i] == 0) {
            continue;
        }

        // Current line
        const std::size_t &c = lines[i];

        // Add the interval until the time difference matches the position (i.e. equal modulo 0)
        for (; ((time += interval) + i) % c != 0;);

        // Ensure all future intervals are multiples of this line's time
        interval *= c;
    }

    return time;
}

auto main(int argc, char** argv) -> int {

    std::cout << "Sample 1 solution: " << day13part1("day-13-sample.txt") << "\n";
    std::cout << "Sample 2 solution: " << day13part2("day-13-sample.txt") << "\n";

    std::cout << "Part 1 solution: " << day13part1("day-13-input.txt") << "\n";
    std::cout << "Part 2 solution: " << day13part2("day-13-input.txt") << "\n";

    return 0;
}
