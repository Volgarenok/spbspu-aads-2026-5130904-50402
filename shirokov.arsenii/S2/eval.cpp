#include "eval.hpp"
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace
{
  bool isNumber(const std::string& line)
  {
    for (size_t i = 0; i < line.length(); ++i)
    {
      if (!('0' <= line[i] && line[i] <= '9'))
      {
        return false;
      }
    }
    return true;
  }

  int getPriority(const std::string& op)
  {
    if (op == "<<")
    {
      return 3;
    }
    if (op == "*" || op == "/" || op == "%")
    {
      return 2;
    }
    if (op == "+" || op == "-")
    {
      return 1;
    }
    return 0;
  }

  bool isOperation(const std::string& s)
  {
    bool res = s == "<<";
    res = res || s == "*" || s == "/" || s == "%";
    res = res || s == "+" || s == "-";
    return res;
  }

  shirokov::Queue< std::string > infixToPostfix(shirokov::Queue< std::string > infix)
  {
    shirokov::Queue< std::string > postfix;
    shirokov::Stack< std::string > stack;
    while (!infix.empty())
    {
      std::string val = infix.front();
      if (val == "(")
      {
        stack.push(val);
      }
      else if (val == ")")
      {
        std::string op = stack.top();
        while (isOperation(op))
        {
          postfix.push(op);
          stack.pop();
          op = stack.top();
        }
        if (stack.top() == "(")
        {
          stack.pop();
        }
      }
      else if (isOperation(val))
      {
        if (!stack.empty())
        {
          std::string op = stack.top();
          while (op != "(" && getPriority(op) >= getPriority(val))
          {
            postfix.push(op);
            stack.pop();
            if (stack.empty())
            {
              break;
            }
            op = stack.top();
          }
        }
        stack.push(val);
      }
      else
      {
        postfix.push(val);
      }
      infix.pop();
    }
    while (!stack.empty())
    {
      postfix.push(stack.top());
      stack.pop();
    }
    return postfix;
  }

  shirokov::Queue< std::string > split(const std::string& line, char delimiter = ' ')
  {
    shirokov::Queue< std::string > res;
    std::string curr;
    for (size_t i = 0; i < line.length(); ++i)
    {
      if (line[i] == delimiter)
      {
        res.push(curr);
        curr.clear();
      }
      else
      {
        curr += line[i];
      }
    }
    if (!curr.empty())
    {
      res.push(curr);
    }
    return res;
  }
}

long long shirokov::eval(std::string line)
{
  Queue< std::string > infix = split(line);
  Queue< std::string > postfix = infixToPostfix(infix);
  Stack< long long > results;
  while (!postfix.empty())
  {
    std::string val = postfix.front();
    postfix.pop();
    if (isNumber(val))
    {
      results.push(std::stoll(val));
    }
    else
    {
      if (results.empty())
      {
        throw std::runtime_error("Not enough operands");
      }
      long long op2 = results.top();
      results.pop();
      if (results.empty())
      {
        throw std::runtime_error("Not enough operands");
      }
      long long op1 = results.top();
      results.pop();

      long long res = 0;
      if (val == "+")
      {
        res = op1 + op2;
      }
      else if (val == "-")
      {
        res = op1 - op2;
      }
      else if (val == "*")
      {
        res = op1 * op2;
      }
      else if (val == "/")
      {
        res = op1 / op2;
      }
      else if (val == "%")
      {
        res = op1 % op2;
        if (op1 < 0 || op2 < 0)
        {
          res += op2;
        }
      }
      else if (val == "<<")
      {
        res = op1 << op2;
      }
      results.push(res);
    }
  }
  return results.top();
}
