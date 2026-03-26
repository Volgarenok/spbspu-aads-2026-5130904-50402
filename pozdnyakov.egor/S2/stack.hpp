#ifndef POZDNYAKOV_STACK_HPP
#define POZDNYAKOV_STACK_HPP

#include "../common/list.hpp"

namespace pozdnyakov
{
  template < typename T >
  class Stack
  {
  private:
    List< T > container;

  public:
    Stack() = default;

    void push(const T& val)
    {
      container.pushFront(val);
    }

    void pop()
    {
      container.popFront();
    }

    T& top()
    {
      return container.front();
    }

    const T& top() const
    {
      return container.front();
    }

    bool empty() const noexcept
    {
      return container.empty();
    }

    void clear() noexcept
    {
      container.clear();
    }
  };
}

#endif
