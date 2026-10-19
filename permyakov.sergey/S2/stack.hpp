#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include "list.hpp"
#include "iterators.hpp"

namespace permyakov
{
  template< class T > class Stack
  {
    List< T > data;
    public:
      void push(const T value);
      void pop();
      T top();
      size_t size();
      bool isEmpty();
  };

  template< class T > void Stack< T >::push(const T value)
  {
    data.push_front(value);
  }

  template< class T > void Stack< T > ::pop()
  {
    data.pop_front();
  }

  template< class T > T Stack< T >::top()
  {
    return data.front();
  }

  template< class T > size_t Stack< T >::size()
  {
    return data.size();
  }

  template< class T > bool Stack< T >::isEmpty()
  {
    return !size();
  }
}

#endif
