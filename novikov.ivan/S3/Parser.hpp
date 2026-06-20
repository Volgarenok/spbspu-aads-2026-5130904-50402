#ifndef PARSER_HPP
#define PARSER_HPP
#include "Graph.hpp"

namespace novikov
{
  std::pair< novikov::graphTable, novikov::List< std::string > > parse(std::istream& in);

  template< class T >
  bool has(const List< T >& list, const T& elem);

  template< class T >
  void sortedInsert(List< T >& list, const T& value);
}

#endif
