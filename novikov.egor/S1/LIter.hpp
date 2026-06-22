#ifndef NOVIKOV_LITER_HPP
#define NOVIKOV_LITER_HPP

#include <cassert>
#include <memory>
#include "Node.hpp"

namespace novikov
{
  template < class T > class List;

  template < class T > class LIter
  {
    Node< T > *node;
    friend class List< T >;

  public:
    LIter(Node< T > *n = nullptr):
      node(n)
    {}

    T &operator*()
    {
      assert(node != nullptr);
      return node->data;
    }

    T *operator->()
    {
      assert(node != nullptr);
      return std::addressof(node->data);
    }

    LIter &operator++()
    {
      node = node->next;
      return *this;
    }

    LIter operator++(int)
    {
      LIter tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const LIter &rhs) const
    {
      return node == rhs.node;
    }

    bool operator!=(const LIter &rhs) const
    {
      return node != rhs.node;
    }
  };
}

#endif
