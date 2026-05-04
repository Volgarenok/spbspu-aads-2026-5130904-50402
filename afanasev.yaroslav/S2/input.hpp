#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <string>
#include "queue.hpp"

namespace afanasev
{
  Queue< Queue< std::string > > input(std::istream & in);
}

afanasev::Queue< afanasev::Queue< std::string > > afanasev::input(std::istream & in)
{
  Queue< Queue< std::string > > res;

  std::string str;

  while (std::getline(in, str))
  {
    if (str.empty())
    {
      continue;
    }

    Queue< std::string > que;

    std::string el;

    for (size_t i = 0; i < str.length(); ++i)
    {
      char ch = str[i];

      if (ch == ' ')
      {
        if (!el.empty())
        {
          que.push(el);
          el.clear();
        }
      }
      else
      {
        el += ch;
      }
    }

    if (!el.empty())
    {
      que.push(el);
    }
    res.push(que);

  }
  return res;
}

#endif
