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

constexpr int MUTATION_RATE{5}; // in percentages
constexpr int POPULATION_SIZE{10};
constexpr int GENERATIONS{8};
constexpr int PARENT_GROUP_SIZE{4}; // the group size from which we will seek the most fit
                                    // parents to make a child to the new generation
const std::string INPUT_FILE{"../run/input.dat"};
std::vector<City> city_locations;

std::vector<std::vector<int>> generate_population(const int city_count, const int path_count)
{
    std::vector<int> base_route(city_count);
    std::iota(base_route.begin(), base_route.end(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::vector<int>> population(path_count);
    for (int i{0}; i < path_count; i++)
    {
        auto new_route = base_route;
        std::shuffle(new_route.begin(), new_route.end(), gen);
        population[i] = new_route;
    }
    return population;
}

int calc_city_distance(const City &a, const City &b)
{
    int x = std::pow(a.x - b.x, 2);
    int y = std::pow(a.y - b.y, 2);
    return std::sqrt(x + y);
}

int calc_route_distance(const std::vector<int> &route)
{

    auto current{route.begin()};
    int distance{0};
    while (current + 1 != route.end())
    {
        distance += calc_city_distance(city_locations[*current], city_locations[*(current + 1)]);
        current++;
    }
    distance += calc_city_distance(city_locations[*current], city_locations[*route.begin()]);
    return distance;
}

std::vector<int> reproduce(std::vector<int> &parent_a, std::vector<int> &parent_b, std::mt19937 &gen)
{
    std::vector<bool> visited(parent_a.size(), false);
    std::vector<int> child(parent_a.size());
    // decide which one is first city without modulo (first bit of random number decides it)
    child[0] = gen() & 1 ? parent_a[0] : parent_b[0];
    visited[child[0]] = true;
    int smaller_city;
    for (int i{1}; i < child.size(); i++)
    {

        if (!visited[parent_a[i]] && !visited[parent_b[i]])
        {
            auto dist_a = calc_city_distance(city_locations[parent_a[i]], city_locations[child[i - 1]]);
            auto dist_b = calc_city_distance(city_locations[parent_b[i]], city_locations[child[i - 1]]);
            child[i] = dist_a < dist_b ? parent_a[i] : parent_b[i];
        }
        else if (visited[parent_a[i]] && !visited[parent_b[i]])
        {
            child[i] = parent_b[i];
        }
        else if (visited[parent_b[i]] && !visited[parent_a[i]])
        {
            child[i] = parent_a[i];
        }
        else
        {
            for (int city = 0; city < child.size(); ++city)
            {
                if (!visited[city])
                {
                    child[i] = city;
                    break;
                }
            }
        }
        visited[child[i]] = true;
    }
    return child;
}

std::vector<int> get_offspring(std::vector<std::vector<int>> &current_generation, std::vector<int> &route_distances, std::mt19937 &gen)
{
    std::size_t population_size{current_generation.size()};
    std::vector<int> parent_pool(PARENT_GROUP_SIZE);
    unsigned int previous_index{2'000'000'000};

    std::uniform_int_distribution<int> parent_dist(0, population_size - 1);
    // generate new generation in this loop
    // for (int i {1}; i < population_size; i++) {

    for (int j{0}; j < PARENT_GROUP_SIZE; j++)
    {
        // in this loop we pick the potential parents for a child
        unsigned long index{};
        // make sure potential parent's are not all the same index
        do
        {
            index = parent_dist(gen);
        } while (index == previous_index);
        parent_pool[j] = index;
        previous_index = index;
    }

    // here we pick the 2 fittest parents to make a child
    std::sort(parent_pool.begin(), parent_pool.end(), [&route_distances](const int a, const int b)
              { return route_distances[a] < route_distances[b]; });
    int parent_1 = parent_pool[0];
    int parent_2{};
    int i{0};
    do
    {
        i++;
        parent_2 = parent_pool[i];
    } while (parent_1 == parent_2);

    return reproduce(current_generation[parent_1], current_generation[parent_2], gen);
}

void mutate(std::vector<int> &route, std::uniform_int_distribution<int> &index_distribution, std::mt19937 &gen)
{
    int i_1 = index_distribution(gen);
    int i_2{};
    do
    {

        i_2 = index_distribution(gen);
    } while (i_1 == i_2);
    std::swap(route[i_1], route[i_2]);
}

void run_one_generation(std::vector<std::vector<int>> &current_generation, std::vector<std::vector<int>> &new_generation, std::mt19937 &gen)
{
    std::bernoulli_distribution mutation_chance(MUTATION_RATE / 100.0);
    std::uniform_int_distribution<int> mutation_index(0, current_generation[0].size() - 1);
    // keep the shortest route at index 0
    int shortest_route{calc_route_distance(current_generation[0])};
    int best_index{0};
    new_generation[0] = current_generation[0];
    std::vector<int> route_distances;
    route_distances.reserve(current_generation.size());
    for (const auto &route : current_generation)
    {
        route_distances.push_back(calc_route_distance(route));
    }
    for (int i{1}; i < current_generation.size(); i++)
    {
        auto child = get_offspring(current_generation, route_distances, gen);
        if (mutation_chance(gen))
        {
            mutate(child, mutation_index, gen);
        }
        new_generation[i] = child;
        int child_distance{calc_route_distance(child)};
        if (shortest_route > child_distance)
        {
            shortest_route = child_distance;
            best_index = i;
        }
    }
    std::swap(new_generation[0], new_generation[best_index]);
}

void tsp_solver(std::vector<std::vector<int>> &current_generation)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::vector<int>> new_generation(current_generation.size());
    new_generation[0] = current_generation[0];
    std::cout << "current_generatrion\n";
    for (const auto &path : current_generation)
    {
        print_vector(path);
        std::cout << ", distance: " << calc_route_distance(path) << "\n";
    }
    for (int i{0}; i < GENERATIONS; i++)
    {
        run_one_generation(current_generation, new_generation, gen);
        std::cout << "\nnew_generatrion\n";
        for (const auto &path : new_generation)
        {
            print_vector(path);
            std::cout << ", distance: " << calc_route_distance(path) << "\n";
        }
        std::swap(current_generation, new_generation);
    }
}

int main()
{
    std::cout << "works?\n";
    city_locations = read_input(INPUT_FILE);
    std::size_t n{city_locations.size()};

    std::vector<int> base_route(n);
    std::iota(base_route.begin(), base_route.end(), 0);
    std::vector<std::vector<int>> pop{base_route};
    auto population = generate_population(n, POPULATION_SIZE);
    tsp_solver(population);

    return 0;
}