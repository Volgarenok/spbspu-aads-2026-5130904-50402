#include <exception>
#include <iostream>
#include "StackQueue.hpp"

int main(int argc, char *argv[])
{
  std::string name;
  try
  {
    if (argc == 2)
    {
      lavrentev::Stack<long long> res = lavrentev::readFile(argv[1]);
      if (res.empty())
      {
        std::cout << "\n";
        return 0;
      }
      bool first = true;
      while (!res.empty())
      {
        if (!first)
        {
          std::cout << " " << res.top();
        }
        else
        {
          std::cout << res.top();
          first = false;
        }
        res.drop();
      }
      std::cout << "\n";
    }
    else if (argc == 1)
    {
      lavrentev::Queue<long long> res = lavrentev::getline();
      if (res.empty())
      {
        std::cout << "\n";
        return 0;
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
    else
    {
      std::cout << "\n";
      return 0;
    }
  } catch (const std::exception &e)
  {
    std::cerr << "Input processing error" << "\n";
    return 2;
  }
}
