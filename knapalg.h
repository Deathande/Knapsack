#ifndef __knap_alg
#define __knap_alg

#include <algorithm>
#include <iostream>

using namespace std;

// The data type for all the data read into
// memory
typedef int d_type;

d_type** build_table(d_type* weights, d_type* values, int max_weight, int num_items, int buff_size);
d_type** init_table(int max_weight, int num_items);

#endif
