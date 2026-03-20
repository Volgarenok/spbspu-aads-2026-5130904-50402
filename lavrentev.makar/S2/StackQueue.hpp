#ifndef STACKQUEUE_HPP
#define STACKQUEUE_HPP
#include "common/List.hpp"

namespace lavrentev{
  template <class T>
  class Queue
  {
    public:
      void push(const T& rhs);
      T drop();
      const T& back() const;
      T& back();
      bool empty() const;
      const T& front() const;
      T& front();

    private:
      List<T> data;
      LIter<T> tail;
  };

  template <class T>
  class Stack
  {
    public:
      void push(const T& rhs);
      T drop();
      bool empty() const;
      T& top();
    
    private:
      List<T> data;
  };
}

template <class T>
void lavrentev::Stack<T>::push(const T& rhs)
{
  data.pushFront(rhs);
}

template <class T>
T lavrentev::Stack<T>::drop()
{
  T f = data.front();
  data.popFront();
  return f;
}

template <class T>
bool lavrentev::Stack<T>::empty() const
{
  return data.empty();
}

template <class T>
T& lavrentev::Stack<T>::top()
{
  return data.front();
}

#endif
