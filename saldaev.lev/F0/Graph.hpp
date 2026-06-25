#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "RobinTable.hpp"

namespace saldaev
{
  template< class T, class S >
  struct Graph
  {
  private:
    struct Hasher
    {
      size_t operator()(const std::string &p) const;
    };

    struct KeyEqual
    {
      bool operator()(const std::string &st, const std::string &nd) const;
    };

    struct Edge;
    struct Vertex
    {
      std::string id;
      T data;
      Vector< Edge * > outgoing_edges;
      Vector< Edge * > incoming_edges;

      Vertex(std::string i, T d);
    };

    struct Edge
    {
      S data;
      Vertex *from;
      Vertex *to;

      Edge(S d, Vertex *f, Vertex *t);
    };

    HashTable< std::string, Vertex *, Hasher, KeyEqual > vertexes;
    Vector< Edge * > edges;

    void removeEdgesOf(Vertex *);
    Vertex *findVertex(const std::string &id);

  public:
    Graph();
    Graph(const Graph &) = delete;
    Graph(Graph &&) = delete;
    Graph &operator=(const Graph &) = delete;
    Graph &operator=(Graph &&) = delete;
    ~Graph();

    Vector< std::string > vertexIds() const;
    bool hasVertex(const std::string &id) const;
    void addVertex(const T &data, const std::string &id);
    void removeVertex(const std::string &id);
    void setVertexData(const std::string &id, const T &data);
    T getVertexData(const std::string &id) const;

    Vector< std::pair< std::string, std::string > > edgesIds() const;
    bool hasEdge(const std::string &from, const std::string &to) const;
    void addEdge(const std::string &from, const std::string &to, const S &data);
    void removeEdge(const std::string &from, const std::string &to);
    void setEdgeData(const std::string &from, const std::string &to, const S &data);
    S getEdgeData(const std::string &from, const std::string &to) const;

    size_t indegree(const std::string &id) const;
    size_t outdegree(const std::string &id) const;
    size_t degree(const std::string &id) const;
    const Vector< std::string > outgoingEdges(const std::string &id) const;
    const Vector< std::string > incomingEdges(const std::string &id) const;

    size_t countVertexes() const;
    size_t countEdges() const;

    void clearEdges();
    void clear();
  };
}

template< class T, class S >
saldaev::Graph< T, S >::Vertex::Vertex(std::string i, T d):
  id(i),
  data(d),
  outgoing_edges(0),
  incoming_edges(0)
{}

template< class T, class S >
saldaev::Graph< T, S >::Edge::Edge(S d, Vertex *f, Vertex *t):
  data(d),
  from(f),
  to(t)
{}

template< class T, class S >
saldaev::Graph< T, S >::Graph():
  edges(0)
{}

template< class T, class S >
saldaev::Graph< T, S >::~Graph()
{
  for (size_t i = 0; i < edges.getSize(); ++i) {
    delete edges[i];
  }

  auto it = vertexes.begin();
  while (it != vertexes.end()) {
    delete it->second;
    ++it;
  }
}

template< class T, class S >
void saldaev::Graph< T, S >::removeEdgesOf(Vertex *v)
{
  for (size_t i = 0; i < v->incoming_edges.getSize(); ++i) {
    Edge *e = v->incoming_edges[i];
    e->from->outgoing_edges.remove(e);
    edges.remove(e);
    delete e;
  }
  for (size_t i = 0; i < v->outgoing_edges.getSize(); ++i) {
    Edge *e = v->outgoing_edges[i];
    e->to->incoming_edges.remove(e);
    edges.remove(e);
    delete e;
  }

  v->incoming_edges.clear();
  v->outgoing_edges.clear();
}

template< class T, class S >
saldaev::Vector< std::string > saldaev::Graph< T, S >::vertexIds() const
{
  Vector< std::string > ret(0);
  auto it = vertexes.begin();
  while (it != vertexes.end()) {
    ret.pushBack(it->first);
    ++it;
  }
  return ret;
}

template< class T, class S >
bool saldaev::Graph< T, S >::hasVertex(const std::string &id) const
{
  return vertexes.has(id);
}

template< class T, class S >
void saldaev::Graph< T, S >::addVertex(const T &data, const std::string &id)
{
  if (vertexes.has(id)) {
    throw std::invalid_argument("ununique vertex name");
  }

  Vertex *nV = new Vertex(id, data);
  vertexes.add(id, nV);
}

template< class T, class S >
void saldaev::Graph< T, S >::removeVertex(const std::string &id)
{
  if (!(vertexes.has(id))) {
    throw std::out_of_range("no vertex with this name");
  }

  Vertex *v = vertexes.get(id);
  removeEdgesOf(v);

  delete v;
  vertexes.remove(id);
}

template< class T, class S >
void saldaev::Graph< T, S >::setVertexData(const std::string &id, const T &data)
{
  if (!(vertexes.has(id))) {
    throw std::logic_error("no vertex with this name");
  }
  vertexes.get(id)->data = data;
}

template< class T, class S >
T saldaev::Graph< T, S >::getVertexData(const std::string &id) const
{
  return vertexes.get(id)->data;
}

template< class T, class S >
saldaev::Vector< std::pair< std::string, std::string > > saldaev::Graph< T, S >::edgesIds() const
{
  Vector< std::pair< std::string, std::string > > ret(0);
  for (size_t i = 0; i < edges.getSize(); ++i) {
    ret.pushBack({edges[i]->from->id, edges[i]->to->id});
  }
  return ret;
}

template< class T, class S >
bool saldaev::Graph< T, S >::hasEdge(const std::string &from, const std::string &to) const
{
  if (!(vertexes.has(from)) || !(vertexes.has(to))) {
    throw std::logic_error("no vertex with this name");
  }

  Vertex *v1 = vertexes.get(from);
  Vertex *v2 = vertexes.get(to);

  const Vector< Edge * > &e = v1->outgoing_edges;
  for (size_t i = 0; i < e.getSize(); ++i) {
    if (e[i]->to == v2) {
      return true;
    }
  }
  return false;
}

template< class T, class S >
void saldaev::Graph< T, S >::addEdge(const std::string &from, const std::string &to, const S &data)
{
  if (hasEdge(from, to)) {
    throw std::logic_error("this edge is already exist");
  }

  Vertex *v1 = vertexes.get(from);
  Vertex *v2 = vertexes.get(to);

  Edge *e = new Edge(data, v1, v2);
  try {
    v1->outgoing_edges.pushBack(e);
  } catch (...) {
    delete e;
    throw;
  }
  try {
    v2->incoming_edges.pushBack(e);
  } catch (...) {
    v1->outgoing_edges.remove(e);
    delete e;
    throw;
  }
  try {
    edges.pushBack(e);
  } catch (...) {
    v1->outgoing_edges.remove(e);
    v2->incoming_edges.remove(e);
    delete e;
    throw;
  }
}

template< class T, class S >
void saldaev::Graph< T, S >::removeEdge(const std::string &from, const std::string &to)
{
  if (!(vertexes.has(from)) || !(vertexes.has(to))) {
    throw std::logic_error("no vertex with this name");
  }

  Vertex *v1 = vertexes.get(from);
  Vertex *v2 = vertexes.get(to);

  Vector< Edge * > &vE = v1->outgoing_edges;
  for (size_t i = 0; i < vE.getSize(); ++i) {
    if (vE[i]->to == v2) {
      Edge *e = vE[i];
      v1->outgoing_edges.remove(e);
      v2->incoming_edges.remove(e);
      edges.remove(e);
      delete e;
      return;
    }
  }
  throw std::invalid_argument("no such edge");
}

template< class T, class S >
void saldaev::Graph< T, S >::setEdgeData(const std::string &from, const std::string &to, const S &data)
{
  if (!(vertexes.has(from)) || !(vertexes.has(to))) {
    throw std::logic_error("no vertex with this name");
  }

  Vertex *v1 = vertexes.get(from);
  Vertex *v2 = vertexes.get(to);

  Vector< Edge * > &vE = v1->outgoing_edges;
  for (size_t i = 0; i < vE.getSize(); ++i) {
    if (vE[i]->to == v2) {
      vE[i]->data = data;
      return;
    }
  }
  throw std::invalid_argument("no such edge");
}

template< class T, class S >
S saldaev::Graph< T, S >::getEdgeData(const std::string &from, const std::string &to) const
{
  if (!(vertexes.has(from)) || !(vertexes.has(to))) {
    throw std::logic_error("no vertex with this name");
  }

  Vertex *v1 = vertexes.get(from);
  Vertex *v2 = vertexes.get(to);

  Vector< Edge * > &vE = v1->outgoing_edges;
  for (size_t i = 0; i < vE.getSize(); ++i) {
    if (vE[i]->to == v2) {
      return vE[i]->data;
    }
  }
  throw std::invalid_argument("no such edge");
}

template< class T, class S >
size_t saldaev::Graph< T, S >::indegree(const std::string &id) const
{
  if (!(vertexes.has(id))) {
    throw std::out_of_range("no vertex with this name");
  }

  return vertexes.get(id)->incoming_edges.getSize();
}

template< class T, class S >
size_t saldaev::Graph< T, S >::outdegree(const std::string &id) const
{
  if (!(vertexes.has(id))) {
    throw std::out_of_range("no vertex with this name");
  }

  return vertexes.get(id)->outgoing_edges.getSize();
}

template< class T, class S >
size_t saldaev::Graph< T, S >::degree(const std::string &id) const
{
  if (!(vertexes.has(id))) {
    throw std::out_of_range("no vertex with this name");
  }

  size_t n = vertexes.get(id)->incoming_edges.getSize();
  return n + vertexes.get(id)->outgoing_edges.getSize();
}

template< class T, class S >
const saldaev::Vector< std::string > saldaev::Graph< T, S >::outgoingEdges(const std::string &id) const
{
  if (!(vertexes.has(id))) {
    throw std::out_of_range("no vertex with this name");
  }

  Vertex *v = vertexes.get(id);
  size_t n = v->outgoing_edges.getSize();
  Vector< std::string > ret(n);
  for (size_t i = 0; i < n; ++i) {
    ret.pushBack(v->outgoing_edges[i]->to->id);
  }
  return ret;
}

template< class T, class S >
const saldaev::Vector< std::string > saldaev::Graph< T, S >::incomingEdges(const std::string &id) const
{
  if (!(vertexes.has(id))) {
    throw std::out_of_range("no vertex with this name");
  }

  Vertex *v = vertexes.get(id);
  size_t n = v->incoming_edges.getSize();
  Vector< std::string > ret(n);
  for (size_t i = 0; i < n; ++i) {
    ret.pushBack(v->incoming_edges[i]->from->id);
  }
  return ret;
}

template< class T, class S >
size_t saldaev::Graph< T, S >::countVertexes() const
{
  return vertexes.size();
}

template< class T, class S >
size_t saldaev::Graph< T, S >::countEdges() const
{
  return edges.getSize();
}

template< class T, class S >
void saldaev::Graph< T, S >::clearEdges()
{
  while (edges.getSize()) {
    Edge *e = edges[0];
    e->from->outgoing_edges.remove(e);
    e->to->incoming_edges.remove(e);
    edges.remove(e);
    delete e;
  }
}

template< class T, class S >
void saldaev::Graph< T, S >::clear()
{
  clearEdges();

  auto it = vertexes.begin();
  while (it != vertexes.end()) {
    delete it->second;
    ++it;
  }
  vertexes.clear();
}

template< class T, class S >
size_t saldaev::Graph< T, S >::Hasher::operator()(const std::string &p) const
{
  return std::hash< std::string >{}(p);
}

template< class T, class S >
bool saldaev::Graph< T, S >::KeyEqual::operator()(const std::string &st, const std::string &nd) const
{
  return st == nd;
}

#endif
