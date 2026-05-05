#ifndef NODE_HPP
#define NODE_HPP

namespace permyakov
{
  template < class T > struct Node 
  {
    T val;
    Node < T > * next;
  };
}

#endif