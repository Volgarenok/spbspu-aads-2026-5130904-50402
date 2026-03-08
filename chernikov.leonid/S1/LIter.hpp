#ifndef LITER_HPP
#define LITER_HPP

#include "vanila_list.hpp"
#include "node.hpp"

namespace chernikov {

  template < typename T > class LIter
  {
    friend class List< T >;

  private:
    Node< T > *ptr;

    explicit LIter(Node< T > *p):
      ptr(p)
    {
    }

  public:
    LIter():
      ptr(nullptr)
    {
    }

    bool operator==(const LIter &other) const
    {
      return ptr == other.ptr;
    }
    bool operator!=(const LIter &other) const
    {
      return ptr != other.ptr;
    }

    T &operator*()
    {
      return ptr->data;
    }
    T *operator->()
    {
      return &ptr->data;
    }

    LIter &operator++()
    {
      ptr = ptr->next;
      return *this;
    }
  };
}
#endif
