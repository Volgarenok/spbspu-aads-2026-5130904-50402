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
    bool empty() const noexcept;
    void push(const T& value);
    void push(T&& value);
    void pop();

  private:
    BiList< T > list_;
  };
}

template < class T >
bool novikov::Stack< T >::empty() const noexcept
{
return list_.empty();
}

template < class T >
void novikov::Stack< T >::push(const T& value)
{
  list_.pushBack(value);
}

template < class T >
void novikov::Stack< T >::push(T&& value)
{
  list_.pushBack(value);
}

template < class T >
const T& novikov::Stack< T >::top() const
{
  return list_.back();
}

template < class T >
T& novikov::Stack< T >::top()
{
  const Stack< T >* cthis = this;
  const T& ret = cthis->top();
  return const_cast< T& >(ret);
}

template < class T >
void novikov::Stack< T >::pop()
{
  list_.popBack();
}

#endif