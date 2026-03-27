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
    void push();
    T &front();
  };
}

template < class T > void karpenkov::Queue< T >::push()
{
  data.push_back();
}
template < class T > T &karpenkov::Queue< T >::front()
{
  return data.top();
}

#endif
