#ifndef _DOCK_NODE
#define _DOCK_NODE

namespace dock_dstructure
{
  template <class d_type>
  class node
  {
    public:
      node();
      node* n_node();
      node* p_node();
      d_type get_data();
      void set_data(d_type);
      void set_n_node(node*);
      void set_p_node(node*);
    private:
      node* p_n;
      node* n_n;
      d_type data;
  };
}

#include "node.cpp"

#endif
