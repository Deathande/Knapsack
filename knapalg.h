#ifndef __knap_alg
#define __knap_alg

#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include <queue>
#include <stdio.h>
#include "node.h"

#define THREADS 2

using namespace std;

// The data type for all the data read into
// memory
typedef unsigned int d_type;

void buffered_table(d_type* weights, 
                    d_type* values, 
		    int max_weight, 
		    int num_items, 
		    int buffer_size);

vector<int> get_items(d_type* weights, 
                      d_type* values, 
		      int max_weight, 
		      int num_items, 
		      int buffer_size);

void start_table(int bs, int mw);
void init_buff(int bs, int mw);
void write_to_disk(int&);

#endif
