#ifndef STACK_HPP
#define STACK_HPP
#include <list.hpp>
#include <utility>
#include <iostream>
namespace lachugin
{
  template< typename T >
  class Stack
  {
    List< T > l;
  public:
    void push(const T& rhs);
    void push(T&& rhs);
    void pop();
    T& top();
    bool empty() const noexcept;
    size_t size() const noexcept;
  };

  template< typename T >
  void Stack< T >::push(const T& rhs)
  {
    l.pushBack(rhs);
  }

  template< typename T>
  void Stack< T >::push(T&& rhs)
  {
    l.pushBack(std::move(rhs));
  }

  template< typename T >
  void Stack< T >::pop()
  {
    l.popEnd();
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return l.empty();
  }

  template< typename T >
  T& Stack< T >::top()
  {
    return l.back();
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return l.size();
  }
}
#endif

