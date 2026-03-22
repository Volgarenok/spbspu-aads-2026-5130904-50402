#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/list.hpp"
#include <cstddef>
#include <stdexcept>
namespace sogdanov {
  template<class T>
  class Queue {
    List<T> data_;
  public:
    void push(const T& v);
    void pop();
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& front();
  };
  template<class T>
  void Queue<T>::push(const T& v) {
    data_.push_back(v);
  }
  template<class T>
  void Queue<T>::pop() {
    if (data_.empty()) {
      throw std::runtime_error("pop on empty queue");
    }
    data_.pop_front();
  }
  template<class T>
  bool Queue<T>::empty() const noexcept {
    return data_.empty();
  }
  template<class T>
  size_t Queue<T>::size() const noexcept {
    return data_.size();
  }
  template<class T>
  T& Queue<T>::front() {
    return *data_.begin();
  }
}
#endif
