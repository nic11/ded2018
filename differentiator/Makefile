CXX=g++
CXXFLAGS=-std=c++14 -g -Wall -Wextra -Wshadow

all: main

clean:
	rm -rf *.o main

main: main.cpp rpn.o expr.o differentiate.o
	$(CXX) $(CXXFLAGS) -o main main.cpp rpn.o expr.o differentiate.o

rpn.o: rpn.cpp
	$(CXX) $(CXXFLAGS) -c rpn.cpp

expr.o: expr.cpp
	$(CXX) $(CXXFLAGS) -c expr.cpp

differentiate.o: differentiate.cpp
	$(CXX) $(CXXFLAGS) -c differentiate.cpp
