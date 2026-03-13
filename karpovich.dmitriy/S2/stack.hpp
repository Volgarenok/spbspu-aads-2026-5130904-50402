#ifndef STACK_HPP
#define STACK_HPP

#include "list.hpp"
#include <cstddef>
namespace karpovich
{
  template < class T > class Stack
  {
    List< T > data_;

  public:
    bool empty() const noexcept;
    size_t size() const noexcept;
    T &top() noexcept;
    void push(const T &value);
    void pop() noexcept;
  };
  template < class T > bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }
  template < class T > size_t Stack< T >::size() const noexcept
  {
    return data_.size();
  }
  template < class T > T &Stack< T >::top() noexcept
  {
    return data_.back();
  }
  template < class T > void Stack< T >::push(const T &rhs)
  {
    data_.push_back(rhs);
  }
  template < class T > void Stack< T >::pop() noexcept
  {
    data_.pop_back();
  }
}
#endif
