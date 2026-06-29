#include "calculator.hpp"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace samarin {

  static bool isOperator(const std::string& token)
  {
    return token == "+" || token == "-" || token == "*"
        || token == "/" || token == "%";
  }

  static int precedence(const std::string& op)
  {
    if (op == "*" || op == "/" || op == "%") {
      return 2;
    }
    return 1;
  }

  static long long parseNumber(const std::string& token)
  {
    std::size_t pos = 0;
    long long value = 0;
    try {
      value = std::stoll(token, &pos);
    } catch (const std::exception&) {
      throw std::logic_error("operand is not a number");
    }
    if (pos != token.size()) {
      throw std::logic_error("operand is not a number");
    }
    return value;
  }

  static long long addValues(long long a, long long b)
  {
    const long long maxValue = std::numeric_limits< long long >::max();
    const long long minValue = std::numeric_limits< long long >::min();
    if (b > 0 && a > maxValue - b) {
      throw std::overflow_error("addition overflow");
    }
    if (b < 0 && a < minValue - b) {
      throw std::overflow_error("addition overflow");
    }
    return a + b;
  }

  static long long subValues(long long a, long long b)
  {
    const long long maxValue = std::numeric_limits< long long >::max();
    const long long minValue = std::numeric_limits< long long >::min();
    if (b < 0 && a > maxValue + b) {
      throw std::overflow_error("subtraction overflow");
    }
    if (b > 0 && a < minValue + b) {
      throw std::overflow_error("subtraction overflow");
    }
    return a - b;
  }

  static long long mulValues(long long a, long long b)
  {
    if (a == 0 || b == 0) {
      return 0;
    }
    const long long maxValue = std::numeric_limits< long long >::max();
    const long long minValue = std::numeric_limits< long long >::min();
    if (a > 0) {
      if (b > 0 && a > maxValue / b) {
        throw std::overflow_error("multiplication overflow");
      }
      if (b < 0 && b < minValue / a) {
        throw std::overflow_error("multiplication overflow");
      }
    } else {
      if (b > 0 && a < minValue / b) {
        throw std::overflow_error("multiplication overflow");
      }
      if (b < 0 && b < maxValue / a) {
        throw std::overflow_error("multiplication overflow");
      }
    }
    return a * b;
  }

  static long long divValues(long long a, long long b)
  {
    if (b == 0) {
      throw std::logic_error("division by zero");
    }
    if (a == std::numeric_limits< long long >::min() && b == -1) {
      throw std::overflow_error("division overflow");
    }
    return a / b;
  }

  static long long modValues(long long a, long long b)
  {
    if (b == 0) {
      throw std::logic_error("division by zero");
    }
    if (a == std::numeric_limits< long long >::min() && b == -1) {
      return 0;
    }
    long long result = a % b;
    if (result < 0) {
      result += (b > 0) ? b : -b;
    }
    return result;
  }

  static long long applyOperator(const std::string& op, long long left, long long right)
  {
    if (op == "+") {
      return addValues(left, right);
    }
    if (op == "-") {
      return subValues(left, right);
    }
    if (op == "*") {
      return mulValues(left, right);
    }
    if (op == "/") {
      return divValues(left, right);
    }
    return modValues(left, right);
  }

  static Queue< std::string > convertToPostfix(Queue< std::string > infix)
  {
    Queue< std::string > output;
    Stack< std::string > operators;
    while (!infix.empty()) {
      const std::string token = infix.drop();
      if (token == "(") {
        operators.push(token);
      } else if (token == ")") {
        while (!operators.empty() && operators.top() != "(") {
          output.push(operators.drop());
        }
        if (operators.empty()) {
          throw std::logic_error("mismatched parentheses");
        }
        operators.drop();
      } else if (isOperator(token)) {
        while (!operators.empty() && operators.top() != "("
            && precedence(operators.top()) >= precedence(token)) {
          output.push(operators.drop());
        }
        operators.push(token);
      } else {
        output.push(token);
      }
    }
    while (!operators.empty()) {
      if (operators.top() == "(") {
        throw std::logic_error("mismatched parentheses");
      }
      output.push(operators.drop());
    }
    return output;
  }

  static long long evaluatePostfix(Queue< std::string > postfix)
  {
    Stack< long long > values;
    while (!postfix.empty()) {
      const std::string token = postfix.drop();
      if (isOperator(token)) {
        if (values.empty()) {
          throw std::logic_error("invalid expression");
        }
        const long long right = values.drop();
        if (values.empty()) {
          throw std::logic_error("invalid expression");
        }
        const long long left = values.drop();
        values.push(applyOperator(token, left, right));
      } else {
        values.push(parseNumber(token));
      }
    }
    if (values.empty()) {
      throw std::logic_error("empty expression");
    }
    const long long result = values.drop();
    if (!values.empty()) {
      throw std::logic_error("invalid expression");
    }
    return result;
  }

}

long long samarin::calculateExpression(const std::string& line)
{
  std::istringstream stream(line);
  Queue< std::string > tokens;
  std::string token;
  while (stream >> token) {
    tokens.push(token);
  }
  return evaluatePostfix(convertToPostfix(std::move(tokens)));
}
