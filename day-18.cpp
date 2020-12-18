#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

#include <fstream>
#include <set>
#include <numeric>

// Loosely based on https://stackoverflow.com/a/11709644/409744
auto evaluate1(const std::string& term) -> long long {
    const std::size_t l = term.size();
    std::size_t depth = 0;
    // Find all addition/multiplication pairs (recursive)
    // Start from the right so we'll solve the inner-/leftmost terms first
    for (std::size_t i = l; i > 0; --i) {
        const char& c = term[i - 1];
        if (c == '(') {
            ++depth;
        }
        else if (c == ')') {
            --depth;
        }
        else if (depth > 0) {
        }
        else if (c == '+') {
            const long long r = evaluate1(term.substr(0, i - 1)) + evaluate1(term.substr(i));
            // std::cout << term << " = " << r << "\n";
            return r;
        }
        else if (c == '*') {
            const long long r = evaluate1(term.substr(0, i - 1)) * evaluate1(term.substr(i));
            // std::cout << term << " = " << r << "\n";
            return r;
        }
    }
    // Starts with a bracket – skip the outermost pair
    if (term[0] == '(') {
        for (std::size_t i = 0; i < l; ++i) {
            const char& c = term[i];
            if (c == '(') {
                ++depth;
            }
            else if (c == ')' && --depth == 0) {
                return evaluate1(term.substr(1, i - 1));
            }
        }
    }
    // It's actually a raw number
    else if (std::isdigit(term[0])) {
        return std::stoll(term);
    }

    // Should never arrive here
    std::cerr << "Unhandled expression: '" << term << "'!\n";
    return 0;
}

auto evaluate2(const std::string& term) -> long long {
    const std::size_t l = term.size();
    std::size_t depth = 0;
    // Find all multiplication pairs (recursive)
    for (std::size_t i = l; i > 0; --i) {
        const char& c = term[i - 1];
        if (c == '(') {
            ++depth;
        }
        else if (c == ')') {
            --depth;
        }
        else if (depth > 0) {
        }
        else if (c == '*') {
            const long long r = evaluate2(term.substr(0, i - 1)) * evaluate2(term.substr(i));
            // std::cout << term << " = " << r << "\n";
            return r;
        }
    }
    // Find all addition pairs (recursive)
    for (std::size_t i = l; i > 0; --i) {
        const char& c = term[i - 1];
        if (c == '(') {
            ++depth;
        }
        else if (c == ')') {
            --depth;
        }
        else if (depth > 0) {
        }
        else if (c == '+') {
            const long long r = evaluate2(term.substr(0, i - 1)) + evaluate2(term.substr(i));
            // std::cout << term << " = " << r << "\n";
            return r;
        }
    }
    // Starts with a bracket – skip the outermost pair
    if (term[0] == '(') {
        for (std::size_t i = 0; i < l; ++i) {
            const char& c = term[i];
            if (c == '(') {
                ++depth;
            }
            else if (c == ')' && --depth == 0) {
                return evaluate2(term.substr(1, i - 1));
            }
        }
    }
    // It's actually a raw number
    else if (std::isdigit(term[0])) {
        return std::stoll(term);
    }
    
    // Should never arrive here
    std::cerr << "Unhandled expression: '" << term << "'!\n";
    return 0;
}

auto day18part1(const char *file) -> long long {
    std::ifstream stream(file);
    std::string line;
    long long sum = 0;
    while (std::getline(stream, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), std::isspace), line.end());

        // My own extra "test" mode for the sample given
        // Check the equation rather than evaluate it
        const std::size_t o = line.find("=");
        if (o != std::string::npos) {
            const long long res1 = evaluate1(line.substr(0, o));
            const long long res2 = evaluate1(line.substr(o + 1));
            std::cout << "\t" << line << " (";
            if (res1 == res2) {
                std::cout << "ok";
            }
            else {
                std::cout << "fail: " << res1 << " != " << res2;
            }
            std::cout << ")\n";
            sum += res1;
        }
        else {
            sum += evaluate1(line);
        }
    }
    return sum;
}


auto day18part2(const char* file) -> long long {
    std::ifstream stream(file);
    std::string line;
    long long sum = 0;
    while (std::getline(stream, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), std::isspace), line.end());

        // My own extra "test" mode for the sample given
        // Check the equation rather than evaluate it
        const std::size_t o = line.find("=");
        if (o != std::string::npos) {
            const long long res1 = evaluate2(line.substr(0, o));
            const long long res2 = evaluate2(line.substr(o + 1));
            std::cout << "\t" << line << " (";
            if (res1 == res2) {
                std::cout << "ok";
            }
            else {
                std::cout << "fail: " << res1;
            }
            std::cout << ")\n";
            sum += res1;
        }
        else {
            sum += evaluate2(line);
        }
    }
    return sum;
}

auto main(int argc, char** argv) -> int {
    const std::size_t r1 = day18part1("day-18-sample-1.txt");
    std::cout << "\nSample 1 solution: " << r1 << "\n\n";
    const std::size_t r2 = day18part2("day-18-sample-2.txt");
    std::cout << "\nSample 2 solution: " << r2 << "\n";

    const std::size_t r3 = day18part1("day-18-input.txt");
    std::cout << "\nPart 1 solution: " << r3 << "\n";
    const std::size_t r4 = day18part2("day-18-input.txt");
    std::cout << "\nPart 2 solution: " << r4 << "\n";
    return 0;
}
