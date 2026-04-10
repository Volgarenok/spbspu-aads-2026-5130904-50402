#include <iostream>
#include "QueueImpl.hpp"
#include "StackImpl.hpp"

size_t getPriority(std::string op)
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

size_t performOp(size_t n1, size_t n2, std::string op)
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
  vishnevskiy::Queue<std::pair<size_t, std::string>>* q = new vishnevskiy::Queue<std::pair<int, std::string>>();
  vishnevskiy::Stack<std::pair<size_t, std::string>>* s = nullptr;
  try
  {
    s = new vishnevskiy::Stack<std::pair<size_t, std::string>>();
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "Bad alloc!" << "\n";
    delete q;
    return 1;
  }
  std::string op;
  size_t num = 0;
  while (std::cin)
  {
    std::cin >> std::ws;
    size_t c = std::cin.peek();
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
            size_t pr = getPriority(op);
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
      size_t num2 = s -> drop().first;
      size_t num1 = s -> drop().first;
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
