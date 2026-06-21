#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <ostream>
#include <fstream>
#include "GraphStorage.hpp"

namespace lachugin
{
  using Command = void (*)(std::istream&, std::ostream&, GraphStorage&);
  void cmdGraphs(std::istream&, std::ostream&, GraphStorage&);
  void cmdVertexes(std::istream&, std::ostream&, GraphStorage&);
  void cmdOutbound(std::istream& in, std::ostream& out, GraphStorage& storage);
  void cmdInbound(std::istream& in, std::ostream& out, GraphStorage& storage);
  void cmdCreate(std::istream& in, std::ostream& out, GraphStorage& storage);
  void cmdBind(std::istream& in, std::ostream&, GraphStorage& storage);
  void cmdCut(std::istream& in, std::ostream&, GraphStorage& storage);
  void cmdMerge(std::istream& in, std::ostream&, GraphStorage& storage);
  void cmdExtract(std::istream& in, std::ostream&, GraphStorage& storage);
}

#endif
