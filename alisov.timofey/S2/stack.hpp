#include <iostream>
#ifndef STACK_HPP
#define STACK_HPP

namespace alisov
{
  template < class T > class Stack
  {
  public:
    void push(const T &rhs);
    T &get() noexcept;
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > data_;
  };
}
template < class T > bool alisov::Stack< T >::empty() const noexcept
{
  return !data_.size();
}

template < class T > size_t alisov::Stack< T >::size() const noexcept
{
  return data_.size();
}
#endif