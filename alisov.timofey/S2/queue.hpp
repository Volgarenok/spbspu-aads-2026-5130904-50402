#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>
#include "../common/list.hpp"

namespace alisov
{
  template < class T >
  class Queue
  {
  public:
    void push(const T &rhs);
    T &get();
    const T &get() const;
    void pop();
    bool empty() const noexcept;

  private:
    BiList< T > data_;
  };
}

template < class T >
void alisov::Queue< T >::push(const T &rhs)
{
  data_.pushBack(rhs);
}

template < class T >
T &alisov::Queue< T >::get()
{
  if (empty()) {
    throw std::underflow_error("Queue is empty");
  }
  return data_.front();
}

template < class T >
const T &alisov::Queue< T >::get() const
{
  if (empty()) {
    throw std::underflow_error("Queue is empty");
  }
  return data_.front();
}

template < class T >
void alisov::Queue< T >::pop()
{
  if (empty()) {
    throw std::underflow_error("Queue is empty");
  }
  data_.pop_front();
}

template < class T >
bool alisov::Queue< T >::empty() const noexcept
{
  return data_.empty();
}

#endif
