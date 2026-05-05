#ifndef STACK_HPP
#define STACK_HPP

#include "list.hpp"

namespace afanasev
{
  template< class T >
  class Stack
  {
  public:
    void push(const T & rhs);
    T & get();
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > data_;
  };
}

template < class T >
void afanasev::Stack< T >::push(const T & rhs)
{
  data_.pushFront(rhs);
}

template < class T >
T & afanasev::Stack< T >::get()
{
  if (empty())
  {
    throw std::out_of_range("stack is empty");
  }
  return *data_.begin();
}

template < class T >
void afanasev::Stack< T >::pop() noexcept
{
  data_.popFront();
}

template < class T >
bool afanasev::Stack< T >::empty() const noexcept
{
  return !data_.size();
}

template < class T >
size_t afanasev::Stack< T >::size() const noexcept
{
  return data_.size();
}

#endif
