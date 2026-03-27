#ifndef CITER_HPP
#define CITER_HPP
#include "list.hpp"
#include <stdexcept>

namespace khalikov
{
  template< class T >
  class List;

  template< class T >
  class Node;

  template< class T >
  class LCIter
  {
    friend class List< T >;
  public:
    LCIter();
    explicit LCIter(const Node< T > * h);
    LCIter(const LCIter< T > & other) = default;
    LCIter(const LIter< T > & other);
    ~LCIter() = default;
    const T & operator[](size_t index) const;
    const T & operator*() const;
    const T * operator->() const;
    LCIter< T > & operator++();
    LCIter< T > operator++(int);
    bool operator==(const LCIter< T > & other) const;
    bool operator!=(const LCIter< T > & other) const;
    LCIter< T > & operator=(const LCIter< T > & other) = default;
  private:
    const Node< T > * curr;
  };
}

template< class T >
khalikov::LCIter< T >::LCIter():
  curr(nullptr)
{}

template< class T >
khalikov::LCIter< T >::LCIter(const Node< T > * h):
  curr(h)
{}

template< class T >
khalikov::LCIter< T >::LCIter(const LIter< T > & other):
  curr(other.curr)
{}

template< class T >
const T & khalikov::LCIter< T >::operator[](size_t index) const
{
  const Node < T > * temp = curr;
  for (size_t k = 0; k < index && temp; k++)
  {
    temp = temp->next;
  }
  if (!temp)
  {
    throw std::out_of_range("Invalid operation");
  }
  return temp->val;
}

template< class T >
const T & khalikov::LCIter< T >::operator*() const
{
  if (!curr)
  {
    throw std::out_of_range("Invalid operation");
  }
  return curr->val;
}

template< class T >
const T * khalikov::LCIter< T >::operator->() const
{
  if (!curr)
  {
    throw std::out_of_range("Invalid operation");
  }
  return &(curr->val);
}

template< class T >
khalikov::LCIter< T > & khalikov::LCIter< T >::operator++()
{
  if (!curr)
  {
    throw std::out_of_range("Invalid operation");
  }
  curr = curr->next;
  return *this;
}

template< class T >
khalikov::LCIter< T > khalikov::LCIter< T >::operator++(int)
{
  LCIter< T > temp = *this;
  ++(*this);
  return temp;
}

template< class T >
bool khalikov::LCIter< T >::operator==(const LCIter< T > & other) const
{
  return curr == other.curr;
}

template< class T >
bool khalikov::LCIter< T >::operator!=(const LCIter< T > & other) const
{
  return !(*this == other);
}


#endif
