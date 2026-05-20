#include "utils.hpp"

std::vector<City> read_input(const std::string& filename) {
    std::vector<City> cities;
    std::ifstream is {filename};

    if (!is.is_open()) {
        std::cerr << "CRITICAL ERROR: Could not open file: " << filename << "\n";
        std::cerr << "Check your file path or your current terminal directory.\n";
        std::cerr << "You should execute commands from inside the src directory\n";
        std::exit(EXIT_FAILURE);
    }

    int x {};
    int y {};

    while (is >> x >> y) {
        cities.emplace_back(x, y);
    }
    return cities;
}

std::ostream& operator<<(std::ostream& os, const City& city) {
    return os << city.x << " " << city.y;
}