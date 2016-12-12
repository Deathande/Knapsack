#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cstdlib>
#include <ctime>
#include "knapalg.h"

using namespace std;

const string file_name = "data.txt";
int max_weight;
int num_items;
vector<d_type> weights;
vector<d_type> values;
int population_size = 5;
int max_value;
int total_weight;
bool c = true;
vector<int> indicies;

void read_file();
void create_table();
pair<d_type, d_type> split_data(string line);

int main(int argc, char** argv)
{
  if (argc > 1)
    population_size = stoi(argv[1]);
    
  cout << "----------------------" << endl;
  cout << "|                    |" << endl;
  cout << "|  Knapsack Problem  |" << endl;
  cout << "|                    |" << endl;
  cout << "----------------------" << endl << endl;
  # ifdef DEBUG
    cout << "Debug output enabled." << endl << endl;
  # endif

  read_file();

  #ifdef DEBUG
    cout << "weights: ";
    for (int i = 0; i < weights.size(); i++)
      cout << weights[i] << " ";
    cout << endl;
    cout << "values: ";
    for (int i = 0; i < values.size(); i++)
      cout << values[i] << " ";
    cout << endl;
  #endif

  cout << "Number of items: " << num_items << endl; 
  cout << "Maximum weight: " << max_weight << endl;
  cout << "Buffer size: " << population_size << endl << endl;
  clock_t t = clock();
  indicies = get_indicies(weights,
                          values,
                          max_weight,
                          population_size,
			  500);
  t = clock() - t;

  cout << "Item indicies: ";
  for (unsigned int i = 0; i < indicies.size(); i++)
    cout << indicies[i] << " ";
  cout << endl;

  cout << "Respective weights: ";
  for (unsigned int i = 0; i < indicies.size(); i++)
    cout << weights[indicies[i]] << " ";
  cout << endl << "Respective values: ";
  for (unsigned int i = 0; i < indicies.size(); i++)
    cout << values[indicies[i]] << " ";
  cout << endl;
  cout << "Max value of: ";
  for (unsigned int i = 0; i < indicies.size(); i++)
    max_value += values[indicies[i]];
  cout << max_value << endl;
  cout << "Total weight of: ";
  for (unsigned int i = 0; i < indicies.size(); i++)
    total_weight += weights[indicies[i]];
  cout << total_weight << endl;
  cout << "Time of: " << t / (float)CLOCKS_PER_SEC;
  cout << " seconds" << endl;
  cout << endl;

  // Clean up
  //delete [] weights;
  //delete [] values;
}

void read_file()
{
  string line;
  ifstream file(file_name);
  if (file.is_open())
  {
    getline(file, line);
    num_items = stoi(line);
    getline(file, line);
    max_weight = stoi(line);
    //weights.push_back(0);
    //values.push_back(0);
    int i = 1;
    pair<d_type, d_type> split;
    while (getline(file, line))
    {
      split = split_data(line);
      weights.push_back(split.first);
      values.push_back(split.second);
      i++;
    }
  }
}

pair<d_type, d_type> split_data(string line)
{
  string str1 = "";
  string str2 = "";
  d_type val1;
  d_type val2;
  unsigned int index = 0;
  while (line[index] != ' ')
  {
    str1 += line[index];
    index++;
  }
  while (line[index] == ' ')
    index++;
  while(index < line.size())
  {
    str2 += line[index];
    index++;
  }
  val1 = stof(str1);
  val2 = stof(str2);
  return pair<d_type, d_type>(val1, val2);
}
