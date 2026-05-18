#ifndef STACK_HPP
#define STACK_HPP
#include "../common/list.hpp"
#include <cstddef>
#include <stdexcept>
namespace sogdanov {
  template<class T>
  class Stack {
    List<T> data_;
  public:
    void push(const T& v);
    void pop();
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& top();
  };
  template<class T>
  void Stack<T>::push(const T& v) {
    data_.push_front(v);
  }
  template<class T>
  void Stack<T>::pop() {
    if (data_.empty()) {
      throw std::runtime_error("pop on empty stack");
    }
    data_.pop_front();
  }
  template<class T>
  bool Stack<T>::empty() const noexcept {
    return data_.empty();
  }
  template<class T>
  size_t Stack<T>::size() const noexcept {
    return data_.size();
  }
  template<class T>
  T& Stack<T>::top() {
    if (data_.empty()) {
      throw std::runtime_error("top on empty stack");
    }
    return *data_.begin();
  }
}
#endif


