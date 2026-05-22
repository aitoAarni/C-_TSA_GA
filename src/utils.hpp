#pragma once
#include <string_view>
#include <iostream>
#include <vector>
#include <fstream>

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
    int threads;
};

Args parse_args(int, char *argv[]);