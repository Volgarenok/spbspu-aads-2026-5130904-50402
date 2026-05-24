#ifndef STACK_HPP
#define STACK_HPP
#include "iters.hpp"
#include "list.hpp"
#include "node.hpp"

namespace strelnikov
{
  template< class T >
  class Stack
  {
  public:
    void push(const T &);
    void push(T &&);
    void pop() noexcept;
    T &get() noexcept;
    bool empty() const noexcept;

  private:
    List< T > data_;
  };

  template< class T >
  void Stack< T >::push(const T &val)
  {
    data_.push_front(val);
  }

  template< class T >
  void Stack< T >::push(T &&val)
  {
    data_.push_front(std::move(val));
  }

  template< class T >
  void Stack< T >::pop() noexcept
  {
    data_.pop_front();
  }

  template< class T >
  T &Stack< T >::get() noexcept
  {
    return data_.get_head()->val;
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }
}

#endif
