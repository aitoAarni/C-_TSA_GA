#pragma once
#include <string_view>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <atomic>

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
};

Args parse_args(int, char *argv[]);

std::vector<std::vector<std::vector<int>>> partition_population(std::vector<std::vector<int>> &population, int population_size, int thread_count);

struct MigrationStruct
{

    int thread_count;
    std::vector<std::atomic<int>> fresh_data_flags{false};
    std::vector<std::vector<std::vector<int>>> routes;
    MigrationStruct(int thread_c) : thread_count(thread_c), fresh_data_flags(thread_c), routes(thread_c) {}
};