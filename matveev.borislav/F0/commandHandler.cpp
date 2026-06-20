#include "commandHandler.hpp"

#include "commands.hpp"
#include "hash-table.hpp"
#include "hashFunctions.hpp"
#include "parserUtils.hpp"
#include "pathfinding.hpp"

#include <cstddef>
#include <exception>
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>

namespace
{
template< class T >
void appendBack(matveev::List< T >& list, const T& value)
{
  matveev::LIter< T > prev = list.beforeBegin();
  matveev::LIter< T > it = list.begin();

  while (it != list.end())
  {
    ++prev;
    ++it;
  }

  list.insertAfter(prev, value);
}

void printHelp(std::ostream& out)
{
  out << "help\n";
  out << "locations\n";
  out << "make <location>\n";
  out << "show <location>\n";
  out << "delete <location>\n";
  out << "rename <location> <new-name>\n";
  out << "clear <location>\n";
  out << "add-item <location> <item> <type>\n";
  out << "delete-item <location> <item>\n";
  out << "rename-item <location> <item> <new-name>\n";
  out << "move-item <item> <from> <to>\n";
  out << "merge <new-location> <location-1> <location-2>\n";
  out << "find <item> <location>...\n";
  out << "filter <location> <type>\n";
  out << "connect <location-1> <location-2> <cost>\n";
  out << "disconnect <location-1> <location-2>\n";
  out << "path <from> <to>\n";
  out << "shortest-path <from> <to> <count> <item>...\n";
  out << "collect <from> <to> <count> <type>\n";
  out << "save <file>\n";
  out << "load <file>\n";
}

void printLocations(std::ostream& out, const matveev::World& world)
{
  const matveev::List< std::string >& order = world.order();
  bool first = true;

  for (matveev::LCIter< std::string > it = order.begin(); it != order.end(); ++it)
  {
    if (!first)
    {
      out << ", ";
    }

    out << *it;
    first = false;
  }

  out << '\n';
}

void printShow(std::ostream& out, const matveev::World& world, const std::string& name)
{
  const matveev::Location& loc = world.location(name);

  std::size_t item_count = 0;
  for (matveev::LCIter< matveev::Item > it = loc.items.begin(); it != loc.items.end(); ++it)
  {
    ++item_count;
  }

  out << "Items: " << item_count << '\n';

  for (matveev::LCIter< matveev::Item > it = loc.items.begin(); it != loc.items.end(); ++it)
  {
    out << "  " << it->name << " (" << it->type << ")\n";
  }

  out << "Connections:";

  bool any = false;
  for (matveev::LCIter< matveev::Connection > it = loc.connections.begin(); it != loc.connections.end(); ++it)
  {
    out << ' ' << it->to << " (" << it->cost << ")";
    any = true;
  }

  if (!any)
  {
    out << " none";
  }

  out << '\n';
}

void printFilter(std::ostream& out, const matveev::World& world, const std::string& location, const std::string& type)
{
  const matveev::Location& loc = world.location(location);

  out << "Type " << type << ":";

  bool any = false;
  for (matveev::LCIter< matveev::Item > it = loc.items.begin(); it != loc.items.end(); ++it)
  {
    if (it->type == type)
    {
      out << (any ? ", " : " ") << it->name;
      any = true;
    }
  }

  if (!any)
  {
    out << " none";
  }

  out << '\n';
}

void printPath(std::ostream& out, const matveev::List< std::string >& path, unsigned long long cost)
{
  bool first = true;
  for (matveev::LCIter< std::string > it = path.begin(); it != path.end(); ++it)
  {
    if (!first)
    {
      out << " => ";
    }

    out << *it;
    first = false;
  }

  out << " (cost " << cost << ")\n";
}

void writeWorld(std::ostream& out, const matveev::World& world)
{
  for (matveev::LCIter< std::string > name = world.order().begin(); name != world.order().end(); ++name)
  {
    out << "make " << *name << '\n';
  }

  for (matveev::LCIter< std::string > name = world.order().begin(); name != world.order().end(); ++name)
  {
    const matveev::Location& loc = world.location(*name);
    for (matveev::LCIter< matveev::Item > item = loc.items.begin(); item != loc.items.end(); ++item)
    {
      out << "add-item " << *name << ' ' << item->name << ' ' << item->type << '\n';
    }
  }

  matveev::HashTable< std::string, char, matveev::StringHash, matveev::StringEqual > done(16);
  for (matveev::LCIter< std::string > name = world.order().begin(); name != world.order().end(); ++name)
  {
    const matveev::Location& loc = world.location(*name);
    for (matveev::LCIter< matveev::Connection > edge = loc.connections.begin(); edge != loc.connections.end(); ++edge)
    {
      if (!done.has(edge->to))
      {
        out << "connect " << *name << ' ' << edge->to << ' ' << edge->cost << '\n';
      }
    }

    done.add(*name, 0);
  }
}
}

bool matveev::executeCommand(std::ostream& out, World& world, const List< std::string >& tokens)
{
  if (tokens.begin() == tokens.end())
  {
    return true;
  }

  LCIter< std::string > it = tokens.begin();
  const std::string& command = *it;

  if (command == HELP_COMMAND)
  {
    if (!hasArgCount(tokens, 1))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    printHelp(out);
    return true;
  }

  if (command == MAKE_COMMAND)
  {
    if (!hasArgCount(tokens, 2))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& name = *it;

    try
    {
      world.createLocation(name);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == LOCATIONS_COMMAND)
  {
    if (!hasArgCount(tokens, 1))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    printLocations(out, world);
    return true;
  }

  if (command == SHOW_COMMAND)
  {
    if (!hasArgCount(tokens, 2))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& name = *it;

    try
    {
      printShow(out, world, name);
    }
    catch (const std::exception&)
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    return true;
  }

  if (command == DELETE_COMMAND)
  {
    if (!hasArgCount(tokens, 2))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& name = *it;

    try
    {
      world.removeLocation(name);
    }
    catch (const std::exception&)
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    return true;
  }

  if (command == RENAME_COMMAND)
  {
    if (!hasArgCount(tokens, 3))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& name = *it;
    ++it;
    const std::string& new_name = *it;

    try
    {
      world.renameLocation(name, new_name);
    }
    catch (const std::out_of_range&)
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }
    catch (const std::logic_error&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == CLEAR_COMMAND)
  {
    if (!hasArgCount(tokens, 2))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& name = *it;

    try
    {
      world.clearLocation(name);
    }
    catch (const std::exception&)
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    return true;
  }

  if (command == ADD_ITEM_COMMAND)
  {
    if (!hasArgCount(tokens, 4))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& location = *it;
    ++it;
    const std::string& item = *it;
    ++it;
    const std::string& type = *it;

    if (!world.hasLocation(location))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    try
    {
      world.addItem(location, item, type);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == DELETE_ITEM_COMMAND)
  {
    if (!hasArgCount(tokens, 3))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& location = *it;
    ++it;
    const std::string& item = *it;

    if (!world.hasLocation(location))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    try
    {
      world.removeItem(location, item);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == RENAME_ITEM_COMMAND)
  {
    if (!hasArgCount(tokens, 4))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& location = *it;
    ++it;
    const std::string& item = *it;
    ++it;
    const std::string& new_name = *it;

    if (!world.hasLocation(location))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    try
    {
      world.renameItem(location, item, new_name);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == MOVE_ITEM_COMMAND)
  {
    if (!hasArgCount(tokens, 4))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& item = *it;
    ++it;
    const std::string& from = *it;
    ++it;
    const std::string& to = *it;

    if (!world.hasLocation(from) || !world.hasLocation(to))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    try
    {
      world.moveItem(item, from, to);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == FIND_COMMAND)
  {
    if (countArgs(tokens) < 3)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& item = *it;
    ++it;

    for (LCIter< std::string > loc = it; loc != tokens.end(); ++loc)
    {
      if (!world.hasLocation(*loc))
      {
        out << LOCATION_NOT_FOUND << '\n';
        return false;
      }
    }

    std::string item_location;
    bool found = false;

    if (world.findItem(item, item_location))
    {
      for (LCIter< std::string > loc = it; loc != tokens.end(); ++loc)
      {
        if (*loc == item_location)
        {
          found = true;
          break;
        }
      }
    }

    if (found)
    {
      out << item_location << '\n';
    }
    else
    {
      out << "not found" << '\n';
    }

    return true;
  }

  if (command == FILTER_COMMAND)
  {
    if (!hasArgCount(tokens, 3))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& location = *it;
    ++it;
    const std::string& type = *it;

    if (!world.hasLocation(location))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    printFilter(out, world, location, type);
    return true;
  }

  if (command == CONNECT_COMMAND)
  {
    if (!hasArgCount(tokens, 4))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& first = *it;
    ++it;
    const std::string& second = *it;
    ++it;
    const std::string& cost_text = *it;

    unsigned long long cost = 0;
    if (!parseUnsignedLongLong(cost_text, cost))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    if (!world.hasLocation(first) || !world.hasLocation(second))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    try
    {
      world.connect(first, second, cost);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == DISCONNECT_COMMAND)
  {
    if (!hasArgCount(tokens, 3))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& first = *it;
    ++it;
    const std::string& second = *it;

    if (!world.hasLocation(first) || !world.hasLocation(second))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    try
    {
      world.disconnect(first, second);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == MERGE_COMMAND)
  {
    if (!hasArgCount(tokens, 4))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& new_name = *it;
    ++it;
    const std::string& first = *it;
    ++it;
    const std::string& second = *it;

    if (!world.hasLocation(first) || !world.hasLocation(second))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    try
    {
      world.mergeLocations(new_name, first, second);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == PATH_COMMAND)
  {
    if (!hasArgCount(tokens, 3))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& from = *it;
    ++it;
    const std::string& to = *it;

    if (!world.hasLocation(from) || !world.hasLocation(to))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    List< std::string > path;
    unsigned long long cost = 0;

    if (!findShortestPath(world, from, to, path, cost))
    {
      out << "No path" << '\n';
      return false;
    }

    printPath(out, path, cost);
    return true;
  }

  if (command == SHORTEST_PATH_COMMAND)
  {
    if (countArgs(tokens) < 4)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& from = *it;
    ++it;
    const std::string& to = *it;
    ++it;
    const std::string& count_text = *it;

    std::size_t count = 0;
    if (!parseSize(count_text, count))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    if (countArgs(tokens) != 4 + count)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    if (!world.hasLocation(from) || !world.hasLocation(to))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    List< std::string > items;
    ++it;
    for (; it != tokens.end(); ++it)
    {
      appendBack(items, *it);
    }

    List< std::string > path;
    unsigned long long cost = 0;
    if (!findItemRoute(world, from, to, items, path, cost))
    {
      out << "No path" << '\n';
      return false;
    }

    printPath(out, path, cost);
    return true;
  }

  if (command == COLLECT_COMMAND)
  {
    if (!hasArgCount(tokens, 5))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    const std::string& from = *it;
    ++it;
    const std::string& to = *it;
    ++it;
    const std::string& count_text = *it;
    ++it;
    const std::string& type = *it;

    std::size_t count = 0;
    if (!parseSize(count_text, count))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    if (!world.hasLocation(from) || !world.hasLocation(to))
    {
      out << LOCATION_NOT_FOUND << '\n';
      return false;
    }

    List< std::string > path;
    unsigned long long cost = 0;
    if (!findTypeRoute(world, from, to, count, type, path, cost))
    {
      out << "No path" << '\n';
      return false;
    }

    printPath(out, path, cost);
    return true;
  }

  if (command == SAVE_COMMAND)
  {
    if (!hasArgCount(tokens, 2))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    std::ofstream file(*it);
    if (!file)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    writeWorld(file, world);
    return true;
  }

  if (command == LOAD_COMMAND)
  {
    if (!hasArgCount(tokens, 2))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    ++it;
    std::ifstream file(*it);
    if (!file)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
      List< std::string > line_tokens = splitLine(line);
      executeCommand(out, world, line_tokens);
    }

    return true;
  }

  out << INVALID_COMMAND << '\n';
  return false;
}
