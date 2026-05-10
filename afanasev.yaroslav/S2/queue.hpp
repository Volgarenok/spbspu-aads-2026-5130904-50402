#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include "list.hpp"

namespace afanasev
{
  template< class T >
  class Queue
  {
   public:
    void push(const T & rhs);
    T & get();
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

   private:
    List< T > data_;
  };
}

template < class T >
void afanasev::Queue< T >::push(const T & rhs)
{
  if (!data_.size())
  {
    data_.pushFront(rhs);
  }
  else
  {
    LIter< T > last = data_.begin();
    LIter< T > next = last;
    ++next;
    while (next != LIter< T >())
    {
      last = next;
      ++next;
    }
    data_.insert(rhs, last);
  }
}

template < class T >
T & afanasev::Queue< T >::get()
{
  if (empty())
  {
    throw std::out_of_range("queue is empty");
  }
  return *data_.begin();
}

template < class T >
void afanasev::Queue< T >::pop() noexcept
{
  data_.popFront();
}

template < class T >
bool afanasev::Queue< T >::empty() const noexcept
{
  return !data_.size();
}

template < class T >
size_t afanasev::Queue< T >::size() const noexcept
{
  return data_.size();
}

#endif
