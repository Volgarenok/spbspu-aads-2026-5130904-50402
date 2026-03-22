#include "calc.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include <cstddef>

bool dirko::getPriority(const std::string &op1, const std::string &op2)
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

bool dirko::isOpt(std::string candidate)
{
  bool ret = false;
  for (size_t i = 0; i < dirko::opt_size; ++i) {
    ret = ret || (candidate == opts[i]);
  }
  return ret;
}

dirko::Queue< std::string > dirko::convert(dirko::Queue< std::string > inf)
{
  Queue< std::string > ret;
  Stack< std::string > st;
  while (!inf.empty()) {
    std::string curr = inf.get();
    inf.pop();
    if (curr == "(") {
      st.push(curr);
      continue;
    }
    if (isOpt(curr)) {
      while (!st.empty() && getPriority(curr, st.get()) && st.get() != "(") {
        ret.push(st.get());
        st.pop();
      }
      st.push(curr);
      continue;
    }
    if (curr == ")") {
      while (!st.empty() && st.get() != "(") {
        ret.push(st.get());
        st.pop();
      }
      if (!st.empty()) {
        st.pop();
      }
      continue;
    }
    ret.push(curr);
  }
  while (!st.empty()) {
    ret.push(st.get());
    st.pop();
  }
  return ret;
}
