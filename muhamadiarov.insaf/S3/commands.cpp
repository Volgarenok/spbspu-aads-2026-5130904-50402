#include "commands.hpp"

namespace muhamadiarov
{
  using HTIter_t = HTIter< std::string, Graph, SipHash<std::string>, std::equal_to<std::string> >;
  using HTIter_k_t = HTIter< Graph::key_t, List< size_t >, PairHash, std::equal_to< Graph::key_t > >;

  void sortStringList(List< std::string >& list)
  {
    if (list.size() <= 1)
    {
      return;
    }

    bool swapped = false;
    do
    {
      swapped = false;
      LIter< std::string > it = list.begin();
      LIter< std::string > next = it;
      ++next;
      for (size_t i = 1; i < list.size(); ++i)
      {
        if (*next < *it)
        {
          std::swap(*it, *next);
          swapped = true;
        }
        ++it;
        ++next;
      }
    } while (swapped);
  }

  void sortWeights(List< size_t >& list)
  {
    if (list.size() <= 1)
    {
      return;
    }

    bool swapped = false;
    do
    {
      swapped = false;
      LIter< size_t > it = list.begin();
      LIter< size_t > next = it;
      ++next;
      for (size_t i = 1; i < list.size(); ++i)
      {
        if (*next < *it)
        {
          std::swap(*it, *next);
          swapped = true;
        }
        ++it;
        ++next;
      }
    } while (swapped);
  }

  void sortOutboundList(List< std::pair< std::string, List< size_t > > >& list)
  {
    if (list.size() <= 1)
    {
      return;
    }

    bool swapped = false;
    do
    {
      swapped = false;
      LIter< std::pair< std::string, List< size_t > > > it = list.begin();
      LIter< std::pair< std::string, List< size_t > > > next = it;
      ++next;
      for (size_t i = 1; i < list.size(); ++i)
      {
        if (next->first < it->first)
        {
          std::swap(*it, *next);
          swapped = true;
        }
        ++it;
        ++next;
      }
    } while (swapped);
  }

  void cmdGraphs(std::istream&, std::ostream& out, GraphTable& graphs)
  {
    List< std::string > names;
    HTIter_t it = graphs.begin();
    while (it != graphs.end())
    {
      names.pushBack(it->key_);
      ++it;
    }

    sortStringList(names);

    LCIter< std::string > printIt = names.cbegin();
    for (size_t i = 0; i < names.size(); ++i)
    {
      out << *printIt << '\n';
      ++printIt;
    }

    if (names.size() == 0)
    {
      out << '\n';
    }
  }

  void cmdVertexes(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;
    in >> graphName;
    if (!graphs.has(graphName))
    {
      throw std::runtime_error("Invalid name of graph");
    }

    const Graph& graph = graphs.cget(graphName);
    List< std::string > vertices;
    LCIter< std::string > it = graph.vertices_.cbegin();
    for (size_t i = 0; i < graph.vertices_.size(); ++i)
    {
      vertices.pushBack(*it);
      ++it;
    }

    sortStringList(vertices);

    LCIter< std::string > printIt = vertices.cbegin();
    for (size_t i = 0; i < vertices.size(); ++i)
    {
      out << *printIt << '\n';
      ++printIt;
    }

    if (vertices.size() == 0)
    {
      out << '\n';
    }
  }

  void cmdOutbound(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;
    std::string vertex;
    in >> graphName >> vertex;
    if (!graphs.has(graphName))
    {
      throw std::runtime_error("Invalid name of graph");
    }

    Graph& graph = graphs.get(graphName);

    if (!graph.findV(vertex))
    {
      throw std::runtime_error("There is not this vertex");
    }

    List< std::pair< std::string, List< size_t > > > outbound;
    HTIter_k_t it = graph.bonds_.begin();
    while (it != graph.bonds_.end())
    {
      const Graph::key_t& key = it->key_;
      if (key.first == vertex)
      {
        std::pair< std::string, List< size_t > > item;
        item.first = key.second;
        List< size_t > weights;
        LCIter<size_t> wIt = it->value_.cbegin();
        for (size_t i = 0; i < it->value_.size(); ++i)
        {
          weights.pushBack(*wIt);
          ++wIt;
        }

        sortWeights(weights);
        item.second = weights;
        outbound.pushBack(item);
      }
      ++it;
    }

    sortOutboundList(outbound);

    bool isPrint = false;
    LIter<std::pair<std::string, List<size_t> > > printIt = outbound.begin();
    for (size_t i = 0; i < outbound.size(); ++i)
    {
      out << printIt->first;
      LCIter<size_t> wIt = printIt->second.cbegin();
      for (size_t j = 0; j < printIt->second.size(); ++j)
      {
        out << " " << *wIt;
        ++wIt;
        isPrint = true;
      }
      out << '\n';
      ++printIt;
    }

    if (!isPrint)
    {
      out << '\n';
    }
  }

  void cmdInbound(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;
    std::string vertex;
    in >> graphName >> vertex;

    if (!graphs.has(graphName))
    {
      throw std::runtime_error("Invalid name of graph");
    }

    Graph& graph = graphs.get(graphName);

    if (!graph.findV(vertex))
    {
      throw std::runtime_error("There is not this vertex");
    }

    List<std::pair<std::string, List<size_t> > > inbound;
    HTIter_k_t it = graph.bonds_.begin();
    while (it != graph.bonds_.end())
    {
      const Graph::key_t& key = it->key_;
      if (key.second == vertex)
      {
        std::pair<std::string, List<size_t> > item;
        item.first = key.first;

        List<size_t> weights;
        LCIter<size_t> wIt = it->value_.cbegin();
        for (size_t i = 0; i < it->value_.size(); ++i)
        {
          weights.pushBack(*wIt);
          ++wIt;
        }

        sortWeights(weights);
        item.second = weights;
        inbound.pushBack(item);
      }
      ++it;
    }

    sortOutboundList(inbound);

    bool isPrint = false;
    LIter<std::pair<std::string, List<size_t> > > printIt = inbound.begin();
    for (size_t i = 0; i < inbound.size(); ++i)
    {
      out << printIt->first;
      LCIter<size_t> wIt = printIt->second.cbegin();
      for (size_t j = 0; j < printIt->second.size(); ++j)
      {
        out << " " << *wIt;
        ++wIt;
        isPrint = true;
      }
      out << '\n';
      ++printIt;
    }

    if (!isPrint)
    {
      out << '\n';
    }
  }

  void cmdBind(std::istream& in, std::ostream&, GraphTable& graphs)
  {
    std::string graphName;
    std::string vertexA;
    std::string vertexB;
    size_t weight;
    in >> graphName >> vertexA >> vertexB >> weight;

    if (!graphs.has(graphName))
    {
      throw std::runtime_error("Invalid name of graph");
    }

    Graph& graph = graphs.get(graphName);

    if (!graph.findV(vertexA))
    {
      graph.addVertex(vertexA);
    }
    if (!graph.findV(vertexB))
    {
      graph.addVertex(vertexB);
    }

    graph.addConnection(vertexA, vertexB, weight);
  }

  void cmdCut(std::istream& in, std::ostream&, GraphTable& graphs)
  {
    std::string graphName;
    std::string vertexA;
    std::string vertexB;
    size_t weight;
    in >> graphName >> vertexA >> vertexB >> weight;

    if (!graphs.has(graphName))
    {
      throw std::runtime_error("Invalid name of graph");
    }

    Graph& graph = graphs.get(graphName);

    if (!graph.findV(vertexA) || !graph.findV(vertexB))
    {
      throw std::runtime_error("There are not these vertexes");
    }
    graph.removeConnection(vertexA, vertexB, weight);
  }

  void cmdCreate(std::istream& in, std::ostream&, GraphTable& graphs)
  {
    std::string graphName;
    in >> graphName;
    if (!in)
    {
      throw std::runtime_error("Invalid input");
    }
    if (graphs.has(graphName))
    {
      throw std::runtime_error("Graph already exists");
    }
    size_t vertexCount = 0;
    in >> vertexCount;
    if (!in)
    {
      throw std::runtime_error("Invalid vertex count");
    }
    Graph newGraph(5, 5);
    for (size_t i = 0; i < vertexCount; ++i)
    {
      std::string vertex;
      in >> vertex;
      if (!in)
      {
        throw std::runtime_error("Invalid vertex name");
      }
      newGraph.addVertex(vertex);
    }

    graphs.add(graphName, newGraph);
  }

  void cmdMerge(std::istream& in, std::ostream&, GraphTable& graphs)
  {
    std::string newName;
    std::string oldName1;
    std::string oldName2;
    in >> newName >> oldName1 >> oldName2;

    if (graphs.has(newName) || !graphs.has(oldName1) || !graphs.has(oldName2))
    {
      throw std::runtime_error("Invalid");
    }

    Graph& g1 = graphs.get(oldName1);
    Graph& g2 = graphs.get(oldName2);
    Graph merged(5, 5);

    LCIter<std::string> vIt = g1.vertices_.cbegin();
    for (size_t i = 0; i < g1.vertices_.size(); ++i)
    {
      merged.addVertex(*vIt);
      ++vIt;
    }

    vIt = g2.vertices_.cbegin();
    for (size_t i = 0; i < g2.vertices_.size(); ++i)
    {
      merged.addVertex(*vIt);
      ++vIt;
    }

    HTIter_k_t eIt= g1.bonds_.begin();
    while (eIt != g1.bonds_.end())
    {
      const Graph::key_t& key = eIt->key_;
      LCIter<size_t> wIt = eIt->value_.cbegin();
      for (size_t i = 0; i < eIt->value_.size(); ++i)
      {
        merged.addConnection(key.first, key.second, *wIt);
        ++wIt;
      }
      ++eIt;
    }

    eIt = g2.bonds_.begin();
    while (eIt != g2.bonds_.end())
    {
      const Graph::key_t& key = eIt->key_;
      LCIter<size_t> wIt = eIt->value_.cbegin();
      for (size_t i = 0; i < eIt->value_.size(); ++i)
      {
        merged.addConnection(key.first, key.second, *wIt);
        ++wIt;
      }
      ++eIt;
    }

    graphs.add(newName, merged);
  }

  void cmdExtract(std::istream& in, std::ostream&, GraphTable& graphs)
  {
    std::string newName;
    std::string oldName;
    size_t vertexCount;
    in >> newName >> oldName >> vertexCount;

    if (graphs.has(newName) || !graphs.has(oldName))
    {
      throw std::runtime_error("Invalid input");
    }

    Graph& oldGraph = graphs.get(oldName);
    Graph extracted(5, 5);

    List<std::string> verticesToKeep;
    for (size_t i = 0; i < vertexCount; ++i)
    {
      std::string vertex;
      in >> vertex;

      if (!oldGraph.findV(vertex))
      {
        throw std::out_of_range("Vertex missing");
      }

      verticesToKeep.pushBack(vertex);
      extracted.addVertex(vertex);
    }

    HTIter_k_t eIt = oldGraph.bonds_.begin();
    while (eIt != oldGraph.bonds_.end())
    {
      const Graph::key_t& key = eIt->key_;

      bool fromInList = false;
      bool toInList = false;

      LCIter<std::string> vIt = verticesToKeep.cbegin();
      for (size_t i = 0; i < verticesToKeep.size(); ++i)
      {
        if (*vIt == key.first)
        {
          fromInList = true;
        }
        if (*vIt == key.second)
        {
          toInList = true;
        }
        ++vIt;
      }

      if (fromInList && toInList)
      {
        LCIter<size_t> wIt = eIt->value_.cbegin();
        for (size_t i = 0; i < eIt->value_.size(); ++i)
        {
          extracted.addConnection(key.first, key.second, *wIt);
          ++wIt;
        }
      }

      ++eIt;
    }

    graphs.add(newName, extracted);
  }
}
