#include "Graph.hpp"

void shirokov::graphs(std::ostream& out, std::istream&, graphTable& gt)
{
  for (auto it = gt.begin(); it != gt.end(); ++it)
  {
    out << (*it).first << '\n';
  }
}
