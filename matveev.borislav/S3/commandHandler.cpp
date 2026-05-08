#include "commandHandler.hpp"

#include <exception>
#include <ostream>

namespace matveev
{
bool executeCommand(std::ostream& out, GraphCollection& graphs, const List< std::string >& tokens)
{
  if (tokens.begin() == tokens.end())
  {
    return true;
  }

  LCIter< std::string > it = tokens.begin();
  const std::string& command = *it;

  if (command == GRAPHS_COMMAND)
  {
    if (!hasArgCount(tokens, 1))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    printGraphs(out, graphs);
    return true;
  }

  if (command == VERTEXES_COMMAND)
  {
    if (!hasArgCount(tokens, 2))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& graph_name = *it;

    try
    {
      printVertexes(out, graphs.at(graph_name));
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  out << INVALID_COMMAND << '\n';
  return false;
}
}
