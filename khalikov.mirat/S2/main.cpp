#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include "stack.hpp"
#include "queue.hpp"

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

long long result(khalikov::Queue< std::string > &queue)
{
  khalikov::Stack< long long > stack;
  std::string str;
  while (!queue.isEmpty()) {
    str += queue.drop() + " ";
  }
  std::string el;
  std::stringstream line(str);
  while (line >> el) {
    if (std::isdigit(el[0]) || (el.size() > 1 && el[0] == '-')) {
      stack.push(std::stoll(el));
    } else {
      long long val2 = stack.drop();
      long long val1 = stack.drop();
      stack.push(op(val1, val2, el));
    }
  }
  return stack.top();
}

size_t priority(const std::string &operation)
{
  if (operation == "+" || operation == "-") {
    return 0;
  } else if (operation == "*" || operation == "/" || operation == "%") {
    return 1;
  } else if (operation == "#") {
    return 2;
  } else {
    throw std::logic_error("Operation does not exist");
  }
}

khalikov::Queue< std::string > transform(const std::string &str)
{
  khalikov::Queue< std::string > queue;
  khalikov::Stack< std::string > stack;
  std::stringstream line(str);
  std::string el;
  while (line >> el) {
    if (std::isdigit(el[0]) || (el.size() > 1 && el[0] == '-')) {
      queue.push(el);
    } else if (el == "(") {
      stack.push(el);
    } else if (el == ")") {
      while (!stack.isEmpty() && stack.top() != "(") {
        queue.push(stack.drop());
      }
      if (!stack.isEmpty()) {
        stack.pop();
      }
    } else {
      if (stack.isEmpty() || stack.top() == "(") {
        stack.push(el);
      } else if (priority(el) > priority(stack.top())) {
        stack.push(el);
      } else {
        while (!stack.isEmpty() && stack.top() != "(" && priority(el) <= priority(stack.top())) {
          queue.push(stack.drop());
        }
        stack.push(el);
      }
    }
  }
  while (!stack.isEmpty()) {
    queue.push(stack.drop());
  }
  return queue;
}

void readExp(std::istream &in)
{
  std::string str;
  khalikov::List< long long > res;
  while (std::getline(in, str)) {
    if (str.empty()) {
      continue;
    }
    try {
      khalikov::Queue< std::string > postfix = transform(str);
      res.pushFront(result(postfix));
    } catch (...) {
      throw std::logic_error("Calculation error");
    }
  }
  if (!res.isEmpty()) {
    auto it = res.cbegin();
    for (size_t i = 0; i < res.size() - 1; i++) {
      std::cout << *it << " ";
      ++it;
    }
    std::cout << *it << '\n';
  }
}

int main(int argc, char **argv)
{
  if (argc == 2) {
    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
      std::cerr << "Read error" << '\n';
      return 1;
    }
    try {
      readExp(inputFile);
    } catch (...) {
      std::cerr << "Read error" << '\n';
      return 1;
    }
  } else {
    try {
      readExp(std::cin);
    } catch (...) {
      std::cerr << "Read error" << '\n';
      return 1;
    }
  }
}
