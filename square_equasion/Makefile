CXX=g++
CXXFLAGS=-std=c++14 -g -Wall -Wextra -Wshadow

all: main run_tests

clean:
	rm -rf *.o main tests

main: main.cpp solve_square.o
	$(CXX) $(CXXFLAGS) -o main main.cpp solve_square.o

solve_square.o: solve_square.h solve_square.cpp
	$(CXX) $(CXXFLAGS) -c solve_square.cpp

tests: tests.cpp solve_square.o
	$(CXX) $(CXXFLAGS) -I googletest/googletest/include/ -pthread googletest/googletest/make/gtest-all.o -o tests tests.cpp solve_square.o

run_tests: tests
	./tests
