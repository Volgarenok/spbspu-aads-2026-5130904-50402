#ifndef STACK_HPP
#define STACK_HPP
#include "../common/list.hpp"

namespace dirko
{
  template < class T >
  class Stack
  {
  public:
    void push(const T &rhs);
    T &get() noexcept;
    void pop() noexcept;

  private:
    List< T > data_;
  };
}

#endif
