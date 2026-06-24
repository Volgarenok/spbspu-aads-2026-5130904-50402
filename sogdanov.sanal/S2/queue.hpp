#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>

#include "../common/list.hpp"

namespace sogdanov {

  template< class T >
  class Queue {
  public:
    void push(const T &v);
    template< class... Args >
    void emplace(Args &&... args);
    void pop();
    bool empty() const noexcept;
    size_t size() const noexcept;
    T &front();

  private:
    List< T > data_;
  };

  template< class T >
  void Queue< T >::push(const T &v)
  {
    data_.push_back(v);
  }
  template< class... Args >
  void emplace(Args &&... args)
  {
    data_.emplace_back(std::forward< Args >(args)...);
  }

  template< class T >
  void Queue< T >::pop()
  {
    if (data_.empty()) {
      throw std::runtime_error("pop on empty queue");
    }
    data_.pop_front();
  }

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
    if (data_.empty()) {
      throw std::runtime_error("front on empty file\n");
    }
    return *data_.begin();
  }

}

#endif
