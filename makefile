CPP=g++
LFLAGS=-lpthread -fopenmp
CFLAGS=-c -g -O1 -Wall -fopenmp# -D DEBUG

all: knapsack generator

generator: generator.cpp
	$(CPP) $< -o $@

knapsack: main.o knapalg.o
	$(CPP) $(LFLAGS) $+ -o $@

main.o: main.cpp
	$(CPP) $(CFLAGS) $<

knapalg.o: knapalg.cpp knapalg.h
	$(CPP) $(CFLAGS) $<

clean:
	rm -f *.o knapsack generator
