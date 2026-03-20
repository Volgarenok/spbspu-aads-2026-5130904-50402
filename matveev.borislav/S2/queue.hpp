#ifndef MATVEEV_QUEUE_HPP
#define MATVEEV_QUEUE_HPP

#include "../common/list.hpp"
#include <stdexcept>
#include <cstddef>

namespace matveev
{

template<class T>
class Queue
{
public:

  Queue():
    tail_(data_.beforeBegin()),
    size_(0)
  {}

  bool empty() const
  {
    return size_ == 0;
  }

  size_t size() const
  {
    return size_;
  }

  void push(const T& value)
  {
    tail_ = data_.insertAfter(tail_, value);
    ++size_;
  }

  T drop()
  {
    if (size_ == 0)
    {
      throw std::runtime_error("queue empty");
    }

    auto it = data_.begin();
    T value = *it;

    data_.removeFront();
    --size_;

    if (size_ == 0)
    {
      tail_ = data_.beforeBegin();
    }

    return value;
  }

  T& front()
  {
    if (size_ == 0)
    {
      throw std::runtime_error("queue empty");
    }

    return *data_.begin();
  }

private:
  List<T> data_;
  LIter<T> tail_;
  size_t size_;
};

}

#endif
