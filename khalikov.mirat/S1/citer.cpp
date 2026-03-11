#ifndef CITER_CPP
#define CITER_CPP

#include "citer.hpp"
#include "node.hpp"
#include <stdexcept>

namespace khalikov
{
  template< class T >
  LCIter< T >::LCIter()
  {
    curr = nullptr;
  }
  template< class T >
  LCIter< T >::LCIter(const Node< T > * h)
  {
    curr = h;
  }
  template< class T >
  LCIter< T >::LCIter(const LIter< T > & other)
  {
    curr = other.curr;
  }
  template< class T >
  const T & LCIter< T >::operator[](size_t index) const
  {
    const Node < T > * temp = curr;
    for (size_t k = 0; k < index; k++)
    {
      if (!temp)
      {
        throw std::out_of_range();
      }
      temp = temp->next;
    }
    return temp->val;
  }
  template< class T >
  const T & LCIter< T >::operator*() const
  {
    if (!curr)
    {
      throw std::runtime_error();
    }
    return curr->val;
  }
  template< class T >
  const T * LCIter< T >::operator->() const
  {
    if (!curr)
    {
      throw std::runtime_error();
    }
    return &(curr->val);
  }
  template< class T >
  LCIter< T > & LCIter< T >::operator++()
  {
    if (!curr)
    {
      throw std::out_of_range();
    }
    curr = curr->next;
    return *this;
  }
  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > temp = *this;
    ++(*this);
    return temp;
  }
  template< class T >
  bool LCIter< T >::operator==(const LCIter< T > & other) const
  {
    return curr == other.curr;
  }
  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T > & other) const
  {
    return !(*this == other);
  }
}

#endif
