#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <iostream>
#include "../common/list.hpp"

namespace karpenkov
{
  template < class T > class Queue
  {
  private:
    karpenkov::List< T > data;

  public:
    void push(const T &rhs);
    T &front();
    void pop();
    bool empty();
  };
}

template < class T > void karpenkov::Queue< T >::push(const T &rhs)
{
  data.push_back(rhs);
}
template < class T > T &karpenkov::Queue< T >::front()
{
  return data.front();
}
template < class T > void karpenkov::Queue< T >::pop()
{
  data.pop_front();
}
template < class T > bool karpenkov::Queue< T >::empty()
{
  return data.empty();
}

#endif
