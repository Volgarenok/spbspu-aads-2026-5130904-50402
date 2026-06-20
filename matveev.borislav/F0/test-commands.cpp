#include "commandHandler.hpp"
#include "parserUtils.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
  int passed = 0;

  void check(bool condition, const char* name)
  {
    if (!condition)
    {
      std::cout << "FAIL: " << name << '\n';
      std::exit(1);
    }

    ++passed;
    std::cout << "ok: " << name << '\n';
  }

  std::string run(const std::vector< std::string >& lines)
  {
    matveev::World world;
    std::ostringstream out;

    for (std::size_t i = 0; i < lines.size(); ++i)
    {
      matveev::List< std::string > tokens = matveev::splitLine(lines[i]);
      matveev::executeCommand(out, world, tokens);
    }

    return out.str();
  }

  bool contains(const std::string& haystack, const std::string& needle)
  {
    return haystack.find(needle) != std::string::npos;
  }

  std::string exec(matveev::World& world, const std::string& line)
  {
    std::ostringstream out;
    matveev::List< std::string > tokens = matveev::splitLine(line);
    matveev::executeCommand(out, world, tokens);
    return out.str();
  }
}

int main()
{
  check(run({ "make forest", "make dungeon", "locations" }) == "forest, dungeon\n", "locations: creation order");
  check(run({ "locations" }) == "\n", "locations: empty world prints blank line");
  check(run({ "make forest", "make forest" }) == "<INVALID COMMAND>\n", "make: duplicate is invalid");
  check(run({ "make forest dungeon" }) == "<INVALID COMMAND>\n", "make: wrong argument count is invalid");
  check(run({ "bogus" }) == "<INVALID COMMAND>\n", "unknown command is invalid");
  check(run({ "" }) == "", "empty line produces no output");

  std::string help = run({ "help" });
  check(contains(help, "make <location>") && contains(help, "shortest-path"), "help: lists commands");

  check(run({ "make a", "make b", "make c" }) == "", "make: success is silent");

  {
    matveev::World w;
    w.createLocation("forest");
    w.createLocation("dungeon");
    w.addItem("forest", "sword", "weapon");
    w.addItem("forest", "axe", "weapon");
    w.connect("forest", "dungeon", 10);
    check(exec(w, "show forest") == "Items: 2\n  sword (weapon)\n  axe (weapon)\nConnections: dungeon (10)\n",
      "show: full format");
    check(exec(w, "show dungeon") == "Items: 0\nConnections: forest (10)\n", "show: no items but a connection");
  }

  {
    matveev::World w;
    w.createLocation("empty");
    check(exec(w, "show empty") == "Items: 0\nConnections: none\n", "show: empty location");
    check(exec(w, "show ghost") == "Location not found\n", "show: missing location");
    check(exec(w, "show a b") == "<INVALID COMMAND>\n", "show: wrong arg count");
  }

  {
    matveev::World w;
    w.createLocation("doomed");
    check(exec(w, "delete doomed") == "", "delete: success is silent");
    check(exec(w, "show doomed") == "Location not found\n", "delete: location is gone");
    check(exec(w, "delete doomed") == "Location not found\n", "delete: missing location");
    check(exec(w, "delete a b") == "<INVALID COMMAND>\n", "delete: wrong arg count");
  }

  check(run({ "make a", "rename a x", "locations" }) == "x\n", "rename: success is silent, name changes");
  check(run({ "rename ghost y" }) == "Location not found\n", "rename: missing source");
  check(run({ "make a", "make b", "rename a b" }) == "<INVALID COMMAND>\n", "rename: target name taken is invalid");
  check(run({ "rename a" }) == "<INVALID COMMAND>\n", "rename: wrong arg count");

  {
    matveev::World w;
    w.createLocation("forest");
    w.addItem("forest", "sword", "weapon");
    w.addItem("forest", "axe", "weapon");
    check(exec(w, "clear forest") == "", "clear: success is silent");
    check(exec(w, "show forest") == "Items: 0\nConnections: none\n", "clear: items removed");
    check(exec(w, "clear ghost") == "Location not found\n", "clear: missing location");
    check(exec(w, "clear a b") == "<INVALID COMMAND>\n", "clear: wrong arg count");
  }

  check(run({ "make forest", "add-item forest sword weapon", "show forest" })
    == "Items: 1\n  sword (weapon)\nConnections: none\n", "add-item: appears in show");
  check(run({ "make a", "make b", "add-item a sword weapon", "add-item b sword tool" })
    == "<INVALID COMMAND>\n", "add-item: globally duplicate name is invalid");
  check(run({ "add-item ghost sword weapon" }) == "Location not found\n", "add-item: missing location");
  check(run({ "make a", "add-item a sword" }) == "<INVALID COMMAND>\n", "add-item: wrong arg count");

  check(run({ "make a", "add-item a sword weapon", "delete-item a sword", "show a" })
    == "Items: 0\nConnections: none\n", "delete-item: removed from show");
  check(run({ "make a", "delete-item a ghost" }) == "<INVALID COMMAND>\n", "delete-item: missing item is invalid");
  check(run({ "delete-item ghost x" }) == "Location not found\n", "delete-item: missing location");

  check(run({ "make a", "add-item a sword weapon", "rename-item a sword blade", "show a" })
    == "Items: 1\n  blade (weapon)\nConnections: none\n", "rename-item: name changes in show");

  check(run({ "make a", "make b", "add-item a sword weapon", "move-item sword a b", "show b" })
    == "Items: 1\n  sword (weapon)\nConnections: none\n", "move-item: appears in destination");
  check(run({ "make a", "move-item sword a ghost" }) == "Location not found\n", "move-item: missing destination");
  check(run({ "make a", "add-item a x t", "add-item a y t", "move-item x a a", "show a" })
    == "Items: 2\n  x (t)\n  y (t)\nConnections: none\n", "move-item: same source and destination keeps order");
  check(run({ "make a", "move-item ghost a a" }) == "<INVALID COMMAND>\n",
    "move-item: same place but missing item is invalid");

  check(run({ "make a", "make b", "add-item a sword weapon", "find sword a b" }) == "a\n",
    "find: locates item in a listed location");
  check(run({ "make a", "make b", "add-item a sword weapon", "find sword b" }) == "not found\n",
    "find: item not in the listed locations");
  check(run({ "make a", "find ghost a" }) == "not found\n", "find: missing item");
  check(run({ "make a", "add-item a sword weapon", "find sword a ghost" }) == "Location not found\n",
    "find: a listed location is missing");
  check(run({ "find sword" }) == "<INVALID COMMAND>\n", "find: needs at least one location");

  check(run({ "make a", "add-item a sword weapon", "add-item a axe weapon", "add-item a potion tool",
    "filter a weapon" })
    == "Type weapon: sword, axe\n", "filter: lists matching items in order");
  check(run({ "make a", "add-item a sword weapon", "filter a tool" }) == "Type tool: none\n",
    "filter: no matching items");
  check(run({ "filter ghost weapon" }) == "Location not found\n", "filter: missing location");
  check(run({ "filter a" }) == "<INVALID COMMAND>\n", "filter: wrong arg count");

  check(run({ "make a", "make b", "connect a b 10", "show a" }) == "Items: 0\nConnections: b (10)\n",
    "connect: shows neighbour");
  check(run({ "make a", "make b", "connect a b 10", "show b" }) == "Items: 0\nConnections: a (10)\n",
    "connect: symmetric in show");
  check(run({ "make a", "connect a ghost 5" }) == "Location not found\n", "connect: missing endpoint");
  check(run({ "make a", "connect a ghost xyz" }) == "<INVALID COMMAND>\n",
    "connect: bad cost outranks missing location");
  check(run({ "make a", "connect a a 5" }) == "<INVALID COMMAND>\n", "connect: self-loop is invalid");
  check(run({ "make a", "make b", "connect a b 5", "connect a b 9" }) == "<INVALID COMMAND>\n",
    "connect: duplicate edge is invalid");
  check(run({ "make a", "make b", "connect a b xyz" }) == "<INVALID COMMAND>\n",
    "connect: non-numeric cost is invalid");
  check(run({ "connect a b" }) == "<INVALID COMMAND>\n", "connect: wrong arg count");

  check(run({ "make a", "make b", "connect a b 5", "disconnect a b", "show a" }) == "Items: 0\nConnections: none\n",
    "disconnect: removes neighbour");
  check(run({ "make a", "make b", "disconnect a b" }) == "<INVALID COMMAND>\n", "disconnect: not connected is invalid");
  check(run({ "make a", "disconnect a ghost" }) == "Location not found\n", "disconnect: missing endpoint");

  check(run({ "make a", "make b", "make c", "add-item a sword weapon", "add-item b axe weapon",
    "connect a c 5", "connect b c 3", "merge m a b", "show m" })
    == "Items: 2\n  sword (weapon)\n  axe (weapon)\nConnections: c (3)\n", "merge: unites items and edges");
  check(run({ "make a", "make b", "make c", "merge m a b", "locations" }) == "c, m\n",
    "merge: consumes operands, appends new");
  check(run({ "make a", "merge m a ghost" }) == "Location not found\n", "merge: missing operand");
  check(run({ "make a", "make b", "make c", "merge c a b" }) == "<INVALID COMMAND>\n",
    "merge: existing target is invalid");
  check(run({ "make a", "merge m a a" }) == "<INVALID COMMAND>\n", "merge: same operands is invalid");
  check(run({ "merge m a" }) == "<INVALID COMMAND>\n", "merge: wrong arg count");

  check(run({ "make a", "make b", "make c", "connect a b 5", "connect b c 3", "path a c" }) == "a => b => c (cost 8)\n",
    "path: follows the only route");
  check(run({ "make a", "make b", "make c", "connect a b 1", "connect b c 1", "connect a c 10",
    "path a c" }) == "a => b => c (cost 2)\n", "path: picks the cheaper route");
  check(run({ "make a", "make b", "path a b" }) == "No path\n", "path: no route");
  check(run({ "make a", "path a a" }) == "a (cost 0)\n", "path: to self is zero cost");
  check(run({ "make a", "path a ghost" }) == "Location not found\n", "path: missing endpoint");
  check(run({ "path a" }) == "<INVALID COMMAND>\n", "path: wrong arg count");

  check(run({ "make a", "make b", "make c", "make d", "add-item c key tool", "connect a b 1", "connect b c 1",
    "connect c d 1", "shortest-path a d 1 key" }) == "a => b => c => d (cost 3)\n",
    "shortest-path: collects an item on the way");
  check(run({ "make a", "make b", "make c", "make d", "add-item c gem tool", "connect a b 1", "connect b c 1",
    "connect b d 1", "shortest-path a d 1 gem" }) == "a => b => c => b => d (cost 4)\n",
    "shortest-path: takes a detour and returns");
  check(run({ "make a", "make b", "make c", "make d", "add-item b first tool", "add-item c second tool",
    "connect a b 1", "connect b c 1", "connect c d 1",
    "shortest-path a d 2 first second" }) == "a => b => c => d (cost 3)\n",
    "shortest-path: collects two items in order");
  check(run({ "make a", "make b", "make c", "add-item a key tool", "connect a b 1", "connect b c 1",
    "shortest-path a c 1 key" }) == "a => b => c (cost 2)\n", "shortest-path: item at the start");
  check(run({ "make a", "make b", "connect a b 1", "shortest-path a b 1 ghost" }) == "No path\n",
    "shortest-path: missing item has no route");
  check(run({ "make a", "make b", "make c", "add-item c key tool", "connect a b 1",
    "shortest-path a b 1 key" }) == "No path\n", "shortest-path: unreachable item has no route");
  check(run({ "make a", "make b", "connect a b 5", "shortest-path a b 0" }) == "a => b (cost 5)\n",
    "shortest-path: zero items is a plain shortest path");
  check(run({ "make a", "make b", "shortest-path a b 2 onlyone" }) == "<INVALID COMMAND>\n",
    "shortest-path: count mismatch is invalid");
  check(run({ "make a", "make b", "shortest-path a b xyz item" }) == "<INVALID COMMAND>\n",
    "shortest-path: non-numeric count is invalid");
  check(run({ "make a", "shortest-path a ghost 0" }) == "Location not found\n", "shortest-path: missing endpoint");
  check(run({ "shortest-path a b" }) == "<INVALID COMMAND>\n", "shortest-path: too few arguments");

  check(run({ "make a", "make b", "make c", "make d", "add-item c sword weapon", "connect a b 1", "connect b c 1",
    "connect c d 1", "collect a d 1 weapon" }) == "a => b => c => d (cost 3)\n",
    "collect: gathers one of a type en route");
  check(run({ "make a", "make b", "make c", "make d", "add-item b sword weapon", "add-item c axe weapon",
    "connect a b 1", "connect b c 1", "connect c d 1", "collect a d 2 weapon" }) == "a => b => c => d (cost 3)\n",
    "collect: gathers from two locations");
  check(run({ "make a", "make b", "make c", "add-item a sword weapon", "connect a b 1", "connect b c 1",
    "collect a c 1 weapon" }) == "a => b => c (cost 2)\n", "collect: start location already satisfies the count");
  check(run({ "make a", "make b", "make c", "add-item b sword weapon", "add-item b axe weapon", "connect a b 1",
    "connect b c 1", "collect a c 2 weapon" }) == "a => b => c (cost 2)\n",
    "collect: one location can supply the whole count");
  check(run({ "make a", "make b", "collect a b 1 weapon extra" }) == "<INVALID COMMAND>\n",
    "collect: extra argument is invalid");
  check(run({ "make a", "make b", "make d", "make x", "add-item b sword weapon", "add-item x relic weapon",
    "connect a b 1", "connect b d 1", "connect a x 100", "connect x d 100",
    "collect a d 1 weapon" }) == "a => b => d (cost 2)\n", "collect: picks the cheapest sufficient subset");
  check(run({ "make a", "make b", "make c", "make d", "add-item c sword weapon", "connect a b 1", "connect b c 1",
    "connect b d 1", "collect a d 1 weapon" }) == "a => b => c => b => d (cost 4)\n",
    "collect: detours for the type and returns");
  check(run({ "make a", "make b", "make c", "add-item b sword weapon", "connect a b 1", "connect b c 1",
    "collect a c 5 weapon" }) == "No path\n", "collect: not enough items in the world");
  check(run({ "make a", "make b", "connect a b 1", "collect a b 1 weapon" }) == "No path\n",
    "collect: type absent has no route");
  check(run({ "make a", "make b", "connect a b 5", "collect a b 0 weapon" }) == "a => b (cost 5)\n",
    "collect: zero count is a plain shortest path");
  check(run({ "make a", "make b", "collect a b xyz weapon" }) == "<INVALID COMMAND>\n",
    "collect: non-numeric count is invalid");
  check(run({ "make a", "collect a ghost 1 weapon" }) == "Location not found\n", "collect: missing endpoint");

  check(run({ "make a", "make b", "connect a b 5", "add-item a sword weapon", "save /tmp/gameworld_rt.txt" }) == "",
    "save: writes a reconstruction script silently");
  check(run({ "load /tmp/gameworld_rt.txt", "locations" }) == "a, b\n", "load: rebuilds locations in order");
  check(run({ "load /tmp/gameworld_rt.txt", "show a" }) == "Items: 1\n  sword (weapon)\nConnections: b (5)\n",
    "load: rebuilds items and edges");
  check(run({ "load /tmp/gameworld_rt.txt", "find sword a" }) == "a\n", "load: item index is rebuilt");
  check(run({ "load /tmp/gameworld_missing_xyz.txt" }) == "<INVALID COMMAND>\n", "load: missing file is invalid");
  check(run({ "save" }) == "<INVALID COMMAND>\n", "save: wrong arg count");
  check(run({ "load" }) == "<INVALID COMMAND>\n", "load: wrong arg count");

  std::cout << "\nALL " << passed << " CHECKS PASSED\n";
  return 0;
}
