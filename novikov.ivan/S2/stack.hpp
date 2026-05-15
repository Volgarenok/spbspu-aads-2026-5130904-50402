#ifndef STACK_HPP
#define STACK_HPP
#include <bilist.hpp>

namespace novikov
{
  template < class T >
  class Stack
  {
  public:
    T& top();
    const T& top() const;
    bool empty() const;
    void push(const T& value);
    void push(T&& value);
    void pop();

  private:
    List< T > list_;
  };
}

#endif
