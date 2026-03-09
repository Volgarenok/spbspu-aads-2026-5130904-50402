#ifndef NODE_HPP
#define NODE_HPP

namespace khalikov
{
  template< class T >
  struct Node
  {
  private:
    T val;
    Node< T > * next;
  };
}

#endif
