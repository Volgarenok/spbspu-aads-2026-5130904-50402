#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "List.hpp"
#include <stdexcept>

namespace chernikov {

  template < typename T > class Queue
  {
  private:
    List< T > data;

  public:
    Queue() = default;
    bool empty() const;
    size_t size() const;
    void push(const T &val);
    T drop();
    const T &front() const;
    T &front();
  };

  template < typename T > bool Queue< T >::empty() const
  {
    return data.empty();
  }
  template < typename T > size_t Queue< T >::size() const
  {
    return data.size();
  }
  template < typename T > void Queue< T >::push(const T &val)
  {
    data.push_back(val);
  }
  template < typename T > T Queue< T >::drop()
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    T val = data.front();
    data.first_delete();
    return val;
  }
  template < typename T > const T &Queue< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    return data.front();
  }
  template < typename T > T &Queue< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("Stack is empty");
    }
    return data.front();
  }
}

#endif