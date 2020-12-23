#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <list>
#include <cassert>
#include <unordered_map>

class Circle {
    std::list<int> numbers;
    std::list<int>::const_iterator selection;
    // Hacky mapping to avoid going over all elements over and over again
    // Can't be bothered to invest more time into this after that really massive step 2 increase :D
    std::unordered_map<int, std::list<int>::const_iterator> refs;
    int lowest, highest;
public:
    Circle(const std::string& input) {
        assert(!input.empty());
        for (const char& c : input) {
            refs.insert(std::make_pair(c - '0', numbers.insert(numbers.end(), c - '0')));
        }
        selection = numbers.begin();
        lowest = highest = *selection;
        for (const auto& n : numbers) {
            if (n < lowest) {
                lowest = n;
            }
            if (n > highest) {
                highest = n;
            }
        }
    }

    // Using `int` here only to avoid extra casting
    auto size() const -> int {
        return static_cast<int>(numbers.size());
    }

    auto pick(int &p1, int &p2, int &p3) -> void {
        assert(size());
        auto n = selection;
        if (++n == numbers.end()) {
            n = numbers.begin();
        }
        p1 = *n;
        n = numbers.erase(n);
        if (n == numbers.end()) {
            n = numbers.begin();
        }
        p2 = *n;
        n = numbers.erase(n);
        if (n == numbers.end()) {
            n = numbers.begin();
        }
        p3 = *n;
        numbers.erase(n);

        // Remove refs
        refs.erase(p1);
        refs.erase(p2);
        refs.erase(p3);

        // Changing lowest/highest value (cheating)
        while (p1 == lowest || p2 == lowest || p3 == lowest) {
            ++lowest;
        }

        while (p1 == highest || p2 == highest || p3 == highest) {
            --highest;
        }
        return;
    }

    auto fill(std::size_t count) -> void {
        for (int n = highest + 1, i = size(); i < count; ++i, ++n) {
            refs.insert(std::make_pair(n, numbers.insert(numbers.end(), n)));
        }
        highest = numbers.back();
    }

    auto move(const bool debug = false) -> void {
        assert(size() > 3);

        if (debug) {
            std::cout << "cups: ";

            for (const auto& n : numbers) {
                if (n == *selection) {
                    std::cout << "(" << n << ")";
                }
                else {
                    std::cout << " " << n << " ";
                }
            }
        }

        int dest = *selection - 1;

        int p1, p2, p3;
        pick(p1, p2, p3);

        if (debug) {
            std::cout << "\npick up: " << p1 << ", " << p2 << ", " << p3 << "\ndestination: ";
        }

        if (dest < lowest) {
            dest = highest;
        }

        bool found = false;

        auto& a = refs.find(dest);
        
        while (a == refs.end()) {
            a = refs.find(--dest);
        }

        if (debug) {
            std::cout << dest << "\n";
        }

        auto n = a->second;

        ++n;
        refs.insert(std::make_pair(p1, numbers.insert(n, p1)));
        refs.insert(std::make_pair(p2, numbers.insert(n, p2)));
        refs.insert(std::make_pair(p3, numbers.insert(n, p3)));

        // Changing lowest/highest value (cheating again)
        if (p1 < lowest) {
            lowest = p1;
        }
        if (p2 < lowest) {
            lowest = p2;
        }
        if (p3 < lowest) {
            lowest = p3;
        }
        if (p1 > highest) {
            highest = p1;
        }
        if (p2 > highest) {
            highest = p2;
        }
        if (p3 > highest) {
            highest = p3;
        }

        if (++selection == numbers.end()) {
            selection = numbers.begin();
        }
    }

    auto result1() -> std::string {
        std::string res1, res2;
        for (auto& n = numbers.begin(); n != numbers.end(); ++n) {
            if (*n == 1) {
                for (auto& m = numbers.begin(); m != n; ++m) {
                    res2 += '0' + *m;
                }
                for (auto& m = ++n; m != numbers.end(); ++m) {
                    res1 += '0' + *m;
                }
                break;
            }
        }
        return res1 + res2;
    }

    auto result2() -> std::size_t {
        auto a = refs[1];
        if (++a == numbers.end()) {
            a = numbers.begin();
        }
        auto b = a;
        if (++b == numbers.end()) {
            b = numbers.begin();
        }
        std::cout << "\n\n\n\n\t" << *refs[1] << "\t" << *a << "\t" << *b << "\n";
        return static_cast<std::size_t>(*a) * *b;
    }
};

auto day23part1(const char* file, const bool debug = false) -> std::string {
    std::string tmp;
    std::ifstream stream(file);
    std::getline(stream, tmp);
    Circle circle(tmp);

    for (int i = 0; i < 100; ++i) {
        if (debug) {
            std::cout << "-- move " << (i + 1) << " --\n";
        }
        circle.move(debug);
    }

    return circle.result1();
}

auto day23part2(const char* file, const bool debug = false) -> std::size_t {
    std::string tmp;
    std::ifstream stream(file);
    std::getline(stream, tmp);
    Circle circle(tmp);
    circle.fill(1000000);
    for (int i = 1; i <= 10000000; ++i) {
        if (debug) {
            std::cout << "-- move " << i << " --\n";
        }
        if (i % 10000 == 0) {
            std::cout << "Move " << i << "...\r";
        }
        circle.move(debug);
    }

    return circle.result2();
}

auto main(int argc, char** argv) -> int {
    const std::string r1 = day23part1("day-23-sample.txt", true);
    std::cout << "Sample 1 solution: " << r1 << "\n";
    const std::size_t r2 = day23part2("day-23-sample.txt");
    std::cout << "Sample 2 solution: " << r2 << "\n";

    const std::string r3 = day23part1("day-23-input.txt");
    std::cout << "Part 1 solution: " << r3 << "\n";
    const std::size_t r4 = day23part2("day-23-input.txt");
    std::cout << "Part 2 solution: " << r4 << "\n";
    return 0;
}
