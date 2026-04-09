#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"
#include <iostream>

namespace khalikov
{
  template < class T > class Stack
  {
  private:
    List< T > list;

  public:
    void push(const T &rhs);
    void pop();
    const T &top() const;
    T drop();
    size_t size() const noexcept;
    bool isEmpty() const noexcept;
  };
}

template < class T > void khalikov::Stack< T >::push(const T &rhs)
{
  list.pushFront(rhs);
}

template < class T > void khalikov::Stack< T >::pop()
{
  if (isEmpty()) {
    throw std::logic_error("Stack is empty");
  }
  list.popFront();
}

template < class T > const T &khalikov::Stack< T >::top() const
{
  if (isEmpty()) {
    throw std::logic_error("Stack is empty");
  }
  auto it = list.cbegin();
  return *it;
}

template < class T > T khalikov::Stack< T >::drop()
{
  T temp = top();
  pop();
  return temp;
}

template < class T > size_t khalikov::Stack< T >::size() const noexcept
{
  return list.size();
}

template < class T > bool khalikov::Stack< T >::isEmpty() const noexcept
{
  return list.isEmpty();
}

#endif
