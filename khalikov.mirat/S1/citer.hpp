#ifndef CITER_HPP
#define CITER_HPP
#include "list.hpp"

namespace khalikov
{
  template< class T >
  class LCIter
  {
    friend class List< T >;
  };
}

#endif
