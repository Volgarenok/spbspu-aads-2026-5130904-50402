#include "action.hpp"
#include <iostream>
#include <string>
#include "queque.hpp"
#include "stack.hpp"

namespace muh = muhamadiarov;

std::istrem& getLine(std::istream& in, std::string& str)
{
  str.clear();
  char c;
  while (in.get(c))
  {
    if (c == '\n')
    {
      break;
    }
    str.append(c);
  }
  return in;
}

QueQue< char > divideString(const& std::string str)
{
  Queque< char > result;
  for (char c : str)
  {
    if (c == ' ')
    {
      continue;
    }
    result.push(c);
  }
  return result;
}

Queque< *Queque< char > > input(std::iostream& in)
{
  Queque < *Queque <std::string>> result;
  std::string str;
  while (getLine(in, str))
  {
    if (str.empty())
    {
      continue;
    }
    result.push(&(divideString(str)));
  }
  return result;
}
