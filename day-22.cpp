#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <vector>

auto day22part1(const char* file) -> std::size_t {
    std::queue<std::size_t> decks[2];
    std::string tmp;
    std::ifstream stream(file);

    // Read player 1 label
    std::getline(stream, tmp);
    // Read deck 1
    while (std::getline(stream, tmp) && !tmp.empty()) {
        decks[0].push(std::stoull(tmp));
    }

    // Read player 2 label
    std::getline(stream, tmp);
    // Read deck 2
    while (std::getline(stream, tmp) && !tmp.empty()) {
        decks[1].push(std::stoull(tmp));
    }

    std::size_t round = 0;

    // Play until one deck is empty
    while (!decks[0].empty() && !decks[1].empty()) {
        ++round;
        // Draw the cards
        const std::size_t c1 = decks[0].front();
        decks[0].pop();
        const std::size_t c2 = decks[1].front();
        decks[1].pop();

        // Calculate the winner
        if (c1 > c2) {
            decks[0].push(c1);
            decks[0].push(c2);
        }
        else {
            decks[1].push(c2);
            decks[1].push(c1);
        }
    }

    // Play the game
    const bool winner = decks[0].empty();

    // Determine the winner and calculate the score
    std::size_t score = 0, i = decks[winner].size();
    while (!decks[winner].empty()) {
        score += i-- * decks[winner].front();
        decks[winner].pop();
    }
    return score;
}

// Play a game of Recursive Combat
auto game(std::vector<std::size_t> decks[2]) -> bool {
    std::unordered_set<std::string> prev[2];
    while (!decks[0].empty() && !decks[1].empty()) {
        // Go over both decks
        for (std::size_t i = 0; i < 2; ++i) {
            std::string tmp;
            // Calculate a hash for the set
            for (const auto& c : decks[i]) {
                tmp += std::to_string(c);
                tmp += ':';
            }
            // And determine if it's been there before
            if (prev[i].find(tmp) != prev[i].end()) {
                // If so, player 1 wins
                return 0;
            }
            else {
                prev->insert(tmp);
            }
        }

        // Draw the top cards
        const std::size_t c1 = decks[0].front();
        decks[0].erase(decks[0].begin());
        const std::size_t c2 = decks[1].front();
        decks[1].erase(decks[1].begin());

        bool winner = 0;
        // Decks are big enough for a recursive round
        if (decks[0].size() >= c1 && decks[1].size() >= c2) {
            // Create the sub decks
            std::vector<std::size_t> tdecks[2];
            tdecks[0].resize(c1);
            tdecks[1].resize(c2);
            for (std::size_t i = 0; i < c1; ++i) {
                tdecks[0][i] = decks[0][i];
            }
            for (std::size_t i = 0; i < c2; ++i) {
                tdecks[1][i] = decks[1][i];
            }
            // Play the sub game
            winner = game(tdecks);
        }
        // Otherwise compare the cards
        else {
            winner = c2 > c1;
        }
        // Did player 2 win?
        if (winner) {
            decks[1].push_back(c2);
            decks[1].push_back(c1);
        }
        // Or player 1?
        else {
            decks[0].push_back(c1);
            decks[0].push_back(c2);
        }
    }
    return decks[0].empty();
}

auto day22part2(const char* file) -> std::size_t {
    std::vector<std::size_t> decks[2];
    std::string tmp;
    std::ifstream stream(file);

    // Read player 1 label
    std::getline(stream, tmp);
    // Read deck 1
    while (std::getline(stream, tmp) && !tmp.empty()) {
        decks[0].push_back(std::stoull(tmp));
    }

    // Read player 2 label
    std::getline(stream, tmp);
    // Read deck 2
    while (std::getline(stream, tmp) && !tmp.empty()) {
        decks[1].push_back(std::stoull(tmp));
    }

    // Play the game
    const bool winner = game(decks);

    // Determine the winner and calculate the score
    std::size_t score = 0, i = decks[winner].size();
    for (std::size_t i = decks[winner].size(); i > 0; --i) {
        score += i * decks[winner][decks[winner].size() - i];
    }
    return score;
}

auto main(int argc, char** argv) -> int {
    const std::size_t r1 = day22part1("day-22-sample.txt");
    std::cout << "Sample 1 solution: " << r1 << "\n";
    const std::size_t r2 = day22part2("day-22-sample.txt");
    std::cout << "Sample 2 solution: " << r2 << "\n";

    const std::size_t r3 = day22part1("day-22-input.txt");
    std::cout << "Part 1 solution: " << r3 << "\n";
    const std::size_t r4 = day22part2("day-22-input.txt");
    std::cout << "Part 2 solution: " << r4 << "\n";
    return 0;
}
