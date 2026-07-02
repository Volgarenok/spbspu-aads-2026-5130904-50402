#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <List.hpp>
#include <stdexcept>
#include <utility>

namespace lavrentev
{
  template <class T>
  class Queue
  {
  public:
    void push(const T &rhs);
    void push(T &&rhs);
    template <class... Args>
    void emplace(Args&&... args);
    void drop();
    const T &back() const;
    T &back();
    bool empty() const noexcept;
    const T &front() const;
    T &front();

  private:
    List<T> data;
    LIter<T> tail;
  };
}

template <class T>
void lavrentev::Queue<T>::push(const T &rhs)
{
  if (data.empty())
  {
    data.insert(data.begin(), rhs);
    tail = data.begin();
  }
  else
  {
    LIter<T> next = tail;
    tail = data.insert(next, rhs);
  }
}

template <class T>
void lavrentev::Queue<T>::push(T&& rhs)
{
  if (data.empty())
  {
    data.insert(data.begin(), std::move(rhs));
    tail = data.begin();
  }
  else
  {
    LIter<T> next = tail;
    tail = data.insert(next, std::move(rhs));
  }
}

template <class T>
template <class... Args>
void lavrentev::Queue<T>::emplace(Args&&... args)
{
  if (data.empty())
  {
    data.emplace(data.begin(), std::forward<Args>(args)...);
    tail = data.begin();
  }
  else
  {
    LIter<T> next = tail;
    tail = data.emplace(next, std::forward<Args>(args)...);
  }
}

template <class T>
void lavrentev::Queue<T>::drop()
{
  data.popFront();
  if (empty())
  {
    tail = LIter<T>{};
  }
}

template <class T>
const T &lavrentev::Queue<T>::back() const
{
  if (!(*this).empty())
  {
    return *tail;
  }
  throw std::runtime_error("Queue is empty");
}

template <class T>
T &lavrentev::Queue<T>::back()
{
  if (!(*this).empty())
  {
    return *tail;
  }
  throw std::runtime_error("Queue is empty");
}

template <class T>
bool lavrentev::Queue<T>::empty() const noexcept
{
  return data.empty();
}

template <class T>
T &lavrentev::Queue<T>::front()
{
  return data.front();
}

template <class T>
const T &lavrentev::Queue<T>::front() const
{
  return data.front();
}

#endif
