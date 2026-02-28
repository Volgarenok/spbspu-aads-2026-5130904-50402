#ifndef ITERATORS_HPP
#define ITERATORS_HPP
#include "list.hpp"
namespace karpovich
{
  template< class T >
  class LIter {
    friend class List< T >;
    Node* ptr;
  public:
    T& operator*() const;
    LIter& operator++();
    LIter& operator--();
    LIter& operator==();
    LIter& operator!=();
  };
  template< class T >
  class LCIter {
    friend class List< T >;
    const Node* ptr;
  public:
    T& operator*() const;
    LCIter& operator++();
    LCIter& operator--();
    LCIter& operator==();
    LCIter& operator!=();
  };
}
#endif
