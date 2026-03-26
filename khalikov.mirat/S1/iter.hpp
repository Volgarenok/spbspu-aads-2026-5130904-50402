#ifndef ITER_HPP
#define ITER_HPP
#include <stdexcept>
#include <cstddef>

namespace khalikov
{
  template< class T >
  class List;

  template< class T >
  class Node;

  template< class T >
  class LIter
  {
    friend class List< T >;
  public:
    LIter();
    explicit LIter(Node< T > * h);
    LIter(const LIter< T > & other) = default;
    ~LIter() = default;
    T & operator*();
    T * operator->();
    LIter< T > & operator++();
    LIter< T > operator++(int);
    bool operator==(const LIter< T > & other) const;
    bool operator!=(const LIter< T > & other) const;
    LIter< T > & operator=(const LIter< T > & other) = default;
  private:
    Node< T > * curr;
  };
}

template< class T >
khalikov::LIter< T >::LIter():
	curr(nullptr)
{}

template< class T >
khalikov::LIter< T >::LIter(Node< T > * h)
	curr(h)
{}

template< class T >
T & khalikov::LIter< T >::operator*()
{
  if (!curr)
  {
    throw std::runtime_error();
  }
  return curr->val;
}

template< class T >
T * khalikov::LIter< T >::operator->()
{
  if (!curr)
  {
    throw std::runtime_error();
  }
  return &(curr->val);
}

template< class T >
khalikov::LIter< T > & khalikov::LIter< T >::operator++()
{
  if (!curr)
  {
    throw std::out_of_range();
  }
  curr = curr->next;
  return *this;
}

template< class T >
khalikov::LIter< T > khalikov::LIter< T >::operator++(int)
{
  LIter< T > temp = *this;
  ++(*this);
  return temp;
}

template< class T >
bool khalikov::LIter< T >::operator==(const LIter< T > & other) const
{
  return curr == other.curr;
}

template< class T >
bool khalikov::LIter< T >::operator!=(const LIter< T > & other) const
{
  return !(*this == other);
}

#endif
