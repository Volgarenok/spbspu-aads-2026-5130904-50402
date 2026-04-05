#ifndef STACK_HPP
#define STACK_HPP
#include "../common/list.hpp"
#include <iostream>
namespace lachugin
{
  template< typename T > class Stack {
    List< T > l;
  public:
    void push(const T& rhs);
    void pop();
    T& top();
    bool empty() const;
    size_t size() const;
    };

  template< typename T >
  void Stack< T >::push(const T& rhs)
  {
    if (l.empty())
    {
      l.add(rhs);
    }
    else
    {
      l.addNext(rhs);
    }
  }

  template< typename T >
  void Stack< T >::pop()
  {
    l.popFront();
  }

  template< typename T >
  bool Stack< T >::empty() const
  {
    return l.empty();
  }

  template< typename T >
  T& Stack< T >::top()
  {
    return l.back();
  }

  template< typename T >
  size_t Stack< T >::size() const
  {
    return l.size();
  }
}
#endif
