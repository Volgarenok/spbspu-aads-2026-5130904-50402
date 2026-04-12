#include <iostream>
#include "QueueImpl.hpp"
#include "StackImpl.hpp"
#include <limits>

long long getPriority(std::string op)
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

long long performOp(long long n1, long long n2, std::string op)
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

bool hasOverflow(long long n1, long long n2, std::string op)
{
  long long mx = std::numeric_limits<long long>::max();
  long long mn = std::numeric_limits<long long>::lowest();
  if (op == "+")
  {
    if ((n2 > 0 && n1 > mx - n2) || (n2 < 0 && n1 < mn - n2))
    {
      return true;
    }
    return false;
  }
  if (op == "-")
  {
    if ((n2 > 0 && n1 < mn + n2) || (n2 < 0 && n1 > mx + n2))
    {
      return true;
    }
    return false;
  }
  if (op == "*")
  {
    if (n1 == 0 || n2 == 0)
    {
      return false;
    }
    if ((n1 > 0 && n2 > 0 && n1 > mx / n2) || (n1 > 0 && n2 < 0 && n2 < mn / n1))
    {
      return true;
    }
    if ((n1 < 0 && n2 > 0 && n1 < mn / n2) || (n1 < 0 && n2 < 0 && -n1 > mx / -n2))
    {
      return true;
    }
    return false;
  }
  else
  {
    return false;
  }
}

int main()
{
  vishnevskiy::Queue<std::pair<long long, std::string>>* q = new vishnevskiy::Queue<std::pair<long long, std::string>>();
  vishnevskiy::Stack<std::pair<long long, std::string>>* s = nullptr;
  try
  {
    s = new vishnevskiy::Stack<std::pair<long long, std::string>>();
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "Bad alloc!" << "\n";
    delete q;
    return 1;
  }
  std::string op;
  long long num = 0;
  while (std::cin)
  {
    std::cin >> std::ws;
    int c = std::cin.peek();
    if (c != EOF)
    {
      if (std::isdigit(c))
      {
        std::cin >> num;
        if (std::cin.fail())
        {
          std::cerr << "Overflow!" << "\n";
          delete q;
          delete s;
          return 2;
        }
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
            long long pr = getPriority(op);
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
      long long num2 = s -> drop().first;
      long long num1 = s -> drop().first;
      if (!hasOverflow(num1, num2, op))
      {
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
      else
      {
        std::cerr << "Overflow!" << "\n";
        delete q;
        delete s;
        return 2;
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
