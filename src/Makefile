CPPFLAGS = -std=c++23 -Wall -Wextra -Wshadow -pedantic -march=native

HEADERS = src/utils.hpp

.PHONY: clean debug

build: tsp_solver.cpp $(HEADERS)
	g++ $(CPPFLAGS) -DNDEBUG -Ofast -o tsp_solver tsp_solver.cpp

clean:
	rm -f tsp_solver

debug: tsp_solver.cpp $(HEADERS)
	g++ $(CPPFLAGS) -DDEBUG -g -o tsp_solver tsp_solver.cpp


%.hpp: