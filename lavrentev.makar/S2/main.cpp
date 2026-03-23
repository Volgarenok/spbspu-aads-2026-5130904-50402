#include <iostream>
#include "StackQueue.hpp"

int main(int argc, char *argv[])
{
  std::string name;
  lavrentev::Queue<int> res;
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
    std::cerr << "No data";
    return 2;
  }
  bool first = true;
  while(!res.empty())
  {
    if (!first)
    {
      std::cout << " " << res.front();
    } else {
      std::cout << res.front();
      first = false;
    }
    res.drop();
  }
}
