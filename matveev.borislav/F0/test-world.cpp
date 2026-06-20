#include "world.hpp"

#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
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

  std::vector< std::string > orderVec(const matveev::World& world)
  {
    std::vector< std::string > result;
    const matveev::List< std::string >& order = world.order();
    for (matveev::LCIter< std::string > it = order.begin(); it != order.end(); ++it)
    {
      result.push_back(*it);
    }
    return result;
  }

  std::vector< std::string > itemNames(const matveev::World& world, const std::string& location)
  {
    std::vector< std::string > result;
    const matveev::Location& loc = world.location(location);
    for (matveev::LCIter< matveev::Item > it = loc.items.begin(); it != loc.items.end(); ++it)
    {
      result.push_back(it->name);
    }
    return result;
  }

  std::map< std::string, unsigned long long > connections(const matveev::World& world, const std::string& location)
  {
    std::map< std::string, unsigned long long > result;
    const matveev::Location& loc = world.location(location);
    for (matveev::LCIter< matveev::Connection > it = loc.connections.begin(); it != loc.connections.end(); ++it)
    {
      result[it->to] = it->cost;
    }
    return result;
  }

  template< class F >
  bool throws(F f)
  {
    try
    {
      f();
    }
    catch (const std::exception&)
    {
      return true;
    }
    return false;
  }
}

int main()
{
  {
    matveev::World w;
    w.createLocation("forest");
    w.createLocation("dungeon");
    check(w.hasLocation("forest") && w.hasLocation("dungeon"), "create: both exist");
    check(!w.hasLocation("cave"), "create: absent reported absent");
    check(orderVec(w) == std::vector< std::string >({ "forest", "dungeon" }), "create: creation order preserved");
    check(throws([&]{ w.createLocation("forest"); }), "create: duplicate throws");
  }

  {
    matveev::World w;
    w.createLocation("a");
    w.createLocation("b");
    w.createLocation("c");
    w.removeLocation("b");
    check(!w.hasLocation("b") && w.hasLocation("a") && w.hasLocation("c"), "remove: target gone, others kept");
    check(orderVec(w) == std::vector< std::string >({ "a", "c" }), "remove: order updated");
    check(throws([&]{ w.removeLocation("b"); }), "remove: missing throws");
  }

  {
    matveev::World w;
    w.createLocation("a");
    w.createLocation("b");
    w.renameLocation("a", "x");
    check(w.hasLocation("x") && !w.hasLocation("a"), "rename: renamed");
    check(orderVec(w) == std::vector< std::string >({ "x", "b" }), "rename: order updated in place");
    check(throws([&]{ w.renameLocation("missing", "y"); }), "rename: missing source throws");
    check(throws([&]{ w.renameLocation("b", "x"); }), "rename: existing target throws");
  }

  {
    matveev::World w;
    w.createLocation("solo");
    check(w.location("solo").name == "solo", "location: accessor returns the location");
    check(throws([&]{ w.location("ghost"); }), "location: missing throws");
  }

  {
    matveev::World w;
    w.createLocation("forest");
    w.createLocation("cave");
    w.addItem("forest", "sword", "weapon");
    w.addItem("forest", "axe", "weapon");
    check(itemNames(w, "forest") == std::vector< std::string >({ "sword", "axe" }), "add-item: order preserved");
    check(throws([&]{ w.addItem("cave", "sword", "tool"); }), "add-item: globally duplicate name throws");
    check(throws([&]{ w.addItem("nowhere", "bow", "weapon"); }), "add-item: missing location throws");

    std::string where;
    check(w.findItem("sword", where) && where == "forest", "find: locates item globally");
    check(!w.findItem("ghost", where), "find: missing item returns false");
  }

  {
    matveev::World w;
    w.createLocation("a");
    w.createLocation("b");
    w.addItem("a", "sword", "weapon");

    w.renameItem("a", "sword", "blade");
    std::string where;
    check(w.findItem("blade", where) && where == "a" && !w.findItem("sword", where), "rename-item: index updated");
    check(throws([&]{ w.renameItem("a", "ghost", "x"); }), "rename-item: missing item throws");

    w.moveItem("blade", "a", "b");
    check(itemNames(w, "a").empty() && itemNames(w, "b") == std::vector< std::string >({ "blade" }),
      "move-item: moved between lists");
    check(w.findItem("blade", where) && where == "b", "move-item: index points to new location");
    check(throws([&]{ w.moveItem("blade", "a", "b"); }), "move-item: missing source item throws");

    w.removeItem("b", "blade");
    check(itemNames(w, "b").empty() && !w.findItem("blade", where), "remove-item: gone from list and index");
    check(throws([&]{ w.removeItem("b", "blade"); }), "remove-item: missing throws");
  }

  {
    matveev::World w;
    w.createLocation("a");
    w.addItem("a", "sword", "weapon");
    w.addItem("a", "axe", "weapon");
    w.clearLocation("a");
    check(itemNames(w, "a").empty(), "clear: items removed");
    std::string where;
    check(!w.findItem("sword", where) && !w.findItem("axe", where), "clear: index updated");
    w.addItem("a", "sword", "weapon");
    check(w.findItem("sword", where), "clear: name reusable after clear");
  }

  {
    matveev::World w;
    w.createLocation("vault");
    w.addItem("vault", "relic", "treasure");
    w.removeLocation("vault");
    std::string where;
    check(!w.findItem("relic", where), "remove-location: items dropped from index");
    w.createLocation("other");
    w.addItem("other", "relic", "treasure");
    check(w.findItem("relic", where) && where == "other", "remove-location: freed item name reusable");
  }

  {
    matveev::World w;
    w.createLocation("a");
    w.addItem("a", "sword", "weapon");
    w.renameLocation("a", "x");
    std::string where;
    check(w.findItem("sword", where) && where == "x", "rename-location: item index follows the new name");
  }

  {
    matveev::World w;
    w.createLocation("x");
    w.createLocation("y");
    w.connect("x", "y", 5);
    check(connections(w, "x").at("y") == 5 && connections(w, "y").at("x") == 5, "connect: symmetric with cost");
    check(w.connected("x", "y") && w.connected("y", "x"), "connect: connected both ways");
    check(throws([&]{ w.connect("x", "x", 1); }), "connect: self-loop throws");
    check(throws([&]{ w.connect("x", "y", 9); }), "connect: duplicate edge throws");
    check(throws([&]{ w.connect("x", "ghost", 1); }), "connect: missing endpoint throws");

    w.disconnect("x", "y");
    check(!w.connected("x", "y") && connections(w, "x").empty() && connections(w, "y").empty(),
      "disconnect: symmetric removal");
    check(throws([&]{ w.disconnect("x", "y"); }), "disconnect: missing edge throws");
  }

  {
    matveev::World w;
    w.createLocation("hub");
    w.createLocation("a");
    w.createLocation("b");
    w.connect("hub", "a", 1);
    w.connect("hub", "b", 2);
    w.removeLocation("hub");
    check(connections(w, "a").empty() && connections(w, "b").empty(), "remove-location: inbound edges cleaned");
  }

  {
    matveev::World w;
    w.createLocation("a");
    w.createLocation("b");
    w.connect("a", "b", 5);
    w.renameLocation("a", "x");
    check(connections(w, "b").count("x") == 1 && connections(w, "b").count("a") == 0,
      "rename-location: neighbour edge relabelled");
    check(connections(w, "x").at("b") == 5, "rename-location: own edge kept");
  }

  {
    matveev::World w;
    w.createLocation("A");
    w.createLocation("B");
    w.createLocation("C");
    w.createLocation("D");
    w.addItem("A", "sword", "weapon");
    w.addItem("B", "axe", "weapon");
    w.connect("A", "C", 5);
    w.connect("B", "C", 3);
    w.connect("A", "D", 7);
    w.connect("A", "B", 10);

    w.mergeLocations("M", "A", "B");

    check(w.hasLocation("M") && !w.hasLocation("A") && !w.hasLocation("B"), "merge: a and b consumed, new exists");
    check(itemNames(w, "M").size() == 2, "merge: items united");
    std::string where;
    check(w.findItem("sword", where) && where == "M" && w.findItem("axe", where) && where == "M",
      "merge: item index points to new");
    std::map< std::string, unsigned long long > m_conn = connections(w, "M");
    check(m_conn.size() == 2 && m_conn.at("C") == 3 && m_conn.at("D") == 7,
      "merge: union of edges, min cost to shared neighbour");
    check(connections(w, "C").size() == 1 && connections(w, "C").at("M") == 3,
      "merge: shared neighbour C redirected to new");
    check(connections(w, "D").size() == 1 && connections(w, "D").at("M") == 7, "merge: neighbour D redirected to new");
    check(orderVec(w) == std::vector< std::string >({ "C", "D", "M" }), "merge: order updated, new appended");
    check(throws([&]{ w.mergeLocations("M", "C", "D"); }), "merge: existing target throws");
    check(throws([&]{ w.mergeLocations("Z", "C", "C"); }), "merge: same operands throw");
  }

  std::cout << "\nALL " << passed << " CHECKS PASSED\n";
  return 0;
}
