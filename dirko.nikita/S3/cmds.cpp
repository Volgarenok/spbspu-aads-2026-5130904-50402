#include "cmds.hpp"
#include <string>
#include "graph.hpp"

namespace dirko
{
  void sort_strings(dirko::Vector< std::string > &vec)
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

  void sort_weights(dirko::Vector< size_t > &vec)
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

void dirko::cmdGraphs(std::istream &, std::ostream &out, GraphTable &graphs)
{
  dirko::Vector< std::string > names;
  for (const std::pair< std::string, Graph > &v : graphs) {
    names.pushBack(v.first);
  }
  sort_strings(names);
  if (names.getSize() == 0) {
    out << "\n";
  } else {
    for (size_t i = 0; i < names.getSize(); ++i) {
      out << names[i] << "\n";
    }
  }
}

void dirko::cmdConections(std::istream &in, std::ostream &out, GraphTable &graphs)
{
  std::string graph;
  in >> graph;
  if (!graphs.has(graph)) {
    throw std::runtime_error("No graph");
  }
  Graph &g = graphs.get(graph);
  Vector< std::string > cons;
  for (const std::string &v : g.vertices) {
    cons.pushBack(v);
  }
  sort_strings(cons);
  if (cons.getSize() == 0) {
    out << "\n";
  } else {
    for (size_t i = 0; i < cons.getSize(); ++i) {
      out << cons[i] << "\n";
    }
  }
}

void dirko::cmdBind(std::istream &in, std::ostream &, GraphTable &graphs)
{
  std::string graph, v1, v2;
  size_t w = 0;
  in >> graph >> v1 >> v2 >> w;
  Graph &g = graphs.get(graph);
  g.addConection(v1, v2, w);
}

void dirko::cmdCut(std::istream &in, std::ostream &, GraphTable &graphs)
{
  std::string graph, v1, v2;
  size_t w = 0;
  in >> graph >> v1 >> v2 >> w;
  Graph &g = graphs.get(graph);
  bool isV1 = false;
  bool isV2 = false;
  for (const std::string &v : g.vertices) {
    if (v == v1) {
      isV1 = true;
    }
    if (v == v2) {
      isV2 = true;
    }
  }
  if (!isV1 || !isV2) {
    throw std::runtime_error("Vertex missing");
  }
  g.removeConection(v1, v2, w);
}

void dirko::cmdCreate(std::istream &in, std::ostream &, GraphTable &graphs)
{
  std::string graph;
  in >> graph;
  if (!in) {
    throw std::runtime_error("Invalid input");
  }
  if (graphs.has(graph)) {
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
  graphs.add(graph, std::move(g));
}

void dirko::cmdOutbound(std::istream &in, std::ostream &out, GraphTable &graphs)
{
  std::string graph, vertex;
  in >> graph >> vertex;
  Graph &g = graphs.get(graph);
  bool found = false;
  for (const std::string &v : g.vertices) {
    if (v == vertex) {
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::out_of_range("Vertex missing");
  }
  using EdgeOut = std::pair< std::string, Vector< size_t > >;
  Vector< EdgeOut > conections_out;
  for (std::pair< Graph::key_t, List< size_t > > &v : g.conections) {
    if (v.first.first == vertex) {
      bool exists = false;
      for (EdgeOut &v_val : conections_out) {
        if (v_val.first == v.first.second) {
          for (size_t &w_val : v.second) {
            v_val.second.pushBack(w_val);
          }
          exists = true;
          break;
        }
      }
      if (!exists) {
        Vector< size_t > ws;
        for (size_t w_val : v.second) {
          ws.pushBack(w_val);
        }
        conections_out.pushBack(std::make_pair(v.first.second, std::move(ws)));
      }
    }
  }
  Vector< std::string > dests;
  for (EdgeOut &v : conections_out) {
    dests.pushBack(v.first);
  }
  sort_strings(dests);
  if (dests.getSize() > 0) {
    for (size_t i = 0; i < dests.getSize(); ++i) {
      out << dests[i];
      for (EdgeOut v : conections_out) {
        if (v.first == dests[i]) {
          sort_weights(v.second);
          for (size_t j = 0; j < v.second.getSize(); ++j) {
            out << " " << v.second[j];
          }
          break;
        }
      }
      if (i < dests.getSize() - 1) {
        out << "\n";
      }
    }
  }
  out << "\n";
}

void dirko::cmdInbound(std::istream &in, std::ostream &out, GraphTable &graphs)
{
  std::string graph, vertex;
  in >> graph >> vertex;
  Graph &g = graphs.get(graph);
  bool found = false;
  for (std::string v : g.vertices) {
    if (v == vertex) {
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::out_of_range("Vertex missing");
  }
  using EdgeIn = std::pair< std::string, Vector< size_t > >;
  Vector< EdgeIn > conections_in;
  for (std::pair< Graph::key_t, List< size_t > > &v : g.conections) {
    if (v.first.second == vertex) {
      bool exists = false;
      for (EdgeIn &v_val : conections_in) {
        if (v_val.first == v.first.first) {
          for (size_t w_val : v.second) {
            v_val.second.pushBack(w_val);
          }
          exists = true;
          break;
        }
      }
      if (!exists) {
        Vector< size_t > ws;
        for (size_t w_val : v.second) {
          ws.pushBack(w_val);
        }
        conections_in.pushBack(std::make_pair(v.first.first, std::move(ws)));
      }
    }
  }
  Vector< std::string > srcs;
  for (EdgeIn &v_val : conections_in) {
    srcs.pushBack(v_val.first);
  }
  sort_strings(srcs);
  for (size_t i = 0; i < srcs.getSize(); ++i) {
    out << srcs[i];
    for (EdgeIn &v_val : conections_in) {
      if (v_val.first == srcs[i]) {
        sort_weights(v_val.second);
        for (size_t j = 0; j < v_val.second.getSize(); ++j) {
          out << " " << v_val.second[j];
        }
        break;
      }
    }
    out << "\n";
  }
}

void dirko::cmdMerge(std::istream &in, std::ostream &, GraphTable &graphs)
{
  std::string new_g, g1, g2;
  in >> new_g >> g1 >> g2;
  if (graphs.has(new_g) || !graphs.has(g1) || !graphs.has(g2)) {
    throw std::runtime_error("Invalid");
  }
  Graph merged(16);
  Graph &src1 = graphs.get(g1);
  Graph &src2 = graphs.get(g2);
  for (std::string &v : src1.vertices) {
    merged.addVertex(v);
  }
  for (std::string &v : src2.vertices) {
    merged.addVertex(v);
  }
  for (std::pair< Graph::key_t, List< size_t > > &v : src1.conections) {
    for (size_t w_val : v.second) {
      merged.addConection(v.first.first, v.first.second, w_val);
    }
  }
  for (std::pair< Graph::key_t, List< size_t > > &v : src2.conections) {
    for (size_t w_val : v.second) {
      merged.addConection(v.first.first, v.first.second, w_val);
    }
  }
  graphs.add(new_g, std::move(merged));
}

void dirko::cmdExtract(std::istream &in, std::ostream &, GraphTable &graphs)
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
    required.push_back(v);
  }
  for (std::string &v : required) {
    bool found = false;
    for (std::string &v_val : src.vertices) {
      if (v_val == v) {
        found = true;
        break;
      }
    }
    if (!found) {
      throw std::out_of_range("Vertex missing");
    }
  }
  Graph ext(20);
  for (std::string &v : required) {
    ext.addVertex(v);
  }
  for (std::pair< Graph::key_t, List< size_t > > &v : src.conections) {
    bool v1_ok = false;
    bool v2_ok = false;
    for (std::string &r_val : required) {
      if (r_val == v.first.first) {
        v1_ok = true;
      }
      if (r_val == v.first.second) {
        v2_ok = true;
      }
    }
    if (v1_ok && v2_ok) {
      for (size_t &w_val : v.second) {
        ext.addConection(v.first.first, v.first.second, w_val);
      }
    }
  }
  graphs.add(new_g, std::move(ext));
}
