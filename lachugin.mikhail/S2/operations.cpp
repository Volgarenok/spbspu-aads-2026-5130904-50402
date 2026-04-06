#include "operations.hpp"
#include <iostream>
#include <climits>

namespace lachugin
{
  int sum(int& a, int& b)
  {
    if ((a > INT_MAX - b) || (a < INT_MIN + b))
    {
      throw std::invalid_argument ("Bad numbers for sum");
    }
    return a + b;
  }

  int subtraction(int &a, int &b)
  {
    if (a < b + INT_MIN)
    {
      throw std::invalid_argument ("Bad numbers for subtraction");
    }
    return a - b;
  }

  int multiply(int &a, int &b)
  {
    if ((a > INT_MAX / b) || (a < INT_MIN / b))
    {
      throw std::invalid_argument ("Bad numbers for multiply");
    }
    return a * b;
  }

  int divide(int &a, int &b)
  {
    if (b == 0)
    {
      throw std::invalid_argument ("Bad numbers for divide");
    }
    return a / b;
  }

  int mod(int &a, int &b)
  {
    if (b == 0)
    {
      throw std::invalid_argument ("Bad numbers for mod");
    }
    return a % b;
  }

}
