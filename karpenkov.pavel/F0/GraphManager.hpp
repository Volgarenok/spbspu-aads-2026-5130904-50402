#ifndef GRAPHMANAGER_HPP
#define GRAPHMANAGER_HPP
#include "cuckooHashTable.hpp"
#include "graph.hpp"

namespace karpenkov {
class GraphManager {
public:
  void createOrder(const std::string &name);
  void deleteOrder(const std::string &name);

  Graph &getOrder(const std::string &name);
  void listOrders(std::ostream &out);
  bool hasOrder(const std::string &name);

private:
  CuckooHashTable<std::string, Graph, StringHash1, StringHash2, StringEqual>
      orders_{StringHash1{}, StringHash2{}, StringEqual{}};
};
} // namespace karpenkov

#endif
