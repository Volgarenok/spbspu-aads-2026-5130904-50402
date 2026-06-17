#include <cstddef>
#include <stdexcept>
#include "Graph.hpp"
#include "readfile.hpp"

int main(int argc, char *argv[]) {
  if (argc != 2)
  {
    std::cerr << "Input processing error" << "\n";
    return 1;
  }
  lavrentev::List<std::pair<std::string, lavrentev::Graph>> grs;
  std::string name;
  try
  {
    lavrentev::readfile(argv[1], grs);
  } catch (const std::runtime_error &) {
    std::cerr << "Input processing error" << "\n";
    return 2;
  }

  lavrentev::HashTable<
    std::string,
    cmd_t,
    Siphash< std::string >,
    std::equal_to<std::string>
  > commands;

  commands["graphs"] = lavrentev::graphs;
  commands["vertexes"] = lavrentev::Graph::vertexes;
  commands["outbound"] = lavrentev::Graph::outbound;
  commands["inbound"] = lavrentev::Graph::inbound;
  commands["bind"] = lavrentev::Graph::bind;
  commands["cut"] = lavrentev::Graph::cut;
  commands["create"] = lavrentev::Graph::create;
  commands["merge"] = lavrentev::Graph::merge;
  commands["extract"] = lavrentev::Graph::extract;

  std::string cmd;
  while (true)
{
  if (!(std::cin >> cmd))
  {
    break;
  }
  try
  {
    if (!commands.has(cmd))
    {
      throw std::logic_error("Unknown command");
    }

    commands[cmd](std::cin, grs);
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>\n";

    std::cin.clear();
    std::cin.ignore(
      std::numeric_limits<std::streamsize>::max(),
      '\n'
    );
  }
}

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
