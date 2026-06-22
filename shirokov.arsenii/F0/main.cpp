#include <Vector.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include "commands.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char* argv[])
{
  shirokov::SymbolTable symbols;
  shirokov::CommandTable cmds;

  cmds.insert("print", shirokov::builtinPrint);
  cmds.insert("newline", shirokov::builtinNewline);
  cmds.insert("zeros", shirokov::builtinZeros);
  cmds.insert("ones", shirokov::builtinOnes);
  cmds.insert("eye", shirokov::builtinEye);
  cmds.insert("fill", shirokov::builtinFill);
  cmds.insert("det", shirokov::builtinDet);
  cmds.insert("tr", shirokov::builtinTr);
  cmds.insert("transpose", shirokov::builtinTranspose);
  cmds.insert("concatH", shirokov::builtinConcatH);
  cmds.insert("concatV", shirokov::builtinConcatV);
  cmds.insert("concatDiag", shirokov::builtinConcatDiag);

  shirokov::Parser parser(symbols, cmds);

  if (argc == 2)
  {
    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "Error: open failed\n";
      return 1;
    }
    std::string line;
    while (std::getline(file, line))
    {
      if (line.empty())
      {
        continue;
      }
      try
      {
        shirokov::Vector< shirokov::Token > tokens = shirokov::tokenize(line);
        parser.parseAndExecute(tokens);
      }
      catch (const std::exception& e)
      {
        std::cerr << e.what() << "\n";
        return 1;
      }
    }
    return 0;
  }

  if (argc == 1)
  {
    std::string line;
    while (true)
    {
      std::cout << ">>> ";
      if (!std::getline(std::cin, line))
      {
        break;
      }
      if (line.empty())
      {
        continue;
      }
      try
      {
        shirokov::Vector< shirokov::Token > tokens = shirokov::tokenize(line);
        parser.parseAndExecute(tokens);
      }
      catch (const std::exception& e)
      {
        std::cerr << e.what() << "\n";
      }
    }
    return 0;
  }
  return 1;
}
