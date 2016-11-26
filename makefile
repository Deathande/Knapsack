CPP=g++
LFLAGS=-lpthread -fopenmp
CFLAGS=-c -g -Wall -fopenmp #-D DEBUG

all: knapsack generator

generator: generator.cpp
	$(CPP) $< -o $@

knapsack: main.o knapalg.o
	$(CPP) $(LFLAGS) $+ -o $@

main.o: main.cpp
	$(CPP) $(CFLAGS) $<

knapalg.o: knapalg.cpp knapalg.h node.h node.cpp
	$(CPP) $(CFLAGS) $<

clean:
	rm -f *.o knapsack generator
