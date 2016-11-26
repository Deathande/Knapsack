#ifndef __knap_alg
#define __knap_alg

#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include "node.h"

#define THREADS 4

using namespace std;

// The data type for all the data read into
// memory
typedef unsigned int d_type;

d_type** buffered_table(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size);
vector<int> get_items(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size);
void start_table(int bs, int mw);

#endif
