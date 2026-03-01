#ifndef ITERATORS_HPP
#define ITERATORS_HPP
#include "node.hpp"
namespace karpovich
{
  template < typename T > class List;
  template < class T > class LIter
  {
    friend class List< T >;
    Node< T > *ptr_;

  public:
    LIter(Node< T > *p);
    T &operator*() const;
    LIter &operator++();
    LIter &operator--();
    LIter &operator++(int);
    LIter &operator--(int);
    bool operator==(const LIter< T > &other) const;
    bool operator!=(const LIter< T > &other) const;
  };
  template < class T > class LCIter
  {
    friend class List< T >;
    const Node< T > *ptr_;

  public:
    LCIter(const Node< T > *p);
    const T &operator*() const;
    LCIter &operator++();
    LCIter &operator--();
    LCIter &operator++(int);
    LCIter &operator--(int);
    bool operator==(const LCIter< T > &other) const;
    bool operator!=(const LCIter< T > &other) const;
  };

  template < class T >
  LIter< T >::LIter(Node< T > *p):
    ptr_(p)
  {}

  template < class T > T &LIter< T >::operator*() const
  {
    return ptr_->val;
  }

  template < class T > bool LIter< T >::operator!=(const LIter< T > &other) const
  {
    return ptr_ != other.ptr_;
  }

  template < class T > bool LIter< T >::operator==(const LIter< T > &other) const
  {
    return ptr_ == other.ptr_;
  }

  template < class T > LIter< T > &LIter< T >::operator++()
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template < class T > LIter< T > &LIter< T >::operator--()
  {
    ptr_ = ptr_->prev;
    return *this;
  }

  template < class T > LIter< T > &LIter< T >::operator++(int)
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template < class T > LIter< T > &LIter< T >::operator--(int)
  {
    ptr_ = ptr_->prev;
    return *this;
  }

  template < class T >
  LCIter< T >::LCIter(const Node< T > *p):
    ptr_(p)
  {}

  template < class T > const T &LCIter< T >::operator*() const
  {
    return ptr_->val;
  }

  template < class T > bool LCIter< T >::operator!=(const LCIter< T > &other) const
  {
    return ptr_ != other.ptr_;
  }

  template < class T > bool LCIter< T >::operator==(const LCIter< T > &other) const
  {
    return ptr_ == other.ptr_;
  }
  template < class T > LCIter< T > &LCIter< T >::operator++()
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template < class T > LCIter< T > &LCIter< T >::operator--()
  {
    ptr_ = ptr_->prev;
    return *this;
  }

  template < class T > LCIter< T > &LCIter< T >::operator++(int)
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template < class T > LCIter< T > &LCIter< T >::operator--(int)
  {
    ptr_ = ptr_->prev;
    return *this;
  }
}
#endif
