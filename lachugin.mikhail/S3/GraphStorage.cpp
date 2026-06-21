#include "GraphStorage.hpp"
#include "Graph.hpp"
#include "HashTable.hpp"

namespace lachugin
{
  GraphStorage::GraphStorage():
    graphs_(50, 4, 20)
  {}

  bool GraphStorage::hasGraph(const std::string &name) const
  {
    return graphs_.has(name);
  }

  Graph &GraphStorage::getGraph(const std::string &name)
  {
    return graphs_.get(name);
  }

  const Graph &GraphStorage::getGraph(const std::string &name) const
  {
    return graphs_.get(name);
  }

  void GraphStorage::addGraph(const std::string &name, const Graph &graph)
  {
    graphs_.add(name, graph);
  }

  void GraphStorage::removeGraph(const std::string &name)
  {
    graphs_.drop(name);
  }

  size_t GraphStorage::size() const noexcept
  {
    return graphs_.size();
  }

  const HashTable< std::string,
  Graph, std::hash< std::string >,
  std::equal_to< std::string> > &GraphStorage::getGraphs() const
  {
    return graphs_;
  }

  void loadGraphs(std::istream &in, GraphStorage &storage)
  {
    std::string graphName;
    while (in >> graphName)
    {
      size_t edgesCount = 0;
      in >> edgesCount;
      Graph graph;
      for (size_t i = 0; i < edgesCount; ++i)
      {
        std::string from;
        std::string to;
        size_t weight = 0;
        in >> from >> to >> weight;
        graph.bind(from, to, weight);
      }
      storage.addGraph(graphName, graph);
    }
  }



}
