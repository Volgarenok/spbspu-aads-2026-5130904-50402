#ifndef LCITER_HPP
#define LCITER_HPP

#include "vanila_list.hpp"
#include "node.hpp"

namespace chernikov
{

  template < typename T > class LCIter
  {
    friend class List< T >;

  private:
    const Node< T > *ptr;

    explicit LCIter(const Node< T > *p):
      ptr(p)
    {}

  public:
    LCIter():
      ptr(nullptr)
    {}
    LCIter(const LIter< T > &it):
      ptr(it.ptr)
    {}

    bool operator==(const LCIter &other) const
    {
      return ptr == other.ptr;
    }
    bool operator!=(const LCIter &other) const
    {
      return ptr != other.ptr;
    }

    const T &operator*() const
    {
      return ptr->data;
    }
    const T *operator->() const
    {
      return &ptr->data;
    }

    LCIter &operator++()
    {
      ptr = ptr->next;
      return *this;
    }
  };

}

#endif
