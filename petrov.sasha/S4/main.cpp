#include "commands.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Invalid parameters\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Cannot open file\n";
    return 1;
  }
  petrov::Storage storage;
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    std::string dataset;
    iss >> dataset;
    petrov::InnerTree tree;
    int key = 0;
    std::string value;
    while (iss >> key >> value) {
      tree.push(key, value);
    }
    storage.push(dataset, tree);
  }
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    std::string commandName;
    iss >> commandName;
    const petrov::Command* command = petrov::findCommand(commandName);
    if (command == nullptr) {
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }
    command->execute(storage, iss, std::cout);
  }
  return 0;
}

