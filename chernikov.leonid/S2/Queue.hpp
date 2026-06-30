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
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push(const T &val);
    void pop();
    const T &front() const;
    T &front();
  };

  template < typename T > bool Queue< T >::empty() const noexcept
  {
    return data.empty();
  }

  template < typename T > size_t Queue< T >::size() const noexcept
  {
    return data.size();
  }

  template < typename T > void Queue< T >::push(const T &val)
  {
    data.push_back(val);
  }

  template < typename T > T &Queue< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("Queue is empty");
    }
    return data.front();
  }

  template < typename T > const T &Queue< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("Queue is empty");
    }
    return data.front();
  }

  template < typename T > void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("Queue is empty");
    }
    data.first_delete();
  }

}

#endif
