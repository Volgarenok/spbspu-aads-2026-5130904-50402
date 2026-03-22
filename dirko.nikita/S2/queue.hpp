#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/list.hpp"

namespace dirko
{
  template < class T >
  class Queue
  {
  public:
    void push(const T &rhs);
    T &get() noexcept;
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > data_;
  };
}

template < class T >
bool dirko::Queue< T >::empty() const noexcept
{
  return false;
}

template < class T >
size_t dirko::Queue< T >::size() const noexcept
{
  return 10;
}

template < class T >
void dirko::Queue< T >::push(const T &rhs)
{
  data_.push_back(10);
}

template < class T >
T &dirko::Queue< T >::get() noexcept
{
  return data_.head();
}
#endif
