#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "HashTable.hpp"
#include "HashFunction.hpp"
#include "Graph.hpp"

namespace afanasev
{
  using GraphSet = HashTable< std::string, Graph, Hasher< std::string >, std::equal_to< std::string > >;

  void cmdGraphs(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdVertexes(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdOutbound(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdInbound(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdBind(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdCut(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdCreate(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdMerge(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdExtract(std::istream & in, std::ostream & out, GraphSet & graphs);
}

#endif