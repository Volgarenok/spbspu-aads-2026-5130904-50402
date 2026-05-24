#ifndef CALCULATE_CPP
#define CALCULATE_CPP
#include <stdexcept>
#include "calculate.hpp"
#include "queue.hpp"
#include "stack.hpp"

namespace permyakov
{
  size_t getPriority(const std::string elm)
  {
    if (elm == "+" || elm == "-") {
      return 1;
    }
    if (elm == "*" || elm == "/" || elm == "%") {
      return 2;
    }
    if (elm == "^") {
      return 3;
    }
    return 0;
  }

  Queue< std::string > convertToPostfix(Queue< std::string > exc)
  {
    Queue< std::string > result;
    Stack< std::string > stk;
    while(!exc.isEmpty()) {
      const std::string element = exc.front();
      if (std::isdigit(static_cast< unsigned char >(element[0]))) {
        result.push(element);
      } else if (element == "(") {
        stk.push(element);
      } else if (element == ")") {
        while (!stk.isEmpty() && stk.top() != "(") {
          result.push(stk.top());
          stk.pop();
        }
        if (!stk.isEmpty()) {
          stk.pop();
        }
      } else {
        size_t elementPriority = getPriority(element);
        while (!stk.isEmpty() && stk.top() != "(") {
          size_t stkPriority = getPriority(stk.top());
          if ((stkPriority > elementPriority) || (stkPriority == elementPriority && element != "^")) {
            result.push(stk.top());
            stk.pop();
          } else {
            break;
          }
        }
        stk.push(element);
      }
      exc.pop();
    }
    while (!stk.isEmpty()) {
      result.push(stk.top());
      stk.pop();
    }
    return result;
  }

  long long sum(const long long elm1, const long long elm2)
  {
    if (elm2 > 0 && elm1 > MAX - elm2) {
      throw std::overflow_error("Sum overflow");
    } else if (elm2 < 0 && elm1 < MIN - elm2) {
      throw std::overflow_error("Sum underflow");
    }
    return elm1 + elm2;
  }

  long long sub(const long long elm1, const long long elm2)
  {
    if (elm2 > 0 && elm1 < MIN + elm2) {
      throw std::overflow_error("Subtract underflow");
    } else if (elm2 < 0 && elm1 > MAX + elm2) {
      throw std::overflow_error("Subtract overflow");
    }
    return elm1 - elm2;
  }

  long long mult(const long long elm1, const long long elm2)
  {
    if (elm1 == 0 || elm2 == 0) {
      return 0;
    }
    if (elm1 > 0) {
      if (elm2 > 0 && elm1 > MAX / elm2) {
        throw std::overflow_error("Mult overflow");
      } else if (elm2 < 0 && elm2 < MIN / elm1) {
        throw std::overflow_error("Mult underflow");
      }
    } else {
      if (elm2 > 0 && elm1 < MIN / elm2) {
        throw std::overflow_error("Mult underflow");
      } else if (elm2 < 0 && elm1 < MAX / elm2) {
        throw std::overflow_error("Mult overflow");
      }
    }
    return elm1 * elm2;
  }

  long long div(const long long elm1, const long long elm2)
  {
    if (elm2 == 0) {
      throw std::logic_error("Division by zero");
    }
    if (elm1 == MIN && elm2 == -1) {
      throw std::overflow_error("Division overflow");
    }
    return elm1 / elm2;
  }

  long long mod(const long long elm1, const long long elm2)
  {
    if (elm2 == 0) {
      throw std::logic_error("Division by zero");
    } else {
      return (elm1 % elm2 + elm2) % elm2;
    }
  }

  long long x_or(const long long elm1, const long long elm2)
  {
    return elm1 ^ elm2;
  }

  long long binCalculate(const std::string op, const long long elm1, const long long elm2)
  {
    if (op == "+") {
      return sum(elm1, elm2);
    } else if (op == "-") {
      return sub(elm1, elm2);
    } else if (op == "*") {
      return mult(elm1, elm2);
    } else if (op == "/") {
      return div(elm1, elm2);
    } else if (op == "%") {
      return mod(elm1, elm2);
    } else if (op == "^") {
      return x_or(elm1, elm2);
    }
    throw std::invalid_argument("Unknown operator");
  }

  long long calculateExpression(Queue< std::string > exc)
  {
    Stack< long long > stk;
    while (!exc.isEmpty()) {
      std::string element = exc.front();
      if (std::isdigit(static_cast< unsigned char >(element[0]))) {
        stk.push(std::stoll(element));
      } else {
        if (stk.size() < 2) {
          throw std::logic_error("Stack underflow");
        }
        long long elm2 = stk.top();
        stk.pop();
        long long elm1 = stk.top();
        stk.pop();
        stk.push(binCalculate(element, elm1, elm2));
      }
      exc.pop();
    }
    if (stk.size() != 1) {
      throw std::logic_error("Incorrect expression structure");
    }
    return stk.top();
  }
}

#endif
