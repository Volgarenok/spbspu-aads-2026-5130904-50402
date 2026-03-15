#ifndef ITTERS_HPP
#define ITTERS_HPP
#include "node.hpp"
namespace lachugin
{
  template < class T > class List;

  template < class T >
  class LIter
  {
    friend class List< T >;
    Node< T >* curr;
  public:
    explicit LIter(Node<T>* n) : curr(n) {}

    LIter& operator++();

    bool operator==(const LIter& other);

    bool operator!=(const LIter& other);
  };

  template < class T >
  class LCIter
  {
    friend class List< T >;
    const Node< T >* curr;
  public:
    explicit LCIter(Node< T >* n) : curr(n) {}
    const T& operator*() const;

    LCIter& operator++();

    bool operator==(const LCIter& other) const;

    bool operator!=(const LCIter& other) const;
  };

  template < class T >
  bool LIter< T >::operator==(const LIter &other)
  {
    return curr == other.curr;
  }

  template < class T >
  bool LIter< T >::operator!=(const LIter &other)
  {
    return curr != other.curr;
  }

  template < class T >
  LIter< T >& LIter< T >::operator++()
  {
    curr = curr->next;
    return *this;
  }

  template < class T > const T& LCIter< T >::operator*() const
  {
    return curr->value;
  }

  template < class T >
  bool LCIter< T >::operator==(const LCIter& other) const
  {
    return curr == other.curr;
  }

  template < class T >
  bool LCIter< T >::operator!=(const LCIter& other) const
  {
    return curr != other.curr;
  }

  template < class T >
  LCIter< T >& LCIter< T >::operator++()
  {
    curr = curr->next;
    return* this;
  }
}
#endif
