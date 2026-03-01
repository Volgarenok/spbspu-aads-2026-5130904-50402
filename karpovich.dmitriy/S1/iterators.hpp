#ifndef ITERATORS_HPP
#define ITERATORS_HPP
#include "list.hpp"
#include "node.hpp"
namespace karpovich
{
  template< class T >
  class LIter {
    friend class List< T >;
    Node< T >* ptr_;
  public:
    LIter(Node< T >* p);
    T& operator*() const;
    LIter& operator++();
    LIter& operator--();
    LIter& operator++(int);
    LIter& operator--(int);
    bool operator==(const LIter& other) const;
    bool operator!=(const LIter& other) const;
  };
  template< class T >
  class LCIter {
    friend class List< T >;
    const Node< T >* ptr_;
  public:
    LCIter(const Node< T >* p);
    const T& operator*() const;
    LCIter& operator++();
    LCIter& operator--();
    LCIter& operator++(int);
    LCIter& operator--(int);
    bool operator==(const LCIter& other) const;
    bool& operator!=(const LCIter& other) const;
  };

  template < class T > LIter< T >::LIter(Node< T >* p):
    ptr_(p)
  {}

  template < class T > T& LIter< T >::operator*() const
  {
    return ptr_->val;
  }

  
}
#endif
