#include <iostream>
#include <istream>
#include <fstream>

#include "queue.hpp"
#include "tools.hpp"
#include <sstream>

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "Bad arguments";
    return 1;
  }
  std::istream *input = &std::cin;
  std::ifstream fin;
  if (argc == 2)
  {
    fin.open(argv[1]);
    if (!fin.is_open())
    {
      std::cerr << "Error opening file\n";
      return 1;
    }
    input = &fin;
  }
  try
  {
    std::string line;
    bool first = true;
    lachugin::Stack< long long > values;

    while (std::getline(*input, line))
    {
      if (line.empty())
      {
        continue;
      }

      std::istringstream iss(line);

      lachugin::Queue< std::string > q;
      lachugin::reedInput(iss, q);

      lachugin::Queue< std::string > postfix = lachugin::infixToPostfix(q);

      lachugin::counting(postfix, values);
    }
    while (!values.empty())
    {
      if (!first)
      {
        std::cout << " ";
      }
      std::cout << values.top();
      first = false;
      values.pop();
    }
    std::cout << "\n";
  }
  catch (const std::invalid_argument& e)
  {
    std::cerr << e.what();
    return 1;
  }
  catch (const std::out_of_range& e)
  {
    std::cerr << e.what();
    return 1;
  }
}

