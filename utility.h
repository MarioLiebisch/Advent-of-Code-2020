#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include <set>

namespace aoc {

    struct PasswordRule {
        std::size_t minimum;
        std::size_t maximum;
        char character;
    };

    struct PasswordData {
        std::vector<PasswordRule> rules;
        std::string password;
    };

    struct BoardingPass {
        unsigned int row = 0;
        unsigned int col = 0;
        unsigned int id = 0;

        BoardingPass() = default;
        auto operator=(const BoardingPass& other) -> BoardingPass& = default;

        auto operator<(const BoardingPass& other) const -> bool {
            return id < other.id;
        }
    };

    struct Instruction {
        std::string op;
        signed int param = 0;
    };

    using Passport = std::unordered_map<std::string, std::string>;
    using CustomsDeclaration = std::set<char>;

    using MapDataLine  = std::map<std::size_t, char>;
    using MapData = std::map<std::size_t, MapDataLine>;
    
    using PackagingRule = std::pair<std::string, std::map<std::string, std::size_t>>;

    template<class T>
    auto convert(std::string input) -> T;

    template<>
    auto convert(std::string input) -> long {
        return std::stol(input);
    }

    template<>
    auto convert(std::string input) -> Instruction {
        Instruction ni;
        if (input.length() < 5) {
            return ni;
        }
        ni.param = std::stol(input.substr(4));
        ni.op = input.substr(0, 3);
        return ni;
    }

    template<>
    auto convert(std::string input) -> PackagingRule {
        PackagingRule rule;
        // Cheating and skipping input validation, because …
        std::size_t o = input.find("s contain ");
        rule.first = input.substr(0, o);
        o += 10;
        for (;;) {
            input = input.substr(o);

            // done anyway or "no other bags"
            if (input.empty() || input[0] == 'n') {
                break;
            }

            const std::size_t n = std::stoll(input, &o);
            const std::size_t o2 = input.find_first_of(",.", o + 1);
            rule.second[input.substr(o + 1, o2 - o - (input[o2 - 1] == 's' ? 2 : 1))] = n;

            o = o2 + 2;

            if (o >= input.length()) {
                break;
            }
        }
        return rule;
    }

    template<>
    auto convert(std::string input) -> BoardingPass {
        BoardingPass pass;
        for (const auto& c : input) {
            switch (c) {
            case 'F':
                pass.row <<= 1;
                break;
            case 'B':
                pass.row = (pass.row << 1) | 1;
                break;
            case 'L':
                pass.col <<= 1;
                break;
            case 'R':
                pass.col = (pass.col << 1) | 1;
                break;
            default:
                throw std::exception("This should never happen!");
            }
        }
        pass.id = pass.row * 8 + pass.col;
        return pass;
    }

    template<>
    auto convert(std::string input) -> PasswordData {
        PasswordData ret;

        while (input[0] != ':') {
            PasswordRule rule = {};
            std::size_t idx = 0;
            rule.minimum = std::stol(input, &idx);
            input = input.substr(idx + 1);
            rule.maximum = std::stol(input, &idx);
            input = input.substr(idx + 1);
            rule.character = input[0];
            input = input.substr(1);
            ret.rules.push_back(rule);
        }

        if (input.length() > 1) {
            ret.password = input.substr(2);
        }

        return ret;
    }

    template<>
    auto convert(std::string input) -> MapDataLine {
        MapDataLine line;
        for (std::size_t x = 0; x < input.length(); ++x) {
            line[x] = input[x];
        }
        return line;
    }


    template<typename T, typename CT>
    auto readFromFile(const char *file, std::back_insert_iterator<CT> target) -> std::size_t {
        std::ifstream stream(file);
        std::string line;
        std::size_t count = 0;
        // No need to check for the stream/file
        // If something is wrong, std::getline() returns false
        while (std::getline(stream, line)) {
            ++count;
            *target++ = convert<T>(line);
        }
        return count;
    }

    auto getKeyValuePairs(const std::string data) -> std::map<std::string, std::string> {
        std::map<std::string, std::string> ret;
        std::istringstream ss(data);
        std::string kv;
        while (ss >> kv) {
            const std::size_t o = kv.find_first_of(':');
            if (o != std::string::npos) {
                ret[kv.substr(0, o)] = kv.substr(o + 1);
            }
        }
        return ret;
    }

    auto readFromFile(const char* file, std::back_insert_iterator<std::vector<Passport>> target) -> std::size_t {
        std::ifstream stream(file);
        std::string line;
        std::size_t count = 0;
        Passport np;
        bool set = false;
        // No need to check for the stream/file
        // If something is wrong, std::getline() returns false
        while (std::getline(stream, line)) {
            if (line.empty()) {
                if (set) {
                    *target++ = np;
                    np = Passport();
                    set = false;
                    ++count;
                }
            }
            else {
                for (const auto& kv : getKeyValuePairs(line)) {
                    np.emplace(kv);
                }
                set = true;
            }
        }
        if (set) {
            *target++ = np;
            ++count;
        }
        return count;
    }

    auto readFromFile(const char* file, std::back_insert_iterator<std::vector<CustomsDeclaration>> target, bool strict) -> std::size_t {
        std::ifstream stream(file);
        std::string line;
        std::size_t count = 0;
        CustomsDeclaration nd;
        bool set = false;
        // No need to check for the stream/file
        // If something is wrong, std::getline() returns false
        while (std::getline(stream, line)) {
            if (line.empty()) {
                if (set) {
                    *target++ = nd;
                    nd.clear();
                    set = false;
                    ++count;
                }
            }
            else if (strict) {
                if (set) {
                    std::set<char> temp;
                    for (const auto& c : nd) {
                        if (line.find(c) == std::string::npos) {
                            temp.insert(c);
                        }
                    }
                    for (const auto& c : temp) {
                        nd.erase(c);
                    }
                }
                else {
                    for (const auto& c : line) {
                        nd.insert(c);
                    }
                    set = true;
                }
            }
            else {
                for (const auto& c : line) {
                    nd.insert(c);
                }
                set = true;
            }
        }
        if (set) {
            *target++ = nd;
            ++count;
        }
        return count;
    }

    auto readFromFile(const char* file, aoc::MapData &target) -> std::size_t {
        std::ifstream stream(file);
        std::string line;
        std::size_t count = 0;
        // No need to check for the stream/file
        // If something is wrong, std::getline() returns false
        while (std::getline(stream, line)) {
            target.insert(std::make_pair(count++, convert<aoc::MapDataLine>(line)));
        }
        return count;
    }

    class VirtualMachine {
        using Operation = void(*)(std::size_t& ip, signed int& accumulator, const signed int param);
        using Callback = bool(*)(std::size_t& ip, signed int& accumulator);

        const std::vector<Instruction> instructions;
        const std::unordered_map<std::string, Operation> operations {
            {"nop", [](std::size_t& ip, signed int& accumulator, const signed int param) { ++ip; }},
            {"acc", [](std::size_t& ip, signed int& accumulator, const signed int param) { ++ip; accumulator += param; }},
            {"jmp", [](std::size_t& ip, signed int& accumulator, const signed int param) { ip += param; }}
        };

        Callback preCallback = nullptr;
    public:
        VirtualMachine(const std::vector<Instruction> instructions) : instructions(instructions) {

        }

        void setPreCallback(Callback callback) {
            preCallback = callback;
        }

        auto operator()() -> signed int {
            if (instructions.empty()) {
                return 0;
            }

            signed int accumulator = 0;
            std::size_t ip = 0;
            for (;;) {
                if (preCallback != nullptr) {
                    if (!preCallback(ip, accumulator)) {
                        break;
                    }
                }

                if (ip >= instructions.size()) {
                    break;
                }

                const Instruction& i = instructions[ip];
                const auto& o = operations.find(i.op);
                if (o != operations.end()) {
                    o->second(ip, accumulator, i.param);
                }
                else {
                    std::cerr << "Unknown opcode: " << i.op << " at " << ip << "!\n";
                    return -1;
                }
            }
            return accumulator;
        }
    };
}
