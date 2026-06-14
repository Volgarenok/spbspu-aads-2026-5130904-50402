#ifndef QUEUEIMPL_HPP
#define QUEUEIMPL_HPP
#include "Queue.hpp"

namespace vishnevskiy
{
  template <class T>
  Queue<T>::Queue():
    fake(new LIter<T>(new List<T>))
  {}

  template <class T>
  Queue<T>::~Queue()
  {
    fake -> clear(fake);
    delete fake;
  }

  template <class T>
  void Queue<T>::push(T rhs)
  {
    if (isEmpty())
    {
      fake->insert(rhs);
    }
    else
    {
      LIter<T> temp = *fake;
      temp.end();
      temp.insert(rhs);
    }
  }

  template <class T>
  T Queue<T>::drop()
  {
    if (isEmpty())
    {
      throw std::runtime_error("No data to drop!");
    }
    LIter<T> temp;
    temp.set(fake->curr);
    ++temp;
    T tempVal = temp.value();
    fake -> curr -> next = temp.curr -> next;
    delete temp.curr;
    return tempVal;
  }

  template <class T>
  T Queue<T>::seeTop()
  {
    if (isEmpty())
    {
      throw std::runtime_error("No data!");
    }
    LIter<T> temp;
    temp.set(fake->curr);
    ++temp;
    return temp.value();
  }

  template <class T>
  bool Queue<T>::isEmpty() const
  {
    return fake -> curr -> next == nullptr;
  }
}

#endif
