#ifndef LITER_HPP
#define LITER_HPP

#include "node.hpp"
#include <ostream>

namespace chernikov {

  template < typename T > class List;

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
    LIter(const LIter &other) = default;
    LIter &operator=(const LIter &other) = default;
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
    const T &operator*() const
    {
      return ptr->data;
    }
    LIter &operator++() // префикс
    {
      ptr = ptr->next;
      return *this;
    }
    LIter operator++(int) // постфикс
    {
      LIter tmp(*this);
      ptr = ptr->next;
      return tmp;
    }
    friend std::ostream &operator<<(std::ostream &os, const LIter< T > &it)
    {
      if (it == LIter< T >())
      {
        os << "LIter(nullptr)";
      } else
      {
        os << "LIter(" << *it << ")";
      }
      return os;
    }
  };
}
#endif
