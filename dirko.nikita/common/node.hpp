#ifndef NODE_HPP
#define NODE_HPP

namespace dirko
{
  template < class T >
  struct Node
  {
    T val;
    Node< T > *next;
    Node< T > *prev;
  };
}
#endif
