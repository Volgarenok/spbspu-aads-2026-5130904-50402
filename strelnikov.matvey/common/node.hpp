#ifndef NODE_HPP
#define NODE_HPP

namespace strelnikov
{
  template < class T >
  struct Node
  {
    T val;
    Node< T > *next;
  };
}

#endif
