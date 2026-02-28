#ifndef ITERATORS_HPP
#define ITERATORS_HPP
#include "list.hpp"
namespace karpovich
{
  template< class T >
  class LIter {
    friend class List< T >;
  };
  template< class T >
  class LCIter {
    friend class List< T >;
  };
}
#endif