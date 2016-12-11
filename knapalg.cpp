#include "knapalg.h"

#define THREADS 2
int mw;
int ni;

vector<int> get_indicies(vector<d_type> weights,
                         vector<d_type> values,
			 int max_weight,
			 int num)
{
  vector< vector<int> > population;
  vector<int> best;
  int best_score = 0;
  vector<indi_score> rated(num); // first = index; second = score
  mw = max_weight;
  ni = values.size();
  double average_weight = average(weights);
  population = generate_pop(num, average_weight);
  #ifdef DEBUG
    cout << "population:" << endl;
    for (int i = 0; i < population.size(); i++)
    {
      cout << "{ ";
      for (int j = 0; j < population[i].size(); j++)
        cout << population[i][j] << " ";
      cout << "}";
      cout << endl;
    }
  #endif
  int sum_values;
  int sum_weights;
  for (int i = 0; i < population.size(); i++)
  {
    sum_weights = 0;
    for (int j = 0; j < population[i].size(); j++)
      sum_weights += weights[population[i][j]];
    rated[i].index = i;
    if (sum_weights > max_weight)
    {
      rated[i].score = -sum_weights;
    }
    else
    {
      sum_values = 0;
      for (int j = 0; j < population[i].size(); j++)
      {
        sum_values += values[population[i][j]];
      }
      rated[i].score = sum_values;
    }
  }
  sort_populus(rated);
  #ifdef DEBUG
    cout << "sorted population:" << endl;
    for (int i = 0; i < rated.size(); i++)
    {
      cout << "{ ";
      for (int j = 0; j < population[rated[i].index].size(); j++)
        cout << population[rated[i].index][j] << " ";
      cout << "}";
      cout << " " << rated[i].score << endl;
    }
  #endif

  if (rated[0].score > best_score)
    best = population[rated[0].index];

  //mix(rated);

   #ifdef DEBUG
    cout << "sorted population:" << endl;
    for (int i = 0; i < rated.size(); i++)
    {
      cout << "{ ";
      for (int j = 0; j < population[rated[i].index].size(); j++)
        cout << population[rated[i].index][j] << " ";
      cout << "}";
      cout << endl;
    }
  #endif
 
  return best;
}

double average(vector<d_type> data)
{
  double sum = 0;
# pragma omp parallel for num_threads(THREADS)
  for (unsigned int i = 0; i < data.size(); i++)
    sum += data[i];
  return sum / data.size();
}

vector< vector<int> > generate_pop(int num, double average_weight)
{
  vector< vector<int> > population;
//# pragma omp parallel for num_threads(THREADS) 
  for (int i = 0; i < num; i++)
  {
    population.push_back(gen_member(average_weight));
  }
  return population;
}

vector<int> gen_member(double average_weight)
{
  int target_num = floor(mw / (double)average_weight);
  vector<int> indices(ni);
  vector<int> elements;
  int num;
  if (target_num < 1)
  {
    num = rand() % ni + 1;
  }
  else
  {
    //num = target_num;
    int diff = ni - target_num;
    if (rand() % 2)
    {
      num = target_num + floor(diff * (rand() / (float)RAND_MAX));
    }
    else
      num = target_num - floor(diff * (rand() / (float)RAND_MAX));
    if (num < 1)
      num = 1;
    if (num > ni)
      num = ni;
  }

  for (int i = 0; i < ni; i++)
    indices[i] = i;

  int last = indices.size()-1;

  elements.resize(num);
  for (int i = 0; i < num; i++)
  {
    int index = rand() % (last+1);
    elements[i] = indices[index];
    unsigned int temp = indices[index];
    indices[index] = indices[last];
    indices[last] = temp;
    last--;
  }
  return elements;
}

void mix(vector<indi_score> &rated, vector< vector<d_type> > &pop)
{
  /*
  for (int i = 0; i < rated.size(); i++)
  {
    if (rated[i].score < 0)
      rated.erase(rated.begin() + i, rated.end());
  }
  */
}

void sort_populus(vector<indi_score> &pop)
{
  int i;
  for (unsigned int j = 1; j < pop.size(); j++)
  {
    indi_score key = pop[j];
    i = j - 1;
    while (i >= 0 && pop[i].score < key.score)
    {
      pop[i+1] = pop[i];
      i--;
    }
    pop[i+1] = key;
  }
}
