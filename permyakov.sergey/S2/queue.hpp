#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include "list.hpp"
#include "iterators.hpp"

namespace permyakov
{
  template< class T > class Queue
  {
    List< T > data;
    public:
      void push(const T value);
      void pop();
      T front();
      size_t size();
      bool isEmpty();
  };

  template< class T > void Queue< T >::push(const T value)
  {
    if (data.isEmpty()) {
      data.push_front(value);
    } else {
      LIter< T > iter = data.begin();
      for (size_t i = 1; i < data.size(); ++i) {
        ++iter;
      }
      data.insert_after(iter, value);
    }
  }

  template< class T > void Queue< T > ::pop()
  {
    data.pop_front();
  }

  template< class T > T Queue< T >::front()
  {
    return data.front();
  }

  template< class T > size_t Queue< T >::size()
  {
    return data.size();
  }

  template< class T > bool Queue< T >::isEmpty()
  {
    return !size();
  }
}

#endif
