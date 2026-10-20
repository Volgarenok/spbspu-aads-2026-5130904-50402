#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "iters.hpp"
#include "list.hpp"
#include "node.hpp"

namespace strelnikov
{
  template< class T >
  class Queue
  {
  public:
    void push(const T &);
    void push(T &&);
    void pop() noexcept;
    T &get() noexcept;
    bool empty() const noexcept;

  private:
    List< T > data_;
    LIter< T > tail_;
  };

  template< class T >
  void Queue< T >::push(const T &val)
  {
    if (!data_.empty()) {
      tail_ = data_.insert_after(tail_, val);
    } else {
      data_.push_front(val);
      tail_ = data_.get_head();
    }
  }

  template< class T >
  void Queue< T >::push(T &&val)
  {
    if (!data_.empty()) {
      tail_ = data_.insert_after(tail_, std::move(val));
    } else {
      data_.push_front(std::move(val));
      tail_ = data_.get_head();
    }
  }

  template< class T >
  void Queue< T >::pop() noexcept
  {
    data_.pop_front();
    if (data_.begin() == data_.end()) {
      tail_ = LIter< T >();
    }
  }

  template< class T >
  T &Queue< T >::get() noexcept
  {
    return data_.get_head()->val;
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }
}

#endif
