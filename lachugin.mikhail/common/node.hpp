#ifndef NODE_HPP
#define NODE_HPP
#include <utility>
namespace lachugin
{
  template< class T >
  struct Node
  {
    T value;
    Node* next;

    template< class... Args >
    Node(Node* n, Args&&... args):
        value(std::forward< Args >(args)...),
        next(n)
    {}
  };
}

#endif
