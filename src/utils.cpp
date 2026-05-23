#include "utils.hpp"

std::vector<City> read_input(const std::string &filename)
{
    std::vector<City> cities;
    std::ifstream is{filename};

    if (!is.is_open())
    {
        std::cerr << "CRITICAL ERROR: Could not open file: " << filename << "\n";
        std::cerr << "Check your file path or your current terminal directory.\n";
        std::cerr << "You should execute commands from inside the src directory\n";
        std::exit(EXIT_FAILURE);
    }

    int x{};
    int y{};

    while (is >> x >> y)
    {
        cities.emplace_back(x, y);
    }
    return cities;
}

std::ostream &operator<<(std::ostream &os, const City &city)
{
    return os << city.x << " " << city.y;
}

Args parse_args(int argc, char *argv[], Args& args)
{
    for (int i{1}; i < argc; i++)
    {
        std::string_view arg{argv[i]};
        if (arg == "-t" && i + 1 < argc)
        {
            args.threads = std::stoi(argv[++i]);
        }
        else if (arg == "-g" && i + 1 < argc)
        {
            args.generations = std::stoi(argv[++i]);
        }
        else if (arg == "-p" && i + 1 < argc)
        {
            args.population_size = std::stoi(argv[++i]);
        }
        else if (arg == "-m" && i + 1 < argc)
        {
            args.migration_frequency = std::stoi(argv[++i]);
        }
        else if (arg == "-i" && i + 1 < argc)
        {
            args.input_file = argv[++i];
        }
    }
    return args;
}

std::vector<std::vector<std::vector<int>>> partition_population(std::vector<std::vector<int>> &population, int population_size, int thread_count)
{
    int partition_size = std::ceil(double(population_size) / thread_count);
    std::vector<std::vector<std::vector<int>>> partitioned_population;

    int start{0};
    int end{partition_size};
    for (int i{0}; i < thread_count; i++)
    {
        partitioned_population.emplace_back(population.begin() + start, population.begin() + end);
        start += partition_size;
        end = std::min(start + partition_size, population_size);
    }
    return partitioned_population;
}

void print_global_champion(const std::vector<std::vector<std::vector<int>>>& partitioned_population)
{
    int global_best_distance = 2'000'000'000;
    std::vector<int> global_best_route;

    std::cout << "\n--- Final Island Results ---\n";
    for (std::size_t i{0}; i < partitioned_population.size(); i++)
    {
        const auto& island_best_route = partitioned_population[i][0];
        int distance = calc_route_distance(island_best_route);

        std::cout << "Thread " << i << " best distance: " << distance << "\n";

        if (distance < global_best_distance)
        {
            global_best_distance = distance;
            global_best_route = island_best_route;
        }
    }

    std::cout << "\n==================================================\n";
    std::cout << " GLOBAL BEST ROUTE FOUND:\n";
    std::cout << "==================================================\n";
    print_vector(global_best_route);
    std::cout << "\nUltimate Distance: " << global_best_distance << "\n\n";
}

void print_intermediate_results(int generation, int thread_id, const std::vector<int>& best_route)
{
    // The static mutex ensures only ONE thread can print to the terminal at a time
    static std::mutex print_mutex;
    int distance = calc_route_distance(best_route);

    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "[Thread " << thread_id << " | Gen " << generation << "] Best Distance: " << distance << "\n";
}