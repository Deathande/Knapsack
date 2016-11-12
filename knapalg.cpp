#include "knapalg.h"

d_type** build_table(d_type* weights, d_type* values, int max_weight, int num_items)
{
  d_type** table = init_table(max_weight+1, num_items+1);
  //int x = 1;
  for (int i = 0; i < num_items+1; i++)
  {
    for (int j = 0; j < max_weight+1; j++)
    {
      if (i == 0 || j == 0)
      {
        table[i][j] = 0;
      }
      else if (weights[i] > j)
      {
        table[i][j] = table[i-1][j];
      }
      else
        table[i][j] = max((int)table[i-1][j], (int)table[i-1][j - weights[i]] + values[i]);
    }
  }
  return table;
}

d_type** init_table(int max_weight, int num_items)
{
  d_type** table;
  table = new d_type*[num_items];
  for (int i = 0; i < num_items; i++)
    table[i] = new d_type[max_weight];
  return table;
}
