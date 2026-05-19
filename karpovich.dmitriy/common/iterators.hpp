#ifndef ITERATORS_HPP
#define ITERATORS_HPP
#include "node.hpp"
namespace karpovich
{
  template< class T >
  class List;

  template< class T >
  class LIter
  {
    friend class List< T >;
    details::Node< T > *ptr_;

  public:
    LIter(details::Node< T > *p) noexcept;
    T &operator*() const noexcept;
    T *operator->() const noexcept;
    LIter &operator++() noexcept;
    LIter &operator--() noexcept;
    LIter operator++(int) noexcept;
    LIter operator--(int) noexcept;
    bool operator==(const LIter< T > &other) const noexcept;
    bool operator!=(const LIter< T > &other) const noexcept;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    const details::Node< T > *ptr_;

  public:
    LCIter(const details::Node< T > *p) noexcept;
    const T &operator*() const noexcept;
    const T *operator->() const noexcept;
    LCIter &operator++() noexcept;
    LCIter &operator--() noexcept;
    LCIter operator++(int) noexcept;
    LCIter operator--(int) noexcept;
    bool operator==(const LCIter< T > &other) const noexcept;
    bool operator!=(const LCIter< T > &other) const noexcept;
  };

  template< class T >
  LIter< T >::LIter(details::Node< T > *p) noexcept:
    ptr_(p)
  {}

  template< class T >
  T &LIter< T >::operator*() const noexcept
  {
    return ptr_->val;
  }

  template< class T >
  T *LIter< T >::operator->() const noexcept
  {
    return &(ptr_->data);
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T > &other) const noexcept
  {
    return ptr_ != other.ptr_;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T > &other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  LIter< T > &LIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template< class T >
  LIter< T > &LIter< T >::operator--() noexcept
  {
    ptr_ = ptr_->prev;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter tmp(*this);
    ptr_ = ptr_->next;
    return tmp;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int) noexcept
  {
    LIter tmp(*this);
    ptr_ = ptr_->prev;
    return tmp;
  }

  template< class T >
  LCIter< T >::LCIter(const details::Node< T > *p) noexcept:
    ptr_(p)
  {}

  template< class T >
  const T &LCIter< T >::operator*() const noexcept
  {
    return ptr_->val;
  }

  template< class T >
  const T *LCIter< T >::operator->() const noexcept
  {
    return &(ptr_->data);
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T > &other) const noexcept
  {
    return ptr_ != other.ptr_;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T > &other) const noexcept
  {
    return ptr_ == other.ptr_;
  }

  template< class T >
  LCIter< T > &LCIter< T >::operator++() noexcept
  {
    ptr_ = ptr_->next;
    return *this;
  }

  template< class T >
  LCIter< T > &LCIter< T >::operator--() noexcept
  {
    ptr_ = ptr_->prev;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter tmp(*this);
    ptr_ = ptr_->next;
    return tmp;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int) noexcept
  {
    LCIter tmp(*this);
    ptr_ = ptr_->prev;
    return tmp;
  }
}
#endif
