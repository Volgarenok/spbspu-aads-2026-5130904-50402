#ifndef STACKQUEUE_HPP
#define STACKQUEUE_HPP
#include <List.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace lavrentev
{
  template <class T> class Queue
  {
  public:
    void push(const T &rhs);
    T drop();
    const T &back() const;
    T &back();
    bool empty() const;
    const T &front() const;
    T &front();

  private:
    List<T> data;
    LIter<T> tail;
  };

  template <class T> class Stack
  {
  public:
    void push(const T &rhs);
    T drop();
    bool empty() const;
    T &top();

  private:
    List<T> data;
  };

  Queue<long long> readFile(char *name);
  Queue<long long> getline();
  long long math(Queue<std::string> exp, long long &res);
  bool operCond(std::string buf);
  bool isNumber(std::string exp);
  long long gcd(long long a, long long b);
  long long count(long long a, long long b, long long &res, std::string operation);
  long long priority(std::string s);
} // namespace lavrentev

template <class T> void lavrentev::Queue<T>::push(const T &rhs)
{
  if (data.empty())
  {
    data.insert(data.begin(), rhs);
    tail = data.begin();
  }
  else
  {
    LIter<T> next = tail;
    tail = data.insert(next, rhs);
  }
}

template <class T> T lavrentev::Queue<T>::drop()
{
  T b = data.front();
  data.popFront();
  if ((*this).empty())
  {
    tail = LIter<T>{};
  }
  return b;
}

template <class T> const T &lavrentev::Queue<T>::back() const
{
  if (!(*this).empty())
  {
    return *tail;
  }
  throw std::runtime_error("Queue is empty");
}

template <class T> T &lavrentev::Queue<T>::back()
{
  if (!(*this).empty())
  {
    return *tail;
  }
  throw std::runtime_error("Queue is empty");
}

template <class T> bool lavrentev::Queue<T>::empty() const
{
  return data.empty();
}

template <class T> T &lavrentev::Queue<T>::front()
{
  return data.front();
}

template <class T> const T &lavrentev::Queue<T>::front() const
{
  return data.front();
}

template <class T> void lavrentev::Stack<T>::push(const T &rhs)
{
  data.pushFront(rhs);
}

template <class T> T lavrentev::Stack<T>::drop()
{
  T f = data.front();
  data.popFront();
  return f;
}

template <class T> bool lavrentev::Stack<T>::empty() const
{
  return data.empty();
}

template <class T> T &lavrentev::Stack<T>::top()
{
  return data.front();
}

inline lavrentev::Queue<long long> lavrentev::readFile(char *name)
{
  lavrentev::Queue<long long> ans{};
  std::ifstream file(name);
  if (file.is_open())
  {
    std::string line;
    while (std::getline(file, line))
    {
      lavrentev::Queue<std::string> exp{};
      std::istringstream iss(line);
      std::string symb;
      while (iss >> symb)
      {
        exp.push(symb);
      }
      long long res = 0;
      if(!exp.empty())
      {
        if(lavrentev::math(exp, res))
        {
          std::cerr << "Error";
          throw;
        }
      }
      else
      {
        continue;
      }
      ans.push(res);
    }
    file.close();
  }
  else
  {
    throw std::runtime_error("File open error");
  }
  return ans;
}

inline lavrentev::Queue<long long> lavrentev::getline()
{
  lavrentev::Queue<long long> ans{};
  std::string line;
  while (std::getline(std::cin, line))
  {
    lavrentev::Queue<std::string> exp{};
    std::istringstream iss(line);
    std::string symb;
    while (iss >> symb)
    {
      exp.push(symb);
    }
    long long res = 0;
    if(!exp.empty())
    {
      if(lavrentev::math(exp, res))
      {
        //std::cerr << "Error";
        throw;
      }
    }
    else
    {
      continue;
    }
    ans.push(res);
  }
  return ans;
}

inline long long lavrentev::math(lavrentev::Queue<std::string> exp, long long &res)
{
  Stack<long long> sRes{};
  Stack<std::string> op{};
  long long brackets = 0;
  while (!exp.empty())
  {
    std::string buf = exp.drop();
    if (buf == "(")
    {
      op.push(buf);
      ++brackets;
    }
    else if (operCond(buf))
    {
      while (!op.empty() && op.top() != "(" && operCond(op.top()) && priority(op.top()) >= priority(buf))
      {
        std::string operation = op.drop();
        long long oper2 = sRes.drop();
        long long oper1 = sRes.drop();
        long long bufAns = 0;
        if (!count(oper1, oper2, bufAns, operation))
        {
          sRes.push(bufAns);
        }
        else
        {
          return 1;
        }
      }
      op.push(buf);
    }
    else if (buf == ")")
    {
      op.drop();
      --brackets;
    }
    else
    {
      std::string operation;
      try
      {
        operation = op.top();
      } catch (...)
      {
        operation = "";
      }
      if (operCond(operation))
      {
        std::string expOper;
        try
        {
          expOper = exp.front();
        } catch (...)
        {
          expOper = "";
        }
        if (operCond(expOper) && priority(expOper) > priority(operation))
        {
          long long oper2;
          try
          {
            oper2 = std::stoll(buf);
          } catch (...)
          {
            std::cerr << "Invalid number" << "\n";
            return 2;
          }
          sRes.push(oper2);
        }
        else
        {
          op.drop();
          long long oper1 = sRes.drop();
          long long oper2;
          try
          {
            oper2 = std::stoll(buf);
          } catch (...)
          {
            std::cerr << "Invalid number" << "\n";
            return 2;
          }
          long long bufAns = 0;
          if (!count(oper1, oper2, bufAns, operation))
          {
            sRes.push(bufAns);
          }
          else
          {
            return 1;
          }
        }
      }
      else
      {
        try
        {
          sRes.push(std::stoll(buf));
        } catch (...)
        {
          std::cerr << "Invalid number" << "\n";
          return 2;
        }
      }
    }
  }
  while (!op.empty())
  {
    std::string operation = op.drop();

    if (operCond(operation))
    {
      long long oper2 = sRes.drop();
      long long oper1 = sRes.drop();
      long long bufAns = 0;
      if (!count(oper1, oper2, bufAns, operation))
      {
        sRes.push(bufAns);
      }
      else
      {
        return 1;
      }
    }
  }
  if (brackets)
  {
    return 2;
  }
  res = sRes.drop();
  return 0;
}

inline bool lavrentev::operCond(std::string buf)
{
  return buf == "+" || buf == "-" || buf == "*" || buf == "/" || buf == "gcd" || buf == "%";
}

inline long long lavrentev::gcd(long long a, long long b)
{
  while (a != 0 && b != 0)
  {
    if (a > b)
    {
      a = a % b;
    }
    else
    {
      b = b % a;
    }
  }
  return a + b;
}

inline long long lavrentev::count(long long a, long long b, long long &res, std::string operation)
{
  if (operation == "+")
  {
    res = a + b;
    return 0;
  }
  else if (operation == "-")
  {
    res = a - b;
    return 0;
  }
  else if (operation == "*")
  {
    res = a * b;
    return 0;
  }
  else if (operation == "/")
  {
    if (b != 0)
    {
      res = a / b;
      return 0;
    }
    else
    {
      std::cerr << "Zero division";
      return 1;
    }
  }
  else if (operation == "gcd")
  {
    res = gcd(a, b);
    return 0;
  }
  else if (operation == "%")
  {
    if (b != 0)
    {
      if (a < 0 || b < 0)
      {
        res = a % b + b;
      }
      res = a % b;
      return 0;
    }
    else
    {
      std::cerr << "Zero division";
      return 1;
    }
  }
  else
  {
    std::cerr << "Invalid operation";
    return 1;
  }
}

inline long long lavrentev::priority(std::string s)
{
  if (s == "+" || s == "-")
  {
    return 1;
  }
  else if (s == "*" || s == "/" || s == "%")
  {
    return 2;
  }
  else if (s == "gcd")
  {
    return 3;
  }
  return 0;
}

#endif
