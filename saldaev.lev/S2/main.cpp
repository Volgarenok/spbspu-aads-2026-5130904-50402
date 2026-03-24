#include "queue.hpp"
#include <fstream>
#include <iostream>

saldaev::Queue< saldaev::Queue< std::string > > parse(saldaev::Queue< std::string > rawLines);

int main(int argc, char *argv[])
{
  if (argc > 2) {
    return 1;
  }

  std::ifstream file;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      return 1;
    }
  }
  std::istream &input = (argc == 2) ? file : std::cin;

  saldaev::Queue< std::string > rawLines;
  std::string currLine;
  while (std::getline(input, currLine)) {
    if (!currLine.empty()) {
      rawLines.push(currLine);
    }
  }

  saldaev::Queue< saldaev::Queue< std::string > > parsedLines = parse(rawLines);

  while (!parsedLines.empty()) {
    saldaev::Queue< std::string > tokens = parsedLines.front();
    parsedLines.pop();

    while (!tokens.empty()) {
      std::cout << tokens.front() << "";
      tokens.pop();
    }
    std::cout << '\n';
  }
}

saldaev::Queue< saldaev::Queue< std::string > > parse(saldaev::Queue< std::string > rawLines)
{
  saldaev::Queue< saldaev::Queue< std::string > > parsedLines;
  while (!rawLines.empty()) {
    saldaev::Queue< std::string > tokens;
    std::string currLine = rawLines.front();
    rawLines.pop();

    size_t start = 0;
    size_t index = currLine.find(' ');
    std::string token = "";
    while (index != std::string::npos) {
      token = currLine.substr(start, index - start);
      tokens.push(token);
      start = index + 1;
      index = currLine.find(' ', start);
    }
    token = currLine.substr(start);
    tokens.push(token);

    parsedLines.push(tokens);
  }
  return parsedLines;
}
