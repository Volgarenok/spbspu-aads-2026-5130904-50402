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

  if (command == OUTBOUND_COMMAND)
  {
    if (!hasArgCount(tokens, 3))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& graph_name = *it;

    ++it;
    const std::string& vertex = *it;

    try
    {
      printOutbound(out, graphs.at(graph_name), vertex);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == INBOUND_COMMAND)
  {
    if (!hasArgCount(tokens, 3))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& graph_name = *it;

    ++it;
    const std::string& vertex = *it;

    try
    {
      printInbound(out, graphs.at(graph_name), vertex);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == BIND_COMMAND)
  {
    if (!hasArgCount(tokens, 5))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& graph_name = *it;

    ++it;
    const std::string& from = *it;

    ++it;
    const std::string& to = *it;

    ++it;
    unsigned long long weight = 0;

    if (!parseUnsignedLongLong(*it, weight))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    try
    {
      graphs.at(graph_name).bind(from, to, weight);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == CUT_COMMAND)
  {
    if (!hasArgCount(tokens, 5))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& graph_name = *it;

    ++it;
    const std::string& from = *it;

    ++it;
    const std::string& to = *it;

    ++it;
    unsigned long long weight = 0;

    if (!parseUnsignedLongLong(*it, weight))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    try
    {
      graphs.at(graph_name).cut(from, to, weight);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == CREATE_COMMAND)
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
      List< std::string > vertexes;
      graphs.createGraph(graph_name, vertexes);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == MERGE_COMMAND)
  {
    if (!hasArgCount(tokens, 4))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& new_name = *it;

    ++it;
    const std::string& lhs = *it;

    ++it;
    const std::string& rhs = *it;

    try
    {
      graphs.mergeGraphs(new_name, lhs, rhs);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == EXTRACT_COMMAND)
  {
    if (countArgs(tokens) < 4)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& new_name = *it;

    ++it;
    const std::string& old_name = *it;

    ++it;
    size_t vertex_count = 0;

    if (!parseSize(*it, vertex_count))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    if (countArgs(tokens) != vertex_count + 4)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    List< std::string > vertexes;

    for (size_t i = 0; i < vertex_count; ++i)
    {
      ++it;
      vertexes.insertAfter(vertexes.beforeBegin(), *it);
    }

    try
    {
      graphs.extractGraph(new_name, old_name, vertexes);
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
