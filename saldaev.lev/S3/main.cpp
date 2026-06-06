#include <fstream>
#include <iostream>
#include <limits>
#include "Graph.hpp"

using GraphHash = std::hash< std::string >;
using GraphEqual = std::equal_to< std::string >;
using GraphStorage = saldaev::HashTable< std::string, saldaev::Graph *, GraphHash, GraphEqual >;

namespace saldaev
{
  template< class T >
  void sort(saldaev::Vector< T > &v)
  {
    if (v.getSize() < 2) {
      return;
    }

    bool swapped = true;
    while (swapped) {
      swapped = false;
      auto curr = v.begin();
      auto next = curr;
      ++next;
      while (next != v.end()) {
        if (*next < *curr) {
          std::swap(*curr, *next);
          swapped = true;
        }
        ++curr;
        ++next;
      }
    }
  }

  void handleGraphs(std::istream &, std::ostream &out, GraphStorage &graphs)
  {
    saldaev::Vector< std::string > g(0);
    auto it = graphs.begin();
    while (it != graphs.end()) {
      g.pushBack(it->first);
      ++it;
    }
    sort(g);
    for (size_t i = 0; i < g.getSize(); ++i) {
      out << g[i] << '\n';
    }

    if (g.getSize() == 0) {
      out << '\n';
    }
  }

  void handleVertexes(std::istream &in, std::ostream &out, GraphStorage &graphs)
  {
    std::string name = "";
    in >> name;
    if (!(graphs.has(name))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    saldaev::Vector< std::string > v = (graphs.get(name))->vertices();
    sort(v);
    for (size_t i = 0; i < v.getSize(); ++i) {
      out << v[i] << '\n';
    }

    if (v.getSize() == 0) {
      out << '\n';
    }
  }

  void handleOutbound(std::istream &in, std::ostream &out, GraphStorage &graphs)
  {
    std::string name = "";
    in >> name;
    if (!(graphs.has(name))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    std::string vertex = "";
    in >> vertex;
    if (!(graphs.get(name)->hasVertex(vertex))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    saldaev::Vector< std::pair< std::string, saldaev::Vector< size_t > > > v =
        (graphs.get(name))->outgoingEdges(vertex);
    for (size_t i = 0; i < v.getSize(); ++i) {
      out << v[i].first;
      saldaev::Vector< size_t > w = v[i].second;
      for (size_t j = 0; j < w.getSize(); ++j) {
        out << ' ' << w[j];
      }
      out << '\n';
    }

    if (v.getSize() == 0) {
      out << '\n';
    }
  }

  void handleInbound(std::istream &in, std::ostream &out, GraphStorage &graphs)
  {
    std::string name = "";
    in >> name;
    if (!(graphs.has(name))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    std::string vertex = "";
    in >> vertex;
    if (!(graphs.get(name)->hasVertex(vertex))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    saldaev::Vector< std::pair< std::string, saldaev::Vector< size_t > > > v =
        (graphs.get(name))->incomingEdges(vertex);
    for (size_t i = 0; i < v.getSize(); ++i) {
      out << v[i].first;
      saldaev::Vector< size_t > w = v[i].second;
      for (size_t j = 0; j < w.getSize(); ++j) {
        out << ' ' << w[j];
      }
      out << '\n';
    }

    if (v.getSize() == 0) {
      out << '\n';
    }
  }

  void handleBind(std::istream &in, std::ostream &out, GraphStorage &graphs)
  {
    std::string gName;
    in >> gName;
    if (!(graphs.has(gName))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    saldaev::Graph *graph = graphs.get(gName);

    std::string vName1, vName2;
    size_t w = 0;
    if (!(in >> vName1 >> vName2 >> w)) {
      out << "<INVALID COMMAND>\n";
      in.clear();
      return;
    }
    if (!(graph->hasVertex(vName1))) {
      graph->addVertex(vName1);
    }
    if (!(graph->hasVertex(vName2))) {
      graph->addVertex(vName2);
    }

    graph->addEdge(vName1, vName2, w);
  }

  void handleCut(std::istream &in, std::ostream &out, GraphStorage &graphs)
  {
    std::string gName;
    in >> gName;
    if (!(graphs.has(gName))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    saldaev::Graph *graph = graphs.get(gName);

    std::string vName1, vName2;
    size_t w = 0;
    if (!(in >> vName1 >> vName2 >> w)) {
      out << "<INVALID COMMAND>\n";
      in.clear();
      return;
    }
    if (!(graph->hasVertex(vName1)) || !(graph->hasVertex(vName2))) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!(graph->hasEdge(vName1, vName2))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try {
      graph->removeEdge(vName1, vName2, w);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>\n";
    }
  }

  void handleCreate(std::istream &in, std::ostream &out, GraphStorage &graphs)
  {
    std::string gName;
    in >> gName;
    if (graphs.has(gName)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    size_t n = 0;
    if (!(in >> n)) {
      out << "<INVALID COMMAND>\n";
      in.clear();
      return;
    }

    saldaev::Graph *graph = new saldaev::Graph;
    for (; n > 0; --n) {
      std::string vName;
      in >> vName;
      if (graph->hasVertex(vName)) {
        out << "<INVALID COMMAND>\n";
        delete graph;
        return;
      }
      graph->addVertex(vName);
    }

    graphs.add(gName, graph);
  }

  void handleMerge(std::istream &in, std::ostream &out, GraphStorage &graphs)
  {
    std::string gName;
    in >> gName;
    if (graphs.has(gName)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    std::string gName1, gName2;
    in >> gName1 >> gName2;
    if (!(graphs.has(gName1)) || !(graphs.has(gName2))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    saldaev::Graph *graph = new saldaev::Graph;
    saldaev::Graph *g1 = graphs.get(gName1);
    saldaev::Graph *g2 = graphs.get(gName2);
    const saldaev::Vector< std::string > &v1 = g1->vertices();
    for (size_t i = 0; i < v1.getSize(); ++i) {
      graph->addVertex(v1[i]);
    }
    const saldaev::Vector< std::string > &v2 = g2->vertices();
    for (size_t i = 0; i < v2.getSize(); ++i) {
      if (!(graph->hasVertex(v2[i]))) {
        graph->addVertex(v2[i]);
      }
    }

    auto edg = g1->getEdges();
    auto it = edg.begin();
    while (it != edg.end()) {
      saldaev::Vector< size_t > w = it->second;
      for (size_t i = 0; i < w.getSize(); ++i) {
        graph->addEdge(it->first.first, it->first.second, w[i]);
      }
      ++it;
    }
    edg = g2->getEdges();
    it = edg.begin();
    while (it != edg.end()) {
      saldaev::Vector< size_t > w = it->second;
      for (size_t i = 0; i < w.getSize(); ++i) {
        graph->addEdge(it->first.first, it->first.second, w[i]);
      }
      ++it;
    }

    graphs.add(gName, graph);
  }

  void handleExtract(std::istream &in, std::ostream &out, GraphStorage &graphs)
  {
    std::string gNameN;
    in >> gNameN;
    if (graphs.has(gNameN)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    std::string gName;
    in >> gName;
    if (!(graphs.has(gName))) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    size_t n = 0;
    if (!(in >> n)) {
      out << "<INVALID COMMAND>\n";
      in.clear();
      return;
    }

    saldaev::Graph *g = graphs.get(gName);
    saldaev::Graph *graph = new saldaev::Graph;
    saldaev::Vector< std::string > v(n);
    for (size_t i = 0; i < n; ++i) {
      std::string vName;
      in >> vName;
      if (!(g->hasVertex(vName))) {
        out << "<INVALID COMMAND>\n";
        delete graph;
        return;
      }
      graph->addVertex(vName);
      v.pushBack(vName);
    }

    auto edg = g->getEdges();
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        if (g->hasEdge(v[i], v[j])) {
          saldaev::Vector< size_t > w = edg.get({v[i], v[j]});
          for (size_t k = 0; k < w.getSize(); ++k) {
            graph->addEdge(v[i], v[j], w[k]);
          }
        }
      }
    }

    graphs.add(gNameN, graph);
  }
}

int main(int argc, char *argv[])
{
  using Command = void (*)(std::istream &in, std::ostream &out, GraphStorage &graphs);
  using Commands = saldaev::HashTable< std::string, Command, std::hash< std::string >, std::equal_to< std::string > >;

  Commands commands(9, std::hash< std::string >(), std::equal_to< std::string >());
  commands.add("graphs", saldaev::handleGraphs);
  commands.add("vertexes", saldaev::handleVertexes);
  commands.add("outbound", saldaev::handleOutbound);
  commands.add("inbound", saldaev::handleInbound);
  commands.add("bind", saldaev::handleBind);
  commands.add("cut", saldaev::handleCut);
  commands.add("create", saldaev::handleCreate);
  commands.add("merge", saldaev::handleMerge);
  commands.add("extract", saldaev::handleExtract);

  if (argc != 2) {
    std::cerr << "wrong amoung of arguments";
    return 1;
  }

  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open()) {
    std::cerr << "failed to open file";
    return 1;
  }

  std::istream &input = file;

  GraphStorage graphs(10, std::hash< std::string >(), std::equal_to< std::string >());

  std::string name = "";
  size_t n = 0;
  while (input >> name && input >> n) {
    if (graphs.has(name)) {
      std::cerr << "problems with the file content";
      return 2;
    }
    saldaev::Graph *g = new saldaev::Graph();

    for (size_t i = 0; i < n; ++i) {
      std::string v1 = "";
      std::string v2 = "";
      size_t w = 0;
      if (!(input >> v1 && input >> v2 && input >> w)) {
        std::cerr << "problems with the file content";
        return 2;
      }

      if (!(g->hasVertex(v1))) {
        g->addVertex(v1);
      }
      if (!(g->hasVertex(v2))) {
        g->addVertex(v2);
      }
      g->addEdge(v1, v2, w);
    }

    graphs.add(name, g);
  }

  file.close();

  std::string cmd = "";
  while (std::cin >> cmd) {
    if (commands.has(cmd)) {
      commands.get(cmd)(std::cin, std::cout, graphs);
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  auto it = graphs.begin();
  while (it != graphs.end()) {
    delete it->second;
    ++it;
  }
}
