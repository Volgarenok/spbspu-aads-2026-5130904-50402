#include "commands.hpp"
#include "graphCollection.hpp"
#include "graphOutput.hpp"

#include <iostream>

namespace matveev
{
void printGraphs(std::ostream& out, const GraphCollection& graphs)
{
  List< std::string > names = collectGraphNames(graphs);
  printStringList(out, names);
}
}
