#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <string>
#include <stack.hpp>
#include "graph.hpp"
#include "hashtable.hpp"
#include "robot.hpp"

namespace muhamadiarov
{
  using GraphTable = RobinTable< std::string, Graph, SipHash< std::string >, std::equal_to< std::string > >;

  void makeRobot(std::istream& in, GraphTable&);
  void setRobot(std::istream& in, GraphTable&);
  void addEdge(std::istream& in, GraphTable& graphs);
  void addVertex(std::istream& in, GraphTable& graphs);
  void rmVertex(std::istream& in, GraphTable& graphs);
  void rmEdge(std::istream& in, GraphTable& graphs);
  void setVertex(std::istream& in, GraphTable& graphs);
  void setEdge(std::istream& in, GraphTable& graphs);
  Graph findPath(std::istream& in, std::ostream& out, GraphTable& graphs);
  Graph findGold(std::istream& in, std::ostream& out, GraphTable& graphs);
  Graph findVisitall(std::istream& in, std::ostream& out, GraphTable& graphs);
  Graph findFastes(std::istream& in, std::ostream& out, GraphTable& graphs);
  Graph findCertain(std::istream& in, std::ostream& out, GraphTable& graphs);

}
#endif
