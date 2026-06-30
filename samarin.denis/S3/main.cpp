#include <fstream>
#include <iostream>
#include "commands.hpp"
#include "graph_collection.hpp"
#include "input.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "no input file provided\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "cannot open input file\n";
    return 1;
  }
  samarin::GraphCollection graphs;
  try {
    samarin::readGraphs(file, graphs);
    samarin::processCommands(std::cin, std::cout, graphs);
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 2;
  }
  return 0;
}
