#include "LCIters.hpp"

namespace muh = muhamadiarov;

template <class T>
muh::LCIter< T >::LCIter(Node< T >* node):
  current_(node)
{}

template <class T>
muh::LCIter< T >& muh::LCIter< T >::operator++()
{
  current_ = current_->next_;
  return *this;
}

template <class T>
muh::LCIter< T >& muh::LCIter< T >::operator--()
{
  current_ = current_->prev_;
  return *this;
}

template <class T>
muh::LCIter< T > muh::LCIter< T >::operator++(int)
{
  LCIter< T > temp = *this;
  ++(*this);
  return temp;
}

template <class T>
muh::LCIter< T > muh::LCIter< T >::operator--(int)
{
  LCIter< T > temp = *this;
  --(*this);
  return temp;
}

template <class T>
bool muh::LCIter< T >::operator==(const LCIter< T >& other) const
{
  return current_ == other.current_;
}

template <class T>
bool muh::LCIter< T >::operator!=(const LCIter< T >& other) const
{
  return !(*this == other);
}
