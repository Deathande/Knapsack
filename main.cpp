#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cstdlib>
#include "knapalg.h"

using namespace std;

const string file_name = "data.txt";
int max_weight;
int num_items;
d_type* weights;
d_type* values;
int buffer_size = 5;

void read_file();
void create_table();
pair<d_type, d_type> split_data(string line);

int main(int argc, char** argv)
{
  string usage = "optional argument: number of items in the buffer, default 5";
  if (argc == 2)
  {
    try
    {
      buffer_size = stoi(argv[1]);
    }
    catch (invalid_argument e)
    {
      cout << usage << endl;
      return 0;
    }
  }
  else if (argc > 2)
  {
    cout << usage << endl;
    return 0;
  }

  cout << "----------------------" << endl;
  cout << "|                    |" << endl;
  cout << "|  Knapsack Problem  |" << endl;
  cout << "|                    |" << endl;
  cout << "----------------------" << endl << endl;
  # ifdef DEBUG
    cout << "Debug output enabled." << endl << endl;
  # endif


  vector<int> indicies;
  d_type max_value = 0;
  d_type total_weight = 0;

  read_file();

  # ifdef DEBUG
    cout << "----------------------------------------" << endl;
    cout << "Debug output" << endl;
  # endif
  indicies = get_items(weights, values, max_weight, num_items, 2);
  # ifdef DEBUG
    cout << "----------------------------------------" << endl;
  # endif
  cout << "Number of items: " << num_items << endl; 
  cout << "Maximum weight: " << max_weight << endl;
  cout << "Buffer size: " << buffer_size << endl << endl;
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
    total_weight += values[indicies[i]];
  cout << total_weight << endl;
  cout << endl;

  // Clean up
  delete [] weights;
  delete [] values;
  /*
  for (int i = 0; i <= num_items; i++)
  {
    if (table[i] != NULL)
      delete [] table[i];
  }
  delete [] table;
  */
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
    weights = new d_type[num_items+1];
    values  = new d_type[num_items+1];
    weights[0] = 0;
    values[0] = 0;
    int i = 1;
    pair<d_type, d_type> split;
    while (getline(file, line))
    {
      split = split_data(line);
      weights[i] = split.first;
      values[i] = split.second;
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
