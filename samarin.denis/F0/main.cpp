#include <iostream>
#include "commands.hpp"
#include "documents.hpp"

int main()
{
  samarin::Documents docs;
  try {
    samarin::processCommands(std::cin, std::cout, docs);
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 2;
  }
  return 0;
}
