#ifndef MATVEEV_PARSER_UTILS_HPP
#define MATVEEV_PARSER_UTILS_HPP

#include "../common/list.hpp"

#include <cstddef>
#include <string>
#include <limits>

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

inline List< std::string > splitLine(const std::string& line)
{
  List< std::string > result;
  std::string current;

  for (size_t i = 0; i < line.size(); ++i)
  {
    if (line[i] == ' ')
    {
      if (!current.empty())
      {
        result.insertAfter(result.beforeBegin(), current);
        current.clear();
      }
    }
    else
    {
      current += line[i];
    }
  }

  if (!current.empty())
  {
    result.insertAfter(result.beforeBegin(), current);
  }

  List< std::string > ordered;

  while (result.begin() != result.end())
  {
    std::string value = *result.begin();
    result.eraseAfter(result.beforeBegin());
    ordered.insertAfter(ordered.beforeBegin(), value);
  }

  return ordered;
}

inline bool parseUnsignedLongLong(const std::string& text, unsigned long long& value)
{
  if (text.empty())
  {
    return false;
  }

  unsigned long long result = 0;

  for (size_t i = 0; i < text.size(); ++i)
  {
    if (text[i] < '0' || text[i] > '9')
    {
      return false;
    }

    unsigned long long digit = static_cast< unsigned long long >(text[i] - '0');

    if (result > (std::numeric_limits< unsigned long long >::max() - digit) / 10)
    {
      return false;
    }

    result = result * 10 + digit;
  }

  value = result;
  return true;
}

inline bool parseSize(const std::string& text, size_t& value)
{
  unsigned long long result = 0;

  if (!parseUnsignedLongLong(text, result))
  {
    return false;
  }

  if (result > std::numeric_limits< size_t >::max())
  {
    return false;
  }

  value = static_cast< size_t >(result);
  return true;
}
}

#endif
