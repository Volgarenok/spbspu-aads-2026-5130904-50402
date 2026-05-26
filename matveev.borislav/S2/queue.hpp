#ifndef MATVEEV_QUEUE_HPP
#define MATVEEV_QUEUE_HPP

#include "../common/list.hpp"
#include <stdexcept>
#include <cstddef>
#include <utility>

namespace matveev
{
template< class T >
class Queue
{
public:
  Queue():
    data_(),
    tail_(data_.beforeBegin()),
    size_(0)
  {}

  bool empty() const noexcept
  {
    return size_ == 0;
  }

  size_t size() const noexcept
  {
    return size_;
  }

  void push(const T& value)
  {
    tail_ = data_.insertAfter(tail_, value);
    ++size_;
  }

  template< class... Args >
  T& emplace(Args&&... args)
  {
    tail_ = data_.emplaceAfter(tail_, std::forward< Args >(args)...);
    ++size_;
    return *tail_;
  }

  void drop()
  {
    if (size_ == 0)
    {
      throw std::runtime_error("queue empty");
    }

    data_.removeFront();
    --size_;

    if (size_ == 0)
    {
      tail_ = data_.beforeBegin();
    }
  }

  T& front()
  {
    if (size_ == 0)
    {
      throw std::runtime_error("queue empty");
    }

    return *data_.begin();
  }

  const T& front() const
  {
    if (size_ == 0)
    {
      throw std::runtime_error("queue empty");
    }

    return *data_.begin();
  }

private:
  List< T > data_;
  LIter< T > tail_;
  size_t size_;
};
}

#endif
