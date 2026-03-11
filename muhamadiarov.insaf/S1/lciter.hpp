#ifndef LCITER_HPP
#define LCITER_HPP
#include "node.hpp"

namespace muhamadiarov
{
  template <class T>
  class List;
  template < class T >
  class LCIter
  {
  public:
    LCIter(Node< T >* node);
    LCIter& operator++();
    LCIter& operator--();
    LCIter& operator++(int);
    LCIter& operator--(int);
    bool operator==(const LCIter< T >& other) const;
    bool operator!=(const LCIter< T >& other) const;
  private:
    const Node< T >* current_;
    friend class List< T >;
  };
};
#endif
