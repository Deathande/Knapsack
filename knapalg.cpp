#include "knapalg.h"
dock_dstructure::node<d_type*>* table;
dock_dstructure::node<d_type*>* directions;

void buffered_table(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size)
{
  #ifdef DEBUG
    cout << "Table excluding zero rows" << endl;
  #endif 
  start_table(buffer_size, max_weight);
  table = table->n_node();
  d_type* current;
  d_type* prev;
  d_type* c_dir;
  for (int i = 1; i <= num_items; i++)
  {
    current = table->get_data();
    prev = table->p_node()->get_data();
    c_dir = directions->get_data();
    //# pragma omp parallel for num_threads(THREADS)
    for (int j = 1; j <= max_weight; j++)
    {
      if (weights[i] > (d_type)j)
      {
        current[j] = prev[j];
	c_dir[j] = j;
	#ifdef DEBUG
	  cout << table->get_data()[j] << " ";
	#endif
      }
      else
      {
        int n_above_index = j - weights[i];
        d_type above = prev[j];
	d_type n_above = prev[n_above_index] + values[i];
	if (above > n_above)
	{
	  current[j] = prev[j];
	  c_dir[j] = j;
	  # ifdef DEBUG
	    cout << table->get_data()[j] << " ";
	  # endif
	}
	else
	{
	  current[j] = n_above;
	  c_dir[j] = n_above_index;
	  # ifdef DEBUG
	    cout << table->get_data()[j] << " ";
	  # endif
	}
      }
    }
    #ifdef DEBUG
      cout << endl;
    #endif
    table = table->n_node();
    directions = directions->n_node();
  }
  table = table->p_node();
  directions = directions->p_node();
}

vector<int> get_items(d_type* weights, d_type* values, int max_weight, int num_items, int buffer_size)
{
  int i = num_items;
  int j = max_weight;
  init_buff(buffer_size, max_weight);
  # ifdef DEBUG
    cout << "Buffer created with  " << buffer_size << " rows and ";
    cout << max_weight << " per rows." << endl;
  # endif
  buffered_table(weights, values, max_weight, num_items, buffer_size);
  vector<int> indicies;
  dock_dstructure::node<d_type*>* hp = table;
  while (table->get_data()[j] != 0)
  {
    if (directions->get_data()[j] < (d_type)j)
      indicies.push_back(i);
    j = directions->get_data()[j];
    directions = directions->p_node();
    table = table->p_node();
    i--;
    if (hp == table)
    {
      num_items -= buffer_size;
      buffered_table(weights, values, max_weight, num_items, buffer_size);
    }
  }
  return indicies;
}

// Build a interconnected linked list where the tail
// pointer points to the head pointer and vis versa.
void init_buff(int bs, int mw)
{
  table = new dock_dstructure::node<d_type*>();
  directions = new dock_dstructure::node<d_type*>();
  dock_dstructure::node<d_type*>* t_cn = table;
  dock_dstructure::node<d_type*>* d_cn = directions;
  table->set_data(new d_type[mw+1]);
  directions->set_data(new d_type[mw+1]);

  for (int i = 0; i < bs; i++)
  {
    dock_dstructure::node<d_type*>* t_new_node = new dock_dstructure::node<d_type*>();
    dock_dstructure::node<d_type*>* d_new_node = new dock_dstructure::node<d_type*>();
    t_new_node->set_data(new d_type[mw+1]);
    d_new_node->set_data(new d_type[mw+1]);
    t_cn->set_n_node(t_new_node);
    d_cn->set_n_node(d_new_node);
    t_new_node->set_p_node(t_cn);
    d_new_node->set_p_node(d_cn);
    t_cn = t_new_node;
    d_cn = d_new_node;
  }
  t_cn->set_n_node(table);
  table->set_p_node(t_cn);
  d_cn->set_n_node(directions);
  directions->set_p_node(d_cn);
}


void start_table(int bs, int mw)
{
  // initialize the first row
  for (int i = 1; i <= mw; i++)
    table->get_data()[i] = 0;
  // Initialize the first column
  dock_dstructure::node<d_type*>* cn = table;
  do
  {
    cn->get_data()[0] = 0;
    cn = cn->n_node();
  } while (cn != table);
}
