#ifndef NODE_HPP
#define NODE_HPP
namespace lachugin
{
  template < class T >
  struct Node
  {
    T value;
    Node* next;
  };
}

#endif
