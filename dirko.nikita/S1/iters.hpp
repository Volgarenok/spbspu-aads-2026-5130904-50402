#ifndef ITERS_HPP
#define ITERS_HPP
#include "node.hpp"

namespace dirko
{
  template < class T >
  class List;
  template < class T >
  class Iter
  {
  public:
    Iter(Node< T > *);
    T &operator*() const;
    Iter &operator++();
    Iter &operator--();
    Iter &operator++(int);
    Iter &operator--(int);
    bool operator==(const Iter< T > &) const;
    bool operator!=(const Iter< T > &) const;

  private:
    Node< T > *curr_;
    friend class List< T >;
  };
  template < class T >
  class CIter
  {
  public:
    CIter(Node< T > *);
    const T &operator*() const;
    CIter &operator++();
    CIter &operator--();
    CIter &operator++(int);
    CIter &operator--(int);
    bool operator==(const Iter< T > &) const;
    bool operator!=(const Iter< T > &) const;

  private:
    const Node< T > *curr_;
    friend class List< T >;
  };
}

#endif
