#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

namespace petrov
{
  std::string trim(const std::string& source);
  std::string toLower(const std::string& source);
  bool containsIgnoreCase(const std::string& haystack, const std::string& needle);
  std::vector< std::string > tokenize(const std::string& line);
  std::vector< std::string > splitList(const std::string& source, char delimiter);
}

#endif

