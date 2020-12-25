#include <iostream>
#include <fstream>
#include <string>

inline auto transform(const std::size_t value, const std::size_t subject) -> std::size_t {
    return (value * subject) % 20201227;
}

auto day25(const char* file, const bool debug = false) -> std::size_t {
    std::string tmp;
    std::ifstream stream(file);

    // Read both public keys
    std::getline(stream, tmp);
    const std::size_t card_pk = std::stoull(tmp);
    std::getline(stream, tmp);
    const std::size_t door_pk = std::stoull(tmp);

    std::size_t card_loop;
    std::size_t value, loops;

    // Now perform the loops to determine the card's loop value
    for (value = 1, card_loop = 0; value != card_pk; ++card_loop, value = transform(value, 7));

    // And calculate the encryption key
    for (value = 1, loops = 0; loops < card_loop; ++loops, value = transform(value, door_pk));

    // This felt too easy...
    return value;
}

auto main(int argc, char** argv) -> int {
    const std::size_t r1 = day25("day-25-sample.txt", true);
    std::cout << "Sample solution: " << r1 << "\n";

    const std::size_t r3 = day25("day-25-input.txt");
    std::cout << "Task solution: " << r3 << "\n";
    return 0;
}
