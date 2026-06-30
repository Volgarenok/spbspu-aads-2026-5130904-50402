#include "commands.hpp"
#include <algorithm>
#include <iostream>
#include "stringUtils.hpp"

namespace pozdnyakov
{
  struct NodeEdgesComparator
  {
    bool operator()(const NodeEdges &a, const NodeEdges &b) const
    {
      return a.vertex < b.vertex;
    }
  };

  Graph *findGraph(Vector< std::pair< std::string, Graph > > &dict, const std::string &name)
  {
    for (std::size_t i = 0; i < dict.size(); ++i) {
      if (dict[i].first == name) {
        return &dict[i].second;
      }
    }
    return nullptr;
  }

  void printInvalid()
  {
    std::cout << "<INVALID COMMAND>\n";
  }

  void cmdGraphs(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens)
  {
    if (tokens.size() != 1) {
      printInvalid();
      return;
    }
    Vector< std::string > names;
    for (std::size_t i = 0; i < dict.size(); ++i) {
      names.pushBack(dict[i].first);
    }

    if (names.empty()) {
      std::cout << "\n";
      return;
    }

    std::sort(names.begin(), names.end());
    for (std::size_t i = 0; i < names.size(); ++i) {
      std::cout << names[i] << "\n";
    }
  }

  void cmdVertexes(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens)
  {
    if (tokens.size() != 2) {
      printInvalid();
      return;
    }
    Graph *targetGraph = findGraph(dict, tokens[1]);
    if (targetGraph) {
      Vector< std::string > verts = targetGraph->getVertices();

      if (verts.empty()) {
        std::cout << "\n";
        return;
      }

      std::sort(verts.begin(), verts.end());
      for (std::size_t i = 0; i < verts.size(); ++i) {
        std::cout << verts[i] << "\n";
      }
    } else {
      printInvalid();
    }
  }

  void cmdOutbound(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens)
  {
    if (tokens.size() != 3) {
      printInvalid();
      return;
    }
    Graph *targetGraph = findGraph(dict, tokens[1]);
    if (targetGraph && targetGraph->hasVertex(tokens[2])) {
      Vector< NodeEdges > edges = targetGraph->getOutboundEdges(tokens[2]);

      if (edges.empty()) {
        std::cout << "\n";
        return;
      }

      std::sort(edges.begin(), edges.end(), NodeEdgesComparator());
      for (std::size_t i = 0; i < edges.size(); ++i) {
        std::cout << edges[i].vertex;
        Vector< unsigned int > w = edges[i].weights;
        std::sort(w.begin(), w.end());
        for (std::size_t j = 0; j < w.size(); ++j) {
          std::cout << " " << w[j];
        }
        std::cout << "\n";
      }
    } else {
      printInvalid();
    }
  }

  void cmdInbound(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens)
  {
    if (tokens.size() != 3) {
      printInvalid();
      return;
    }
    Graph *targetGraph = findGraph(dict, tokens[1]);
    if (targetGraph && targetGraph->hasVertex(tokens[2])) {
      Vector< NodeEdges > edges = targetGraph->getInboundEdges(tokens[2]);

      if (edges.empty()) {
        std::cout << "\n";
        return;
      }

      std::sort(edges.begin(), edges.end(), NodeEdgesComparator());
      for (std::size_t i = 0; i < edges.size(); ++i) {
        std::cout << edges[i].vertex;
        Vector< unsigned int > w = edges[i].weights;
        std::sort(w.begin(), w.end());
        for (std::size_t j = 0; j < w.size(); ++j) {
          std::cout << " " << w[j];
        }
        std::cout << "\n";
      }
    } else {
      printInvalid();
    }
  }

  void cmdBind(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens)
  {
    if (tokens.size() != 5 || !utils::isUnsignedNumber(tokens[4])) {
      printInvalid();
      return;
    }
    Graph *targetGraph = findGraph(dict, tokens[1]);
    if (targetGraph) {
      targetGraph->addEdge(tokens[2], tokens[3], std::stoul(tokens[4]));
    } else {
      printInvalid();
    }
  }

  void cmdCut(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens)
  {
    if (tokens.size() != 5 || !utils::isUnsignedNumber(tokens[4])) {
      printInvalid();
      return;
    }
    Graph *targetGraph = findGraph(dict, tokens[1]);
    if (targetGraph && targetGraph->hasVertex(tokens[2]) && targetGraph->hasVertex(tokens[3])) {
      if (!targetGraph->removeEdge(tokens[2], tokens[3], std::stoul(tokens[4]))) {
        printInvalid();
      }
    } else {
      printInvalid();
    }
  }

  void cmdCreate(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens)
  {
    if (tokens.size() < 2) {
      printInvalid();
      return;
    }

    std::size_t n = 0;
    if (tokens.size() >= 3) {
      if (!utils::isUnsignedNumber(tokens[2])) {
        printInvalid();
        return;
      }
      n = std::stoul(tokens[2]);
    }

    if (tokens.size() != 2 && tokens.size() != 3 + n) {
      printInvalid();
      return;
    }

    if (!findGraph(dict, tokens[1])) {
      Graph newGraph;
      for (std::size_t i = 0; i < n; ++i) {
        newGraph.addVertex(tokens[3 + i]);
      }
      dict.pushBack({tokens[1], std::move(newGraph)});
    } else {
      printInvalid();
    }
  }

  void cmdMerge(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens)
  {
    if (tokens.size() != 4) {
      printInvalid();
      return;
    }
    Graph *g1 = findGraph(dict, tokens[2]);
    Graph *g2 = findGraph(dict, tokens[3]);
    if (g1 && g2 && !findGraph(dict, tokens[1])) {
      Graph newGraph;

      const Vector< std::string > &v1 = g1->getVertices();
      for (std::size_t i = 0; i < v1.size(); ++i) {
        newGraph.addVertex(v1[i]);
      }
      const Vector< std::string > &v2 = g2->getVertices();
      for (std::size_t i = 0; i < v2.size(); ++i) {
        newGraph.addVertex(v2[i]);
      }

      Vector< EdgeInfo > e1 = g1->getAllEdges();
      for (std::size_t i = 0; i < e1.size(); ++i) {
        newGraph.addEdge(e1[i].from, e1[i].to, e1[i].weight);
      }
      Vector< EdgeInfo > e2 = g2->getAllEdges();
      for (std::size_t i = 0; i < e2.size(); ++i) {
        newGraph.addEdge(e2[i].from, e2[i].to, e2[i].weight);
      }

      dict.pushBack({tokens[1], std::move(newGraph)});
    } else {
      printInvalid();
    }
  }

  void cmdExtract(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens)
  {
    if (tokens.size() < 4 || !utils::isUnsignedNumber(tokens[3])) {
      printInvalid();
      return;
    }
    std::size_t n = std::stoul(tokens[3]);
    if (tokens.size() != 4 + n) {
      printInvalid();
      return;
    }
    Graph *sourceGraph = findGraph(dict, tokens[2]);
    if (sourceGraph && !findGraph(dict, tokens[1])) {
      bool allVerticesExist = true;
      for (std::size_t i = 0; i < n; ++i) {
        if (!sourceGraph->hasVertex(tokens[4 + i])) {
          allVerticesExist = false;
          break;
        }
      }

      if (allVerticesExist) {
        Graph extractedGraph;
        for (std::size_t i = 0; i < n; ++i) {
          extractedGraph.addVertex(tokens[4 + i]);
        }

        Vector< EdgeInfo > oldEdges = sourceGraph->getAllEdges();
        for (std::size_t i = 0; i < oldEdges.size(); ++i) {
          if (extractedGraph.hasVertex(oldEdges[i].from) && extractedGraph.hasVertex(oldEdges[i].to)) {
            extractedGraph.addEdge(oldEdges[i].from, oldEdges[i].to, oldEdges[i].weight);
          }
        }
        dict.pushBack({tokens[1], std::move(extractedGraph)});
      } else {
        printInvalid();
      }
    } else {
      printInvalid();
    }
  }

  void CommandDispatcher::add(const std::string &name, CommandFn fn)
  {
    cmds_.pushBack({name, fn});
  }

  void CommandDispatcher::execute(const std::string &name, Vector< std::pair< std::string, Graph > > &dict,
                                  const Vector< std::string > &tokens)
  {
    for (std::size_t i = 0; i < cmds_.size(); ++i) {
      if (cmds_[i].first == name) {
        cmds_[i].second(dict, tokens);
        return;
      }
    }
    printInvalid();
  }
}
