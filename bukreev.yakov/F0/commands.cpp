#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

size_t mysub(size_t a, size_t b)
{
  if (a > b)
  {
    return a - b;
  }
  else
  {
    return b - a;
  }
}

void bukreev::commandLoad(List< std::string > args, GraphMap& map)
{
  static bool first = true;

  if (args.size() > 4 || args.size() < 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  std::ifstream file;
  std::string name;
  char symbol;
  size_t i = 0;
  for (const std::string& arg : args)
  {
    if (i == 1)
    {
      name = arg;
    }
    else if (i == 2)
    {
      file.open(arg);
    }
    else if (i == 3)
    {
      symbol = arg[0];
    }

    i++;
  }

  if (args.size() == 3)
  {
    if (first)
    {
      symbol = '*';
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
  }

  if (!file.is_open())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Graph* graph = new Graph;

  while (file)
  {
    size_t x, y;
    file >> x;
    if (!file)
    {
      break;
    }

    file >> y;
    if (!file)
    {
      delete graph;
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    graph->points.pushBack({x, y});
  }

  graph->symbol = symbol;

  if (map.exist(name))
  {
    delete graph;
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  map.put(name, graph);

  first = false;
}

void bukreev::commandAdd(List< std::string > args, GraphMap& map, List< std::string >& names)
{
  if (args.size() != 2)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  std::string name;
  size_t i = 0;
  for (const std::string& arg : args)
  {
    if (i == 1)
    {
      name = arg;
    }

    i++;
  }

  if (!map.exist(name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  names.pushBack(name);
}

void bukreev::commandRemove(List< std::string > args, List< std::string >& names)
{
  if (args.size() != 2)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  std::string name;
  size_t i = 0;
  for (const std::string& arg : args)
  {
    if (i == 1)
    {
      name = arg;
    }

    i++;
  }

  if (!names.remove(name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
}

void bukreev::commandRename(List< std::string > args, GraphMap& map, List< std::string >& names)
{
  if (args.size() != 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  std::string name1, name2;
  size_t i = 0;
  for (const std::string& arg : args)
  {
    if (i == 1)
    {
      name1 = arg;
    }
    else if (i == 2)
    {
      name2 = arg;
    }

    i++;
  }

  if (!map.exist(name1) || map.exist(name2))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  map.put(name2, map.get(name1));
  map.erase(name1);

  names.remove(name1);
  names.pushBack(name2);
}

void bukreev::commandClear(List< std::string >& names)
{
  names.clear();
}

void bukreev::commandWidth(List< std::string > args, Canvas& canvas)
{
  if (args.size() != 2)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  size_t width;
  size_t i = 0;
  for (const std::string& arg : args)
  {
    if (i == 1)
    {
      width = std::stoull(arg);
    }

    i++;
  }

  if (width < 2)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  canvas.setSize(width, canvas.getHeight());
}

void bukreev::commandHeight(List< std::string > args, Canvas& canvas)
{
  if (args.size() != 2)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  size_t height;
  size_t i = 0;
  for (const std::string& arg : args)
  {
    if (i == 1)
    {
      height = std::stoull(arg);
    }

    i++;
  }

  if (height < 2)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  canvas.setSize(canvas.getWidth(), height);
}

void bukreev::commandDisplay(
  GraphMap& map,
  List< std::string >& names,
  Canvas& canvas,
  std::ostream& out
)
{
  canvas.clear();
  for (const std::string& name : names)
  {
    canvas.drawGraph(map.get(name));
  }

  canvas.display(out);
}

void bukreev::commandSave(
  List< std::string > args,
  GraphMap& map,
  List< std::string >& names,
  Canvas& canvas
)
{
  if (args.size() != 2)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  std::string filename;
  size_t i = 0;
  for (const std::string& arg : args)
  {
    if (i == 1)
    {
      filename = arg;
    }

    i++;
  }

  std::ofstream file(filename);
  commandDisplay(map, names, canvas, file);
}

void bukreev::commandList(GraphMap& map, List< std::string >& names)
{
  const std::pair< std::string, Graph* >* pairs = map.getPairs();
  for (size_t i = 0; i < map.capacity(); i++)
  {
    if (!map.occupied(i))
    {
      continue;
    }

    std::pair< std::string, Graph* > p = pairs[i];

    bool added = names.count(p.first) != 0;

    Graph* gr = p.second;
    std::cout << p.first << '\n';
    std::cout << "  symbol: " << gr->symbol << '\n';
    std::cout << "  added: " << (added ? "Yes" : "No") << '\n';

    std::cout << '\n';
  }
}

void bukreev::commandApproximate(List< std::string > args, GraphMap& map, List< std::string >& names)
{
  if (args.size() != 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  std::string name;
  char symbol;
  size_t i = 0;
  for (const std::string& arg : args)
  {
    if (i == 1)
    {
      name = arg;
    }
    else if (i == 2)
    {
      symbol = arg[0];
    }

    i++;
  }

  Graph* gr = map.get(name);
  size_t deviations[6] = {};
  for (const point_t pt : gr->points)
  {
    size_t v = pt.x * pt.x;
    deviations[0] += mysub(v, pt.y);

    v = static_cast< size_t >(sqrt(pt.x));
    deviations[1] += mysub(v, pt.y);

    v = static_cast< size_t >(exp(pt.x));
    deviations[2] += mysub(v, pt.y);

    v = static_cast< size_t >(log(pt.x));
    deviations[3] += mysub(v, pt.y);

    v = static_cast< size_t >(log10(pt.x));
    deviations[4] += mysub(v, pt.y);

    v = static_cast< size_t >(log2(pt.x));
    deviations[5] += mysub(v, pt.y);
  }

  size_t mindev = deviations[0];
  size_t mini = 0;
  for (size_t i = 1; i < 6; i++)
  {
    if (deviations[i] < mindev)
    {
      mindev = deviations[i];
      mini = i;
    }
  }

  std::string stdnames[] = {"parabola", "sqrt", "exp", "log", "log10", "log2"};
  std::cout << stdnames[mini] << "\n";

  Graph* graph = new Graph;
  for (const point_t pt : gr->points)
  {
    size_t v;
    switch (mini)
    {
    case 0:
      v = pt.x * pt.x;
      break;

    case 1:
      v = static_cast< size_t >(sqrt(pt.x));
      break;

    case 2:
      v = static_cast< size_t >(exp(pt.x));
      break;

    case 3:
      v = static_cast< size_t >(log(pt.x));
      break;

    case 4:
      v = static_cast< size_t >(log10(pt.x));
      break;

    case 5:
      v = static_cast< size_t >(log2(pt.x));
      break;
    }

    graph->points.pushBack({pt.x, v});
  }

  graph->symbol = symbol;

  map.put(name + "_approximated", graph);

  if (names.count(name))
  {
    names.pushBack(name + "_approximated");
  }
}
