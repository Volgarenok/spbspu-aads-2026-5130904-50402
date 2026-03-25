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
void shirokov::Stack< T >::push(T&& value)
{
  list_.pushBack(value);
}

template < class T >
const T& shirokov::Stack< T >::top() const
{
  return list_.back();
}

template < class T >
T& shirokov::Stack< T >::top()
{
  const Stack< T >* cthis = this;
  const T& ret = cthis->top();
  return const_cast< T& >(ret);
}

template < class T >
void shirokov::Stack< T >::pop()
{
  return;
}

#endif
