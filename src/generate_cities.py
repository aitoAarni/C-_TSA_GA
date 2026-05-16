from random import randint

def get_random_city(x: int = 10_000, y: int = 10_000):
    x_coord = randint(0, x)
    y_coord = randint(0, y)
    return (x_coord, y_coord)

def generate_cities(n: int = 20):
    cities = set()

    while len(cities) < n:
        cities.add(get_random_city())

    return list(cities)


def save_cities_to_file(cities: list, filename: str = "./run/input.dat"):
    with open(filename, "w") as f:
        for x, y in cities:
            f.write(f"{x} {y}\n")


if __name__ == "__main__":
    cities = generate_cities()
    save_cities_to_file(cities)