#ifndef ITER_HPP
#define ITER_HPP

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
    LIter(Node< T > * h);
    LIter(const LIter< T > & other) = default;
    ~LIter() = default;
    T & operator[](size_t index);
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

#endif
