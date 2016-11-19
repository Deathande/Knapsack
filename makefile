CPP=g++
LFLAGS=-lpthread -fopenmp
CFLAGS=-c -g -Wall -fopenmp

all: knapsack

knapsack: main.o knapalg.o
	$(CPP) $(LFLAGS) $+ -o $@

main.o: main.cpp
	$(CPP) $(CFLAGS) $<

knapalg.o: knapalg.cpp knapalg.h
	$(CPP) $(CFLAGS) $<

clean:
	rm -f *.o knapsack
