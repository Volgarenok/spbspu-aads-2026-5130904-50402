#include "commands.hpp"
#include <limits>
#include "../common/Vector.hpp"

namespace details
{
  void sort_strings(karpovich::Vector< std::string > &vec)
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

  void sort_weights(karpovich::Vector< size_t > &vec)
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

void karpovich::cmdGraphs(std::istream &, std::ostream &out, GraphSet &graphs)
{
  karpovich::Vector< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    names.pushBack((*it).first);
  }
  details::sort_strings(names);
  if (names.getSize() == 0) {
    out << "\n";
  } else {
    for (size_t i = 0; i < names.getSize(); ++i) {
      out << names[i] << "\n";
    }
  }
}

void karpovich::cmdVertexes(std::istream &in, std::ostream &out, GraphSet &graphs)
{
  std::string g_name;
  in >> g_name;
  if (!graphs.has(g_name)) {
    throw std::runtime_error("No graph");
  }
  auto &g = graphs.get(g_name);
  karpovich::Vector< std::string > verts;
  for (auto it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    verts.pushBack(*it);
  }
  details::sort_strings(verts);
  if (verts.getSize() == 0) {
    out << "\n";
  } else {
    for (size_t i = 0; i < verts.getSize(); ++i) {
      out << verts[i] << "\n";
    }
  }
}

void karpovich::cmdBind(std::istream &in, std::ostream &, GraphSet &graphs)
{
  std::string g_name, v1, v2;
  size_t w = 0;
  in >> g_name >> v1 >> v2 >> w;
  auto &g = graphs.get(g_name);
  g.addEdge(v1, v2, w);
}

void karpovich::cmdCut(std::istream &in, std::ostream &, GraphSet &graphs)
{
  std::string g_name, v1, v2;
  size_t w = 0;
  in >> g_name >> v1 >> v2 >> w;
  auto &g = graphs.get(g_name);
  bool v1_exists = false, v2_exists = false;
  for (auto it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    if (*it == v1) {
      v1_exists = true;
    }
    if (*it == v2) {
      v2_exists = true;
    }
  }
  if (!v1_exists || !v2_exists) {
    throw std::runtime_error("Vertex missing");
  }
  g.removeEdge(v1, v2, w);
}

void karpovich::cmdCreate(std::istream &in, std::ostream &, GraphSet &graphs)
{
  std::string g_name;
  in >> g_name;
  if (!in) {
    throw std::runtime_error("Invalid input");
  }
  if (graphs.has(g_name)) {
    throw std::runtime_error("Graph already exists");
  }
  size_t k = 0;
  in >> k;
  if (!in) {
    throw std::runtime_error("Invalid vertex count");
  }
  Graph g;
  for (size_t i = 0; i < k; ++i) {
    std::string v;
    in >> v;
    if (!in) {
      throw std::runtime_error("Invalid vertex name");
    }
    g.addVertex(v);
  }
  graphs.add(g_name, std::move(g));
}

void karpovich::cmdOutbound(std::istream &in, std::ostream &out, GraphSet &graphs)
{
  std::string g_name, v_name;
  in >> g_name >> v_name;
  auto &g = graphs.get(g_name);
  bool found = false;
  for (auto it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    if (*it == v_name) {
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::out_of_range("Vertex missing");
  }
  karpovich::Vector< std::pair< std::string, karpovich::Vector< size_t > > > edges_out;
  for (auto it = g.edges.begin(); it != g.edges.end(); ++it) {
    if ((*it).first.first == v_name) {
      bool exists = false;
      for (auto &e : edges_out) {
        if (e.first == (*it).first.second) {
          for (auto w : (*it).second) {
            e.second.pushBack(w);
          }
          exists = true;
          break;
        }
      }
      if (!exists) {
        karpovich::Vector< size_t > ws;
        for (auto w : (*it).second) {
          ws.pushBack(w);
        }
        edges_out.pushBack(std::make_pair((*it).first.second, ws));
      }
    }
  }
  karpovich::Vector< std::string > dests;
  for (auto &e : edges_out) {
    dests.pushBack(e.first);
  }
  details::sort_strings(dests);
  if (dests.getSize() > 0) {
    for (size_t i = 0; i < dests.getSize(); ++i) {
      out << dests[i];
      for (auto &e : edges_out) {
        if (e.first == dests[i]) {
          details::sort_weights(e.second);
          for (size_t j = 0; j < e.second.getSize(); ++j) {
            out << " " << e.second[j];
          }
          break;
        }
      }
      if (i < dests.getSize() - 1) {
        out << "\n";
      }
    }
    out << "\n";
  } else {
    out << "\n";
  }
}

void karpovich::cmdInbound(std::istream &in, std::ostream &out, GraphSet &graphs)
{
  std::string g_name, v_name;
  in >> g_name >> v_name;
  auto &g = graphs.get(g_name);
  bool found = false;
  for (auto it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    if (*it == v_name) {
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::out_of_range("Vertex missing");
  }
  karpovich::Vector< std::pair< std::string, karpovich::Vector< size_t > > > edges_in;
  for (auto it = g.edges.begin(); it != g.edges.end(); ++it) {
    if ((*it).first.second == v_name) {
      bool exists = false;
      for (auto &e : edges_in) {
        if (e.first == (*it).first.first) {
          for (auto w : (*it).second) {
            e.second.pushBack(w);
          }
          exists = true;
          break;
        }
      }
      if (!exists) {
        karpovich::Vector< size_t > ws;
        for (auto w : (*it).second) {
          ws.pushBack(w);
        }
        edges_in.pushBack(std::make_pair((*it).first.first, ws));
      }
    }
  }
  karpovich::Vector< std::string > srcs;
  for (auto &e : edges_in) {
    srcs.pushBack(e.first);
  }
  details::sort_strings(srcs);
  for (size_t i = 0; i < srcs.getSize(); ++i) {
    out << srcs[i];
    for (auto &e : edges_in) {
      if (e.first == srcs[i]) {
        details::sort_weights(e.second);
        for (size_t j = 0; j < e.second.getSize(); ++j) {
          out << " " << e.second[j];
        }
        break;
      }
    }
    out << "\n";
  }
}

void karpovich::cmdMerge(std::istream &in, std::ostream &, GraphSet &graphs)
{
  std::string new_g, g1, g2;
  in >> new_g >> g1 >> g2;
  if (graphs.has(new_g) || !graphs.has(g1) || !graphs.has(g2)) {
    throw std::runtime_error("Invalid");
  }
  Graph merged(16);
  auto &src1 = graphs.get(g1);
  auto &src2 = graphs.get(g2);
  for (auto it = src1.vertices.begin(); it != src1.vertices.end(); ++it) {
    merged.addVertex(*it);
  }
  for (auto it = src2.vertices.begin(); it != src2.vertices.end(); ++it) {
    merged.addVertex(*it);
  }
  for (auto it = src1.edges.begin(); it != src1.edges.end(); ++it) {
    for (auto w : (*it).second) {
      merged.addEdge((*it).first.first, (*it).first.second, w);
    }
  }
  for (auto it = src2.edges.begin(); it != src2.edges.end(); ++it) {
    for (auto w : (*it).second) {
      merged.addEdge((*it).first.first, (*it).first.second, w);
    }
  }
  graphs.add(new_g, std::move(merged));
}

void karpovich::cmdExtract(std::istream &in, std::ostream &, GraphSet &graphs)
{
  std::string new_g, old_g;
  size_t k = 0;
  in >> new_g >> old_g >> k;
  if (graphs.has(new_g) || !graphs.has(old_g)) {
    throw std::runtime_error("Invalid");
  }
  auto &src = graphs.get(old_g);
  karpovich::List< std::string > required;
  for (size_t i = 0; i < k; ++i) {
    std::string v;
    in >> v;
    required.pushBack(v);
  }
  for (auto it = required.begin(); it != required.end(); ++it) {
    bool found = false;
    for (auto vit = src.vertices.begin(); vit != src.vertices.end(); ++vit) {
      if (*vit == *it) {
        found = true;
        break;
      }
    }
    if (!found) {
      throw std::out_of_range("Vertex missing");
    }
  }
  Graph ext(16);
  for (auto it = required.begin(); it != required.end(); ++it) {
    ext.addVertex(*it);
  }
  for (auto it = src.edges.begin(); it != src.edges.end(); ++it) {
    bool v1_ok = false;
    bool v2_ok = false;
    for (auto rit = required.begin(); rit != required.end(); ++rit) {
      if (*rit == (*it).first.first) {
        v1_ok = true;
      }
      if (*rit == (*it).first.second) {
        v2_ok = true;
      }
    }
    if (v1_ok && v2_ok) {
      for (auto w : (*it).second) {
        ext.addEdge((*it).first.first, (*it).first.second, w);
      }
    }
  }
  graphs.add(new_g, std::move(ext));
}
