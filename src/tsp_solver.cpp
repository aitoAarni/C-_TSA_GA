#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include "utils.hpp"

constexpr int MUTATION_RATE {5}; // in percentages
constexpr std::string input_file {"./run/input.dat"};

// TODO: City data generator
// TODO: read data to vector, to get cities and their locations
// TODO: generate initial mutations (and distributed them to threads) 

// in threads
// TODO: generate next generation, choose from current population a subset and find 2 best distances,
// and create a child

// DO this 



struct City {
    int x;
    int y;
};

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

int main() {
    auto cities {read_input(input_file)};
    print_vector(cities);
    return 0;
}