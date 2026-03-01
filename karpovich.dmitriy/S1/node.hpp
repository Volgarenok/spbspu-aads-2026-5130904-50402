#ifndef NODE_HPP
#define NODE_HPP

namespace karpovich
{
  template < class T > struct Node
  {
    T val;
    Node< T > *next;
    Node< T > *prev;
  };
}
#endif
