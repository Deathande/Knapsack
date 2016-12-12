#include "knapalg.h"

#define THREADS 2
#define SOLUTIONS "solutions.out"
FILE* sol;
int mw;
int ni;

vector<int> get_indicies(vector<d_type> weights,
                         vector<d_type> values,
                         int max_weight,
                         int num,
			 int iterations)
{
  srand(time(NULL));
  vector< vector<int> > population;
  vector<int> best;
  int best_score = 0;
  vector<indi_score> rated(num); // first = index; second = score
  mw = max_weight;
  ni = values.size();
  population = generate_pop(num);
  // truncate file
  sol = fopen(SOLUTIONS, "w");
  fclose(sol);

  #ifdef DEBUG
    cout << "population:" << endl;
    for (unsigned int i = 0; i < population.size(); i++)
    {
      cout << "{ ";
      for (unsigned int j = 0; j < population[i].size(); j++)
        cout << population[i][j] << " ";
      cout << "}";
      cout << endl;
    }
  #endif
  int sum_values;
  int sum_weights;
  int x = 0;
  while (x < iterations)
  {
    //cout  << x << endl;
    for (unsigned int i = 0; i < population.size(); i++)
    {
      sum_weights = 0;
      for (unsigned int j = 0; j < population[i].size(); j++)
        sum_weights += weights[population[i][j]];
      rated[i].index = i;
      if (sum_weights > max_weight)
      {
        rated[i].score = -sum_weights;
      }
      else
      {
        sum_values = 0;
        for (unsigned int j = 0; j < population[i].size(); j++)
        {
          sum_values += values[population[i][j]];
        }
        rated[i].score = sum_values;
      }
    }
    sort_populus(rated);
    #ifdef DEBUG
      cout << "sorted population:" << endl;
      for (unsigned int i = 0; i < rated.size(); i++)
      {
        cout << "{ ";
        for (unsigned int j = 0; j < population[rated[i].index].size(); j++)
          cout << population[rated[i].index][j] << " ";
        cout << "}";
        cout << " " << rated[i].score << endl;
      }
    #endif
  
    if (rated[0].score > best_score)
    {
      best = population[rated[0].index];
      best_score = rated[0].score;
      sol = fopen(SOLUTIONS, "w");
      for (int i = 0; i < best.size(); i++)
        fprintf(sol, "%d\n", best[i]);
      fclose(sol);
    }
  
    mix(rated, population);
  
     #ifdef DEBUG
      cout << "mixed population:" << endl;
      for (unsigned int i = 0; i < population.size(); i++)
      {
        cout << "{ ";
        for (unsigned int j = 0; j < population[i].size(); j++)
          cout << population[i][j] << " ";
        cout << "}";
        cout << endl;
      }
    #endif
    rated.clear();
    rated.resize(num);
    x++;
  }
 
  return best;
}

double average(vector<d_type> data)
{
  double sum = 0;
//# pragma omp parallel for num_threads(THREADS)
  for (unsigned int i = 0; i < data.size(); i++)
    sum += data[i];
  return sum / data.size();
}

vector< vector<int> > generate_pop(int num)
{
  vector< vector<int> > population;
//# pragma omp parallel for num_threads(THREADS) 
  for (int i = 0; i < num; i++)
  {
    population.push_back(gen_member());
  }
  return population;
}

vector<int> gen_member()
{
  vector<int> indices(ni);
  vector<int> elements;
  int num;
  num = rand() % (ni-1) + 1;

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

void mix(vector<indi_score> &rated, vector< vector<int> > &pop)
{
  int mid_index = floor(rated.size() / 2);
  vector< vector<int> > new_vect(mid_index);
  // copy top half of population into new population
  for (int i = 0; i < mid_index; i++)
    new_vect[i] = pop[rated[i].index];
  #ifdef DEBUG
    for (int i = 0; i < mid_index; i++)
    {
      for (unsigned int j = 0; j < pop[rated[i].index].size(); j++)
        cout << pop[rated[i].index][j] << " ";
      cout << endl;
    }
  #endif

  vector<int> v1;
  vector<int> v2;
  vector<int> temp;
  int split_point1;
  int split_point2;
  while (new_vect.size() < pop.size())
  {
    v1 = pop[rated[rand() % mid_index].index];
    v2 = pop[rated[rand() % mid_index].index];
    split_point1 = rand() % v1.size();
    split_point2 = rand() % v2.size();
    for (int i = 0; i < split_point1; i++)
    {
      temp.push_back(v1[i]);
    }
    for (unsigned int i = split_point2; i < v2.size(); i++)
    {
      temp.push_back(v2[i]);
    }
    correct_vect(temp);
    new_vect.push_back(temp);
    temp.clear();
  }
  pop = new_vect;
  /*
  int path;
  for (int i = 0; i < mid_index; i++)
  {
    path = rand() % 2;
    //cout << path << endl;
    if (path)
    {
      //cout << "add" << endl;
      mutate_add(pop[i]);
    }
    else
    {
      //cout << "change" << endl;
      mutate_change(pop[i]);
    }
  }
  */
}

void correct_vect(vector<int> &vect)
{
  map<int, int> keys;
  for (unsigned int i = 0; i < vect.size(); i++)
    keys[vect[i]] = 1;
  vect.clear();
  for (map<int, int>::iterator it = keys.begin(); it != keys.end(); it++)
    vect.push_back(it->first);
}

void mutate_change(vector<int> &vect)
{
  map<int, int> hash;
  vector<int> indices;
  int num_changes;
  int index;
  int temp;
  int last;
  for (unsigned int i = 0; i < vect.size(); i++)
    hash[vect[i]] = 1;
  for (int i = 0; i < ni; i++)
  {
    if (hash[i] == 0)
      indices.push_back(i);
  }
  num_changes = rand() % vect.size();
  last = indices.size() - 1;
  for (int i = 0; i < num_changes; i++)
  {
    index = rand() % (last + 1);
    vect[rand() % vect.size()] = indices[index];
    temp = indices[index];
    indices[index] = indices[last];
    indices[last] = indices[index];
    last--;
  }
}

void mutate_add(vector<int> &vect)
{
  map<int, int> hash;
  vector<int> indices;
  int temp;
  int num_additions;
  int last;
  int index;

  for (unsigned int i = 0; i < vect.size(); i++)
    hash[vect[i]] = 1;
  for (int i = 0; i < ni; i++)
  {
    if (hash[i] == 0)
      indices.push_back(i);
  }
  num_additions = rand() % (ni - vect.size());
  last = indices.size() - 1;
  for (int i = 0; i < num_additions; i++)
  {
    index = rand() % (last + 1);
    vect.push_back(indices[index]);
    temp = indices[index];
    indices[index] = indices[last];
    indices[last] = temp;
    last--;
  }
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
