#ifndef STACK_HPP
#define STACK_HPP

#include "List.hpp"
#include <stdexcept>

namespace chernikov {

  template < typename T > class Stack
  {
  private:
    List< T > data;

  public:
    Stack() = default;
    bool empty() const;
    size_t size() const;
    void push(const T &val);
    T drop();
    const T &top() const;
    T &top();
  };

  template < typename T > bool Stack< T >::empty() const
  {
    return data.empty();
  }
  template < typename T > size_t Stack< T >::size() const
  {
    return data.size();
  }
  template < typename T > void Stack< T >::push(const T &val)
  {
    data.add(val);
  }
  template < typename T > T Stack< T >::drop()
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    T val = data.front();
    data.first_delete();
    return val;
  }
  template < typename T > const T &Stack< T >::top() const
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    return data.front();
  }
  template < typename T > T &Stack< T >::top()
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    return data.front();
  }
}

#endif
