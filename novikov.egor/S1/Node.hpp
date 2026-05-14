#ifndef NOVIKOV_NODE_HPP
#define NOVIKOV_NODE_HPP

namespace novikov
{
  template < class T > struct Node
  {
    T data;
    Node< T > *next;
  };
}

#endif
