#ifndef QUEQUE_HPP
#define QUEQUE_HPP
#include <list.hpp>
#include <liter.hpp>

namespace muhamadiarov
{
  template < class T >
  class Queque
  {
  public:
    bool empty() const noexcept;
    T& top() noexcept;
    void push(const T& val);
    void pop() noexcept;
    size_t size() const noexcept;
  private:
    List< T > data_;
  };
}

namespace muh = muhamadiarov;

template <class T>
bool muh::Queque< T >::empty() const noexcept
{
  if (data_.size() == 0)
  {
    return true;
  }
  return false;
}

template <class T>
T& muh::Queque< T >::top() noexcept
{
  return *(data_.begin());
}

template <class T>
void muh::Queque< T >::push(const T& val)
{
  data_.pushBack(val);
}

template <class T>
void muh::Queque< T >::pop() noexcept
{
  data_.popFront();
}

template <class T>
size_t muh::Queque< T >::size() const noexcept
{
  return data_.size();
}
#endif
