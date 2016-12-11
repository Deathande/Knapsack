#ifndef __knap_alg
#define __knap_alg

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <utility>

using namespace std;

// The data type for all the data read into
// memory
typedef unsigned int d_type;

vector<int> get_indicies(vector<d_type> weights,
                         vector<d_type> values,
			 int max_weight,
			 int num);

double average(vector<d_type> data);
vector< vector<d_type> > generate_pop(int num, double average_weight);
vector<d_type> gen_member(double average_weight);

#endif
