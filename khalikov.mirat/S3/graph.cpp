#include "graph.hpp"
#include "../S1/list.hpp"
#include <stdexcept>
#include <string>
#include <utility>

namespace khalikov
{

  void swap(Graph &gr1, Graph &gr2) noexcept
  {
    gr1.connections.swap(gr2.connections);
    gr1.vertexes.swap(gr2.vertexes);
    gr1.pairs.swap(gr2.pairs);
  }

  graphTable parse(std::istream &in)
  {
    graphTable gt;
    std::string name;
    while (in >> name) {
      size_t count = 0;
      if (!(in >> count)) {
        break;
      }
      Graph graph;
      for (size_t i = 0; i < count; ++i) {
        std::string v1, v2;
        size_t weight = 0;
        in >> v1 >> v2 >> weight;
        pairOfVertexes edge{v1, v2};
        graph.connections[edge].pushBack(weight);
        graph.pairs.pushBack(edge);
        graph.vertexes.pushBack(v1);
        graph.vertexes.pushBack(v2);
      }
      graph.pairs.uniqueSort();
      graph.vertexes.uniqueSort();
      gt.insert(name, std::move(graph));
    }

    return gt;
  }

  template< class T >
  bool contains(const List< T > &list, const T &value)
  {
    if (list.isEmpty()) {
      return false;
    }
    auto it = list.cbegin();
    auto start = it;
    do {
      if (*it == value)
        return true;
      ++it;
    } while (it != start);
    return false;
  }

  void graphs(std::ostream &out, std::istream &, graphTable &table)
  {
    if (table.isEmpty()) {
      out << '\n';
      return;
    }
    List< std::string > names;
    for (auto it = table.begin(); it != table.end(); ++it) {
      names.pushBack(it->key);
    }
    if (names.isEmpty()) {
      return;
    }
    names.sort();
    auto it = names.cbegin();
    auto start = it;
    do {
      out << *it << '\n';
      ++it;
    } while (it != start);
  }

  void vertexes(std::ostream &out, std::istream &in, graphTable &table)
  {
    std::string graph;
    in >> graph;
    auto it = table.find(graph);
    if (it == table.end()) {
      throw std::runtime_error("not found");
    }
    List< std::string > vxs = it->value.vertexes;
    if (vxs.isEmpty()) {
      out << '\n';
      return;
    }
    auto vit = vxs.cbegin();
    auto vstart = vit;
    do {
      out << *vit << '\n';
      ++vit;
    } while (vit != vstart);
  }

  void schWeights(std::ostream &out, const Graph &graph, const pairOfVertexes &key)
  {
    auto it = graph.connections.find(key);
    List< size_t > weights = it->value;
    weights.sort();
    auto yait = weights.cbegin();
    auto start = yait;
    do {
      out << " " << *yait;
      ++yait;
    } while (yait != start);
  }

  void outbound(std::ostream &out, std::istream &in, graphTable &table)
  {
    std::string graph, vertex;
    in >> graph >> vertex;
    auto git = table.find(graph);
    if (git == table.end() || !contains(git->value.vertexes, vertex)) {
      throw std::runtime_error("Not found");
    }
    List< std::string > targets;
    if (!git->value.pairs.isEmpty()) {
      auto pit = git->value.pairs.cbegin();
      auto pstart = pit;
      do {
        if (pit->first == vertex) {
          if (!contains(targets, pit->second)) {
            targets.pushBack(pit->second);
          }
        }
        ++pit;
      } while (pit != pstart);
      targets.sort();
      if (targets.isEmpty()) {
        out << '\n';
        return;
      }
      auto tit = targets.cbegin();
      auto tstart = tit;
      do {
        out << *tit;
        schWeights(out, git->value, {vertex, *tit});
        out << '\n';
        ++tit;
      } while (tit != tstart);
    }
  }

  void inbound(std::ostream &out, std::istream &in, graphTable &table)
  {
    std::string graph, vertex;
    in >> graph >> vertex;
    auto git = table.find(graph);
    if (git == table.end() || !contains(git->value.vertexes, vertex)) {
      throw std::runtime_error("Not found");
    }
    List< std::string > sources;
    if (!git->value.pairs.isEmpty()) {
      auto pit = git->value.pairs.cbegin();
      auto pstart = pit;
      do {
        if (pit->second == vertex) {
          if (!contains(sources, pit->first)) {
            sources.pushBack(pit->first);
          }
        }
        ++pit;
      } while (pit != pstart);
      sources.sort();
      if (sources.isEmpty()) {
        out << '\n';
        return;
      }
      auto sit = sources.cbegin();
      auto sstart = sit;
      do {
        out << *sit;
        schWeights(out, git->value, {*sit, vertex});
        out << '\n';
        ++sit;
      } while (sit != sstart);
    }
  }

  void bind(std::ostream &, std::istream &in, graphTable &table)
  {
    std::string graph, v1, v2;
    size_t weight;
    in >> graph >> v1 >> v2 >> weight;
    auto git = table.find(graph);
    if (git == table.end()) {
      throw std::runtime_error("Invalid input");
    }
    if (!contains(git->value.vertexes, v1)) {
      git->value.vertexes.pushBack(v1);
    }
    if (!contains(git->value.vertexes, v2)) {
      git->value.vertexes.pushBack(v2);
    }
    pairOfVertexes key = {v1, v2};
    if (git->value.connections.has(key)) {
      git->value.connections.find(key)->value.pushBack(weight);
    } else {
      List< size_t > weights;
      weights.pushBack(weight);
      git->value.connections.insert(key, weights);
      git->value.pairs.pushBack(key);
    }
  }

  void cut(std::ostream &, std::istream &in, graphTable &table)
  {
    std::string graph, v1, v2;
    size_t weight;
    if (!(in >> graph >> v1 >> v2 >> weight)) {
      in.clear();
      throw std::runtime_error("Invalid input");
    }
    auto git = table.find(graph);
    pairOfVertexes key = {v1, v2};
    if (git == table.end() || !git->value.connections.has(key)) {
      throw std::runtime_error("Invalid input");
    }
    List< size_t > &weights = git->value.connections.find(key)->value;
    if (!contains(weights, weight)) {
      throw std::runtime_error("Invalid input");
    }
    weights.remove(weight);
    if (weights.isEmpty()) {
      git->value.connections.remove(key);
      git->value.pairs.remove(key);
    }
  }

  void create(std::ostream &, std::istream &in, graphTable &table)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::runtime_error("Invalid input");
    }
    while (in.peek() == ' ' || in.peek() == '\t') {
      in.get();
    }
    if (in.peek() == '\n' || in.peek() == EOF || !std::isdigit(in.peek())) {
      throw std::runtime_error("Invalid input");
    }
    size_t count = 0;
    if (!(in >> count)) {
      in.clear();
      throw std::runtime_error("Invalid input");
    }
    Graph graph;
    for (size_t i = 0; i < count; ++i) {
      std::string vertex;
      if (!(in >> vertex)) {
        in.clear();
        throw std::runtime_error("Invalid input");
      }
      if (!contains(graph.vertexes, vertex)) {
        graph.vertexes.pushBack(vertex);
      }
    }
    if (table.has(name)) {
      throw std::runtime_error("Already exist");
    }
    table.insert(name, std::move(graph));
  }

  void merge(std::ostream &, std::istream &in, graphTable &table)
  {
    std::string res, name1, name2;
    if (!(in >> res >> name1 >> name2)) {
      in.clear();
      throw std::runtime_error("Invalid input");
    }
    if (table.has(res) || !table.has(name1) || !table.has(name2)) {
      throw std::runtime_error("Invalid input");
    }
    Graph merged;
    Graph g1 = table.find(name1)->value;
    Graph g2 = table.find(name2)->value;
    if (!g1.vertexes.isEmpty()) {
      auto it = g1.vertexes.cbegin();
      auto start = it;
      do {
        if (!contains(merged.vertexes, *it)) {
          merged.vertexes.pushBack(*it);
        }
        ++it;
      } while (it != start);
    }
    if (!g2.vertexes.isEmpty()) {
      auto it = g2.vertexes.cbegin();
      auto start = it;
      do {
        if (!contains(merged.vertexes, *it)) {
          merged.vertexes.pushBack(*it);
        }
        ++it;
      } while (it != start);
    }
    if (!g1.pairs.isEmpty()) {
      auto it = g1.pairs.cbegin();
      auto start = it;
      do {
        pairOfVertexes key = *it;
        List< size_t > w1 = g1.connections.find(key)->value;
        merged.connections.insert(key, w1);
        merged.pairs.pushBack(key);
        ++it;
      } while (it != start);
    }
    if (!g2.pairs.isEmpty()) {
      auto it = g2.pairs.cbegin();
      auto start = it;
      do {
        pairOfVertexes key = *it;
        List< size_t > w2 = g2.connections.find(key)->value;
        if (merged.connections.has(key)) {
          List< size_t > &temp = merged.connections.find(key)->value;
          auto yait = w2.cbegin();
          auto yast = yait;
          do {
            temp.pushBack(*yait);
            ++yait;
          } while (yait != yast);
        } else {
          merged.connections.insert(key, w2);
          merged.pairs.pushBack(key);
        }
        ++it;
      } while (it != start);
    }
    table.insert(res, merged);
  }

  void extract(std::ostream &, std::istream &in, graphTable &table)
  {
    std::string res, name;
    size_t count;
    if (!(in >> res >> name >> count)) {
      in.clear();
      throw std::runtime_error("Invalid input");
    }
    if (table.has(res) || !table.has(name)) {
      throw std::runtime_error("Invalid input");
    }
    Graph extracted;
    Graph graph = table.find(name)->value;
    for (size_t i = 0; i < count; ++i) {
      std::string v;
      in >> v;
      if (contains(graph.vertexes, v) && !contains(extracted.vertexes, v)) {
        extracted.vertexes.pushBack(v);
      }
    }
    if (!graph.pairs.isEmpty()) {
      auto it = graph.pairs.cbegin();
      auto start = it;
      do {
        pairOfVertexes key = *it;
        if (contains(extracted.vertexes, key.first) && contains(extracted.vertexes, key.second)) {
          List< size_t > weights = graph.connections.find(key)->value;
          extracted.connections.insert(key, weights);
          extracted.pairs.pushBack(key);
        }
        ++it;
      } while (it != start);
    }
    table.insert(res, extracted);
  }
}
