CXX=g++
CXXFLAGS=-std=c++14 -g -Wall -Wextra -Wshadow

all: main

clean:
	rm -rf *.o main tests

main: main.o armystack.o
	$(CXX) $(CXXFLAGS) -o main main.o armystack.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

armystack.o: armystack.h armystack.cpp
	$(CXX) $(CXXFLAGS) -c armystack.cpp
