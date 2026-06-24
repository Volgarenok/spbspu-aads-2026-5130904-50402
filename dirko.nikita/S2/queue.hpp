#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <list.hpp>

namespace dirko
{
  template< class T >
  class Queue
  {
  public:
    void push(const T &rhs);
    void push(T &&rhs);
    T &get() noexcept;
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

    template< class... Args >
    void emplace(Args &&...args);

  private:
    List< T > data_;
  };
}

template< class T >
bool dirko::Queue< T >::empty() const noexcept
{
  return !data_.size();
}

template< class T >
size_t dirko::Queue< T >::size() const noexcept
{
  return data_.size();
}

template< class T >
void dirko::Queue< T >::push(const T &rhs)
{
  data_.push_back(rhs);
}

template< class T >
void dirko::Queue< T >::push(T &&rhs)
{
  data_.push_back(std::move(rhs));
}

template< class T >
T &dirko::Queue< T >::get() noexcept
{
  return data_.head();
}

template< class T >
void dirko::Queue< T >::pop() noexcept
{
  data_.pop_front();
}

template< class T >
template< class... Args >
void dirko::Queue< T >::emplace(Args &&...args)
{
  data_.emplace_back(std::forward< Args >(args)...);
}
#endif
