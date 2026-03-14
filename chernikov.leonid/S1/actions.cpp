#include "actions.hpp"
#include "LIter.hpp"
#include "LCIter.hpp"
#include <istream>
#include <string>
#include <cctype>

namespace chernikov {

  List< Sequence > parser(std::istream &in)
  {
    List< Sequence > result;
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
        continue;
      size_t pos = 0;
      size_t len = line.length();
      while (pos < len && std::isspace(line[pos]))
      {
        ++pos;
      }
      if (pos >= len)
        continue;
      std::string name;
      while (pos < len && std::isalpha(line[pos]))
      {
        name += line[pos];
        ++pos;
      }
      if (name.empty())
      {
        continue;
      }
      List< int > numbers;
      while (pos < len)
      {
        while (pos < len && std::isspace(line[pos]))
        {
          ++pos;
        }
        if (pos >= len)
          break;
        if (!std::isdigit(line[pos]))
        {
          break;
        }
        int value = 0;
        while (pos < len && std::isdigit(line[pos]))
        {
          value = value * 10 + (line[pos] - '0');
          ++pos;
        }
        numbers.push_back(value);
      }
      result.add({name, numbers});
    }
    List< Sequence > final_result;
    auto insert_pos = final_result.end();
    for (auto it = result.begin(); it != result.end(); ++it)
    {
      insert_pos = final_result.insert_after(insert_pos, *it);
    }
    return final_result;
  }
}
