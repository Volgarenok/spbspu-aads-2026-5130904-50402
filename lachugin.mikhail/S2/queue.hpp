#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/list.hpp"
#include <iostream>

namespace lachugin
{
  template< typename T > class Queue
  {
    List< T > l;
  public:
    void push(const T& rhs);
    void pop();//     удалить
    T& front();//     получить
    bool empty() const;
    size_t size() const;
  };

  template< typename T >
  void Queue< T >::pop()
  {
    l.popFront();
  }

  template< typename T >
  void Queue< T >::push(const T& rhs)
  {
    if (l.empty())
    {
      l.add(rhs);
    }
    else
    {
      l.addNext(rhs);
    }
  }

  template< typename T >
  T& Queue< T >::front()
  {
    return l.front();
  }

  template< typename T >
  bool Queue< T >::empty() const
  {
    return l.empty();
  }

  template< typename T >
  size_t Queue< T >::size() const
  {
    return l.size();
  }
}

#endif
