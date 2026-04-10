#include <iostream>
#include "QueueImpl.hpp"
#include "StackImpl.hpp"

int getPriority(std::string op)
{
  if (op == "+" || op == "-")
  {
    return 1;
  }
  else if (op == "/" || op == "*" || op == "%")
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
  if (op == "%")
  {
    return n1 % n2;
  }
  return n1 / n2;
}

bool isOp(std::string op)
{
  if (op == "+" || op == "-" || op == "/" || op == "*" || op == "%")
  {
    return true;
  }
  return false;
}

int main()
{
  Tools::Queue<std::pair<int, std::string>>* q = new Tools::Queue<std::pair<int, std::string>>();
  Tools::Stack<std::pair<int, std::string>>* s = nullptr;
  try
  {
    s = new Tools::Stack<std::pair<int, std::string>>();
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "Bad alloc!" << "\n";
    delete q;
    return 1;
  }
  std::string op;
  int num = 0;
  bool hasInput = false;
  while (std::cin)
  {
    std::cin >> std::ws;
    int c = std::cin.peek();
    hasInput = true;
    if (c != EOF)
    {
      if (std::isdigit(c))
      {
        std::cin >> num;
        try
        {
          q -> push({num, ""});
        }
        catch (const std::bad_alloc& e)
        {
          std::cerr << "Bad alloc!" << "\n";
          delete q;
          delete s;
          return 1;
        }
      }
      else
      {
        std::cin >> op;
        if (isOp(op))
        {
          if (s -> isEmpty() || s -> seeTop().second == "(")
          {
            try
            {
              s -> push({-1, op});
            }
            catch (const std::bad_alloc& e)
            {
              std::cerr << "Bad alloc!" << "\n";
              delete q;
              delete s;
              return 1;
            }
          }
          else if (getPriority(op) > getPriority(s -> seeTop().second))
          {
            try
            {
              s -> push({-1, op});
            }
            catch (const std::bad_alloc& e)
            {
              std::cerr << "Bad alloc!" << "\n";
              delete q;
              delete s;
              return 1;
            }
          }
          else
          {
            int pr = getPriority(op);
            while (!s -> isEmpty() && pr <= getPriority(s -> seeTop().second) && s -> seeTop().second != "(")
            {
              try
              {
                q -> push({-1, s -> drop().second});
              }
              catch (const std::bad_alloc& e)
              {
                std::cerr << "Bad alloc!" << "\n";
                delete q;
                delete s;
                return 1;
              }
            }
            try
            {
              s -> push({-1, op});
            }
            catch (const std::bad_alloc& e)
            {
              std::cerr << "Bad alloc!" << "\n";
              delete q;
              delete s;
              return 1;
            }
          }
        }
        else if (op == "(")
        {
          s -> push({-1, op});
        }
        else if (op == ")")
        {
          while (!s -> isEmpty() && s -> seeTop().second != "(")
          {
            q -> push({-1, s -> drop().second});
          }
          if (!s -> isEmpty())
          {
            s -> drop();
          }
        }
        else
        {
          std::cerr << "Unknown operator!" << "\n";
          return 1; 
        }
      }
    }
  }

  while (!s -> isEmpty())
  {
    try
    {
      q -> push({-1, s -> drop().second});
    }
    catch (const std::bad_alloc& e)
    {
      std::cerr << "Bad alloc!" << "\n";
      delete q;
      delete s;
      return 1;
    }
  }
  while (!q -> isEmpty())
  {
    if (q -> seeTop().second == "")
    {
      try
      {
        s -> push({q -> drop().first, ""});
      }
      catch (const std::bad_alloc& e)
      {
        std::cerr << "Bad alloc!" << "\n";
        delete q;
        delete s;
        return 1;
      }
    }
    else
    {
      std::string op = q -> drop().second;
      int num2 = s -> drop().first;
      int num1 = s -> drop().first;
      try
      {
        s -> push({performOp(num1, num2, op), ""});
      }
      catch (const std::bad_alloc& e)
      {
        std::cerr << "Bad alloc!" << "\n";
        delete q;
        delete s;
        return 1;
      }
    }
  }
  if (!s -> isEmpty())
  {
    std::cout << s -> drop().first << "\n";
  }
  delete q;
  delete s;
  return 0;
}
