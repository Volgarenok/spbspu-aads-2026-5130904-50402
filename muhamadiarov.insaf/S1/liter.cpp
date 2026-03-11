#include "LIters.hpp"

namespace muh = muhamadiarov;

template <class T>
muh::LIter< T >::LIter(Node< T >* node):
  current_(node)
{}

template <class T>
T& muh::LIter< T >::operator*() const
{
  if (!current_)
  {
    throw std::runtime_error("incrementing a nullptr");
  }
  return current_->data_;
}

template <class T>
muh::LIter< T >& muh::LIter< T >::operator++()
{
  current_ = current_->next_;
  return *this;
}

template <class T>
muh::LIter< T >& muh::LIter< T >::operator--()
{
  current_ = current_->prev_;
  return *this;
}

template <class T>
muh::LIter< T > muh::LIter< T >::operator++(int)
{
  LIter< T > temp = *this;
  ++(*this);
  return temp;
}

template <class T>
muh::LIter< T > muh::LIter< T >::operator--(int)
{
  LIter< T > temp = *this;
  --(*this);
  return temp;
}

template <class T>
bool muh::LIter< T >::operator==(const LIter< T >& other) const
{
  return current_ == other.current_;
}

template <class T>
bool muh::LIter< T >::operator!=(const LIter< T >& other) const
{
  return !(*this == other);
}
