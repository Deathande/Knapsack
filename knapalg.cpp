#include "knapalg.h"

#define OUT_FILE "temp_data.out"

FILE* disk;
int mw;
int ni;
int bs;

void buffered_table(d_type* weights,
                    d_type* values)
{
  #ifdef DEBUG
    cout << "Table excluding zero rows" << endl;
  #endif 
  int* current = new int[mw+1];
  int* prev = new int[mw+1];
  int* c_dir= new int[mw+1];
  disk = fopen(OUT_FILE, "w");

  for (int i = 0; i <= mw; i++)
  {
    current[i] = 0;
  }
  prev = current;
  for (int i = 1; i <= ni; i++)
  {
    current = new int[mw+1];
    current[0] = 0;
    c_dir[0] = i-1;
    for (int j = 1; j <= mw; j++)
    {
      if (weights[i] > (d_type)j)
      {
        current[j] = prev[j];
        c_dir[j] = j;
        #ifdef DEBUG
          //cout << current[j] << " ";
          cout << c_dir[j] << " ";
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
            //cout << current[j] << " ";
            cout << c_dir[j] << " ";
          # endif
        }
        else
        {
          current[j] = n_above;
          c_dir[j] = n_above_index;
          # ifdef DEBUG
            //cout << current[j] << " ";
            cout << c_dir[j] << " ";
          # endif
        }
      }
    }
    fwrite(c_dir,
           sizeof(int),
           mw+1,
           disk);
    prev = current;

    #ifdef DEBUG
      cout << endl;
    #endif
  }
  cout << current[mw] << endl;
  //delete [] current;
  //delete [] prev;
  //delete [] c_dir;
  fclose(disk);
}

vector<int> get_items( d_type* weights,
                       d_type* values,
        	       int max_weight,
        	       int num_items,
        	       int buffer_size)
{
  int last;
  mw = max_weight;
  ni = num_items;
  bs = buffer_size;
  int i = ni;
  int j = mw;

  buffered_table(weights, values);
  vector<int> indices;
  disk = fopen(OUT_FILE, "r"); 
  while (i > 0)
  {
    last = j;
    fseek(disk,
        (i-1) * (mw+1) * sizeof(int) + j * sizeof(int),
	SEEK_SET);
    fread(&j, sizeof(int), 1, disk);
    if (last > j)
      indices.push_back(i);
    i--;
  }

  return indices;
}



// Build a interconnected linked list where the tail
// pointer points to the head pointer and vis versa.
void init_buff()
{
  /*
  table = new dock_dstructure::node<d_type*>();
  directions = new dock_dstructure::node<d_type*>();
  dock_dstructure::node<d_type*>* t_cn = table;
  dock_dstructure::node<d_type*>* d_cn = directions;
  table->set_data(new d_type[mw+1]);
  directions->set_data(new d_type[mw+1]);

  for (int i = 0; i < bs; i++)
  {
    dock_dstructure::node<d_type*>* t_new_node;
    dock_dstructure::node<d_type*>* d_new_node;

    t_new_node = new dock_dstructure::node<d_type*>();
    d_new_node = new dock_dstructure::node<d_type*>();

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
  */
}


void start_table()
{
  /*
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
  */
}
