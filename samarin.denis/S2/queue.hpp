#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include <utility>
#include <list.hpp>

namespace samarin {

  template< class T >
  class Queue {
  public:
    Queue():
      data_(),
      tail_(data_.before_begin())
    {}

    Queue(const Queue< T >& other):
      data_(other.data_),
      tail_(data_.before_begin())
    {
      for (LIter< T > it = data_.begin(); it != data_.end(); ++it) {
        tail_ = it;
      }
    }

    Queue(Queue< T >&& other) noexcept:
      data_(std::move(other.data_)),
      tail_(other.tail_)
    {}

    Queue< T >& operator=(const Queue< T >& other)
    {
      if (this != &other) {
        Queue< T > tmp(other);
        std::swap(data_, tmp.data_);
        std::swap(tail_, tmp.tail_);
      }
      return *this;
    }

    Queue< T >& operator=(Queue< T >&& other) noexcept
    {
      if (this != &other) {
        data_ = std::move(other.data_);
        tail_ = other.tail_;
      }
      return *this;
    }

    bool empty() const
    {
      return data_.empty();
    }

    void push(const T& rhs)
    {
      tail_ = data_.insert_after(tail_, rhs);
    }

    T drop()
    {
      if (data_.empty()) {
        throw std::logic_error("queue is empty");
      }
      T value = std::move(data_.front());
      data_.pop_front();
      if (data_.empty()) {
        tail_ = data_.before_begin();
      }
      return value;
    }

  private:
    List< T > data_;
    LIter< T > tail_;
  };

}

#endif
