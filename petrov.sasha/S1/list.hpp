#ifndef LIST_HPP
#define LIST_HPP

#include <utility>
#include <cstddef>
#include <limits>
#include <string>
#include <iostream>

namespace petrov
{
  const std::size_t max_size_value = std::numeric_limits< std::size_t >::max();

  template< class T >
  class List;

  template< class T >
  class LIter {

  };
  template< class T >
  class LCIter {

  };
}

#endif