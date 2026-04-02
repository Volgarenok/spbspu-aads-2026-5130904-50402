#ifndef QUEUEIMPL_HPP
#define QUEUEIMPL_HPP
#include "Queue.hpp"

namespace vishnevskiy
{
  template <class T>
  Queue<T>::Queue():
    fake(new List<T>),
    last(nullptr)
  {}

  template <class T>
  void Queue<T>::push(T rhs)
  {
    last.end();
    last.insert(rhs);
  }

  template <class T>
  T drop()
  {
    LIter<T>* temp = fake;
    ++temp;
    T tempVal = temp.value();
    delete temp->curr;
    delete temp;
    return tempVal;
  }

}

#endif
