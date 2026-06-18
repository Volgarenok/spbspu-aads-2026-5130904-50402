#include <iostream>
#include <string>
#include <list.hpp>
#include "graph.hpp"
#include "commands.hpp"

namespace bukreev
{
  List< std::string > splitLine(std::string line);
}

int main()
{
  bukreev::GraphMap map;
  bukreev::Canvas canvas(5, 10);
  bukreev::List< std::string > added;

  std::string line;
  while (std::getline(std::cin, line))
  {
    bukreev::List< std::string > args = bukreev::splitLine(line);
    std::string cmdname = *(args.cbegin());

    try
    {
      if (cmdname == "load")
      {
        bukreev::commandLoad(args, map);
      }
      else if (cmdname == "width")
      {
        bukreev::commandWidth(args, canvas);
      }
      else if (cmdname == "height")
      {
        bukreev::commandHeight(args, canvas);
      }
      else if (cmdname == "add")
      {
        bukreev::commandAdd(args, map, added);
      }
      else if (cmdname == "remove")
      {
        bukreev::commandRemove(args, added);
      }
      else if (cmdname == "rename")
      {
        bukreev::commandRename(args, map, added);
      }
      else if (cmdname == "clear")
      {
        bukreev::commandClear(added);
      }
      else if (cmdname == "display")
      {
        bukreev::commandDisplay(map, added, canvas, std::cout);
      }
      else if (cmdname == "save")
      {
        bukreev::commandSave(args, map, added, canvas);
      }
      else if (cmdname == "list")
      {
        bukreev::commandList(map, added);
      }
      else if (cmdname == "approximate")
      {
        bukreev::commandApproximate(args, map, added);
      }
    }
    catch(...)
    {
      return 1;
    }
  }
}

bukreev::List< std::string > bukreev::splitLine(std::string line)
{
  List< std::string > res;

  bool word = false;
  size_t wordsti;
  for (size_t i = 0; i < line.size(); i++)
  {
    if (line[i] == ' ')
    {
      if (word)
      {
        res.pushBack(line.substr(wordsti, i - wordsti));
        word = false;
      }
    }
    else
    {
      if (!word)
      {
        wordsti = i;
        word = true;
      }
    }
  }

  if (word)
  {
    res.pushBack(line.substr(wordsti, line.size() - wordsti));
  }

  return res;
}
