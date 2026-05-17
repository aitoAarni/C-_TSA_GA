#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>
#include <format>
#include "utils.hpp"

constexpr int MUTATION_RATE {5}; // in percentages
constexpr int POPULATION_COUNT {5};
const std::string INPUT_FILE {"../run/input.dat"};

// TODO: generate initial mutations (and distributed them to threads) 

// in threads
// take in population, 
// TODO: generate next generation, choose from current population a subset and find 2 best distances,
// and create a child

// DO this 





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

int calc_city_distance(City& a, City& b) {
    int x = std::pow(a.x - b.x, 2);
    int y = std::pow(a.y - b.y, 2);
    return std::sqrt(x+y);

}

int calc_route_distance(std::vector<int>& route, std::vector<City>& city_coordinates) {

    auto current {route.begin()};
    int distance {0};
    while (current+1 != route.end()) {
        distance += calc_city_distance(city_coordinates[*current], city_coordinates[*(current + 1)]);
        // std::cout << std::format("From city {} (x: {}, y: {}) to {} (x: {}, y: {}))", *current);
        current++;
    }
    distance += calc_city_distance(city_coordinates[*current], city_coordinates[*route.begin()]);
    return distance;
}


void tsp_solver(std::vector<std::vector<int>>& population, std::vector<City>& city_coordinates) {
    int answer = calc_route_distance(population[0], city_coordinates);
    std::cout << population << city_coordinates;
    std::cout << "ansewr is: " << answer << "\n";
}

int main() {
    auto city_locations {read_input(INPUT_FILE)};
    std::size_t n {city_locations.size()};

    std::vector<int> base_route(n);
    std::iota(base_route.begin(), base_route.end(), 0);
    std::vector<std::vector<int>> pop {base_route};
    tsp_solver(pop, city_locations);
    // auto population {generate_population(n, POPULATION_COUNT)};
    // std::cout << " " << population;


    return 0;
}