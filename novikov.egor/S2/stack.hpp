#ifndef NOVIKOV_STACK_HPP
#define NOVIKOV_STACK_HPP

#include "../common/list.hpp"
#include <stdexcept>
#include <utility>

namespace novikov
{
  template < class T >
  class Stack
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

    void push(T &&val)
    {
      data.push_front(std::move(val));
    }

    T drop()
    {
      if (empty()) {
        throw std::runtime_error("Stack is empty");
      }
      T val = std::move(*data.begin());
      data.pop_front();
      return val;
    }

    const T &front() const
    {
      if (empty()) {
        throw std::runtime_error("Stack is empty");
      }
      return *data.begin();
    }
  };
}
#endif
