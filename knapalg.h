#ifndef __knap_alg
#define __knap_alg

#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <map>
#include <cmath>

using namespace std;

// The data type for all the data read into
// memory
typedef unsigned int d_type;

struct indi_score
{
  int index;
  int score;
};

vector<int> get_indicies(vector<d_type> weights,
                         vector<d_type> values,
			 int max_weight,
			 int num,
			 int iterations);

double average(vector<d_type> data);
vector< vector<int> > generate_pop(int num);
vector<int> gen_member();
void sort_populus(vector<indi_score> &pop);
void mix(vector<indi_score> &rated, vector< vector<int> > &pop);
void correct_vect(vector<int> &vect);
void mutate_add(vector<int> &vect);
void mutate_change(vector<int> &vect);

#endif
