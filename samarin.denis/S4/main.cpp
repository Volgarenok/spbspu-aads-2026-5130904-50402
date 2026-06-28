#include <fstream>
#include <iostream>
#include "commands.hpp"
#include "dataset.hpp"
#include "input.hpp"

int main(int argc, char * argv[])
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
  samarin::DatasetCollection datasets;
  try {
    samarin::readDatasets(file, datasets);
    samarin::processCommands(std::cin, std::cout, datasets);
  } catch (const std::exception & e) {
    std::cerr << e.what() << "\n";
    return 2;
  }
  return 0;
}
