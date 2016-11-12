CPP=g++
CFLAGS=-c -g -Wall

all: knapsack

knapsack: main.o knapalg.o
	$(CPP) $+ -o $@

main.o: main.cpp
	$(CPP) $(CFLAGS) $<

knapalg.o: knapalg.cpp knapalg.h
	$(CPP) $(CFLAGS) $<

clean:
	rm -f *.o knapsack
