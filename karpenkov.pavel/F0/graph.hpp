#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "../S2/queue.hpp"
#include "../common/list.hpp"
#include "cuckooHashTable.hpp"
#include <iostream>
namespace karpenkov {
enum Color { WHITE, GRAY, BLACK };
struct StringHash1 {
  size_t operator()(const std::string &s) const {
    return std::hash<std::string>{}(s);
  }
};
struct StringHash2 {
  size_t operator()(const std::string &str) const {
    size_t hash = 0;
    for (char c : str) {
      hash = hash * 37 + c;
    }
    return hash;
  }
};

struct StringEqual {
  bool operator()(const std::string &a, const std::string &b) const {
    return a == b;
  }
};
using AdjTable = CuckooHashTable<std::string, karpenkov::List<std::string>,
                                 StringHash1, StringHash2, StringEqual>;
class Graph {
public:
  Graph()
      : adj_(StringHash1{}, StringHash2{}, StringEqual{}),
        reverse_(StringHash1{}, StringHash2{}, StringEqual{}) {}
  void insertMovie(const std::string &A, const std::string &B) {
    if (!adj_.has(B)) {
      adj_.add(B, karpenkov::List<std::string>{});
    }
    if (!adj_.has(A)) {
      adj_.add(A, karpenkov::List<std::string>{});
    }
    if (adj_.at(A).contains(B)) {
      std::cerr << "edge already exist";
      return;
    }
    adj_.at(A).push_back(B);
    if (!reverse_.has(A)) {
      reverse_.add(A, karpenkov::List<std::string>{});
    }
    if (!reverse_.has(B)) {
      reverse_.add(B, karpenkov::List<std::string>{});
    }
    reverse_.at(B).push_back(A);
  }
  void showOrder(std::ostream &out) {
    CuckooHashTable<std::string, size_t, StringHash1, StringHash2, StringEqual>
        indegree(StringHash1{}, StringHash2{}, StringEqual{});
    using itHash = CuckooIterator<std::string, karpenkov::List<std::string>,
                                  StringHash1, StringHash2, StringEqual>;
    for (itHash it = adj_.begin(); it != adj_.end(); ++it) {
      indegree.add((*it).key, 0);
    }
    for (itHash a = adj_.begin(); a != adj_.end(); ++a) {
      karpenkov::List<std::string> l = (*a).value;
      for (LCIter<std::string> b = l.cbegin(); b != l.cend(); ++b) {
        indegree.at(*b)++;
      }
    }
    karpenkov::Queue<std::string> q;
    for (itHash it = adj_.begin(); it != adj_.end(); ++it) {
      if (indegree.at((*it).key) == 0) {
        q.push((*it).key);
      }
    }
    karpenkov::List<std::string> result;
    while (!q.empty()) {
      std::string v = q.front();
      q.pop();
      result.push_back(v);
      karpenkov::List<std::string> &neighbors = adj_.at(v);
      for (LCIter<std::string> it = neighbors.cbegin(); it != neighbors.cend();
           ++it) {
        std::string to = *it;
        indegree.at(to)--;
        if (indegree.at(to) == 0) {
          q.push(to);
        }
      }
    }
    size_t count = 0;
    for (LCIter<std::string> it = result.cbegin(); it != result.cend(); ++it) {
      ++count;
    }
    if (count != adj_.size()) {
      out << "Error: cycle detected\n";
      return;
    }
    for (LCIter<std::string> it = result.cbegin(); it != result.cend(); ++it) {
      out << *it;
      LCIter<std::string> next = it;
      ++next;
      if (next != result.cend()) {
        out << " -> ";
      }
    }
    out << "\n";
  }
  void showDependencies(const std::string &movie, std::ostream &out) {
    if (!adj_.has(movie)) {
      std::cerr << "error: movie not found\n";
      return;
    }
    CuckooHashTable<std::string, bool, StringHash1, StringHash2, StringEqual>
        visit{StringHash1{}, StringHash2{}, StringEqual{}};
    karpenkov::List<std::string> result;
    dfs(movie, adj_, visit, result);
    for (karpenkov::LCIter<std::string> i = result.cbegin(); i != result.cend();
         ++i) {
      out << (*i) << ' ';
    }
  }
  void showSuccessors(const std::string &movie, std::ostream &out) {
    if (!reverse_.has(movie)) {
      std::cerr << "error: movie not found\n";
      return;
    }
    CuckooHashTable<std::string, bool, StringHash1, StringHash2, StringEqual>
        visit{StringHash1{}, StringHash2{}, StringEqual{}};
    karpenkov::List<std::string> result;
    dfs(movie, reverse_, visit, result);
    for (karpenkov::LCIter<std::string> i = result.cbegin(); i != result.cend();
         ++i) {
      out << (*i) << ' ';
    }
  }
  void findCycles(std::ostream &out) {
    CuckooHashTable<std::string, Color, StringHash1, StringHash2, StringEqual>
        colors{StringHash1{}, StringHash2{}, StringEqual{}};
    CuckooHashTable<std::string, std::string, StringHash1, StringHash2,
                    StringEqual>
        parent{StringHash1{}, StringHash2{}, StringEqual{}};
    karpenkov::List<std::string> cycle;
    using It = CuckooIterator<std::string, karpenkov::List<std::string>,
                              StringHash1, StringHash2, StringEqual>;
    for (It it = adj_.begin(); it != adj_.end(); ++it) {
      colors.add((*it).key, WHITE);
    }
    for (It it = adj_.begin(); it != adj_.end(); ++it) {
      const std::string &movie = (*it).key;
      if (colors.at(movie) == WHITE) {
        if (dfsCycle(movie, colors, parent, cycle)) {
          for (karpenkov::LCIter<std::string> c = cycle.cbegin();
               c != cycle.cend(); ++c) {
            out << *c;
            karpenkov::LCIter<std::string> next = c;
            ++next;
            if (next != cycle.cend()) {
              out << " -> ";
            }
          }
          out << '\n';
          return;
        }
      }
    }
    out << "No cycles found\n";
  }

private:
  CuckooHashTable<std::string, karpenkov::List<std::string>, StringHash1,
                  StringHash2, StringEqual>
      adj_;
  CuckooHashTable<std::string, karpenkov::List<std::string>, StringHash1,
                  StringHash2, StringEqual>
      reverse_;
  void dfs(const std::string &movie, AdjTable &graph,
           CuckooHashTable<std::string, bool, StringHash1, StringHash2,
                           StringEqual> &visited,
           karpenkov::List<std::string> &result) {
    if (!visited.has(movie)) {
      visited.add(movie, false);
    }
    if (visited.at(movie)) {
      return;
    }
    visited.at(movie) = true;
    karpenkov::List<std::string> &neighbors = graph.at(movie);
    for (karpenkov::LCIter<std::string> it = neighbors.cbegin();
         it != neighbors.cend(); ++it) {
      std::string next = *it;
      if (!visited.has(next)) {
        visited.add(next, false);
      }
      if (!visited.at(next)) {
        result.push_back(next);
        dfs(next, graph, visited, result);
      }
    }
  }
  bool dfsCycle(const std::string &movie,
                CuckooHashTable<std::string, Color, StringHash1, StringHash2,
                                StringEqual> &colors,
                CuckooHashTable<std::string, std::string, StringHash1,
                                StringHash2, StringEqual> &parent,
                karpenkov::List<std::string> &cycle) {
    colors.at(movie) = GRAY;
    karpenkov::List<std::string> &neighbors = adj_.at(movie);
    for (karpenkov::LCIter<std::string> i = neighbors.cbegin();
         i != neighbors.cend(); ++i) {
      std::string next = *i;
      if (colors.at(next) == WHITE) {
        parent.add(next, movie);
        if (dfsCycle(next, colors, parent, cycle))
          return true;
      } else if (colors.at(next) == GRAY) {
        cycle.push_front(next);
        std::string cur = movie;
        while (cur != next) {
          cycle.push_front(cur);
          cur = parent.at(cur);
        }
        cycle.push_front(next);
        return true;
      }
    }
    colors.at(movie) = BLACK;
    return false;
  }
};
} // namespace karpenkov
#endif
