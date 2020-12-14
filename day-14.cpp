#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

#include <fstream>
#include <set>
#include <numeric>

auto day14part1(const char *file) -> std::size_t {
    std::ifstream stream(file);
    std::string line;
    std::unordered_map<unsigned char, bool> mask;
    std::unordered_map<std::size_t, std::size_t> memory;
    while (std::getline(stream, line)) {
        const std::size_t l = line.size();
        if (line.substr(0, 4) == "mask") {
            mask.clear();
            for (unsigned char i = 0; i < 36; ++i) {
                switch (line[l - 1 - i]) {
                case '0':
                    mask[i] = false;
                    break;
                case '1':
                    mask[i] = true;
                    break;
                }
            }
        }
        else { // assume it's "mem[.]"
            const std::size_t address = std::stoull(line.substr(4));
            std::size_t value = std::stoull(line.substr(line.find('=') + 2));
            for (const auto& m : mask) {
                if (m.second) {
                    // Set the m.first-th bit to 1
                    value |= 1ull << m.first;
                }
                else {
                    // Set the m.first-th bit to 0
                    value &= ~(1ull << m.first);
                }
            }
            memory[address] = value;
        }
    }
    std::size_t sum = 0;
    for (const auto& m : memory) {
        sum += m.second;
    }
    return sum;
}


auto day14part2(const char *file) -> std::size_t {
    std::ifstream stream(file);
    std::string line;
    std::unordered_map<unsigned char, bool> mask;
    std::unordered_map<std::size_t, std::size_t> memory;
    while (std::getline(stream, line)) {
        const std::size_t l = line.size();
        if (line.substr(0, 4) == "mask") {
            mask.clear();
            for (unsigned char i = 0; i < 36; ++i) {
                switch (line[l - 1 - i]) {
                case 'X':
                    // Since we're not interested in changing `0`s this time, we'll abuse
                    // `false` for `X` this time
                    mask[i] = false;
                    break;
                case '1':
                    mask[i] = true;
                    break;
                }
            }
        }
        else { // assume it's "mem[.]"
            std::size_t address = std::stoull(line.substr(4));
            const std::size_t value = std::stoull(line.substr(line.find('=') + 2));
            std::size_t count = 1;
            for (const auto& m : mask) {
                if (m.second) {
                    // Directly modify the bit
                    address |= 1ull << m.first;
                }
                else {
                    // Every floating bit doubles the possible address count
                    count <<= 1;
                }
            }

            // Nothing floating, i.e. only one address
            if (count == 1) {
                memory[address] = value;
            }
            // Otherwise try all variations
            else {
                // Iterate over all possible variations
                // By using the bits of this counter, we'll
                // get all the different bit patterns we need
                for (std::size_t val = 0; val < count; ++val) {
                    std::size_t faddress = address;
                    std::size_t n = 0;
                    // Apply the whole mask
                    for (const auto& m : mask) {
                        // Floating bits are marked as "false"
                        if (!m.second) {
                            // Get the n-th bit to use this time
                            const bool b = (val >> n++) & 1;
                            if (b) {
                                // Set the m.first-th bit to 1
                                faddress |= 1ull << m.first;
                            }
                            else {
                                // Set the m.first-th bit to 0
                                faddress &= ~(1ull << m.first);
                            }
                        }
                    }
                    memory[faddress] = value;
                }
            }
        }
    }
    std::size_t sum = 0;
    for (const auto& m : memory) {
        sum += m.second;
    }
    return sum;
}

auto main(int argc, char** argv) -> int {

    std::cout << "Sample 1 solution: " << day14part1("day-14-sample-1.txt") << "\n";
    std::cout << "Sample 2 solution: " << day14part2("day-14-sample-2.txt") << "\n";

    std::cout << "Part 1 solution: " << day14part1("day-14-input.txt") << "\n";
    std::cout << "Part 2 solution: " << day14part2("day-14-input.txt") << "\n";

    return 0;
}
