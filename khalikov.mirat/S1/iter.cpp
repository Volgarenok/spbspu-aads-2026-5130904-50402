#ifndef ITER_CPP
#define ITER_CPP

#include "iter.hpp"
#include "node.hpp"
#include <stdexcept>

namespace khalikov
{
  template< class T >
  LIter< T >::LIter()
  {
    curr = nullptr;
  }
  template< class T >
  LIter< T >::LIter(Node< T > * h)
  {
    curr = h;
  }
  template< class T >
  T & LIter< T >::operator[](size_t index)
  {
    Node < T > * temp = curr;
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
  T & LIter< T >::operator*()
  {
    return curr->val;
  }
  template< class T >
  T * LIter< T >::operator->()
  {
    return &(curr->val);
  }
  template< class T >
  LIter< T > & LIter< T >::operator++()
  {
    curr = curr->next;
    return *this;
  }
  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > temp = *this;
    ++(*this);
    return temp;
  }
  template< class T >
  bool LIter< T >::operator==(const LIter< T > & other) const
  {
    return curr == other.curr;
  }
  template< class T >
  bool LIter< T >::operator!=(const LIter< T > & other) const
  {
    return !(*this == other);
  }
}

#endif
