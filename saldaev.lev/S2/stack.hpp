#ifndef STACK_HPP
#define STACK_HPP
#include "../common/List.hpp"

namespace saldaev
{
  template< class T >
  struct Stack
  {
    void push(const T &value);
    T &top();
    const T &top() const;
    void pop();
    void clear() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > data_;
  };

}

#endif
