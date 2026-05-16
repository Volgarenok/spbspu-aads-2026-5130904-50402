#include <iostream>
#ifndef QUEUE_HPP
#define QUEUE_HPP

namespace alisov
{
  template < class T > class Queue
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
template < class T > bool alisov::Queue< T >::empty() const noexcept
{
  return !data_.size();
}

template < class T > size_t alisov::Queue< T >::size() const noexcept
{
  return data_.size();
}

#endif