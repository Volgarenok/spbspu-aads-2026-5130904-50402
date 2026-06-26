#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include "command.hpp"
#include "hashtable.hpp"
#include "graph.hpp"
#include "iostream"

int main(int argc, char* argv[])
{
  namespace muh = muhamadiarov;
  if (argc != 2)
  {
    std::cerr << "Incorrect usage\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Can't open file\n";
    return 1;
  }

  muh::GraphTable graphs(10);

  std::string graphName;
  size_t edges = 0;
  size_t vertexes = 0;
  try
  {
    while (file >> graphName >> vertexes >> edges)
    {
      muh::Graph g;
      for (size_t i = 0; i < edges; ++i)
      {
        int name1, name2;
        char type;
        size_t distance = 0;
        file >> name1 >> name2 >> type >> distance;
        g.addConnection(name1, name2, type, distance);
        g.addConnection(name2, name1, type, distance);
      }
      for (size_t i = 0; i < vertexes; ++i)
      {
        int name, value;
        file >> name >> value;
        g.setPointValue(name, value);
      }
      graphs.add(graphName, std::move(g));
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error loading graph: " << e.what() << '\n';
    return 1;
  }
  file.close();

  using vcmd_t = void (*)(std::istream&, muh::GraphTable&);
  using gcmd_t = muh::Graph (*)(std::istream&, std::ostream&, muh::GraphTable&);

  using hash_t = muh::SipHash<std::string>;

  muh::RobinTable<std::string, vcmd_t, hash_t, std::equal_to<std::string> > commandsToChangeState;
  muh::RobinTable<std::string, gcmd_t, hash_t, std::equal_to<std::string> > commandsAlgorithm;

  commandsToChangeState.add("Make-Robot", muh::makeRobot);
  commandsToChangeState.add("SetRobot", muh::setRobot);
  commandsToChangeState.add("AddVertex", muh::addVertex);
  commandsToChangeState.add("AddEdge", muh::addEdge);
  commandsToChangeState.add("RemoveVertex", muh::rmVertex);
  commandsToChangeState.add("RemoveEdge", muh::rmEdge);
  commandsToChangeState.add("SetVertex", muh::setVertex);
  commandsToChangeState.add("SetEdge", muh::setEdge);

  commandsAlgorithm.add("Path", muh::findPath);
  commandsAlgorithm.add("Gold", muh::findGold);
  commandsAlgorithm.add("Visit", muh::findVisitall);
  commandsAlgorithm.add("Fast", muh::findFastes);
  commandsAlgorithm.add("Certain", muh::findCertain);

  std::string cmd;
  while (std::cin >> cmd) {
    try
    {
      if (commandsAlgorithm.has(cmd))
      {
        commandsAlgorithm.get(cmd)(std::cin, std::cout, graphs);
      }
      else
      {
        commandsToChangeState.get(cmd)(std::cin, graphs);
      }
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
