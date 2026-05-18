#include <iostream>
#include <fstream>
#include <string>
#include "queue.hpp"

namespace bukreev
{
  using Expression = Queue< std::string >;
  void inputExpressions(std::istream& file, Queue< Expression >& exprs);
}

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "Invalid usage\n";
    return 1;
  }

  std::istream *input;
  if (argc == 2)
  {
    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "Failed to open file\n";
      return 1;
    }

    input = &file;
  }
  else
  {
    input = &std::cin;
  }

  bukreev::Queue< bukreev::Expression > infix;
  bukreev::inputExpressions(*input, infix);
}

void bukreev::inputExpressions(std::istream& file, Queue< Expression >& exprs)
{
  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    Expression expr;
    std::string token;
    for (size_t i = 0; i < line.size(); i++)
    {
      if (line[i] != ' ')
      {
        token += line[i];
      }
      else
      {
        if (!token.empty())
        {
          expr.push(token);
          token.clear();
        }
      }
    }
    if (!token.empty())
    {
      expr.push(token);
      token.clear();
    }

    exprs.push(expr);
  }
}
