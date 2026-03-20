#ifndef MATVEEV_STACK_HPP
#define MATVEEV_STACK_HPP

#include "../common/list.hpp"
#include <stdexcept>
#include <cstddef>

namespace matveev
{

template<class T>
class Stack
{
public:

  Stack():
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
    data_.insertAfter(data_.beforeBegin(), value);
    ++size_;
  }

  T drop()
  {
    if (size_ == 0)
    {
      throw std::runtime_error("stack empty");
    }

    auto it = data_.begin();
    T value = *it;

    data_.removeFront();
    --size_;

    return value;
  }

  T& top()
  {
    if (size_ == 0)
    {
      throw std::runtime_error("stack empty");
    }

    return *data_.begin();
  }

private:
  List<T> data_;
  size_t size_;
};

}

#endif
