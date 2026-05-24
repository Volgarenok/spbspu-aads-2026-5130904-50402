#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <list.hpp>

namespace karpovich
{
  template< class T >
  class Queue
  {
    List< T > data_;

  public:
    bool empty() const noexcept;
    size_t size() const noexcept;
    T &front();
    void push(const T &rhs);
    void push(T &&rhs);
    void pop() noexcept;
  };

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return data_.size();
  }

  template< class T >
  T &Queue< T >::front()
  {
    return data_.front();
  }

  template< class T >
  void Queue< T >::push(const T &rhs)
  {
    data_.push_back(rhs);
  }

  template< class T >
  void Queue< T >::push(T &&rhs)
  {
    data_.push_back(std::move(rhs));
  }

  template< class T >
  void Queue< T >::pop() noexcept
  {
    data_.pop_front();
  }
}

#endif
