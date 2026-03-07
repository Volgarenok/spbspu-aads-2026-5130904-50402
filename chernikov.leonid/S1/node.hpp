#ifndef NODE_HPP
#define NODE_HPP

namespace chernikov
{
  template < typename T > struct Node
  {
    T data;
    Node *next;

    Node(const T &value, Node *nxt = nullptr):
      data(value),
      next(nxt)
    {}
  };
}

#endif
