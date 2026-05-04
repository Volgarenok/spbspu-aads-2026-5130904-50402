#include <iostream>
#include <fstream>
#include "action.hpp"
#include "queque.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
  namespace muh = muhamadiarov;
  if (argc > 2)
  {
    std::cerr << "too many arguments\n";
    return 1;
  }
  std::ifstream file;
  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "failed openning file\n";
      return 1;
    }
  }
  std::istream& is = (argc == 2)? file : std::cin;
  muh::Stack< long long > result;
  try
  {
    muh::Queque< muh::Queque< std::string > > data = muh::input(is);
    size_t sizeData = data.size();
    for (size_t i = 0; i < sizeData; ++i)
    {
      result.push(muh::calcExpr(data.top()));
      data.pop();
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  if (!result.empty())
  {
    std::cout << result.top();
    result.pop();
  }
  while (!result.empty())
  {
    std::cout << ' ' << result.top();
    result.pop();
  }
  std::cout << '\n';
  return 0;
}
