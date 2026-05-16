#include "utils.hpp"

std::vector<City> read_input(const std::string& filename) {
    std::vector<City> cities;
    std::ifstream is {filename};

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