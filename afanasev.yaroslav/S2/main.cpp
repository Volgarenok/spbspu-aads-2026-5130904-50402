#include <iostream>
#include <fstream>
#include "input.hpp"
#include "stack.hpp"
#include "queue.hpp"

int main(int argc, char * argv[])
{
  if (argc > 2)
  {
    std::cerr << "too many args\n";
    return 1;
  }

  std::ifstream file;
  std::istream * in = & std::cin;

  if (argc == 2)
  {
    file.open(argv[1]);

    if (!file.is_open())
    {
      std::cerr << "error open file\n";
      return 1;
    }
    in = & file;
  }

  afanasev::Stack< long long > res;

  try
  {
    afanasev::Queue<afanasev::Queue<std::string>> que = afanasev::input(*in);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }





  if (!res.empty())
  {
    std::cout << res.get();
    res.pop();
  }
  else
  {
    std::cerr << "file is empty\n";
    return 1;
  }

  while (!res.empty())
  {
    std::cout << ' ' << res.get();
    res.pop();
  }

  std::cout << '\n';

  return 0;
}
