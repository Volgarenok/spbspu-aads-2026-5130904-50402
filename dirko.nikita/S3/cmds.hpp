#ifndef CMDS_HPP
#define CMDS_HPP

#include <string>
#include "graph.hpp"
#include "hash-table.hpp"
#include "hasher.hpp"
namespace dirko
{
  using GraphTable = HashTable< std::string, Graph, SipHasher< std::string >, std::equal_to< std::string > >;

  void cmdGraphs(std::istream &in, std::ostream &out, GraphTable &graphs);
  void cmdConections(std::istream &in, std::ostream &out, GraphTable &graphs);
  void cmdOutbound(std::istream &in, std::ostream &out, GraphTable &graphs);
  void cmdInbound(std::istream &in, std::ostream &out, GraphTable &graphs);
  void cmdBind(std::istream &in, std::ostream &out, GraphTable &graphs);
  void cmdCut(std::istream &in, std::ostream &out, GraphTable &graphs);
  void cmdCreate(std::istream &in, std::ostream &out, GraphTable &graphs);
  void cmdMerge(std::istream &in, std::ostream &out, GraphTable &graphs);
  void cmdExtract(std::istream &in, std::ostream &out, GraphTable &graphs);
}
#endif
