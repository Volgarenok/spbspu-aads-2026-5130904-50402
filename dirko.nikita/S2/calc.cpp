#include "calc.hpp"

bool dirko::priority(const std::string &op1, const std::string &op2)
{
  short pr1 = 0, pr2 = 0;
  if (op1 == "lcm") {
    pr1 = 3;
  } else if (op1 == "+" || op1 == "-") {
    pr1 = 1;
  } else {
    pr1 = 2;
  }
  if (op2 == "lcm") {
    pr2 = 3;
  } else if (op2 == "+" || op2 == "-") {
    pr2 = 1;
  } else {
    pr2 = 2;
  }
  return pr1 >= pr2;
}
