#ifndef STACKIMPL_HPP
#define STACKIMPL_HPP
#include "Stack.hpp"

namespace vishnevskiy
{
  template <class T>
  Stack<T>::Stack():
    fake(new LIter<T>(new List<T>))
  {}

  template <class T>
  Stack<T>::~Stack()
  {
    fake -> clear(fake);
    delete fake;
  }

  template <class T>
  void Stack<T>::push(T rhs)
  {
    fake -> insert(rhs);
  }

  template <class T>
  T Stack<T>::drop()
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
  T Stack<T>::seeTop()
  {
    if (isEmpty())
    {
      throw std::runtime_error("No data!");
    }
    return fake -> curr -> next -> val;
  }

  template <class T>
  bool Stack<T>::isEmpty() const
  {
    return fake -> curr -> next == nullptr;
  }
}

#endif
