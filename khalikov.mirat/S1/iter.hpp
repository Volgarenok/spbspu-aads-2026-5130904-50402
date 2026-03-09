#ifndef ITER_HPP
#define ITER_HPP
#include "list.hpp"

namespace khalikov
{
  template< class T >
  class LIter
  {
    friend class List< T >;
  };
}

#endif
