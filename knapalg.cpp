#include "knapalg.h"

#define OUT_FILE "temp_data.out"
#define SOL_FILE "solution.out"

FILE* disk;
FILE* solutions;
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
  int* prev;
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
    delete [] prev;
    prev = current;

    #ifdef DEBUG
      cout << endl;
    #endif
  }
  delete [] current;
  delete [] c_dir;
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
  solutions = fopen(SOL_FILE, "w");
  fprintf(solutions, "indicies\n");
  fclose(solutions);

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
    {
      solutions = fopen(SOL_FILE, "a");
      //fseek(solutions, 0, SEEK_END);
      fprintf(solutions, "%d\n", (i-1));
      fclose(solutions);
      cout << (i-1) << " ";
      indices.push_back(i);
    }
    i--;
  }
  //fclose(solutions);
  cout << endl;
  return indices;
}
