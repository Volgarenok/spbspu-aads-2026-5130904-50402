#ifndef STACKQUEUE_HPP
#define STACKQUEUE_HPP
#include "common/List.hpp"
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

  Queue<int> readFile(char *name);
  Queue<int> getline();
  int math(Queue<std::string> exp);
  bool operCond(std::string buf);
  bool isNumber(std::string exp);
  int gcd(int a, int b);
  int count(int a, int b, std::string operation);
  int priority(std::string s);
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

inline lavrentev::Queue<int> lavrentev::readFile(char *name)
{
  lavrentev::Queue<int> ans{};
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

      ans.push(lavrentev::math(exp));
    }
    file.close();
  }
  else
  {
    throw std::runtime_error("File open error");
  }
  return ans;
}

inline lavrentev::Queue<int> lavrentev::getline()
{
  lavrentev::Queue<int> ans{};
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
    ans.push(lavrentev::math(exp));
  }
  return ans;
}

inline int lavrentev::math(lavrentev::Queue<std::string> exp)
{
  Stack<int> res{};
  Stack<std::string> op{};
  while (!exp.empty())
  {
    std::string buf = exp.drop();
    if (buf == "(")
    {
      op.push(buf);
    }
    else if (operCond(buf))
    {
      while (!op.empty() && op.top() != "(" && operCond(op.top()) && priority(op.top()) >= priority(buf))
      {
        std::string operation = op.drop();
        int oper2 = res.drop();
        int oper1 = res.drop();

        res.push(count(oper1, oper2, operation));
      }

      op.push(buf);
    }
    else if (buf == ")")
    {
      op.drop();
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
          int oper2;
          try
          {
            oper2 = std::stoi(buf);
          } catch (...)
          {
            std::cerr << "Invalid number";
            throw;
          }
          res.push(oper2);
        }
        else
        {
          op.drop();
          int oper1 = res.drop();
          int oper2;
          try
          {
            oper2 = std::stoi(buf);
          } catch (...)
          {
            std::cerr << "Invalid number";
            throw;
          }
          res.push(count(oper1, oper2, operation));
        }
      }
      else
      {
        try
        {
          res.push(std::stoi(buf));
        } catch (...)
        {
          std::cerr << "Invalid number";
          throw;
        }
      }
    }
  }

  while (!op.empty())
  {
    std::string operation = op.drop();

    if (operCond(operation))
    {
      int oper2 = res.drop();
      int oper1 = res.drop();

      res.push(count(oper1, oper2, operation));
    }
  }

  return res.drop();
}

inline bool lavrentev::operCond(std::string buf)
{
  return buf == "+" || buf == "-" || buf == "*" || buf == "/" || buf == "gcd" || buf == "%";
}

inline int lavrentev::gcd(int a, int b)
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

inline int lavrentev::count(int a, int b, std::string operation)
{
  if (operation == "+")
  {
    return a + b;
  }
  else if (operation == "-")
  {
    return a - b;
  }
  else if (operation == "*")
  {
    return a * b;
  }
  else if (operation == "/")
  {
    return a / b;
  }
  else if (operation == "gcd")
  {
    return gcd(a, b);
  }
  else if (operation == "%")
  {
    return a % b;
  }
  else
  {
    std::cerr << "Invalid operation";
    throw;
  }
}

inline int lavrentev::priority(std::string s)
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
