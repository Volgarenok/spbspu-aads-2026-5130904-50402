#include "commands.hpp"
#include "Graph.hpp"
#include "sorts.hpp"

namespace lachugin
{
  void cmdGraphs(std::istream&, std::ostream& out, GraphStorage& storage)
  {
    List< std::string > names;
    const auto& graphs = storage.getGraphs();
    for (auto it = graphs.begin(); it != graphs.end(); ++it)
    {
      names.pushBack(it->key);
    }
    if (names.empty())
    {
      out << '\n';
      return;
    }
    sortList(names);
    for (auto it = names.begin(); it != names.end(); ++it)
    {
      out << *it << '\n';
    }
  }

  void cmdVertexes(std::istream& in, std::ostream& out, GraphStorage& storage)
  {
    std::string graphName;
    in >> graphName;
    const Graph& graph = storage.getGraph(graphName);
    List< std::string > verts = graph.getVertexes();
    if (verts.empty())
    {
      out << '\n';
      return;
    }
    sortList(verts);
    for (auto it = verts.begin(); it != verts.end(); ++it)
    {
      out << *it << '\n';
    }
  }

  void cmdOutbound(std::istream& in, std::ostream& out, GraphStorage& storage)
  {
    std::string name;
    std::string vertex;
    in >> name >> vertex;
    const Graph& graph = storage.getGraph(name);

    if (!graph.hasVertex(vertex))
    {
      throw std::logic_error("no vertex");
    }

    List< Edge > edges = graph.outbound(vertex);
    if (edges.empty())
    {
      out << '\n';
      return;
    }
    sortEdgesByTo(edges);
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      out << (*it).to;
      List< size_t > weights = graph.getEdges().get(*it);
      sortList(weights);

      for (auto w = weights.begin(); w != weights.end(); ++w)
      {
        out << ' ' << *w;
      }
      out << '\n';
    }
  }

  void cmdInbound(std::istream& in, std::ostream& out, GraphStorage& storage)
  {
    std::string name;
    std::string vertex;
    in >> name >> vertex;
    const Graph& graph = storage.getGraph(name);
    if (!graph.hasVertex(vertex))
    {
      throw std::logic_error("no vertex");
    }

    List< Edge > edges = graph.inbound(vertex);
    sortEdgesByFrom(edges);
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      out << (*it).from;
      List< size_t > weights = graph.getEdges().get(*it);
      sortList(weights);

      for (auto w = weights.begin(); w != weights.end(); ++w)
      {
        out << ' ' << *w;
      }
      out << '\n';
    }
  }

  void cmdCreate(std::istream& in, std::ostream&, GraphStorage& storage)
  {
    std::string name;
    in >> name;

    if (storage.hasGraph(name))
    {
      throw std::logic_error("Graph exists");
    }

    size_t count = 0;

    if (!(in >> count))
    {
      in.clear();

      Graph graph;
      storage.addGraph(name, graph);
      return;
    }

    Graph graph;

    for (size_t i = 0; i < count; ++i)
    {
      std::string vertex;
      if (!(in >> vertex))
      {
        throw std::logic_error("Bad create");
      }
      graph.addVertex(vertex);
    }

    storage.addGraph(name, graph);
  }

  void cmdBind(std::istream& in, std::ostream&, GraphStorage& storage)
  {
    std::string name;
    std::string from;
    std::string to;
    size_t weight = 0;
    in >> name >> from >> to >> weight;

    storage.getGraph(name).bind(from, to, weight);
  }

  void cmdCut(std::istream& in, std::ostream&, GraphStorage& storage)
  {
    std::string name;
    std::string from;
    std::string to;
    size_t weight = 0;
    in >> name >> from >> to >> weight;

    storage.getGraph(name).cut(from, to, weight);
  }

  void cmdMerge(std::istream& in, std::ostream&, GraphStorage& storage)
  {
    std::string newName;
    std::string g1Name;
    std::string g2Name;
    in >> newName >> g1Name >> g2Name;

    if (storage.hasGraph(newName))
    {
      throw std::logic_error("Graph exists");
    }

    const Graph& g1 = storage.getGraph(g1Name);
    const Graph& g2 = storage.getGraph(g2Name);
    Graph result;
    for (auto it = g1.getVertexes().begin(); it != g1.getVertexes().end(); ++it)
    {
      result.addVertex(*it);
    }

    for (auto it = g2.getVertexes().begin(); it != g2.getVertexes().end(); ++it)
    {
      result.addVertex(*it);
    }
    const auto& edges1 = g1.getEdges();

    for (auto it = edges1.begin(); it != edges1.end(); ++it)
    {
      for (auto w = it->value.begin(); w != it->value.end(); ++w)
      {
        result.bind(it->key.from, it->key.to, *w);
      }
    }
    const auto& edges2 = g2.getEdges();
    for (auto it = edges2.begin(); it != edges2.end();++it)
    {
      for (auto w = it->value.begin(); w != it->value.end(); ++w)
      {
        result.bind(it->key.from, it->key.to, *w);
      }
    }
    storage.addGraph(newName, result);
  }

  void cmdExtract(std::istream& in, std::ostream&, GraphStorage& storage)
  {
    std::string newName;
    std::string sourceName;
    size_t count = 0;

    in >> newName >> sourceName >> count;
    if (storage.hasGraph(newName))
    {
      throw std::logic_error("Graph exists");
    }

    const Graph& source = storage.getGraph(sourceName);
    List< std::string > selected;
    for (size_t i = 0; i < count; ++i)
    {
      std::string vertex;
      in >> vertex;
      if (!source.hasVertex(vertex))
      {
        throw std::logic_error("Vertex not found");
      }
      selected.pushBack(vertex);
    }
    Graph result;
    for (auto it = selected.begin(); it != selected.end(); ++it)
    {
      result.addVertex(*it);
    }
    const auto& edges = source.getEdges();
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      bool fromFound = false;
      bool toFound = false;
      for (auto v = selected.begin(); v != selected.end(); ++v)
      {
        if (*v == it->key.from)
        {
          fromFound = true;
        }
        if (*v == it->key.to)
        {
          toFound = true;
        }
      }

      if (fromFound && toFound)
      {
        for (auto w = it->value.begin(); w != it->value.end(); ++w)
        {
          result.bind(it->key.from, it->key.to, *w);
        }
      }
    }
    storage.addGraph(newName, result);
  }


}
