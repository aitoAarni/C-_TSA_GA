#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <numeric>
#include <algorithm>
#include <random>
#include "utils.hpp"

constexpr int MUTATION_RATE {5}; // in percentages
const std::string input_file {"../run/input.dat"};

// TODO: generate initial mutations (and distributed them to threads) 

// in threads
// TODO: generate next generation, choose from current population a subset and find 2 best distances,
// and create a child

// DO this 




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

std::vector<std::vector<int>> generate_population(const int city_count, const int path_count) {
    std::vector<int> base_route(city_count);
    std::iota(base_route.begin(), base_route.end(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::vector<int>> population(path_count);
    for (int i {0}; i < path_count; i++) {
        auto new_route = base_route;
        std::shuffle(new_route.begin(), new_route.end(), gen);
        population[i] = new_route;
    }
    return population;
}

int main() {
    auto city_locations {read_input(input_file)};
    std::size_t n {city_locations.size()};

    auto population {generate_population(n, 5)};
    std::cout << " " << population;


    return 0;
}