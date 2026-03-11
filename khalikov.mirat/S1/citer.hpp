#ifndef CITER_HPP
#define CITER_HPP
#include "list.hpp"

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
    LCIter(const Node< T > * h);
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
    Node< T > * curr;
  };
}

#endif
