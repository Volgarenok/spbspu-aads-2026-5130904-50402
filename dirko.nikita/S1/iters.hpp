#ifndef ITERS_HPP
#define ITERS_HPP
#include "node.hpp"

namespace dirko
{
  template < class T >
  class List;
  template < class T >
  class Iter
  {
  public:
    Iter(Node< T > *);
    T &operator*() const;
    Iter &operator++();
    Iter &operator--();
    Iter &operator++(int);
    Iter &operator--(int);
    bool operator==(const Iter< T > &) const;
    bool operator!=(const Iter< T > &) const;

  private:
    Node< T > *curr_;
    friend class List< T >;
  };
  template < class T >
  class CIter
  {
  public:
    CIter(Node< T > *);
    const T &operator*() const;
    CIter &operator++();
    CIter &operator--();
    CIter &operator++(int);
    CIter &operator--(int);
    bool operator==(const CIter< T > &) const;
    bool operator!=(const CIter< T > &) const;

  private:
    const Node< T > *curr_;
    friend class List< T >;
  };
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
    return curr_ == rhs.curr_;
  }
  template < class T >
  bool Iter< T >::operator!=(const Iter< T > &rhs) const
  {
    return !(*this == rhs);
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
    Iter< T > &prev = curr_;
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
    Iter< T > &next = curr_;
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
  bool CIter< T >::operator==(const CIter< T > &rhs) const
  {
    return curr_ == rhs.curr_;
  }
  template < class T >
  bool CIter< T >::operator!=(const CIter< T > &rhs) const
  {
    return !(*this == rhs);
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
    CIter< T > &prev = curr_;
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
    CIter< T > &next = curr_;
    curr_ = curr_->prev;
    return next;
  }
}

#endif
