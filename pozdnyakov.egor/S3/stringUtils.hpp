#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <cctype>
#include <string>
#include "vector.hpp"

namespace pozdnyakov
{
  namespace utils
  {
    inline Vector< std::string > splitBySpace(const std::string &str)
    {
      Vector< std::string > tokens;
      std::size_t start = 0;
      std::size_t end = 0;

      while ((start = str.find_first_not_of(" \t\r\n", end)) != std::string::npos) {
        end = str.find_first_of(" \t\r\n", start);
        tokens.pushBack(str.substr(start, end - start));
      }

      return tokens;
    }

    inline bool isUnsignedNumber(const std::string &str)
    {
      if (str.empty()) {
        return false;
      }
      for (char c : str) {
        if (!std::isdigit(static_cast< unsigned char >(c))) {
          return false;
        }
      }
      return true;
    }
  }
}

#endif
