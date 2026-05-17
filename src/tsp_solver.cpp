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
constexpr int POPULATION_SIZE {10};
constexpr int GENERATIONS {10};
constexpr int PARENT_GROUP_SIZE {4};    // the group size from which we will seek the most fit
                                        // parents to make a child to the new generation
const std::string INPUT_FILE {"../run/input.dat"};
std::vector<City> city_locations;
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

int calc_city_distance(const City& a, const City& b) {
    int x = std::pow(a.x - b.x, 2);
    int y = std::pow(a.y - b.y, 2);
    return std::sqrt(x+y);

}

int calc_route_distance(const std::vector<int>& route) {

    auto current {route.begin()};
    int distance {0};
    while (current+1 != route.end()) {
        distance += calc_city_distance(city_locations[*current], city_locations[*(current + 1)]);
        current++;
    }
    distance += calc_city_distance(city_locations[*current], city_locations[*route.begin()]);
    return distance;
}

void run_one_generation(std::vector<std::vector<int>>& current_generation, std::vector<std::vector<int>>& new_generation, std::mt19937& gen) {
    std::vector<int> route_distances;
    route_distances.reserve(current_generation.size());
    for (const auto& route : current_generation) {
        route_distances.push_back(calc_route_distance(route));
    }
    auto population_size {current_generation.size()};
    std::cout << "routes:\n " << current_generation;
    std::cout << "route_distances:\n " << route_distances;
    std::vector<int> parent_pool(PARENT_GROUP_SIZE);
    unsigned int previous_index {2'000'000'000};

    // generate new generation in this loop
    // for (int i {1}; i < population_size; i++) {

        for (int j {0}; j < PARENT_GROUP_SIZE; j++) {
            // in this loop we pick the potential parents for a child
            unsigned long index {};
            // make sure potential parent's are not all the same index
            do {
                index = {gen() % population_size};
            } while (index == previous_index);
            parent_pool[j] = index;
            previous_index = index;
        }

        // here we pick the 2 fittest parents to make a child
        std::sort(parent_pool.begin(), parent_pool.end(), [&route_distances](const int a, const int b){
            return route_distances[a] < route_distances[b];
        });
        std::cout << "sorted parent pool\n " << parent_pool;
        std::cout << "parent pool sizes in order:\n\n";
        for (const auto& index : parent_pool) {
            std::cout << route_distances[index] << "\n";
        }
        std::cout << "\n";
    // }
}

void tsp_solver(std::vector<std::vector<int>>& current_generation) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::vector<int>> new_generation(current_generation.size());
    run_one_generation(current_generation, new_generation, gen);

    // for (int i {0}; i < GENERATIONS; i++) {

    // }
}

int main() {
    city_locations = read_input(INPUT_FILE);
    std::size_t n {city_locations.size()};

    std::vector<int> base_route(n);
    std::iota(base_route.begin(), base_route.end(), 0);
    std::vector<std::vector<int>> pop {base_route};
    auto population = generate_population(n, POPULATION_SIZE);
    tsp_solver(population);
    // auto population {generate_population(n, POPULATION_SIZE)};
    // std::cout << " " << population;


    return 0;
}