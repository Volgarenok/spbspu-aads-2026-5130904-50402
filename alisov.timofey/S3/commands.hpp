#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "HashTable.hpp"
#include "graph.hpp"
#include "sort.hpp"

namespace alisov
{
  using GraphTable = HashTable< std::string, Graph, SHA1< std::string >, std::equal_to< std::string > >;

  inline void handleGraphs(const GraphTable &graphs)
  {
    Vector< std::string > keys = graphs.getAllKeys();
    sortVector(keys);
    for (size_t i = 0; i < keys.size(); ++i) {
      std::cout << keys[i] << "\n";
    }
  }

  inline void handleVertexes(const GraphTable &graphs, const std::string &gname)
  {
    if (!graphs.contains(gname)) {
      std::cout << "Invalid command\n";
      return;
    }
    Vector< std::string > v = graphs.at(gname).vertexes.getAllKeys();
    sortVector(v);
    for (size_t i = 0; i < v.size(); ++i) {
      std::cout << v[i] << "\n";
    }
  }

  inline void handleOutbound(const GraphTable &graphs, const std::string &gname, const std::string &vname)
  {
    if (!graphs.contains(gname) || !graphs.at(gname).vertexes.contains(vname)) {
      std::cout << "Invalid command\n";
      return;
    }
    const Graph &g = graphs.at(gname);
    Vector< std::pair< std::string, std::string > > edges_keys = g.edges.getAllKeys();
    Vector< std::string > targets;
    for (size_t i = 0; i < edges_keys.size(); ++i) {
      if (edges_keys[i].first == vname) {
        bool duplicate = false;
        for (size_t j = 0; j < targets.size(); ++j) {
          if (targets[j] == edges_keys[i].second) {
            duplicate = true;
            break;
          }
        }
        if (!duplicate) {
          targets.push_back(edges_keys[i].second);
        }
      }
    }
    sortVector(targets);
    for (size_t i = 0; i < targets.size(); ++i) {
      Vector< unsigned int > weights = g.edges.at(std::make_pair(vname, targets[i]));
      sortVector(weights);
      for (size_t j = 0; j < weights.size(); ++j) {
        std::cout << targets[i] << " " << weights[j] << "\n";
      }
    }
  }

  inline void handleInbound(const GraphTable &graphs, const std::string &gname, const std::string &vname)
  {
    if (!graphs.contains(gname) || !graphs.at(gname).vertexes.contains(vname)) {
      std::cout << "Invalid command\n";
      return;
    }
    const Graph &g = graphs.at(gname);
    Vector< std::pair< std::string, std::string > > edges_keys = g.edges.getAllKeys();
    Vector< std::string > sources;
    for (size_t i = 0; i < edges_keys.size(); ++i) {
      if (edges_keys[i].second == vname) {
        bool duplicate = false;
        for (size_t j = 0; j < sources.size(); ++j) {
          if (sources[j] == edges_keys[i].first) {
            duplicate = true;
            break;
          }
        }
        if (!duplicate) {
          sources.push_back(edges_keys[i].first);
        }
      }
    }
    sortVector(sources);
    for (size_t i = 0; i < sources.size(); ++i) {
      Vector< unsigned int > weights = g.edges.at(std::make_pair(sources[i], vname));
      sortVector(weights);
      for (size_t j = 0; j < weights.size(); ++j) {
        std::cout << sources[i] << " " << weights[j] << "\n";
      }
    }
  }

  inline void handleBind(GraphTable &graphs, const std::string &gname, const std::string &from, const std::string &to,
                         unsigned int w)
  {
    if (!graphs.contains(gname)) {
      std::cout << "Invalid command\n";
      return;
    }
    graphs[gname].addEdge(from, to, w);
  }

  inline void handleCut(GraphTable &graphs, const std::string &gname, const std::string &from, const std::string &to,
                        unsigned int w)
  {
    if (!graphs.contains(gname)) {
      std::cout << "Invalid command\n";
      return;
    }
    Graph &g = graphs[gname];
    std::pair< std::string, std::string > p(from, to);
    if (!g.edges.contains(p)) {
      std::cout << "Invalid command\n";
      return;
    }
    Vector< unsigned int > &weights = g.edges[p];
    bool found = false;
    for (size_t i = 0; i < weights.size(); ++i) {
      if (weights[i] == w) {
        weights.erase(i);
        found = true;
        break;
      }
    }
    if (!found) {
      std::cout << "Invalid command\n";
    }
  }

  inline void handleCreate(GraphTable &graphs, const std::string &gname)
  {
    if (graphs.contains(gname)) {
      std::cout << "Invalid command\n";
      return;
    }
    graphs.insert(gname, Graph());
  }

  inline void handleMerge(GraphTable &graphs, const std::string &res, const std::string &g1, const std::string &g2)
  {
    if (!graphs.contains(g1) || !graphs.contains(g2) || graphs.contains(res)) {
      std::cout << "Invalid command\n";
      return;
    }
    Graph ng = graphs[g1];
    Graph &graph2 = graphs[g2];
    Vector< std::string > v2 = graph2.vertexes.getAllKeys();
    for (size_t i = 0; i < v2.size(); ++i) {
      ng.addVertex(v2[i]);
    }
    Vector< std::pair< std::string, std::string > > e2 = graph2.edges.getAllKeys();
    for (size_t i = 0; i < e2.size(); ++i) {
      Vector< unsigned int > w = graph2.edges.at(e2[i]);
      for (size_t j = 0; j < w.size(); ++j) {
        ng.addEdge(e2[i].first, e2[i].second, w[j]);
      }
    }
    graphs.insert(res, ng);
  }

  inline void handleExtract(GraphTable &graphs, const std::string &res, const std::string &src,
                            const Vector< std::string > &vert)
  {
    if (!graphs.contains(src) || graphs.contains(res)) {
      std::cout << "Invalid command\n";
      return;
    }
    Graph &source = graphs[src];
    for (size_t i = 0; i < vert.size(); ++i) {
      if (!source.vertexes.contains(vert[i])) {
        std::cout << "Invalid command\n";
        return;
      }
    }
    Graph ng;
    for (size_t i = 0; i < vert.size(); ++i) {
      ng.addVertex(vert[i]);
    }
    Vector< std::pair< std::string, std::string > > e = source.edges.getAllKeys();
    for (size_t i = 0; i < e.size(); ++i) {
      bool f_ok = false, t_ok = false;
      for (size_t k = 0; k < vert.size(); ++k) {
        if (vert[k] == e[i].first)
          f_ok = true;
        if (vert[k] == e[i].second)
          t_ok = true;
      }
      if (f_ok && t_ok) {
        Vector< unsigned int > w = source.edges.at(e[i]);
        for (size_t j = 0; j < w.size(); ++j) {
          ng.addEdge(e[i].first, e[i].second, w[j]);
        }
      }
    }
    graphs.insert(res, ng);
  }
}
#endif
