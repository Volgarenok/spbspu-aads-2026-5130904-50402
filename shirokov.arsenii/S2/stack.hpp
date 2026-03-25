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

template < class T >
bool shirokov::Stack< T >::empty() const noexcept
{
  return list_.empty();
}

template < class T >
void shirokov::Stack< T >::push(const T& value)
{
  list_.pushBack(value);
}

template < class T >
void shirokov::Stack< T >::push(T&&)
{
  return;
}

#endif
