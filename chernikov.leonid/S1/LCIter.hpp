#ifndef LCITER_HPP
#define LCITER_HPP

#include "node.hpp"
#include <ostream>

namespace chernikov {

  template < typename T > class List;
  template < typename T > class LIter;

  template < typename T > class LCIter
  {
    friend class List< T >;

  private:
    const Node< T > *ptr;

    explicit LCIter(const Node< T > *p):
      ptr(p)
    {
    }

  public:
    LCIter():
      ptr(nullptr)
    {
    }
    LCIter(const LIter< T > &it):
      ptr(it.ptr)
    {
    }

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
    LCIter &operator++() // префикс
    {
      ptr = ptr->next;
      return *this;
    }
    LCIter operator++(int) // постфикс
    {
      LCIter tmp(*this);
      ptr = ptr->next;
      return tmp;
    }
    friend std::ostream &operator<<(std::ostream &os, const LCIter &it)
    {
      if (it.ptr == nullptr)
      {
        os << "LCIter(end)";
      } else
      {
        os << "LCIter(" << it.ptr->data << ")";
      }
      return os;
    }
  };

}

#endif
