#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "utility.h"

#include <fstream>
#include <set>
#include <numeric>

auto day16part1(const char *file) -> std::size_t {
    std::ifstream stream(file);
    std::string line;
    std::unordered_map<std::string, aoc::TicketFieldRule> rules;
    std::size_t o;
    while (std::getline(stream, line) && !line.empty()) {
        o = line.find(':');
        auto& rule = rules[line.substr(0, o)];
        line = line.substr(o + 2);
        for (;;) {
            const std::size_t lower = std::stoull(line, &o);
            line = line.substr(o + 1);
            const std::size_t upper = std::stoull(line, &o);
            rule.emplace_back(std::make_pair(lower, upper));
            if (o != line.size()) {
                line = line.substr(o + 4);
            }
            else {
                break;
            }
        }
    }

    // Skip "your ticket:" section
    for (; std::getline(stream, line) && !line.empty(); );

    // Skip "nearby tickets:" line
    std::getline(stream, line);

    std::size_t error_rate = 0;
    while (std::getline(stream, line)) {
        for (;;) {
            const std::size_t value = std::stoull(line, &o);

            // Validate this field
            bool okay = false;
            // iterate over all rules
            for (const auto& rule : rules) {
                // Go over all included value ranges
                for (const auto& range : rule.second) {
                    // Check this range
                    if (range.first <= value && range.second >= value) {
                        okay = true;
                        break;
                    }
                }
                if (okay) {
                    break;
                }
            }

            if (!okay) {
                // This field doesn't match anything
                error_rate += value;
            }

            if (o != line.size()) {
                line = line.substr(o + 1);
            }
            else {
                break;
            }
        }
    }

    return error_rate;
}


auto day16part2(const char *file) -> std::size_t {
    std::ifstream stream(file);
    std::string line;
    std::unordered_map<std::string, aoc::TicketFieldRule> rules;
    std::size_t o;
    while (std::getline(stream, line) && !line.empty()) {
        o = line.find(':');
        auto& rule = rules[line.substr(0, o)];
        line = line.substr(o + 2);
        for (;;) {
            const std::size_t lower = std::stoull(line, &o);
            line = line.substr(o + 1);
            const std::size_t upper = std::stoull(line, &o);
            rule.emplace_back(std::make_pair(lower, upper));
            if (o != line.size()) {
                line = line.substr(o + 4);
            }
            else {
                break;
            }
        }
    }

    // Skip "your ticket:" line
    std::getline(stream, line);
    // Read "your ticket"
    std::getline(stream, line);
    std::vector<std::size_t> mine;
    for (;;) {
        const std::size_t value = std::stoull(line, &o);
        mine.push_back(value);
        if (o != line.size()) {
            line = line.substr(o + 1);
        }
        else {
            break;
        }
    }

    // Skip empty line and "nearby tickets:" line
    std::getline(stream, line);
    std::getline(stream, line);

    std::vector<std::vector<std::size_t>> tickets;
    tickets.push_back(mine);

    while (std::getline(stream, line)) {
        bool ticket_okay = true;
        std::vector<std::size_t> ticket;
        for (;;) {
            bool okay = false;
            const std::size_t value = std::stoull(line, &o);
            ticket.push_back(value);

            // Validate this field
            // iterate over all rules
            for (const auto& rule : rules) {
                // Go over all included value ranges
                for (const auto& range : rule.second) {
                    // Check this range
                    if (range.first <= value && range.second >= value) {
                        okay = true;
                        break;
                    }
                }
                if (okay) {
                    break;
                }
            }

            // If a value is not okay or the line is empty, we're done
            if (okay && o != line.size()) {
                line = line.substr(o + 1);
            }
            else {
                if (!okay) {
                    ticket_okay = false;
                }
                break;
            }
        }
        if (ticket_okay) {
            tickets.push_back(ticket);
        }
    }

    const std::size_t col_count = mine.size();

    std::unordered_map<std::size_t, std::set<std::string>> matches;
    // Iterate over all columns
    for (std::size_t i = 0; i < col_count; ++i) {
        // Iterate over all rules
        for (const auto& rule : rules) {
            bool valid = true;
            
            // Iterate over all tickets
            for (const auto& ticket : tickets) {
                bool value_matches = false;
                const std::size_t& value = ticket[i];
                // iterate over all ranges
                for (const auto& range : rule.second) {
                    if (range.first <= value && range.second >= value) {
                        value_matches = true;
                        break;
                    }
                }
                if (!value_matches) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                // Add it as a matching rule
                matches[i].insert(rule.first);
            }
        }
    }
    
    std::size_t cols_done = 0;
    std::size_t res = 1;
    // Now go through the matches and repeatedly find the colum with the least possibilities (always just 1 to be solvable)
    while (cols_done < col_count) {
        std::string col;
        std::size_t coli = 0;
        for (const auto& m : matches) {
            if (m.second.size() == 1) {
                col = *m.second.begin();
                coli = m.first;
                break;
            }
        }
        // Nothing found? Should never happen (although it did happen! hmpf!)
        if (col.empty()) {
            std::cerr << "Something went wrong!";
            break;
        }
        // Erase this column from all matches, hopefully leaving another single solution
        for (auto& m : matches) {
            m.second.erase(col);
        }
        
        if (col.length() > 9 && col.substr(0, 9) == "departure") {
            res *= mine[coli];
        }
        ++cols_done;
    }

    return res;
}

auto main(int argc, char** argv) -> int {

    std::cout << "Sample 1 solution: " << day16part1("day-16-sample-1.txt") << "\n";
    std::cout << "Sample 2 solution: " << day16part2("day-16-sample-2.txt") << "\n";

    std::cout << "Part 1 solution: " << day16part1("day-16-input.txt") << "\n";
    std::cout << "Part 2 solution: " << day16part2("day-16-input.txt") << "\n";

    return 0;
}
