#include "actions.hpp"
#include <iostream>
#include <string>
#include "queue.hpp"

dirko::Queue< std::string > dirko::parseString(std::string str)
{
  Queue< std::string > ret;
  size_t start = 0;
  size_t index = str.find(' ');
  std::string subString = "";
  while (index != std::string::npos) {
    subString = str.substr(start, index - start);
    ret.push(subString);
    start = index + 1;
    index = str.find(' ', start);
  }
  subString = str.substr(start);
  ret.push(subString);

  return ret;
}

dirko::Queue< dirko::Queue< std::string > > dirko::input(std::istream &is)
{
  Queue< Queue< std::string > > ret;
  std::string str;
  while (std::getline(is, str)) {
    if (str.empty()) {
      continue;
    }
    ret.push(parseString(str));
  }
  return ret;
}
