#include "Graph.hpp"
#include "bilist.hpp"

void shirokov::graphs(std::ostream& out, std::istream&, graphTable&, BiList< std::string >& names)
{
  for (auto it = names.begin(); it != names.end(); ++it)
  {
    out << *it << '\n';
  }
}
