#ifndef __knap_alg
#define __knap_alg

#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>

#define THREADS 2

using namespace std;

// The data type for all the data read into
// memory
typedef unsigned int d_type;

d_type** build_table(d_type* weights, d_type* values, int max_weight, int num_items);
d_type** buffered_table(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size);
vector<int> get_items(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size);
d_type** init_table(int max_weight, int num_items);

#endif
