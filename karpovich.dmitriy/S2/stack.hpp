#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <list.hpp>

namespace karpovich
{
  template< class T >
  class Stack
  {
    List< T > data_;

  public:
    bool empty() const noexcept;
    size_t size() const noexcept;
    T &top();
    void push(const T &value);
    void push(T &&value);
    template< class... Args >
    void emplace(Args &&...args);
    void pop() noexcept;
  };

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return data_.size();
  }

  template< class T >
  T &Stack< T >::top()
  {
    return data_.back();
  }

  template< class T >
  void Stack< T >::push(const T &rhs)
  {
    data_.push_back(rhs);
  }

  template< class T >
  void Stack< T >::push(T &&rhs)
  {
    data_.push_back(std::move(rhs));
  }

  template< class T >
  void Stack< T >::pop() noexcept
  {
    data_.pop_back();
  }

  template< class T >
  template< class... Args >
  void Stack< T >::emplace(Args &&...args)
  {
    data_.emplace_back(std::forward< Args >(args)...);
  }
}

#endif
