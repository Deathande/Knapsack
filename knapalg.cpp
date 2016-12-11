#include "knapalg.h"

int mw;
int ni;

vector<int> get_indicies(vector<d_type> weights,
                         vector<d_type> values,
			 int max_weight,
			 int num)
{
  vector< vector<d_type> > population;
  vector<d_type> best;
  vector< pair<int, int> > rated(num); // first = index; second = score
  mw = max_weight;
  ni = values.size();
  double average_weight = average(weights);
  population = generate_pop(num, average_weight);
  cout << "--------------------------------------------------" << endl;
  for (int i = 0; i < population.size(); i++)
  {
    for (int j = 0; j < population[i].size(); j++)
      cout << population[i][j] << " ";
    cout << endl;
  }
  /*
  unsigned int sum_values;
  unsigned int sum_weights;
  for (int i = 0; i < population.size(); i++)
  {
    sum_weights = 0;
    for (int j = 0; j < population[i].size(); j++)
      sum_weights += weights[population[i][j]];
    rated[i].first = i;
    if (sum_weights > max_weight)
    {
      rated[i].second = -sum_weights;
    }
    else
    {
      sum_values = 0;
      for (int j = 0; j < population.size(); j++)
        sum_values += population[i][j];
      rated[i].second = sum_values;
    }
  }
  */
  return vector<int>();
}

double average(vector<d_type> data)
{
  double sum = 0;
  for (unsigned int i = 0; i < data.size(); i++)
    sum += data[i];
  return sum / data.size();
}

vector< vector<d_type> > generate_pop(int num, double average_weight)
{
  vector< vector<d_type> > population;
  for (int i = 0; i < num; i++)
  {
    population.push_back(gen_member(average_weight));
  }
  return population;
}

vector<d_type> gen_member(double average_weight)
{
  int target_num = floor(mw / (double)average_weight);
  int num;
  if (target_num < 1)
  {
    num = rand() % ni + 1;
  }
  else
  {
    int diff = ni - num;
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

  vector<unsigned int> indices(ni);
  vector<unsigned int> elements;
  //cout << "num " << num << endl;
  //cout << ni << endl;
  for (int i = 0; i < ni; i++)
    indices[i] = i;

  cout << "num " << num << endl;
  for (int i = 0; i < num; i++)
  {
    cout << "size: " << indices.size() << endl;
    cout << "i " << i << endl;
    cout << "num: " << num << endl;
    int index = rand() % indices.size();
    cout << "index: " << index << endl;
    elements.push_back(indices[index]);
    indices.erase(indices.begin()+index);
  }
  return elements;
}
