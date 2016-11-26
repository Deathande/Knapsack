
namespace dock_dstructure
{
  template <class d_type>
  node<d_type>::node ()
  {
  }

  template <class d_type>
  node<d_type>* node<d_type>::n_node()
  {
    return n_n;
  }

  template <class d_type>
  node<d_type>* node<d_type>::p_node()
  {
    return p_n;
  }

  template <class d_type>
  d_type node<d_type>::get_data()
  {
    return data;
  }
  
  template <class d_type>
  d_type node<d_type>::set_data(d_type d)
  {
    data = d;
  }

  template <class d_type>
  void node<d_type>::set_n_node(node* n)
  {
    n_n = n;
  }
  template <class d_type>
  void node<d_type>::set_p_node(node* p)
  {
    p_n = p;
  }
}
