#ifndef GRAPHSTORAGE_HPP
#define GRAPHSTORAGE_HPP
#include "Graph.hpp"
#include "HashTable.hpp"
#include <string>
namespace lachugin {
  class GraphStorage
  {
  public:
    GraphStorage();

    bool hasGraph(const std::string& name) const;
    Graph& getGraph(const std::string& name);
    const Graph& getGraph(const std::string& name) const;
    void addGraph(const std::string& name, const Graph& graph);
    void removeGraph(const std::string& name);
    size_t size() const noexcept;
    const HashTable<
      std::string,
      Graph,
      std::hash< std::string >,
      std::equal_to< std::string >
    >& getGraphs() const;

  private:
    HashTable< std::string, Graph, std::hash< std::string >, std::equal_to< std::string > > graphs_;
  };

  void loadGraphs(std::istream& in, GraphStorage& storage);
}

#endif
