#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <utility>
#include <list.hpp>

namespace samarin {

  template< class T >
  class Stack {
  public:
    bool empty() const
    {
      return data_.empty();
    }

    T& top()
    {
      return data_.front();
    }

    const T& top() const
    {
      return data_.front();
    }

    void push(const T& rhs)
    {
      data_.push_front(rhs);
    }

    T drop()
    {
      if (data_.empty()) {
        throw std::logic_error("stack is empty");
      }
      T value = std::move(data_.front());
      data_.pop_front();
      return value;
    }

  private:
    List< T > data_;
  };

}

#endif
