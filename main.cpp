#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "knapalg.h"

using namespace std;

const string file_name = "data.txt";
int max_weight;
int num_items;
d_type* weights;
d_type* values;

void read_file();
void create_table();
pair<d_type, d_type> split_data(string line);

int main(int argc, char** argv)
{
  d_type** table;
  read_file();
  table = build_table(weights, values, max_weight, num_items);
  cout << table[num_items][max_weight] << endl;

  // Clean up
  delete [] weights;
  delete [] values;
  for (int i = 0; i < num_items; i++)
    delete [] table[i];
  delete [] table;
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
    weights = new d_type[num_items];
    values  = new d_type[num_items];
    int i = 0;
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
