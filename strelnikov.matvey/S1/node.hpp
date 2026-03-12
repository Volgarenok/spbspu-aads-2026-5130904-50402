#ifndef NODE_HPP
#define NODE_HPP

namespace strelnikov
{
  template < class T > struct node
  {
    T val;
    Node< T > *next;
  };
}

#endif