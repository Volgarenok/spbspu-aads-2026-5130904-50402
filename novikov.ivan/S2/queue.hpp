#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <bilist.hpp>

namespace novikov
{
  template < typename T >
  class Queue
  {
  public:
    T& front();
    const T& front() const;
    bool empty() const noexcept;
    void push(const T& value);
    void push(T&& value);
    void pop();

  private:
    BiList< T > list_;
  };
}

template < class T >
bool novikov::Queue< T >::empty() const noexcept
{
return list_.empty();
}

template < class T >
void novikov::Queue< T >::push(const T& value)
{
  list_.pushBack(value);
}

template < class T >
void novikov::Queue< T >::push(T&& value)
{
  list_.pushBack(value);
}

template < class T >
const T& novikov::Queue< T >::front() const
{
  return list_.front();
}

template < class T >
T& novikov::Queue< T >::front()
{
  const Queue< T >* cthis = this;
  const T& ret = cthis->front();
  return const_cast< T& >(ret);
}

template < class T >
void novikov::Queue< T >::pop()
{
  list_.popFront();
}

#endif