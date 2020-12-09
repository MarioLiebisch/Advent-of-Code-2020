#include <iostream>
#include <algorithm>
#include <vector>

#include "utility.h"


auto day09part1(const std::vector<std::size_t>& numbers, std::size_t preamble = 25) -> std::size_t {
    const std::size_t l = numbers.size();
    for (std::size_t co = preamble; co < l; ++co) {
        const std::size_t &target = numbers[co];
        bool found = false;
        for (std::size_t lo1 = co - preamble; lo1 < co && !found; ++lo1) {
            for (std::size_t lo2 = lo1 + 1; lo2 < co; ++lo2) {
                if (target == numbers[lo1] + numbers[lo2]) {
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            return target;
        }
    }
    return 0;
}

auto day09part2(const std::vector<std::size_t>& numbers, std::size_t preamble = 25) -> std::size_t {
    const std::size_t l = numbers.size();
    const std::size_t violation = day09part1(numbers, preamble);
    for (std::size_t o = 0; o < l - 1; ++o) {
        std::size_t sum = 0;
        std::size_t min = -1;
        std::size_t max = 0;
        for (std::size_t oa = o; oa < l && sum < violation; ++oa) {
            const std::size_t & c = numbers[oa];
            sum += c;
            if (c < min) {
                min = c;
            }
            if (c > max) {
                max = c;
            }
        }
        if (violation == sum) {
            return min + max;
        }
    }
    return 0;
}

auto main(int argc, char **argv) -> int {
    std::vector<std::size_t> numbers;

    aoc::readFromFile<std::size_t>("day-09-sample.txt", std::back_inserter(numbers));
    std::cout << "Part 1 sample: " << day09part1(numbers, 5) << "\n";
    std::cout << "Part 2 sample: " << day09part2(numbers, 5) << "\n";

    numbers.clear();
    aoc::readFromFile<std::size_t>("day-09-input.txt", std::back_inserter(numbers));
    std::cout << "Part 1 solution: " << day09part1(numbers) << "\n";
    std::cout << "Part 2 solution: " << day09part2(numbers) << "\n";

    return 0;
}
