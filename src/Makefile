CXX = g++
CPPFLAGS = -std=c++23 -Wall -Wextra -Wshadow -pedantic -march=native -pthread

HEADERS = utils.hpp

SOURCES = tsp_solver.cpp utils.cpp

.PHONY: clean debug build

build: tsp_solver.cpp $(HEADERS)
	g++ $(CPPFLAGS) -DNDEBUG -Ofast -o tsp_solver $(SOURCES)

clean:
	rm -f tsp_solver

debug: tsp_solver.cpp $(HEADERS)
	g++ $(CPPFLAGS) -DDEBUG -g -o tsp_solver tsp_solver.cpp

