#pragma once
#include <string_view>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <atomic>
#include <memory>
#include <string>
#include <random>
#include <numeric>
#include <algorithm>


template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec)
{
    for (const T &value : vec)
    {
        os << value << " ";
    }
    os << "\n";
    return os;
}

template <typename T>
void print_vector(const std::vector<T> &vec)
{
    for (const T &value : vec)
    {
        std::cout << value << " ";
    }
}

struct City
{
    int x;
    int y;
};

std::ostream &operator<<(std::ostream &os, const City &city);

std::vector<City> read_input(const std::string &);

struct Args
{
    int threads{1};
    int generations {100};
    int population_size {100};
    int migration_frequency {50};
    std::string input_file {"../run/input.dat"};
};

Args parse_args(int, char *argv[], Args&);

std::vector<std::vector<std::vector<int>>> partition_population(std::vector<std::vector<int>> &population, int population_size, int thread_count);

int calc_route_distance(const std::vector<int> &route);

void print_global_champion(const std::vector<std::vector<std::vector<int>>>& partitioned_population);

std::vector<std::vector<int>> generate_population(const int city_count, const int path_count);