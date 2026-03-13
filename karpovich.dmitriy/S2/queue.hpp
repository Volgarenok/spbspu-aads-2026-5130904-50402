#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"
#include <cstddef>

namespace karpovich
{
  template< class T >
  class Queue {
    List< T > data_;
  public:
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& front() noexcept;
    void push(const T& rhs);
    void pop() noexcept;
  };

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }
  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return data_.size();
  }
  template< class T >
  T& Queue< T >::front() noexcept
  {
    return data_.front();
  }
  template< class T >
  void Queue< T >::push(const T& rhs)
  {
    data_.push_back(rhs);
  }
  template< class T >
  void Queue< T >::pop() noexcept
  {
    data_.pop_front();
  }
}
#endif
