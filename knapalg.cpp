#include "knapalg.h"
dock_dstructure::node<d_type*>* table;
dock_dstructure::node<d_type*>* directions;
queue<d_type*> t_disk_queue;
queue<d_type*> d_disk_queue;
pthread_t disk_io_thread;
sem_t sem_full;
sem_t sem_empty;
d_type* c_dir;
FILE* disk;
bool cont = true;
int mw;
int ni;
int bs;

void buffered_table(d_type* weights,
                    d_type* values)
{
  #ifdef DEBUG
    cout << "Table excluding zero rows" << endl;
  #endif 
  d_type* current = new d_type[mw+1];
  d_type* temp;
  d_type* prev = new d_type[mw+1];
  d_type* c_dir = new d_type[mw+1];

  sem_init(&sem_empty, 0, 0);
  sem_init(&sem_full, 0, bs);
  for (int i = 0; i <= mw; i++)
    current[i] = 0;
  sem_wait(&sem_full);
  t_disk_queue.push(current);
  sem_post(&sem_empty);
  prev = current;

  pthread_create(&disk_io_thread, 
                 NULL, 
		 write_to_disk,
		 NULL);

  for (int i = 1; i <= ni; i++)
  {
    current[0] = 0;
    for (int j = 1; j <= mw; j++)
    {
      if (weights[i] > (d_type)j)
      {
        current[j] = prev[j];
	c_dir[j] = j;
	#ifdef DEBUG
	  cout << current[j] << " ";
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
	    cout << current[j] << " ";
	  # endif
	}
	else
	{
	  current[j] = n_above;
	  c_dir[j] = n_above_index;
	  # ifdef DEBUG
	    cout << current[j] << " ";
	  # endif
	}
      }
    }
    sem_wait(&sem_full);
    t_disk_queue.push(current);
    d_disk_queue.push(c_dir);
    sem_post(&sem_empty);
    temp = prev;
    prev = current;
    current = temp;
    #ifdef DEBUG
      cout << endl;
    #endif
  }
  cont = false;
  pthread_join(disk_io_thread, NULL);
  delete [] current;
  delete [] c_dir;
  sem_destroy(&sem_full);
  sem_destroy(&sem_empty);
  //delete [] prev;
}

void* write_to_disk(void* non_used)
{
  disk = fopen("temp_file.dat", "w+");
  while (cont)
  {
    sem_wait(&sem_empty);
    d_type* row = t_disk_queue.front();
    fwrite(row, sizeof(row[0]), mw+1, disk);
    fwrite(c_dir, sizeof(row[0]), mw+1, disk);
    t_disk_queue.pop();
    sem_post(&sem_full);
  }
  fclose(disk);
  return NULL;
}

void* read_from_disk(void* not_used)
{
  disk = fopen("temp_file.dat", "r");
  while (cont)
  {
    sem_wait(&sem_full);
    d_type* row;
    fread(row, sizeof(row[0]), mw+1, disk);
    t_disk_queue.push(row);
    fread(row, sizeof(row[0]), mw+1, disk);
    d_disk_queue.push(row);
    sem_post(&sem_empty);
  }
  fclose(disk);
  return NULL;
}

vector<int> get_items( d_type* weights,
                       d_type* values,
		       int max_weight,
		       int num_items,
		       int buffer_size)
{
  int i = num_items;
  int j = max_weight;
  mw = max_weight;
  ni = num_items;
  bs = buffer_size;
  buffered_table(weights, values);
  vector<int> indicies;
  pthread_create(&disk_io_thread, 
                 NULL, 
		 read_from_disk,
		 NULL);
  sem_init(&sem_empty, 0, 0);
  sem_init(&sem_full, 0, bs);
  sem_wait(&sem_empty);
  d_type* c_table = t_disk_queue.front();
  d_type* c_dir = d_disk_queue.front();
  while (c_table[j] != 0)
  {
    if (c_dir[j] < (d_type)j)
      indicies.push_back(i);
    j = c_dir[j];
    sem_wait(&sem_empty);
    d_disk_queue.pop();
    c_dir = d_disk_queue.front();
    t_disk_queue.pop();
    c_table = t_disk_queue.front();
    sem_post(&sem_full);
    i--;
    /*
    if (hp == table)
    {
      ni -= buffer_size;
      buffered_table(weights, values);
    }
    */
  }
  sem_destroy(&sem_full);
  sem_destroy(&sem_empty);
  return indicies;
}



// Build a interconnected linked list where the tail
// pointer points to the head pointer and vis versa.
void init_buff()
{
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
}


void start_table()
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
