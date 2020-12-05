#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>

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

        BoardingPass() {};
        BoardingPass& operator=(const BoardingPass& other) {
            row = other.row;
            col = other.col;
            id = other.id;
            return *this;
        }

        auto operator<(const BoardingPass& other) const -> bool {
            return id < other.id;
        }
    };

    typedef std::unordered_map<std::string, std::string> Passport;

    typedef std::map<std::size_t, char> MapDataLine;
    typedef std::map<std::size_t, MapDataLine> MapData;
    
    template<class T>
    auto convert(std::string input) -> T;

    template<>
    auto convert(std::string input) -> long {
        return std::stol(input);
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
            PasswordRule rule;
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

    template<>
    auto readFromFile<Passport, std::vector<Passport>>(const char* file, std::back_insert_iterator<std::vector<Passport>> target) -> std::size_t {
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
                for (const auto &kv : getKeyValuePairs(line))
                    np.emplace(kv);
                set = true;
            }
        }
        if (set) {
            *target++ = np;
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

}
