#ifndef STACK_HPP
#define STACK_HPP

#include <utility>
#include "../common/list.hpp"

namespace pozdnyakov
{
  template< class T >
  class Stack
  {
  private:
    List< T > container_{};

  public:
    Stack():
      container_()
    {}

    void push(const T &val)
    {
      container_.pushFront(val);
    }

    void push(T &&val)
    {
      container_.pushFront(std::move(val));
    }

    template< class... Args >
    void emplace(Args &&...args)
    {
      container_.emplaceFront(std::forward< Args >(args)...);
    }

    void pop()
    {
      if (!empty()) {
        container_.popFront();
      }
    }

    T &top()
    {
      return container_.front();
    }

    const T &top() const
    {
      return container_.front();
    }

    bool empty() const noexcept
    {
      return container_.empty();
    }

    void clear() noexcept
    {
      container_.clear();
    }
  };
}

#endif
