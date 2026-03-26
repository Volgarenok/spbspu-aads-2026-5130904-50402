#ifndef STACK_HPP
#define STACK_HPP

#include "list.hpp"
#include <stdexcept>

namespace chernikov {

  template < class T > class Stack
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
  }

  template < T >
  bool empty() const
  {
    return data.empty();
  }
  template < T > size_t size() const
  {
    return data.size();
  }
  template < T > void push(const T &val)
  {
    data.add(val);
  }
  template < T > T drop()
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    T val = data.front();
    data.first_delete();
    return val;
  }
  template < T > const T &top() const
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    return data.front();
  }
  template < T > T &top()
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    return data.front();
  }
}

#endif