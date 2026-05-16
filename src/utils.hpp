#pragma once
#include <iostream>
#include <vector>

template <typename T>
void print_vector(std::vector<T>& vec) {
    for (const T& value : vec) {
        std::cout << value << "\n";
    }
}