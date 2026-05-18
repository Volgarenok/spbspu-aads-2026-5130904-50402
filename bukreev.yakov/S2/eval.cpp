#include "eval.hpp"

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
