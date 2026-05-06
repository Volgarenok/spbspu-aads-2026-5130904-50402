#include "commands.hpp"
#include <Vector.hpp>
#include <limits>
#include "graph.hpp"
namespace karpovich
{
  namespace details
  {
    void sort_strings(karpovich::Vector< std::string > &vec)
    {
      for (size_t i = 1; i < vec.getSize(); ++i) {
        std::string key = vec[i];
        size_t j = i;
        while ((j > 0) && (vec[j - 1] > key)) {
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
        while ((j > 0) && (vec[j - 1] > key)) {
          vec[j] = vec[j - 1];
          --j;
        }
        vec[j] = key;
      }
    }
  }
}

void karpovich::cmdGraphs(std::istream &, std::ostream &out, GraphSet &graphs)
{
  karpovich::Vector< std::string > names;
  for (GraphSet::HIter it = graphs.begin(); it != graphs.end(); ++it) {
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
  Graph &g = graphs.get(g_name);
  Vector< std::string > verts;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it) {
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
  Graph &g = graphs.get(g_name);
  g.addEdge(v1, v2, w);
}

void karpovich::cmdCut(std::istream &in, std::ostream &, GraphSet &graphs)
{
  std::string g_name, v1, v2;
  size_t w = 0;
  in >> g_name >> v1 >> v2 >> w;
  Graph &g = graphs.get(g_name);
  bool v1_exists = false;
  bool v2_exists = false;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it) {
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
  Graph &g = graphs.get(g_name);
  bool found = false;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    if (*it == v_name) {
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::out_of_range("Vertex missing");
  }
  using EdgeOut = std::pair< std::string, Vector< size_t > >;
  Vector< EdgeOut > edges_out;
  for (HashIter< Graph::str_t, List< size_t >, PairHasher< std::string >, std::equal_to< Graph::str_t > > eit =
           g.edges.begin();
       eit != g.edges.end(); ++eit) {
    if ((*eit).first.first == v_name) {
      bool exists = false;
      for (VIter< EdgeOut > vit = edges_out.begin(); vit != edges_out.end(); ++vit) {
        if ((*vit).first == (*eit).first.second) {
          for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit) {
            (*vit).second.pushBack(*wit);
          }
          exists = true;
          break;
        }
      }
      if (!exists) {
        Vector< size_t > ws;
        for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit) {
          ws.pushBack(*wit);
        }
        edges_out.pushBack(std::make_pair((*eit).first.second, std::move(ws)));
      }
    }
  }
  Vector< std::string > dests;
  for (VIter< EdgeOut > vit = edges_out.begin(); vit != edges_out.end(); ++vit) {
    dests.pushBack((*vit).first);
  }
  details::sort_strings(dests);
  if (dests.getSize() > 0) {
    for (size_t i = 0; i < dests.getSize(); ++i) {
      out << dests[i];
      for (VIter< EdgeOut > vit = edges_out.begin(); vit != edges_out.end(); ++vit) {
        if ((*vit).first == dests[i]) {
          details::sort_weights((*vit).second);
          for (size_t j = 0; j < (*vit).second.getSize(); ++j) {
            out << " " << (*vit).second[j];
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
  Graph &g = graphs.get(g_name);
  bool found = false;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    if (*it == v_name) {
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::out_of_range("Vertex missing");
  }
  using EdgeIn = std::pair< std::string, Vector< size_t > >;
  Vector< EdgeIn > edges_in;
  for (HashIter< Graph::str_t, List< size_t >, PairHasher< std::string >, std::equal_to< Graph::str_t > > eit =
           g.edges.begin();
       eit != g.edges.end(); ++eit) {
    if ((*eit).first.second == v_name) {
      bool exists = false;
      for (VIter< EdgeIn > vit = edges_in.begin(); vit != edges_in.end(); ++vit) {
        if ((*vit).first == (*eit).first.first) {
          for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit) {
            (*vit).second.pushBack(*wit);
          }
          exists = true;
          break;
        }
      }
      if (!exists) {
        Vector< size_t > ws;
        for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit) {
          ws.pushBack(*wit);
        }
        edges_in.pushBack(std::make_pair((*eit).first.first, std::move(ws)));
      }
    }
  }
  Vector< std::string > srcs;
  for (VIter< EdgeIn > vit = edges_in.begin(); vit != edges_in.end(); ++vit) {
    srcs.pushBack((*vit).first);
  }
  details::sort_strings(srcs);
  for (size_t i = 0; i < srcs.getSize(); ++i) {
    out << srcs[i];
    for (VIter< EdgeIn > vit = edges_in.begin(); vit != edges_in.end(); ++vit) {
      if ((*vit).first == srcs[i]) {
        details::sort_weights((*vit).second);
        for (size_t j = 0; j < (*vit).second.getSize(); ++j) {
          out << " " << (*vit).second[j];
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
  Graph &src1 = graphs.get(g1);
  Graph &src2 = graphs.get(g2);
  for (LIter< std::string > it = src1.vertices.begin(); it != src1.vertices.end(); ++it) {
    merged.addVertex(*it);
  }
  for (LIter< std::string > it = src2.vertices.begin(); it != src2.vertices.end(); ++it) {
    merged.addVertex(*it);
  }
  for (HashIter< Graph::str_t, List< size_t >, PairHasher< std::string >, std::equal_to< Graph::str_t > > eit =
           src1.edges.begin();
       eit != src1.edges.end(); ++eit) {
    for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit) {
      merged.addEdge((*eit).first.first, (*eit).first.second, *wit);
    }
  }
  for (HashIter< Graph::str_t, List< size_t >, PairHasher< std::string >, std::equal_to< Graph::str_t > > eit =
           src2.edges.begin();
       eit != src2.edges.end(); ++eit) {
    for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit) {
      merged.addEdge((*eit).first.first, (*eit).first.second, *wit);
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
  Graph &src = graphs.get(old_g);
  List< std::string > required;
  for (size_t i = 0; i < k; ++i) {
    std::string v;
    in >> v;
    required.pushBack(v);
  }
  for (LIter< std::string > it = required.begin(); it != required.end(); ++it) {
    bool found = false;
    for (LIter< std::string > vit = src.vertices.begin(); vit != src.vertices.end(); ++vit) {
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
  for (LIter< std::string > it = required.begin(); it != required.end(); ++it) {
    ext.addVertex(*it);
  }
  for (HashIter< Graph::str_t, List< size_t >, PairHasher< std::string >, std::equal_to< Graph::str_t > > eit =
           src.edges.begin();
       eit != src.edges.end(); ++eit) {
    bool v1_ok = false;
    bool v2_ok = false;
    for (LIter< std::string > rit = required.begin(); rit != required.end(); ++rit) {
      if (*rit == (*eit).first.first) {
        v1_ok = true;
      }
      if (*rit == (*eit).first.second) {
        v2_ok = true;
      }
    }
    if (v1_ok && v2_ok) {
      for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit) {
        ext.addEdge((*eit).first.first, (*eit).first.second, *wit);
      }
    }
  }
  graphs.add(new_g, std::move(ext));
}
