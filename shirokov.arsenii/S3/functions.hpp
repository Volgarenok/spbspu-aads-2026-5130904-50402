#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "Graph.hpp"

namespace shirokov
{
  std::pair< shirokov::graphTable, shirokov::BiList< std::string > > parse(std::istream& in);

  template< class T >
  bool has(const BiList< T >& list, const T& elem);

  template< class T >
  void sortSaveInsertion(BiList< T >& list, const T& value);
}

#endif
