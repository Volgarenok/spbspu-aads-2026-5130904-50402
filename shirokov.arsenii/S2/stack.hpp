#ifndef STACK_HPP
#define STACK_HPP
#include <bilist.hpp>

namespace shirokov
{
  template < class T >
  class Stack
  {
  public:
    T& top();
    const T& top() const;
    bool empty() const noexcept;
    void push(const T& value);
    void push(T&& value);
    void pop();

  private:
    BiList< T > list_;
  };
}

#endif
