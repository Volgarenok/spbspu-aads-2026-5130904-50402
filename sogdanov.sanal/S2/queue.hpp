#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/list.hpp"
#include <cstddef>
#include <stdexcept>
namespace sogdanov {
  template<class T>
  class Queue {
    List<T> data_;
    LIter<T> tailIter_;
  public:
    void push(const T& v);
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& front() noexcept;
  };
  template<class T>
  void push(const T& v) {
    if (data_.empty()) {
      data_.push_front(v);
      tailIter_ = data_.begin();
    } else {
      tailIter_ = data_.insert_after(tailIter_, v);
    }
  }
  template<class T>
  void pop() noexcept {
  template<class T>
  bool empty() const noexcept {
    return data_.empty();
  }
  template<class T>
  size_t size() const noexcept {
    return data_.size();
  }
  template<class T>
  T& front() noexcept {

