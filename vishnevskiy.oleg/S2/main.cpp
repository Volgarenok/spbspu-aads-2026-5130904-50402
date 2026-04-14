#include <iostream>
#include "QueueImpl.hpp"
#include "StackImpl.hpp"
#include <limits>
#include <fstream>

long long getPriority(std::string op)
{
  if (op == "|")
  {
    return 1;
  }
  else if (op == "+" || op == "-")
  {
    return 2;
  }
  else if (op == "/" || op == "*" || op == "%")
  {
    return 3;
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
    if ((n1 % n2) >= 0)
    {
      return n1 % n2;
    }
    return n2 + (n1 % n2);
  }
  if (op == "|")
  {
    return n1 | n2;
  }
  return n1 / n2;
}

bool isOp(std::string op)
{
  if (op == "+" || op == "-" || op == "/" || op == "*" || op == "%" || op == "|")
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

int process(std::istream* in, long long& out)
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
  (*in) >> std::ws;
  if ((*in).peek() == EOF)
  {
    delete q;
    delete s;
    return 3;
  }
  while ((*in) && (*in).peek() != '\n')
  {
    *in >> std::ws;
    int c = (*in).peek();
    if (c != EOF && (*in).peek() != '\n')
    {
      if (std::isdigit(c))
      {
        (*in) >> num;
        if ((*in).fail())
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
        (*in) >> op;
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
      if (s -> isEmpty())
      {
        std::cerr << "Invalid expression!" << "\n";
        delete q;
        delete s;
        return 2;
      }
      long long num2 = s -> drop().first;
      if (s -> isEmpty())
      {
        std::cerr << "Invalid expression!" << "\n";
        delete q;
        delete s;
        return 2;
      }
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
    out = s -> drop().first;
  }
  delete q;
  delete s;
  return 0;
}

int main(int argc, char* argv[])
{
  int r = 0;
  long long val = 0;
  vishnevskiy::Stack<long long>* res = new vishnevskiy::Stack<long long>();
  std::ifstream f;
  std::istream* in = &std::cin;
  if (argc > 1)
  {
    f.open(argv[1]);
    if (!f)
    {
      std::cerr << "File not found" << "\n";
      delete res;
      return 2;
    }
    in = &f;
  }
  while (in -> peek() != EOF && r == 0)
  {
    try
    {
      r = process(in, val);
      if (r == 0)
      {
        res -> push(val);
      }
    }
    catch (const std::bad_alloc& e)
    {
      std::cerr << "Bad alloc!" << "\n";
      delete res;
      return 1;
    }
  }
  while (!res -> isEmpty())
  {
    std::cout << res -> drop();
    if (!res -> isEmpty())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  delete res;
  if (r == 3)
  {
    r = 0;
  }
  return r;
}
