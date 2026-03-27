#ifndef NOVIKOV_STACK_HPP
#define NOVIKOV_STACK_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace novikov
{
  template < class T > class Stack
  {
    List< T > data;

  public:
    bool empty() const
    {
      return data.empty();
    }
    void push(const T &val)
    {
      data.push_front(val);
    }
    T drop()
    {
      if (empty()) {
        std::cerr << "Stack is empty\n";
        throw;
      }
      T val = *data.begin();
      data.pop_front();
      return val;
    }
    T &front()
    {
      if (empty()) {
        std::cerr << "Stack is empty\n";
        throw;
      }
      return *data.begin();
    }
    const T &front() const
    {
      if (empty()) {
        std::cerr << "Stack is empty\n";
        throw;
      }
      return *data.begin();
    }
  };
}
#endif
