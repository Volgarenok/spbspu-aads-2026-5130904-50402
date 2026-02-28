#ifndef NODE_HPP
#define NODE_HPP
#include "list.hpp"

namespace karpovich
{
  template< class T >
  struct Node
  {
    T val;
    List< T >* prev;
    List< T >* next;
  };
}
#endif
