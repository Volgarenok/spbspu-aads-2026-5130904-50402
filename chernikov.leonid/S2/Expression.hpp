#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "Stack.hpp"
#include "Queue.hpp"
#include <string>
#include <sstream>
#include <cctype>
#include <climits>
#include <limits>

namespace chernikov {

  inline bool willAddOverflow(long long a, long long b)
  {
    if (b > 0 && a > std::numeric_limits< long long >::max() - b)
      return true;
    if (b < 0 && a < std::numeric_limits< long long >::min() - b)
      return true;
    return false;
  }
  inline bool willSubOverflow(long long a, long long b)
  {
    if (b < 0 && a > std::numeric_limits< long long >::max() + b)
      return true;
    if (b > 0 && a < std::numeric_limits< long long >::min() + b)
      return true;
    return false;
  }
  inline bool willMulOverflow(long long a, long long b)
  {
    if (a == 0 || b == 0)
      return false;
    if (a > 0 && b > 0 && a > std::numeric_limits< long long >::max() / b)
      return true;
    if (a > 0 && b < 0 && b < std::numeric_limits< long long >::min() / a)
      return true;
    if (a < 0 && b > 0 && a < std::numeric_limits< long long >::min() / b)
      return true;
    if (a < 0 && b < 0 && a < std::numeric_limits< long long >::max() / b)
      return true;
    return false;
  }

  inline int getPriority(char operation)
  {
    switch (operation)
    {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
    case '%':
      return 2;
    default:
      return 0;
    }
  }
  inline bool isNumber(const std::string &str)
  {
    if (str.empty())
    {
      return false;
    }
    size_t first = (str[0] == '-') ? 1 : 0;
    if (first >= str.size())
    {
      return false;
    }
    for (size_t i = first; i < str.size(); ++i)
    {
      if (!std::isdigit(str[i]))
      {
        return false;
      }
    }
    return true;
  }
  Queue< std::string > infixToPostfix(const std::string &expression)
  {
    std::istringstream iss(expression);
    std::string token;
    Stack< char > operators;
    Queue< std::string > output;
    while (iss >> token)
    {
      if (isNumber(token))
      {
        output.push(token);
      } else if (token == "(")
      {
        operators.push('(');
      } else if (token == ")")
      {
        while (!operators.empty() && operators.top() != '(')
        {
          output.push(std::string(1, operators.drop()));
        }
        if (operators.empty())
        {
          throw std::logic_error("Incorect close bracket or missed open bracket");
        }
        operators.drop();
      } else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%")
      {
        char op = token[0];
        while (!operators.empty() && operators.top() != '(' && getPriority(operators.top()) >= getPriority(op))
        {
          output.push(std::string(1, operators.drop()));
        }
        operators.push(op);
      } else
      {
        throw std::logic_error("Invalid token: " + token);
      }
    }
    while (!operators.empty())
    {
      if (operators.top() == '(')
      {
        throw std::logic_error("Incorect open bracket or missed close bracket");
      }
      output.push(std::string(1, operators.drop()));
    }
    return output;
  }
  inline long long modPositive(long long a, long long b)
  {
    long long result = a % b;
    if (result < 0)
      result += b;
    return result;
  }
  long long evaluatePostfix(Queue< std::string > &postfix)
  {
    Stack< long long > values;
    while (!postfix.empty())
    {
      std::string token = postfix.drop();
      if (isNumber(token))
      {
        values.push(std::stoll(token));
      } else
      {
        if (values.size() < 2)
        {
          throw std::logic_error("Not enough operands");
        }
        long long b = values.drop();
        long long a = values.drop();
        long long result = 0;
        switch (token[0])
        {
        case '+':
          if (willAddOverflow(a, b))
          {
            throw std::logic_error("Addition overflow");
          }
          result = a + b;
          break;
        case '-':
          if (willSubOverflow(a, b))
          {
            throw std::logic_error("Subtraction overflow");
          }
          result = a - b;
          break;
        case '*':
          if (willMulOverflow(a, b))
          {
            throw std::logic_error("Multiplication overflow");
          }
          result = a * b;
          break;
        case '/':
          if (b == 0)
            throw std::logic_error("Division by zero");
          result = a / b;
          break;
        case '%':
          if (b == 0)
            throw std::logic_error("Modulo by zero");
          result = modPositive(a, b);
          break;
        default:
          throw std::logic_error("Unknown operator");
        }
        values.push(result);
      }
    }
    if (values.size() != 1)
    {
      throw std::logic_error("Invalid expression");
    }
    return values.drop();
  }
  inline long long evaluateExpression(const std::string &expression)
  {
    if (expression.empty() || expression.find_first_not_of(" \t") == std::string::npos)
    {
      throw std::logic_error("Empty expression");
    }
    chernikov::Queue< std::string > postfix = chernikov::infixToPostfix(expression);
    return evaluatePostfix(postfix);
  }
}
#endif
