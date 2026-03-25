#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <bilist.hpp>

namespace shirokov
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
bool shirokov::Queue< T >::empty() const noexcept
{
  return list_.empty();
}

template < class T >
void shirokov::Queue< T >::push(const T& value)
{
  list_.pushBack(value);
}

template < class T >
void shirokov::Queue< T >::push(T&& value)
{
  list_.pushBack(value);
}

template < class T >
const T& shirokov::Queue< T >::front() const
{
  return list_.back();
}

#endif
