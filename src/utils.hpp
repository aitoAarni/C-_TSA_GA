#pragma once
#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (const T& value : vec) {
        os << value << " ";
    }
    os << "\n";
    return os;
}

template <typename T>
void print_vector(std::vector<T>& vec) {
    for (const T& value : vec) {
        std::cout << value << " ";
    }
    std::cout << "\n";
}


struct City {
    int x;
    int y;
};

std::ostream& operator<<(std::ostream& os, const City& city) {
    return os << city.x << " " << city.y;
}
