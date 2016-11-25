#include "knapalg.h"
d_type** table;
d_type** directions;
bool allocated = false;
int last;

d_type** buffered_table(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size)
{
  if (!allocated)
  {
    table = new d_type*[buffer_size];
    directions = new d_type*[buffer_size];
    for (int i = 0; i < buffer_size; i++)
    {
      table[i] = new d_type[max_weight+1];
      directions[i] = new d_type[max_weight+1];
    }
    allocated = true;
  }
  last = 0;
  # ifdef DEBUG
    cout << "Buffer created with  " << buffer_size << " rows and ";
    cout << max_weight << " per rows." << endl;
  # endif
  for (int i = 0; i <= num_items; i++)
  {
    # ifdef DEBUG
      cout << "i: " << i << endl;
      cout << "i \% buffer_size: " << i % buffer_size << endl;
    # endif
    //table[i] = new d_type[max_weight+1];
    //directions[i] = new d_type[max_weight+1];
    //# pragma omp parallel for num_threads(THREADS)
    for (int j = 0; j <= max_weight; j++)
    {
      if (i == 0 || j == 0)
      {
        table[i % buffer_size][j] = 0;
      }
      else if (weights[i] > (unsigned int)j)
      {
        table[i % buffer_size][j] = table[(i-1) % buffer_size][j];
	directions[i % buffer_size][j] = j;
      }
      else
      {
        int n_above_index = j - weights[i];
        d_type above = table[(i-1) % buffer_size][j];
	d_type n_above = table[(i-1) % buffer_size][n_above_index] + values[i];
	if (above > n_above)
	{
	  table[i % buffer_size][j] = j;
	  directions[i % buffer_size][j] = j;
	}
	else
	{
	  table[i % buffer_size][j] = n_above;
	  directions[i % buffer_size][j] = n_above_index;
	}
      }
    }
    last = i % buffer_size;
  }
  return table;
}

vector<int> get_items(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size)
{
  int i = num_items;
  int j = max_weight;
  buffered_table(weights, values, max_weight, num_items, buffer_size);
  vector<int> indicies;
  while (table[i % buffer_size][j] != 0)
  {
    # ifdef DEBUG
      cout << "get_items i: " << i << endl;
      cout << "get_items i \% buffer_size: " << i % buffer_size << endl;
      cout << last << endl;
    # endif
    if (directions[i % buffer_size][j] < (unsigned int)j)
      indicies.push_back(i);
    j = directions[i % buffer_size][j];
    # ifdef DEBUG
      cout << "get_items j: " << j << endl;
    # endif
    i--;
    if (i % buffer_size == last)
    {
      num_items -= buffer_size;
      # ifdef DEBUG
        cout << "get_items num_items: " << num_items << endl;
      # endif
      buffered_table(weights, values, max_weight, num_items, buffer_size);
    }
  }
  // clean up tables
  for (int i = 0; i < buffer_size; i++)
  {
    delete [] table[i];
    delete [] directions[i];
  }
  delete [] table;
  delete [] directions;
  return indicies;
}
