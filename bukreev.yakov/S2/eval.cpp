#include "eval.hpp"
#include <stdexcept>

bukreev::Expression bukreev::toPostfix(Expression infix)
{
  Expression postfix;
  Stack< std::string > tempStack;

  while (!infix.empty())
  {
    std::string token = infix.pop();
    if (token == "(")
    {
      tempStack.push(token);
      continue;
    }

    if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%")
    {
      while (!tempStack.empty())
      {
        std::string top = tempStack.pop();
        if (top == "(")
        {
          tempStack.push("(");
          break;
        }
        postfix.push(top);
      }
      tempStack.push(token);

      continue;
    }

    if (token == ")")
    {
      while (!tempStack.empty())
      {
        std::string top = tempStack.pop();
        if (top == "(")
        {
          tempStack.push("(");
          break;
        }
        postfix.push(top);
      }

      if (!tempStack.empty())
      {
        tempStack.pop();
      }

      continue;
    }
    postfix.push(token);
  }

  while (!tempStack.empty())
  {
    postfix.push(tempStack.pop());
  }

  return postfix;
}

int bukreev::evaluatePostfix(Expression postfix)
{
  Stack< int > tempStack;
  while (!postfix.empty())
  {
    std::string token = postfix.pop();
    if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%")
    {
      if (tempStack.empty())
      {
        throw std::logic_error("Incorrect expression");
      }

      int b = tempStack.pop();
      if (tempStack.empty())
      {
        throw std::logic_error("Incorrect expression");
      }

      int a = tempStack.pop();
      tempStack.push(evaluateOperation(token, a, b));
    }
    else
    {
      tempStack.push(std::stoi(token));
    }
  }

  return tempStack.pop();
}

int bukreev::evaluateOperation(std::string op, int a, int b)
{
  switch (op[0])
  {
  case '+':
    return a + b;
  
  case '-':
    return a - b;

  case '*':
    return a * b;

  case '/':
    return a / b;

  case '%':
    return a % b;

  default:
    return 0;
  }
}
