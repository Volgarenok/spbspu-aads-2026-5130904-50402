#include "commands.hpp"
#include "../common/Vec.hpp"
#include "Hashtable.hpp"
#include "Graph.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace sogdanov
{

  HashTable<std::string, Graph> app_graphs(100);

  template <class Iter>
  void sort(Iter begin, Iter end)
  {
    if (begin == end)
    {
      return;
    }
    for (Iter i = begin + 1; i < end; ++i)
    {
      auto key = *i;
      Iter j = i - 1;
      while (j >= begin && key < *j)
      {
        *(j + 1) = *j;
        if (j == begin)
        {
          j--;
          break;
        }
        --j;
      }
      *(j + 1) = key;
    }
  }

  void cmd_graphs(std::istream &, std::ostream &out)
  {
    Vector<std::string> names;
    for (auto it = app_graphs.begin(); it != app_graphs.end(); ++it)
    {
      names.pushBack((*it).k);
    }
    sort(names.begin(), names.end());
    for (size_t i = 0; i < names.getSize(); ++i)
    {
      out << names[i] << "\n";
    }
  }

  void cmd_vertexes(std::istream &in, std::ostream &out)
  {
    std::string gname;
    in >> gname;
    if (!app_graphs.has(gname))
    {
      throw std::logic_error("Graph not found");
    }
    Vector<std::string> verts = app_graphs.get(gname).get_vertices();
    sort(verts.begin(), verts.end());
    for (size_t i = 0; i < verts.getSize(); ++i)
    {
      out << verts[i] << "\n";
    }
  }

  void cmd_outbound(std::istream &in, std::ostream &out)
  {
    std::string gname, vname;
    in >> gname >> vname;
    if (!app_graphs.has(gname))
    {
      throw std::logic_error("Graph not found");
    }
    Graph &g = app_graphs.get(gname);
    if (!g.has_vertex(vname))
    {
      throw std::logic_error("Vertex not found");
    }
    Vector<std::string> dests = g.get_vertices();
    sort(dests.begin(), dests.end());

    auto &edges = g.get_edges();
    for (size_t i = 0; i < dests.getSize(); ++i)
    {
      std::pair<std::string, std::string> key{vname, dests[i]};
      if (edges.has(key))
      {
        Vector<size_t> w = edges.get(key);
        sort(w.begin(), w.end());
        out << dests[i];
        for (size_t j = 0; j < w.getSize(); ++j)
        {
          out << " " << w[j];
        }
        out << "\n";
      }
    }
  }

  void cmd_inbound(std::istream &in, std::ostream &out)
  {
    std::string gname, vname;
    in >> gname >> vname;
    if (!app_graphs.has(gname))
    {
      throw std::logic_error("Graph not found");
    }
    Graph &g = app_graphs.get(gname);
    if (!g.has_vertex(vname))
    {
      throw std::logic_error("Vertex not found");
    }
    Vector<std::string> srcs = g.get_vertices();
    sort(srcs.begin(), srcs.end());

    auto &edges = g.get_edges();
    for (size_t i = 0; i < srcs.getSize(); ++i)
    {
      std::pair<std::string, std::string> key{srcs[i], vname};
      if (edges.has(key))
      {
        Vector<size_t> w = edges.get(key);
        sort(w.begin(), w.end());
        out << srcs[i];
        for (size_t j = 0; j < w.getSize(); ++j)
        {
          out << " " << w[j];
        }
        out << "\n";
      }
    }
  }

  void cmd_bind(std::istream &in, std::ostream&)
  {
    std::string gname, u, v;
    size_t w;
    in >> gname >> u >> v >> w;
    if (!app_graphs.has(gname))
    {
      throw std::logic_error("Graph not found");
    }
    app_graphs.get(gname).bind(u, v, w);
  }

  void cmd_cut(std::istream &in, std::ostream&)
  {
    std::string gname, u, v;
    size_t w;
    in >> gname >> u >> v >> w;
    if (!app_graphs.has(gname))
    {
      throw std::logic_error("Graph not found");
    }
    app_graphs.get(gname).cut(u, v, w);
  }

  void cmd_create(std::istream &in, std::ostream&)
  {
    std::string gname;
    int k;
    in >> gname >> k;
    if (app_graphs.has(gname))
    {
      throw std::logic_error("Graph already exists");
    }
    Graph new_g;
    for (int i = 0; i < k; ++i)
    {
      std::string v;
      in >> v;
      new_g.add_vertex(v);
    }
    app_graphs.add(gname, new_g);
  }

  void cmd_merge(std::istream &in, std::ostream&)
  {
    std::string gnew, g1, g2;
    in >> gnew >> g1 >> g2;
    if (app_graphs.has(gnew))
    {
      throw std::logic_error("New graph already exists");
    }
    if (!app_graphs.has(g1) || !app_graphs.has(g2))
    {
      throw std::logic_error("Source graph not found");
    }
    Graph merged;
    Graph &src1 = app_graphs.get(g1);
    Graph &src2 = app_graphs.get(g2);

    auto v1 = src1.get_vertices();
    for (size_t i = 0; i < v1.getSize(); ++i)
    {
      merged.add_vertex(v1[i]);
    }
    auto v2 = src2.get_vertices();
    for (size_t i = 0; i < v2.getSize(); ++i)
    {
      merged.add_vertex(v2[i]);
    }
    auto copy_edges = [&](Graph &src)
    {
      auto &edges = src.get_edges();
      for (auto it = edges.begin(); it != edges.end(); ++it)
      {
        for (size_t i = 0; i < (*it).v.getSize(); ++i)
        {
          merged.bind((*it).k.first, (*it).k.second, (*it).v[i]);
        }
      }
    };
    copy_edges(src1);
    copy_edges(src2);

    app_graphs.add(gnew, merged);
  }

  void cmd_extract(std::istream &in, std::ostream&)
  {
    std::string gnew, gold;
    int k;
    in >> gnew >> gold >> k;
    if (app_graphs.has(gnew))
    {
      throw std::logic_error("New graph already exists");
    }
    if (!app_graphs.has(gold))
    {
      throw std::logic_error("Old graph not found");
    }
    Vector<std::string> target_verts;
    Graph &src = app_graphs.get(gold);

    for (int i = 0; i < k; ++i)
    {
      std::string v;
      in >> v;
      if (!src.has_vertex(v))
      {
        throw std::logic_error("Vertex not in old graph");
      }
      target_verts.pushBack(v);
    }

    Graph extracted;
    for (size_t i = 0; i < target_verts.getSize(); ++i)
    {
      extracted.add_vertex(target_verts[i]);
    }

    auto &edges = src.get_edges();
    for (size_t i = 0; i < target_verts.getSize(); ++i)
    {
      for (size_t j = 0; j < target_verts.getSize(); ++j)
      {
        std::pair<std::string, std::string> key{target_verts[i], target_verts[j]};
        if (edges.has(key))
        {
          auto w = edges.get(key);
          for (size_t wi = 0; wi < w.getSize(); ++wi)
          {
            extracted.bind(key.first, key.second, w[wi]);
          }
        }
      }
    }
    app_graphs.add(gnew, extracted);
  }

  void load_file(const std::string &filename)
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::runtime_error("Could not open file");
    }
    std::string line;
    while (std::getline(file, line))
    {
      if (line.empty())
      {
        continue;
      }
      std::istringstream iss(line);
      std::string gname;
      int edges_count;
      if (!(iss >> gname >> edges_count))
      {
        continue;
      }

      Graph g;
      for (int i = 0; i < edges_count; ++i)
      {
        std::string u, v;
        size_t w;
        while (std::getline(file, line))
        {
          if (!line.empty())
          {
            break;
          }
        }
        std::istringstream edge_iss(line);
        if (edge_iss >> u >> v >> w)
        {
          g.bind(u, v, w);
        }
      }
      app_graphs.add(gname, g);
    }
  }

}
