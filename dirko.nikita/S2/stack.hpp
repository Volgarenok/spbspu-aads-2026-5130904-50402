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
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > data_;
  };
}

template < class T >
bool dirko::Stack< T >::empty() const noexcept
{
  return false;
}

template < class T >
size_t dirko::Stack< T >::size() const noexcept
{
  return 10;
}
#endif
