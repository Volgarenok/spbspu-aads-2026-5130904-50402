#ifndef LIST_HPP
#define LIST_HPP

#include <utility>
#include <cstddef>
#include <limits>
#include <string>
#include <iostream>

namespace petrov
{
  const std::size_t max_size_value = std::numeric_limits< std::size_t >::max();

  template< class T >
  class List;

  template< class T >
  struct Node {
    T value_;
    Node< T >* next_, * prev_;

    Node(const T& value):
      value_(value)
      next_(nullptr)
      prev_(nullptr)
    {}
  };

  template< class T >
  class LIter {
    friend class List< T >;
  public:
    LIter(Node< T >* ptr = nullptr):
      ptr_(ptr)
    {}
    T * operator->() const
    {
      return std::addressof(ptr_->value_);
    }
    LIter< T >& operator++() {
      ptr_ = ptr_->next;
      return *this;
    }
    LIter< T > operator++(int) {
      LIter< T > temp(*this);
      ++(*this);
      return temp;
    }
    LIter< T >& operator--() {
      ptr_ = ptr_->prev;
      return *this;
    }
    LIter< T > operator--(int) {
      LIter< T > temp(*this);
      --(*this);
      return temp;
    }
    bool operator==(const LIter< T >& other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const LIter< T >& other) const {
      return !(*this == other);
    }
  private:
    Node< T >* ptr_;
  };
  template< class T >
  class LCIter {

  };
}

#endif