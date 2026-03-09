#include "iters.hpp"

namespace dirko
{
  template < class T >
  Iter< T >::Iter(Node< T > *node):
    curr_(node)
  {}
  template < class T >
  T &Iter< T >::operator*() const
  {
    return curr_->val;
  }
  template < class T >
  bool Iter< T >::operator==(const Iter< T > &rhs) const
  {
    return curr_->val == rhs.curr_->val;
  }
  template < class T >
  bool Iter< T >::operator!=(const Iter< T > &rhs) const
  {
    return !*this == rhs;
  }
  template < class T >
  Iter< T > &Iter< T >::operator++()
  {
    curr_ = curr_->next;
    return *this;
  }
  template < class T >
  Iter< T > &Iter< T >::operator++(int)
  {
    Node< T > *prev = curr_;
    curr_ = curr_->next;
    return prev;
  }
  template < class T >
  Iter< T > &Iter< T >::operator--()
  {
    curr_ = curr_->prev;
    return *this;
  }
  template < class T >
  Iter< T > &Iter< T >::operator--(int)
  {
    Node< T > *next = curr_;
    curr_ = curr_->prev;
    return next;
  }
  template < class T >
  CIter< T >::CIter(Node< T > *node):
    curr_(node)
  {}
  template < class T >
  const T &CIter< T >::operator*() const
  {
    return curr_->val;
  }
  template < class T >
  bool CIter< T >::operator==(const Iter< T > &rhs) const
  {
    return curr_->val == rhs.curr_->val;
  }
  template < class T >
  bool CIter< T >::operator!=(const Iter< T > &rhs) const
  {
    return !*this == rhs;
  }
  template < class T >
  CIter< T > &CIter< T >::operator++()
  {
    curr_ = curr_->next;
    return *this;
  }
  template < class T >
  CIter< T > &CIter< T >::operator++(int)
  {
    Node< T > *prev = curr_;
    curr_ = curr_->next;
    return prev;
  }
  template < class T >
  CIter< T > &CIter< T >::operator--()
  {
    curr_ = curr_->prev;
    return *this;
  }
  template < class T >
  CIter< T > &CIter< T >::operator--(int)
  {
    Node< T > *next = curr_;
    curr_ = curr_->prev;
    return next;
  }
}
