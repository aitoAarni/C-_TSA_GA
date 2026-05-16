#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <numeric>
#include <algorithm>
#include <random>
#include <math>
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
    int y = std::pow(y.y - b.y, 2);
    return std::sqrt(x+y)

}

int calc_route_distance(std::vector<int>& route, std::vector<City>& city_coordinates) {
    auto current {route.begin()};
    int distance {0};
    while (current+1 != route.end()) {
        distance += calc_city_distance(*current, *(current + 1));
        current++;
    }
    distance += calc_city_distance(*current, *route.begin());
    return distance
}


void tsp_solver(std::vector<std::vector<int>>& population) {
    
}

int main() {
    auto city_locations {read_input(INPUT_FILE)};
    std::size_t n {city_locations.size()};

    auto population {generate_population(n, POPULATION_COUNT)};
    std::cout << " " << population;


    return 0;
}