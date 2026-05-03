#include "commands.hpp"
#include "../common/Vector.hpp"

namespace details
{

  void sortStrings(karpovich::Vector< std::string > &vec)
  {
    for (size_t i = 1; i < vec.getSize(); ++i) {
      std::string key = vec[i];
      size_t j = i;
      while (j > 0 && vec[j - 1] > key) {
        vec[j] = vec[j - 1];
        --j;
      }
      vec[j] = key;
    }
  }

  void sortWeights(karpovich::Vector< size_t > &vec)
  {
    for (size_t i = 1; i < vec.getSize(); ++i) {
      size_t key = vec[i];
      size_t j = i;
      while (j > 0 && vec[j - 1] > key) {
        vec[j] = vec[j - 1];
        --j;
      }
      vec[j] = key;
    }
  }

}

void karpovich::cmdGraphs(std::istream &, std::ostream &out, karpovich::GraphSet &graphs)
{
  karpovich::Vector< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    names.pushBack((*it).first);
  }
  details::sortStrings(names);
  for (size_t i = 0; i < names.getSize(); ++i) {
    out << names[i] << "\n";
  }
}

void karpovich::cmdVertexes(std::istream &in, std::ostream &out, karpovich::GraphSet &graphs)
{
  std::string g_name;
  in >> g_name;
  karpovich::Graph &g = graphs.get(g_name);
  karpovich::Vector< std::string > verts;
  for (auto it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    verts.pushBack(*it);
  }
  details::sortStrings(verts);
  for (size_t i = 0; i < verts.getSize(); ++i) {
    out << verts[i] << "\n";
  }
}

void karpovich::cmdBind(std::istream &in, std::ostream &, karpovich::GraphSet &graphs)
{
  std::string g_name, v1, v2;
  size_t w = 0;
  in >> g_name >> v1 >> v2 >> w;
  karpovich::Graph &g = graphs.get(g_name);
  g.addEdge(v1, v2, w);
}

void karpovich::cmdCut(std::istream &in, std::ostream &, karpovich::GraphSet &graphs)
{
  std::string g_name, v1, v2;
  size_t w = 0;
  in >> g_name >> v1 >> v2 >> w;
  karpovich::Graph &g = graphs.get(g_name);
  g.removeEdge(v1, v2, w);
}

void karpovich::cmdCreate(std::istream &in, std::ostream &, karpovich::GraphSet &graphs)
{
  std::string g_name;
  size_t k = 0;
  in >> g_name >> k;
  if (graphs.has(g_name)) {
    throw std::runtime_error("Exists");
  }
  karpovich::Graph g(16);
  for (size_t i = 0; i < k; ++i) {
    std::string v;
    in >> v;
    g.addVertex(v);
  }
  graphs.add(g_name, std::move(g));
}

void karpovich::cmdOutbound(std::istream &in, std::ostream &out, karpovich::GraphSet &graphs)
{
  std::string g_name, v_name;
  in >> g_name >> v_name;
  karpovich::Graph &g = graphs.get(g_name);
  bool v_exists = false;
  for (auto it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    if (*it == v_name) {
      v_exists = true;
      break;
    }
  }
  if (!v_exists) {
    throw std::out_of_range("Vertex missing");
  }
  karpovich::List< std::pair< std::string, karpovich::List< size_t > > > out_edges;
  for (auto it = g.edges.begin(); it != g.edges.end(); ++it) {
    if ((*it).first.first == v_name) {
      out_edges.push_back(std::make_pair((*it).first.second, (*it).second));
    }
  }
  karpovich::Vector< std::string > dests;
  for (auto it = out_edges.begin(); it != out_edges.end(); ++it) {
    dests.pushBack((*it).first);
  }
  details::sortStrings(dests);
  for (size_t i = 0; i < dests.getSize(); ++i) {
    out << dests[i];
    for (auto it = out_edges.begin(); it != out_edges.end(); ++it) {
      if ((*it).first == dests[i]) {
        karpovich::Vector< size_t > ws;
        for (auto wit = (*it).second.begin(); wit != (*it).second.end(); ++wit) {
          ws.pushBack(*wit);
        }
        details::sortWeights(ws);
        for (size_t j = 0; j < ws.getSize(); ++j) {
          out << " " << ws[j];
        }
        break;
      }
    }
    out << "\n";
  }
}

void karpovich::cmdInbound(std::istream &in, std::ostream &out, karpovich::GraphSet &graphs)
{
  std::string g_name, v_name;
  in >> g_name >> v_name;
  karpovich::Graph &g = graphs.get(g_name);
  bool v_exists = false;
  for (auto it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    if (*it == v_name) {
      v_exists = true;
      break;
    }
  }
  if (!v_exists) {
    throw std::out_of_range("Vertex missing");
  }
  karpovich::List< std::pair< std::string, karpovich::List< size_t > > > in_edges;
  for (auto it = g.edges.begin(); it != g.edges.end(); ++it) {
    if ((*it).first.second == v_name) {
      in_edges.push_back(std::make_pair((*it).first.first, (*it).second));
    }
  }
  karpovich::Vector< std::string > srcs;
  for (auto it = in_edges.begin(); it != in_edges.end(); ++it) {
    srcs.pushBack((*it).first);
  }
  details::sortStrings(srcs);
  for (size_t i = 0; i < srcs.getSize(); ++i) {
    out << srcs[i];
    for (auto it = in_edges.begin(); it != in_edges.end(); ++it) {
      if ((*it).first == srcs[i]) {
        karpovich::Vector< size_t > ws;
        for (auto wit = (*it).second.begin(); wit != (*it).second.end(); ++wit) {
          ws.pushBack(*wit);
        }
        details::sortWeights(ws);
        for (size_t j = 0; j < ws.getSize(); ++j) {
          out << " " << ws[j];
        }
        break;
      }
    }
    out << "\n";
  }
}
