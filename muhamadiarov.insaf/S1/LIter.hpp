#ifndef LITER_HPP
#define LITER_HPP
#include "node.hpp"

namespace muhamadiarov
{
  template < class T >
  class LIter
  {
  public:
    LIter(Node< T >* node);
    T& operator*() const;
    LIter& operator++();
    LIter& operator--();
    LIter& operator++(int);
    LIter& operator--(int);
    bool operator==(const LIter< T >& other) const;
    bool operator!=(const LIter< T >& other) const;
  private:
    Node< T >* current_;
    friend class List< T >;
  };
};
#endif
