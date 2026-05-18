#ifndef STACK_HPP
#define STACK_HPP

#include <list.hpp>

namespace bukreev
{
  template< class T >
  class Stack
  {
  public:
    void push(const T& val);
    T pop() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;

  private:
    List< T > m_data;
  };

  template< class T >
  void Stack< T >::push(const T& val)
  {
    m_data.pushBack(val);
  }

  template< class T >
  T Stack< T >::pop() noexcept
  {
    return m_data.popBack();
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return m_data.size();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return size() == 0;
  }
}

#endif
