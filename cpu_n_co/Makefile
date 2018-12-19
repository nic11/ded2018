CXX=g++
CXXFLAGS=-std=c++14 -g -Wall -Wextra -Wshadow

all: cpu assembler

clean:
	rm -rf *.o main tests

cpu: cpu_main.cpp cpu.o
	$(CXX) $(CXXFLAGS) -o cpu cpu.o cpu_main.cpp

cpu.o: cpu.cpp
	$(CXX) $(CXXFLAGS) -c cpu.cpp

assembler: assembler_main.cpp assembler.o
	$(CXX) $(CXXFLAGS) -o assembler assembler.o assembler_main.cpp

assembler.o: assembler.cpp
	$(CXX) $(CXXFLAGS) -c assembler.cpp

armystack.o: ../stack/armystack.h ../stack/armystack.cpp
	$(CXX) $(CXXFLAGS) -c ../stack/armystack.cpp
