#ifndef OP_HPP
#define OP_HPP

#include <string>
#include <sstream>

namespace khalikov
{
  long long op(const long long val1, const long long val2, const std::string &operation)
  {
    long long res = 0;
    if (operation == "+") {
      res = val1 + val2;
    } else if (operation == "-") {
      res = val1 - val2;
    } else if (operation == "*") {
      res = val1 * val2;
    } else if (operation == "/") {
      res = val1 / val2;
    } else if (operation == "%") {
      res = val1 % val2;
    } else if (operation == "#") {
      std::string value1 = std::to_string(val1);
      std::string value2 = std::to_string(val2);
      std::string result = value1 + value2;
      res = std::stoll(result);
    }
    return res;
  }
}

#endif
