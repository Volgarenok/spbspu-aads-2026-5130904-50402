#include <iostream>
#include <string>
#include <vector>

#include "commands.hpp"
#include "database.hpp"
#include "utils.hpp"

int main()
{
  petrov::Database database;
  std::string line;
  while (std::getline(std::cin, line)) {
    const std::vector< std::string > tokens = petrov::tokenize(line);
    if (tokens.empty()) {
      continue;
    }
    if (tokens[0] == "exit" || tokens[0] == "quit") {
      break;
    }
    const petrov::Command* command = petrov::findCommand(tokens[0]);
    if (command == nullptr) {
      std::cout << "<INVALID COMMAND> unknown command " << tokens[0] << "\n";
      continue;
    }
    command->execute(database, tokens, std::cin, std::cout);
  }
  return 0;
}

