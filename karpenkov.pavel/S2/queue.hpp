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
  };
}

template < class T > void karpenkov::Queue< T >::push()
{
  data.push_back();
}

#endif
