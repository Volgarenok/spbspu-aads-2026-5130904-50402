#ifndef LCITER_HPP
#define LCITER_HPP
#include "node.hpp"

namespace muhamadiarov
{
  template <class T>
  class List;
  template < class T >
  class LCIter
  {
  public:
    LCIter(Node< T >* node);
    LCIter& operator++();
    LCIter& operator--();
    LCIter operator++(int);
    LCIter operator--(int);
    bool operator==(const LCIter< T >& other) const;
    bool operator!=(const LCIter< T >& other) const;
    const T& operator*() const;
  private:
    const Node< T >* current_;
    friend class List< T >;
  };
}

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

template <class T>
const T& muh::LCIter< T >::operator*() const
{
  return current_->data_;
}
#endif
