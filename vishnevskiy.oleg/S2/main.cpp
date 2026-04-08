#include <iostream>
#include "QueueImpl.hpp"
#include "StackImpl.hpp"

int getPriority(std::string op)
{
  if (op == "+" || op == "-")
  {
    return 1;
  }
  else if (op == "/" || op == "*")
  {
    return 2;
  }
  return 0;
}

int performOp(int n1, int n2, std::string op)
{
  if (op == "+")
  {
    return n1 + n2;
  }
  if (op == "-")
  {
    return n1 - n2;
  }
  if (op == "*")
  {
    return n1 * n2;
  }
  return n1 / n2;
}

bool isOp(std::string op)
{
  if (op == "+" || op == "-" || op == "/" || op == "*")
  {
    return true;
  }
  return false;
}

int main()
{
  Tools::Queue<std::pair<int, std::string>> q;
  Tools::Stack<std::pair<int, std::string>> s;
  std::string op;
  int num = 0;
  while (std::cin)
  {
    std::cin >> std::ws;
    int c = std::cin.peek();
    if (c != EOF)
    {
      if (std::isdigit(c))
      {
        std::cin >> num;
        q.push({num, ""});
      }
      else
      {
        std::cin >> op;
        if (isOp(op))
        {
          if (s.isEmpty() || s.seeTop().second == "(")
          {
            s.push({-1, op});
          }
          else if (getPriority(op) > getPriority(s.seeTop().second))
          {
            s.push({-1, op});
          }
          else
          {
            int pr = getPriority(op);
            while (!s.isEmpty() && pr <= getPriority(s.seeTop().second) && s.seeTop().second != "(")
            {
              q.push({-1, s.drop().second});
            }
            s.push({-1, op});
          }
        }
        else if (op == "(")
        {
          s.push({-1, op});
        }
        else if (op == ")")
        {
          while (!s.isEmpty() && s.seeTop().second != "(")
          {
            q.push({-1, s.drop().second});
          }
          s.drop();
        }
      }
    }
  }
  while (!s.isEmpty())
  {
    q.push({-1, s.drop().second});
  }
  while (!q.isEmpty())
  {
    if (q.seeTop().second == "")
    {
      s.push({q.drop().first, ""});
    }
    else
    {
      std::string op = q.drop().second;
      int num2 = s.drop().first;
      int num1 = s.drop().first;
      s.push({performOp(num1, num2, op), ""});
    }
  }
  std::cout << s.drop().first << "\n";
  return 0;
}
