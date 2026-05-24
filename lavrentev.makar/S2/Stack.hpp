#ifndef STACK_HPP
#define STACK_HPP
#include <List.hpp>

namespace lavrentev
{
  template <class T>
  class Stack
  {
  public:
    void push(const T &rhs);
    void drop();
    bool empty() const noexcept;
    T &top();

  private:
    List<T> data;
  };
}

template <class T>
void lavrentev::Stack<T>::push(const T &rhs)
{
  data.pushFront(rhs);
}

template <class T>
void lavrentev::Stack<T>::drop()
{
  data.popFront();
}

template <class T>
bool lavrentev::Stack<T>::empty() const noexcept
{
  return data.empty();
}

template <class T>
T &lavrentev::Stack<T>::top()
{
  return data.front();
}

#endif
