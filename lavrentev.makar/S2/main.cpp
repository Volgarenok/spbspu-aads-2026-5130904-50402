#include <exception>
#include <iostream>
#include "StackQueue.hpp"

int main(int argc, char *argv[])
{
  std::string name;
  lavrentev::Queue<long long> res;
  try
  {
    if (argc == 2)
    {
      res = lavrentev::readFile(argv[1]);
    }
    else if (argc == 1)
    {
      res = lavrentev::getline();
    }
    else
    {
      std::cout << "\n";
      return 0;
    }
    if (res.empty())
    {
      std::cout << "\n";
      return 0;
    }
  } catch (const std::exception &e)
  {
    std::cerr << "Input processing error" << "\n";
    return 2;
  }
  bool first = true;
  while (!res.empty())
  {
    if (!first)
    {
      std::cout << " " << res.front();
    }
    else
    {
      std::cout << res.front();
      first = false;
    }
    res.drop();
  }
  std::cout << "\n";
}
