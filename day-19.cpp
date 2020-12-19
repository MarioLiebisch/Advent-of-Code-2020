#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

struct Rule {
    // Character for direct match
    char match = 0;
    // Lists of sub roules
    std::vector<std::size_t> seq1, seq2;
    // Minimal cached length this match requires
    std::size_t req = 0;
};

auto split(std::string& text, std::vector<std::size_t>& vec) -> std::size_t {
    vec.clear();
    std::size_t o = text.find_first_not_of(' ');
    if (o && o != std::string::npos) {
        text.erase(0, o);
    }
    o = text.find_last_not_of(' ');
    if (o != std::string::npos && o != text.length() - 1) {
        text.erase(o + 1, -1);
    }
    std::stringstream ss(text);
    std::string seg;
    while (std::getline(ss, seg, ' ')) {
        vec.push_back(std::stoull(seg));
    }
    return vec.size();
}

auto match(std::set<std::size_t> &res, const std::string& text, const std::unordered_map<std::size_t, Rule>& rules, std::size_t index = 0, std::size_t irule = 0) -> void {
    // Out of bounds
    if (index >= text.size()) {
        return;
    }

    const auto& a = rules.find(irule);

    // Invalid rule, should never be the case
    if (a == rules.end()) {
        return;
    }

    const Rule& rule = a->second;

    // Direct match
    if (rule.match) {
        if (text[index] == rule.match) {
            res.insert(1);
        }
        return;
    }

    std::size_t m1 = 0, m2 = 0;
    std::set<std::size_t> res1 = { 0 }, res2 = { 0 };

    // Go over all rules in the first sequence and try to match them
    for (std::size_t i = 0; i < rule.seq1.size(); ++i) {
        std::set<std::size_t> next;
        // Iterate over allcurrent offsets/results
        for (const auto& m : res1) {
            std::set<std::size_t> tmp;
            match(tmp, text, rules, index + m, rule.seq1[i]);

            // Create the new offsets for the next round
            for (const auto& t : tmp) {
                next.insert(m + t);
            }
        }
        res1 = next;
    }

    // Go over all rules in the first sequence and try to match them
    for (std::size_t i = 0; i < rule.seq2.size(); ++i) {
        std::set<std::size_t> next;
        // Iterate over allcurrent offsets/results
        for (const auto& m : res2) {
            std::set<std::size_t> tmp;
            match(tmp, text, rules, index + m, rule.seq2[i]);

            // Create the new offsets for the next round
            for (const auto& t : tmp) {
                next.insert(m + t);
            }
        }
        res2 = next;
    }

    res.insert(res1.begin(), res1.end());
    res.insert(res2.begin(), res2.end());
    // 0 (matched characters) shouldn't appear, but just in case...
    res.erase(0);
}

// A small overlad/helper to return a boolean value (perfect match yes/no)
auto match(const std::string& text, const std::unordered_map<std::size_t, Rule>& rules) -> bool {
    std::set<std::size_t> res;
    match(res, text, rules);
    const std::size_t& l = text.length();
    for (const auto& r : res) {
        if (r == l) {
            return true;
        }
    }
    return false;
}

auto day19part1(const char* file) -> long long {
    std::ifstream stream(file);
    std::string line;

    // Read the rules
    std::unordered_map<std::size_t, Rule> rules;
    while (std::getline(stream, line) && !line.empty()) {
        std::size_t o;
        const std::size_t idx = std::stoll(line, &o);
        line.erase(0, o + 2);
        Rule& nr = rules[idx];
        // Direct match
        if (line[0] == '"') {
            nr.match = line[1];
        }
        else {
            o = line.find('|');
            // Two sequences
            if (o != std::string::npos) {
                split(line.substr(0, o), nr.seq1);
                split(line.substr(o + 1), nr.seq2);
            }
            // One sequence
            else {
                split(line, nr.seq1);
            }
        }
    }

    // Read the strings to be validated
    std::size_t matches = 0;
    while (std::getline(stream, line)) {
        const bool res = match(line, rules);
#ifndef NDEBUG
        std::cout << line << ": " << (res ? "ok\n" : "fail\n");
#endif
        // Did we match the whole line?
        if (res) {
            ++matches;
        }
    }
    return matches;
}


auto day19part2(const char* file) -> long long {
    std::ifstream stream(file);
    std::string line;

    // Read the rules
    std::unordered_map<std::size_t, Rule> rules;
    while (std::getline(stream, line) && !line.empty()) {
        std::size_t o;
        const std::size_t idx = std::stoll(line, &o);
        line.erase(0, o + 2);
        Rule& nr = rules[idx];
        // Direct match
        if (line[0] == '"') {
            nr.match = line[1];
        }
        else {
            o = line.find('|');
            // Two sequences
            if (o != std::string::npos) {
                split(line.substr(0, o), nr.seq1);
                split(line.substr(o + 1), nr.seq2);
            }
            // One sequence
            else {
                split(line, nr.seq1);
            }
        }
    }

    // Fix-ups in problem description:
    rules[8].seq2 = { 42, 8 };
    rules[11].seq2 = { 42, 11, 31 };

    // Read the strings to be validated
    std::size_t matches = 0;
    while (std::getline(stream, line)) {
        const bool res = match(line, rules);
#ifndef NDEBUG
        std::cout << line << ": " << (res ? "ok\n" : "fail\n");
#endif
        // Did we match the whole line?
        if (res) {
            ++matches;
        }
    }
    return matches;
}

auto main(int argc, char** argv) -> int {
    const std::size_t r1 = day19part1("day-19-sample-1.txt");
    std::cout << "Sample 1 solution: " << r1 << "\n";
    const std::size_t r2 = day19part2("day-19-sample-2.txt");
    std::cout << "Sample 2 solution: " << r2 << "\n";

    const std::size_t r3 = day19part1("day-19-input.txt");
    std::cout << "Part 1 solution: " << r3 << "\n";
    const std::size_t r4 = day19part2("day-19-input.txt");
    std::cout << "Part 2 solution: " << r4 << "\n";
    return 0;
}
