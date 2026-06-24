#ifndef STACK_HPP
#define STACK_HPP
#include <list.hpp>
#include <liter.hpp>

namespace muhamadiarov
{
  template <class T>
  class Stack
  {
  public:
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& top();
    void push(const T& val);
    void pop() noexcept;
  private:
    List< T > data_;
  };
}

namespace muh = muhamadiarov;

template <class T>
bool muh::Stack< T >::empty() const noexcept
{
  if (data_.size() == 0)
  {
    return true;
  }
  return false;
}

template <class T>
size_t muh::Stack< T >::size() const noexcept
{
  return data_.size();
}

template <class T>
T& muh::Stack< T >::top()
{
  return *(data_.end());
}

template <class T>
void muh::Stack< T >::push(const T& val)
{
  data_.pushBack(val);
}

template <class T>
void muh::Stack< T >::pop() noexcept
{
  data_.popBack();
}
#endif
