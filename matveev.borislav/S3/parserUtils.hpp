#ifndef MATVEEV_PARSER_UTILS_HPP
#define MATVEEV_PARSER_UTILS_HPP

#include "../common/list.hpp"

#include <cstddef>
#include <string>

namespace matveev
{
inline size_t countArgs(const List< std::string >& args)
{
  size_t count = 0;

  for (LCIter< std::string > it = args.begin(); it != args.end(); ++it)
  {
    ++count;
  }

  return count;
}

inline bool hasArgCount(const List< std::string >& args, size_t expected)
{
  return countArgs(args) == expected;
}
}

#endif
