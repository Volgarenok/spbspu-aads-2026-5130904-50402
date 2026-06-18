#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <list.hpp>

namespace bukreev
{
  template< class T >
  class Queue
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
  void Queue< T >::push(const T& val)
  {
    m_data.pushBack(val);
  }

  template< class T >
  T Queue< T >::pop() noexcept
  {
    return m_data.popFront();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return m_data.size();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return size() == 0;
  }
}

#endif
