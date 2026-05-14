#ifndef NOVIKOV_LCITER_HPP
#define NOVIKOV_LCITER_HPP

#include <cassert>
#include <memory>
#include "Node.hpp"

namespace novikov
{
  template < class T > class List;

  template < class T > class LCIter
  {
    const Node< T > *node;
    friend class List< T >;

  public:
    LCIter(const Node< T > *n = nullptr):
      node(n)
    {}

    const T &operator*() const
    {
      assert(node != nullptr);
      return node->data;
    }

    const T *operator->() const
    {
      assert(node != nullptr);
      return std::addressof(node->data);
    }

    LCIter &operator++()
    {
      node = node->next;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const LCIter &rhs) const
    {
      return node == rhs.node;
    }

    bool operator!=(const LCIter &rhs) const
    {
      return node != rhs.node;
    }
  };
}

#endif
