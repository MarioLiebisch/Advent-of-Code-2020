#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>

auto day21part1(const char* file) -> long long {
    std::ifstream stream(file);
    std::string line;
    std::unordered_map<std::string, std::set<std::string>> contained_in;
    std::map<std::string, std::size_t> ingredients;
    while (std::getline(stream, line)) {
        const std::size_t o = line.find(" (contains ");
        std::stringstream ing_ss(line.substr(0, o));
        std::stringstream all_ss(o == std::string::npos ? "" : line.substr(o + 10, line.length() - o - 11));
        std::set<std::string> allergens;

        // Get all the allergenes
        while (std::getline(all_ss, line, ',')) {
            // Add to list, but skip first character (always a space)
            allergens.insert(line.substr(1));
        }

        // Get all the recipe contents
        std::set<std::string> recipe;
        while (std::getline(ing_ss, line, ' ')) {
            ++ingredients[line];
            recipe.insert(line);
        }

        // Save the information for later
        for (const auto& a : allergens) {
            auto& list = contained_in[a];
            // No ingredients for this allergen
            if (list.empty()) {
                list.insert(recipe.begin(), recipe.end());
            }
            // We have a list, so stuff not in this recipe
            // can't be the one containing it.
            else {
                std::set<std::string> newlist;
                for (const auto& o : list) {
                    if (recipe.find(o) != recipe.end()) {
                        newlist.insert(o);
                    }
                }
                list = newlist;
            }
        }

        std::cout << "";
    }

    std::unordered_map<std::string, std::string> association;
    // As long as we've unprocessed list entries
    while (!contained_in.empty()) {
        // Go over all entries
        for (const auto& a : contained_in) {
            // This is a found/exact association
            if (a.second.size() == 1) {
                std::string current = *a.second.begin();
                // Save it
                association.insert(std::make_pair(current, a.first));
                // Remove it
                contained_in.erase(a.first);
                // Remove the possibilities from others
                for (auto& b : contained_in) {
                    b.second.erase(current);
                }
                break;
            }
        }
    }
    std::size_t sum = 0;
    // Go over all ingredients and  their counts
    for (const auto& i : ingredients) {
        // This is not on our allergenes list?
        if (association.find(i.first) == association.end()) {
            sum += i.second;
        }
    }
    return sum;
}


auto day21part2(const char* file) -> std::string {
    std::ifstream stream(file);
    std::string line;
    std::unordered_map<std::string, std::set<std::string>> contained_in;
    std::map<std::string, std::size_t> ingredients;
    while (std::getline(stream, line)) {
        const std::size_t o = line.find(" (contains ");
        std::stringstream ing_ss(line.substr(0, o));
        std::stringstream all_ss(o == std::string::npos ? "" : line.substr(o + 10, line.length() - o - 11));
        std::set<std::string> allergens;

        // Get all the allergenes
        while (std::getline(all_ss, line, ',')) {
            // Add to list, but skip first character (always a space)
            allergens.insert(line.substr(1));
        }

        // Get all the recipe contents
        std::set<std::string> recipe;
        while (std::getline(ing_ss, line, ' ')) {
            ++ingredients[line];
            recipe.insert(line);
        }

        // Save the information for later
        for (const auto& a : allergens) {
            auto& list = contained_in[a];
            // No ingredients for this allergen
            if (list.empty()) {
                list.insert(recipe.begin(), recipe.end());
            }
            // We have a list, so stuff not in this recipe
            // can't be the one containing it.
            else {
                std::set<std::string> newlist;
                for (const auto& o : list) {
                    if (recipe.find(o) != recipe.end()) {
                        newlist.insert(o);
                    }
                }
                list = newlist;
            }
        }

        std::cout << "";
    }

    // Just let `std::map` do the sorting already
    std::map<std::string, std::string> association;

    // As long as we've unprocessed list entries
    while (!contained_in.empty()) {
        // Go over all entries
        for (const auto& a : contained_in) {
            // This is a found/exact association
            if (a.second.size() == 1) {
                std::string current = *a.second.begin();
                // Save it
                association.insert(std::make_pair(a.first, current));
                // Remove it
                contained_in.erase(a.first);
                // Remove the possibilities from others
                for (auto& b : contained_in) {
                    b.second.erase(current);
                }
                break;
            }
        }
    }

    std::string res;
    // Build an output list
    for (const auto& e : association) {
        res += "," + e.second;
    }
    // Skip the first comma
    return res.substr(1);
}

auto main(int argc, char** argv) -> int {
    const std::size_t r1 = day21part1("day-21-sample.txt");
    std::cout << "Sample 1 solution: " << r1 << "\n";
    const std::string r2 = day21part2("day-21-sample.txt");
    std::cout << "Sample 2 solution: " << r2 << "\n";

    const std::size_t r3 = day21part1("day-21-input.txt");
    std::cout << "Part 1 solution: " << r3 << "\n";
    const std::string r4 = day21part2("day-21-input.txt");
    std::cout << "Part 2 solution: " << r4 << "\n";
    return 0;
}
