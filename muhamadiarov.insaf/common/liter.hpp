#ifndef LITER_HPP
#define LITER_HPP
#include "node.hpp"
#include <stdexcept>

namespace muhamadiarov
{
  template <class T>
  class List;
  template < class T >
  class LIter
  {
  public:
    LIter(Node< T >* node);
    LIter(const LIter& other) = default;
    LIter(LIter&& other) = default;
    T& operator*() const;
    LIter& operator++();
    LIter& operator--();
    LIter operator++(int);
    LIter operator--(int);
    bool operator==(const LIter< T >& other) const;
    bool operator!=(const LIter< T >& other) const;
    T* operator->() const;
    LIter& operator=(const LIter& other);
    LIter& operator=(LIter&& other) = default;
  private:
    Node< T >* current_;
    friend class List< T >;
  };
}

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

template < class T >
T* muh::LIter< T >::operator->() const
{
  if (!current_)
  {
    throw std::runtime_error("dereferencing nullptr");
  }
  return &(current_->data_);
}

template < class T >
muh::LIter< T >& muh::LIter< T >::operator=(const LIter& other)
{
  if (this != &other)
  {
    current_ = other.current_;
  }
  return *this;
}
#endif
