#ifndef OP_HPP
#define OP_HPP

#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <cmath>

namespace khalikov
{
  long long op(const long long val1, const long long val2, const std::string &operation)
  {
    long long maxValue = std::numeric_limits< long long >::max();
    long long minValue = std::numeric_limits< long long >::min();
    long long res = 0;
    if (operation == "+") {
      if ((val2 > 0 && val1 > maxValue - val2) || (val2 < 0 && val1 < minValue - val2)) {
        throw std::overflow_error("Overflow");
      }
      res = val1 + val2;
    } else if (operation == "-") {
      if ((val2 > 0 && val1 < minValue + val2) || (val2 < 0 && val1 > maxValue + val2)) {
        throw std::underflow_error("Underflow");
      }
      res = val1 - val2;
    } else if (operation == "*") {
      if (val1 > 0) {
        if (val2 > 0) {
          if (val1 > maxValue / val2) {
            throw std::overflow_error("Overflow");
          }
        } else {
          if (val2 < minValue / val1) {
            throw std::underflow_error("Underflow");
          }
        }
      } else if (val1 < 0) {
        if (val2 > 0) {
          if (val1 < minValue / val2) {
            throw std::underflow_error("Underflow");
          } else {
            if (val2 < maxValue / val1) {
              throw std::overflow_error("Overflow");
            }
          }
        }
      }
      res = val1 * val2;
    } else if (operation == "/") {
      if (val2 == 0) {
        throw std::logic_error("Zero division");
      }
      res = val1 / val2;
    } else if (operation == "%") {
      if (val2 == 0) {
        throw std::logic_error("Zero division");
      }
      res = val1 % val2;
      if (res < 0) {
        res += std::abs(val2);
      }
    } else if (operation == "#") {
      std::string value1 = std::to_string(val1);
      std::string value2 = std::to_string(val2);
      std::string result = value1 + value2;
      try {
        res = std::stoll(result);
      } catch (const std::out_of_range &) {
        throw std::overflow_error("Overflow");
      }
    }
    return res;
  }
}

#endif
