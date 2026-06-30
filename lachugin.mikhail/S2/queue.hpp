#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <list.hpp>
#include <itters.hpp>
#include <utility>
#include <iostream>
namespace lachugin
{
  template< typename T >
  class Queue
  {
    List< T > l;
  public:
    void push(const T& rhs);
    void push(T&& rhs);
    void pop();
    T& front();
    bool empty() const noexcept;
    size_t size() const noexcept;
  };

  template< typename T >
  void Queue< T >::pop()
  {
    l.popFront();
  }

  template< typename T >
  void Queue< T >::push(const T& rhs)
  {
    l.pushBack(rhs);
  }

  template<class T>
  void Queue<T>::push(T&& rhs)
  {
    l.pushBack(std::move(rhs));
  }

  template< typename T >
  T& Queue< T >::front()
  {
    return l.front();
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return l.empty();
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return l.size();
  }
}
#endif

