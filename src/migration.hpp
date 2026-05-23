#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include <numeric>

std::vector<int> get_route_distances(std::vector<std::vector<int>> &current_generation);

// Lock-free struct for shared data when doing migrations
struct MigrationStruct
{

    int thread_count;
    std::unique_ptr<std::atomic<bool>[]> fresh_data_flags;
    std::vector<std::vector<std::vector<int>>> routes;
    MigrationStruct(int thread_c) : thread_count(thread_c), fresh_data_flags(std::make_unique<std::atomic<bool>[]>(thread_c)), routes(thread_c) {}
};

// Get indices for the n best routes to be sent to an adjacent thread
std::vector<int> get_best_routes(const std::vector<int> &route_distances, int n)
{
    int actual_n = std::min(n, static_cast<int>(route_distances.size()));
    std::vector<int> indices(route_distances.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::partial_sort(indices.begin(), indices.begin() + actual_n, indices.end(),
                      [&route_distances](int a, int b)
                      {
                          return route_distances[a] < route_distances[b];
                      });

    indices.resize(actual_n);
    return indices;
}

// Get indices for the n worst routes to be replaced by an adjacent thread
std::vector<int> get_worst_routes(const std::vector<int> &route_distances, int n)
{
    int actual_n = std::min(n, static_cast<int>(route_distances.size()));
    std::vector<int> indices(route_distances.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::partial_sort(indices.begin(), indices.begin() + actual_n, indices.end(),
                      [&route_distances](int a, int b)
                      {
                          return route_distances[a] > route_distances[b];
                      });

    indices.resize(actual_n);
    return indices;
}

// The migration orchestrator function
void execute_migration(
    std::vector<std::vector<int>> &current_generation,
    MigrationStruct &migration_routes, // shared data routes between threads with a flag to avoid data race
    int thread_id)
{
    auto route_distances = get_route_distances(current_generation);
    
    int write_index{thread_id == migration_routes.thread_count - 1 ? 0 : thread_id + 1};

    int send_count = migration_routes.routes[write_index].size();
    int receive_count = migration_routes.routes[thread_id].size();
    auto best_route_indexes = get_best_routes(route_distances, send_count);
    auto worst_route_indexes = get_worst_routes(route_distances, receive_count);


    // Send best routes to adjacent thread
    migration_routes.fresh_data_flags[write_index].wait(true);
    for (int i{0}; i < best_route_indexes.size(); i++)
    {
        migration_routes.routes[write_index][i] = current_generation[best_route_indexes[i]];
    }
    migration_routes.fresh_data_flags[write_index] = true;
    migration_routes.fresh_data_flags[write_index].notify_one();

    // Receive best routes from the adjacent thread
    migration_routes.fresh_data_flags[thread_id].wait(false);
    for (int i{0}; i < worst_route_indexes.size(); i++)
    {
        // don't replace thread's best route
        if (worst_route_indexes[i] != 0)
        {
            current_generation[worst_route_indexes[i]] = migration_routes.routes[thread_id][i];
        }
    }
    migration_routes.fresh_data_flags[thread_id] = false;
    migration_routes.fresh_data_flags[thread_id].notify_one();
}