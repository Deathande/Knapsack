#include "knapalg.h"
d_type** table;
d_type** directions;

d_type** buffered_table(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size)
{
  table = new d_type*[num_items+1];
  directions = new d_type*[num_items+1];
  int to_delete = 0;
  for (int i = 0; i <= num_items; i++)
  {
    table[i] = new d_type[max_weight+1];
    directions[i] = new d_type[max_weight+1];
    # pragma omp parallel for num_threads(THREADS)
    for (int j = 0; j <= max_weight; j++)
    {
      if (i == 0 || j == 0)
      {
        table[i][j] = 0;
      }
      else if (weights[i] > (unsigned int)j)
      {
        table[i][j] = table[i-1][j];
	directions[i][j] = j;
      }
      else
      {
        int n_above_index = j - weights[i];
        d_type above = table[i-1][j];
	d_type n_above = table[i-1][n_above_index] + values[i];
	if (above > n_above)
	{
	  table[i][j] = j;
	  directions[i][j] = j;
	}
	else
	{
	  table[i][j] = n_above;
	  directions[i][j] = n_above_index;
	}
      }
    }
    if (i > buffer_size)
    {
      delete [] table[to_delete];
      delete [] directions[to_delete];
      table[to_delete] = NULL;
      directions[to_delete] = NULL;
      to_delete++;
    } 
  }
  return table;
}

vector<int> get_items(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size)
{
  vector<int> indicies;
  int i = num_items;
  int j = max_weight;
  buffered_table(weights, values, max_weight, num_items, buffer_size);
  while (table[i][j] != 0)
  {
    # ifdef DEBUG
      cout << "get_items i: " << i << endl;
      //cout << "get_items table[" << i << "]: ";
      //for (int x = 0; x < max_weight; x++)
        //cout << table[i][x] << " ";
    # endif
    if (directions[i][j] < (unsigned int)j)
      indicies.push_back(i);
    j = directions[i][j];
    # ifdef DEBUG
      cout << "get_items j: " << j << endl;
    # endif
    i--;
    if (directions[i] == NULL)
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
