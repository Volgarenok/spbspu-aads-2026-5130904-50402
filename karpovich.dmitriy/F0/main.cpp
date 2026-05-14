#include <iostream>
#include <limits>
#include <string>
#include "engine.hpp"

int main()
{
  karpovich::Engine engine;
  std::string input_line;
  while (std::getline(std::cin, input_line)) {
    try {
      engine.processCommand(input_line);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
