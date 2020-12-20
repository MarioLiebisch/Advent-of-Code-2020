#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iomanip>
#include <set>

class Image;

class SubImage {
    std::size_t borders[8] = {};
    std::vector<std::string> data;
    std::size_t width = 0;
    std::size_t height = 0;

    enum Edge {
        None = 0,
        T = 1,
        S = 2,
        L = 4,
        R = 8
    };

    unsigned char edges = Edge::None;

public:
    auto update_borders() -> void {
        if (data.empty()) {
            return;
        }
        borders[0] = 0; // Top
        borders[1] = 0; // Bottom
        borders[2] = 0; // Left
        borders[3] = 0; // Right

        borders[4] = 0; // Top flipped
        borders[5] = 0; // Bottom flipped
        borders[6] = 0; // Left flipped
        borders[7] = 0; // Right flipped

        const auto& first = *this->data.begin();
        const auto& last = *this->data.rbegin();

        // Let's calculate unique border values
        // by using the pixels as bits.
        // They'll be LSB/MSB order based on orientation
        // To allow them to match after rotation.

        // From LSB to MSB:
        // Top: Left to right
        // Bottom: Right to left
        // Left: Bottom to top
        // Right: Top to bottom
        // Flipped indices have the opposite directions

        // Recalc top/bottom
        for (std::size_t i = 0; i < width; ++i) {
            if (first[width - i - 1] == '#') {
                borders[0] |= 1ull << i;
                borders[4] |= 1ull << (width - i - 1);
            }
            if (last[i] == '#') {
                borders[1] |= 1ull << i;
                borders[5] |= 1ull << (width - i - 1);
            }
        }
        // Recalc left/right
        for (std::size_t i = 0; i < height; ++i) {
            if (data[i][0] == '#') {
                borders[2] |= 1ull << i;
                borders[6] |= 1ull << (height - i - 1);
            }
            if (data[height - i - 1][height - 1] == '#') {
                borders[3] |= 1ull << i;
                borders[7] |= 1ull << (height - i - 1);
            }
        }
    
    }

    auto flip() -> void {
        std::reverse(data.begin(), data.end());
        update_borders();
    }

    auto append(const std::string& data) -> void {
        this->data.push_back(data);
        ++height;

        if (height == 1) {
            width = data.length();
        }
        else {
            // Make sure all lines are equal length
            assert(width == data.length());
        }
        update_borders();
    }

    auto rotate() -> void {
        // Only allow rotation for quadratic images
        assert(width == height);
        const std::vector<std::string> old = data;
        for (std::size_t y = 0; y < height; ++y) {
            for (std::size_t x = 0; x < width; ++x) {
                data[y][x] = old[height - x - 1][y];
            }
        }
        // Could just move the values but don't want to bother now
        update_borders();
    }

    auto roughness() -> std::size_t {
        std::size_t res = 0;
        for (const auto& line : data) {
            for (const auto& c : line) {
                if (c == '#') {
                    ++res;
                }
            }
        }
        return res;
    }

    auto remove_pattern(const SubImage& pattern) -> bool {
        const std::size_t pw = pattern.width;
        const std::size_t ph = pattern.height;
        std::vector<std::string> work = data;
        std::size_t count = 0;
        for (std::size_t yo = 0; yo < height - ph; ++yo) {
            for (std::size_t xo = 0; xo < width - pw; ++xo) {
                std::size_t found = 0;
                for (std::size_t y = 0; y < ph && found != -1; ++y) {
                    for (std::size_t x = 0; x < pw; ++x) {
                        if (pattern.data.at(y).at(x) == '#') {
                            if (work.at(yo + y).at(xo + x) == '#') {
                                ++found;
                                work.at(yo + y).at(xo + x) = 'O';
                            }
                            else {
                                found = -1;
                                break;
                            }
                        }
                    }
                }
                if (found && found != -1) {
                    data = work;
                    update_borders();
                    ++count;
                }
                else {
                    work = data;
                }
            }
        }
        return count;
    }

    friend std::ostream& operator<<(std::ostream& stream, const SubImage& image);
    friend std::ostream& operator<<(std::ostream& stream, const Image& image);
    friend class Image;
};

std::ostream& operator<<(std::ostream& stream, const SubImage& image) {
    for (const auto& s : image.data) {
        stream << s << "\n";
    }
    return stream;
}

class Image {
    std::unordered_map<std::size_t, SubImage> subs;
    std::vector<std::size_t> order;
    std::size_t width = 0;
    std::size_t height = 0;
    std::size_t sub_width = 0;
    std::size_t sub_height = 0;
public:

    auto solve_part1() -> std::size_t {
        reorder_tiles();
        return order[0] * order[width - 1] * order[(height - 1) * width] * order[height * width - 1];
    }

    auto reorder_tiles() -> void {
        std::vector<std::size_t> corners;
        std::set<std::size_t> outside_borders;

        std::map<std::size_t, std::size_t> border_counts;
        for (const auto& s : subs) {
            for (std::size_t i = 0; i < 8; ++i) {
                ++border_counts[s.second.borders[i]];
            }
        }

        std::unordered_map<std::size_t, std::set<long long>> border_to_tile;

        for (const auto& s : subs) {
            std::set<std::size_t> connections;
            SubImage& si = subs.at(s.first);
            // Only have to check the first 4 values,
            // the others are duplicates
            for (std::size_t i = 0; i < 4; ++i) {
                if (border_counts[si.borders[i]] == 2) {
                    connections.insert(i);
                    border_to_tile[si.borders[i]].insert(s.first);
                    border_to_tile[si.borders[i + 4]].insert(-static_cast<long long>(s.first)); // Identify flipped association
                }
                else {
                    outside_borders.insert(si.borders[i]);
                    outside_borders.insert(si.borders[i + 4]);
                }
            }

            if (connections.size() == 2) { // Corner
                corners.push_back(s.first);
            }
            /* else if (connections.size() == 3) { // Edge
                edges.push_back(s.first);
            }
            else { // Center Piece
                centers.push_back(s.first);
            } */
        }

        order.clear();
        order.resize(width * height);

        assert(corners.size() == 4);

        // Place the first corner
        long long last = order[0] = corners[0];
        SubImage* current = &subs.at(corners[0]);

        // Rotate until right/down are not outside
        while (outside_borders.find(current->borders[1]) != outside_borders.end()
            || outside_borders.find(current->borders[3]) != outside_borders.end()) {
            current->rotate();
        }
        std::size_t next = current->borders[3];

        // Build the top border
        for (std::size_t i = 1; i < width; ++i) {
            long long tile = *border_to_tile[next].begin();
            // Ensure this is not the same tile as the last one,
            // there's always a second with this border
            if (tile == last || -tile == last) {
                tile = *border_to_tile[next].rbegin();
            }
            assert(tile);

            if (tile > 0) {
                current = &subs.at(tile);
                order[i] = last = tile;
            }
            else {
                current = &subs.at(-tile);
                current->flip();
                order[i] = -(last = tile);
            }
            // Make sure the connection fits
            while (current->borders[2] != next) {
                current->rotate();
            }

            // Is the top not an outside border? Flip it!
            if (outside_borders.find(current->borders[0]) == outside_borders.end()) {
                current->flip();
            }

            // get the next connection
            next = current->borders[3];
        }

        // Now everything is set, just go down from top to bottom, find and flip/rotate stuff to fit in place
        for (std::size_t x = 0; x < width; ++x) {
            last = order[x];
            next = subs.at(last).borders[1]; // Bottom border of the tile above

            for (std::size_t y = 1; y < height; ++y) {
                long long tile = *border_to_tile[next].begin();
                // Ensure this is not the same tile as the last one,
                // there's always a second with this border
                if (tile == last || -tile == last) {
                    tile = *border_to_tile[next].rbegin();
                }

                if (tile > 0) {
                    current = &subs.at(tile);
                    order[y * width + x] = last = tile;
                }
                else {
                    current = &subs.at(-tile);
                    current->flip();
                    order[y * width + x] = -(last = tile);
                }

                // Make sure the top border is the one we're looking for
                while (current->borders[0] != next) {
                    current->rotate();
                }

                // Make sure the left border is correct
                if (x == 0) {
                    if (outside_borders.find(current->borders[2]) == outside_borders.end()) {
                        // Border isn't on the left, rotate twice and flip to mirror
                        current->rotate();
                        current->rotate();
                        current->flip();
                    }
                }
                else {
                    auto& t = border_to_tile.find(current->borders[2]);
                    if (t == border_to_tile.end()) {
                        t = border_to_tile.find(current->borders[6]);
                    }
                    const long long tile_left = order[y * width + x - 1];
                    if (t == border_to_tile.end() || t->second.find(tile_left) == t->second.end()) {
                        // Connection to the left doesn't fit? rotate twice and flip to mirror
                        current->rotate();
                        current->rotate();
                        current->flip();
                    }
                }

                // get the next connection
                next = current->borders[1];
            }
        }
    }

    auto append(const std::size_t id, const SubImage& new_sub) -> void {
        if (subs.empty()) {
            sub_width = new_sub.width;
            sub_height = new_sub.height;
        }

        // Make sure all images are equal width/height
        assert(sub_width == new_sub.width && sub_height == new_sub.height);

        subs.insert(std::make_pair(id, new_sub));
        order.push_back(id);

        width = static_cast<std::size_t>(std::sqrt(subs.size()));
        height = std::max(1ull, subs.size() / width);
    }

    // Generate a full image, remove the sea monster, and count the roughness
    auto solve_part2() -> std::size_t {
        SubImage full;
        for (std::size_t yi = 0; yi < height; ++yi) {
            for (std::size_t y = 1; y < sub_height - 1; ++y) {
                std::string line;
                for (std::size_t xi = 0; xi < width; ++xi) {
                    line += subs.at(order[yi * width + xi]).data.at(y).substr(1, sub_width - 2);
                }
                full.append(line);
            }
        }

        SubImage monster; // R A W R ! 🐉
        monster.append("                  # ");
        monster.append("#    ##    ##    ###");
        monster.append(" #  #  #  #  #  #   ");

        for (std::size_t i = 0; i < 4; ++i) {
            bool changed = full.remove_pattern(monster);
            if (!changed) {
                full.flip();
                changed = full.remove_pattern(monster);
            }
            if (!changed) {
                full.flip();
                full.rotate();
            }
            else {
                std::cout << full << "\n";
                break;
            }
        }
        return full.roughness();
    }

    friend std::ostream& operator<<(std::ostream& stream, const Image& image);
};

std::ostream& operator<<(std::ostream& stream, const Image& image) {
    for (std::size_t y = 0; y < image.height; ++y) {
        for (std::size_t x = 0; x < image.width; ++x) {
            std::cout << std::setw(image.sub_width) << image.order[y * image.width + x] << "    ";
        }
        stream << "\n";
        for (std::size_t sy = 0; sy < image.sub_height; ++sy) {
            for (std::size_t x = 0; x < image.width; ++x) {
                const std::size_t& idx = image.order[y * image.width + x];
                stream << image.subs.at(idx).data.at(sy) << "    ";
            }
            stream << "\n";
        }
        stream << "\n";
    }
    return stream;
}

auto day20part1(const char* file) -> long long {
    Image img;
    std::ifstream stream(file);
    std::string line;
    while (stream.good()) {
        // Read tile id
        std::getline(stream, line);
        assert(line.substr(0, 5) == "Tile ");
        const std::size_t idx = std::stoull(line.substr(5));
        SubImage simg;
        while (std::getline(stream, line) && !line.empty()) {
            simg.append(line);
        }
        img.append(idx, simg);
    }
    return img.solve_part1();
}


auto day20part2(const char* file) -> long long {
    Image img;
    std::ifstream stream(file);
    std::string line;
    while (stream.good()) {
        // Read tile id
        std::getline(stream, line);
        assert(line.substr(0, 5) == "Tile ");
        const std::size_t idx = std::stoull(line.substr(5));
        SubImage simg;
        while (std::getline(stream, line) && !line.empty()) {
            simg.append(line);
        }
        img.append(idx, simg);
    }
    img.reorder_tiles();
    return img.solve_part2();
}

auto main(int argc, char** argv) -> int {
    const std::size_t r1 = day20part1("day-20-sample.txt");
    std::cout << "Sample 1 solution: " << r1 << "\n";
    const std::size_t r2 = day20part2("day-20-sample.txt");
    std::cout << "Sample 2 solution: " << r2 << "\n";

    const std::size_t r3 = day20part1("day-20-input.txt");
    std::cout << "Part 1 solution: " << r3 << "\n";
    const std::size_t r4 = day20part2("day-20-input.txt");
    std::cout << "Part 2 solution: " << r4 << "\n";
    return 0;
}
