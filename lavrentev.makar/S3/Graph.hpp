#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "HashTable.hpp"
#include <List.hpp>

namespace lavrentev
{
  struct Graph;
}

using cmd_t = void (*)(std::istream &in, lavrentev::List<std::pair<std::string, lavrentev::Graph>> &);

namespace lavrentev
{
  struct Graph
  {
    friend void bindWithArg(
      std::string name, std::string v1, std::string v2,
      size_t weight,
      List<std::pair<std::string, Graph>> &grs);
    friend void readfile(std::string name, List<std::pair<std::string, Graph>> &grs);

  public:
    static void vertexes(std::istream &in, List<std::pair<std::string, Graph>> &grs);
    static void outbound(std::istream &in, List<std::pair<std::string, Graph>> &grs);
    static void inbound(std::istream &in, List<std::pair<std::string, Graph>> &grs);
    static void bind(std::istream &in, List<std::pair<std::string, Graph>> &grs);
    static void cut(std::istream &in, List<std::pair<std::string, Graph>> &grs);
    static void create(std::istream &in, List<std::pair<std::string, Graph>> &grs);
    static void merge(std::istream &in, List<std::pair<std::string, Graph>> &grs);
    static void extract(std::istream &in, List<std::pair<std::string, Graph>> &grs);

  private:
    HashTable<std::pair<std::string, std::string>, List<size_t>,
              Siphash<std::pair<std::string, std::string>>,
              std::equal_to<std::pair<std::string, std::string>>>
      gr;
    List<std::string> vrtxs;
    bool hasVertex(std::string v) const;
    void insertToVrtxs(std::string v);
  };

  void graphs(std::istream &, List<std::pair<std::string, Graph>> &grs);
  void createWithArg(std::string gr, List<std::pair<std::string, Graph>> &grs);
  void bindWithArg(
    std::string name,
    std::string v1,
    std::string v2,
    size_t weight,
    List<std::pair<std::string, Graph>> &grs
  );
}

inline void lavrentev::graphs(std::istream &, List<std::pair<std::string, Graph>> &grs)
{
  LCIter<std::pair<std::string, Graph>> it;
  for (it = grs.cbegin(); it != grs.cend(); ++it)
  {
    std::cout << (*it).first << "\n";
  }
}

inline void lavrentev::Graph::vertexes(std::istream &in, List<std::pair<std::string, Graph>> &grs)
{
  std::string name;
  in >> name;

  LCIter<std::pair<std::string, Graph>> it;
  for (it = grs.cbegin(); it != grs.cend(); ++it)
  {
    if ((*it).first == name)
    {
      LCIter<std::string> vrtxIt;
      for (vrtxIt = (*it).second.vrtxs.cbegin(); vrtxIt != (*it).second.vrtxs.cend(); ++vrtxIt)
      {
        std::cout << (*vrtxIt) << "\n";
      }
      return;
    }
  }
  throw;
}

inline void lavrentev::Graph::outbound(std::istream &in, List<std::pair<std::string, Graph>> &grs)
{
  std::string name, v;
  in >> name >> v;

  LIter<std::pair<std::string, Graph>> it;
  for (it = grs.begin(); it != grs.end(); ++it)
  {
    if ((*it).first == name)
    {
      Graph &g = (*it).second;

      if (!g.hasVertex(v))
      {
        throw;
      }
      bool hasEdges = false;
      LCIter<std::string> vrtxIt;
      for (vrtxIt = g.vrtxs.cbegin(); vrtxIt != g.vrtxs.cend(); ++vrtxIt)
      {
        std::pair<std::string, std::string> key(v, (*vrtxIt));

        if (g.gr.has(key))
        {
          hasEdges = true;
          std::cout << (*vrtxIt) << " ";
          List<size_t> &weights = g.gr[key];
          LCIter<size_t> wIt = weights.cbegin();
          if (wIt != weights.cend())
          {
            std::cout << (*wIt);
            ++wIt;

            while (wIt != weights.cend())
            {
              std::cout << " " << (*wIt);
              ++wIt;
            }
          }
          std::cout << "\n";
        }
      }
      if (!hasEdges)
      {
        throw;
      }
      return;
    }
  }
  throw;
}

inline void lavrentev::Graph::inbound(std::istream &in, List<std::pair<std::string, Graph>> &grs)
{
  std::string name, v;
  in >> name >> v;

  bool graphFound = false;
  bool vertexExists = false;
  LCIter<std::pair<std::string, Graph>> it;
  for (it = grs.cbegin(); it != grs.cend(); ++it)
  {
    if ((*it).first == name)
    {
      graphFound = true;
      if (!(*it).second.hasVertex(v))
      {
        break;
      }
      vertexExists = true;

      HashCIter<std::pair<std::string, std::string>, List<size_t>,
                Siphash<std::pair<std::string, std::string>>,
                std::equal_to<std::pair<std::string, std::string>>>
      vrtxIt(
        (*it).second.gr.ht_,
        (*it).second.gr.ht_ + (*it).second.gr.bucket_count(),
        (*it).second.gr.ht_[0].cbegin()
      );

      HashCIter<std::pair<std::string, std::string>, List<size_t>,
                Siphash<std::pair<std::string, std::string>>,
                std::equal_to<std::pair<std::string, std::string>>>
      vrtxItEnd(
        (*it).second.gr.ht_ + (*it).second.gr.bucket_count(),
        (*it).second.gr.ht_ + (*it).second.gr.bucket_count(),
        LCIter<typename HashTable<
            std::pair<std::string, std::string>, List<size_t>,
            Siphash<std::pair<std::string, std::string>>,
            std::equal_to<std::pair<std::string, std::string>>>::Node>()
      );

      while (vrtxIt != vrtxItEnd)
      {
        if ((*vrtxIt).key.second == v)
        {
          std::cout << (*vrtxIt).key.first << " ";
          LCIter<size_t> weightIter = (*vrtxIt).value.cbegin();
          if (weightIter != (*vrtxIt).value.cend())
          {
            std::cout << (*weightIter);
            ++weightIter;
            while (weightIter != (*vrtxIt).value.cend())
            {
              std::cout << " " << (*weightIter);
              ++weightIter;
            }
          }
          std::cout << "\n";
        }
        ++vrtxIt;
      }
      break;
    }
  }
  if (!graphFound || !vertexExists)
  {
    throw;
  }
}

inline bool lavrentev::Graph::hasVertex(std::string v) const
{
  LCIter<std::string> it;
  for (it = vrtxs.cbegin(); it != vrtxs.cend(); ++it)
  {
    if ((*it) == v)
    {
      return true;
    }
  }
  return false;
}

inline void lavrentev::Graph::insertToVrtxs(std::string v)
{
  LIter<std::string> vrtxIt = vrtxs.begin();
  LIter<std::string> preVrtxIt = vrtxs.begin();

  for (; vrtxIt != vrtxs.end(); ++vrtxIt)
  {
    if (*vrtxIt == v)
    {
      return;
    }
    if (*vrtxIt > v)
    {
      if (vrtxIt == vrtxs.begin())
      {
        vrtxs.pushFront(v);
      }
      else
      {
        vrtxs.insert(preVrtxIt, v);
      }
      break;
    }
    preVrtxIt = vrtxIt;
  }

  if (vrtxIt == vrtxs.end())
  {
    if (vrtxs.begin() == vrtxs.end())
    {
      vrtxs.pushFront(v);
    }
    else
    {
      vrtxs.insert(preVrtxIt, v);
    }
  }
}

inline void lavrentev::Graph::bind(std::istream &in, List<std::pair<std::string, Graph>> &grs)
{
  std::string name, v1, v2;
  size_t weight;
  in >> name >> v1 >> v2 >> weight;
  bindWithArg(name, v1, v2, weight, grs);
}

inline void lavrentev::bindWithArg(
  std::string name,
  std::string v1,
  std::string v2,
  size_t weight,
  List<std::pair<std::string, Graph>> &grs)
{
  LIter<std::pair<std::string, Graph>> it;
  for (it = grs.begin(); it != grs.end(); ++it)
  {
    if ((*it).first == name)
    {
      Graph &g = (*it).second;
      if (!g.hasVertex(v1))
      {
        g.insertToVrtxs(v1);
      }

      if (!g.hasVertex(v2))
      {
        g.insertToVrtxs(v2);
      }

      List<size_t> &weights = g.gr[std::pair<std::string, std::string>(v1, v2)];

      if (weights.begin() == weights.end())
      {
        weights.pushFront(weight);
        return;
      }

      LIter<size_t> itW = weights.begin();

      if (weight < *itW)
      {
        weights.pushFront(weight);
        return;
      }

      LIter<size_t> prev = itW;
      ++itW;

      while (itW != weights.end())
      {
        if (weight < *itW)
        {
          weights.insert(prev, weight);
          return;
        }
        prev = itW;
        ++itW;
      }

      weights.insert(prev, weight);
      return;
    }
  }
  throw;
}

inline void lavrentev::Graph::cut(std::istream &in, List<std::pair<std::string, Graph>> &grs)
{
  std::string name, v1, v2;
  size_t weight;
  in >> name >> v1 >> v2 >> weight;

  LIter<std::pair<std::string, Graph>> it;
  for (it = grs.begin(); it != grs.end(); ++it)
  {
    if ((*it).first == name)
    {
      Graph &g = (*it).second;
      if (!g.hasVertex(v1) || !g.hasVertex(v2))
      {
        break;
      }

      std::pair<std::string, std::string> keyToFind = {v1, v2};

      if (!g.gr.has(keyToFind))
      {
        break;
      }

      List<size_t> &weights = g.gr[keyToFind];
      LIter<size_t> wIt = weights.begin();

      for (; wIt != weights.end(); ++wIt)
      {
        if (*wIt == weight)
        {
          weights.erase(wIt);
          if (weights.empty())
          {
            g.gr.drop(keyToFind);
          }
          return;
        }
      }
      break;
    }
  }
  throw;
}

inline void lavrentev::Graph::create(std::istream &in, List<std::pair<std::string, Graph>> &grs)
{
  std::string name;
  in >> name;
  createWithArg(name, grs);
}

inline void lavrentev::createWithArg(std::string name, List<std::pair<std::string, Graph>> &grs)
{
  if (grs.begin() == grs.end())
  {
    grs.pushFront({name, Graph{}});
    return;
  }
  LIter<std::pair<std::string, Graph>> it = grs.begin();
  LIter<std::pair<std::string, Graph>> preIt = grs.begin();
  for (; it != grs.end(); ++it)
  {
    if ((*it).first == name)
    {
      throw;
    }
    else if ((*it).first > name)
    {
      if (it == grs.begin())
      {
        grs.pushFront({name, Graph{}});
      }
      else
      {
        grs.insert(preIt, {name, Graph{}});
      }
      return;
    }
    preIt = it;
  }
  grs.insert(preIt, {name, Graph{}});
}

inline void lavrentev::Graph::merge(std::istream &in, List<std::pair<std::string, Graph>> &grs)
{
  std::string newGrName, gr1Name, gr2Name;
  in >> newGrName >> gr1Name >> gr2Name;

  Graph *g1 = nullptr;
  Graph *g2 = nullptr;

  for (LIter<std::pair<std::string, Graph>> it = grs.begin(); it != grs.end(); ++it)
  {
    if ((*it).first == newGrName)
    {
      throw;
    }
    if ((*it).first == gr1Name)
    {
      g1 = &(*it).second;
    }
    if ((*it).first == gr2Name)
    {
      g2 = &(*it).second;
    }
  }

  if (!g1 || !g2)
  {
    throw;
  }

  createWithArg(newGrName, grs);

  Graph *newGr = nullptr;
  for (LIter<std::pair<std::string, Graph>> it = grs.begin(); it != grs.end(); ++it)
  {
    if ((*it).first == newGrName)
    {
      newGr = &(*it).second;
      break;
    }
  }

  for (LCIter<std::string> vIt = g1->vrtxs.cbegin(); vIt != g1->vrtxs.cend(); ++vIt)
  {
    newGr->insertToVrtxs(*vIt);
  }
  for (LCIter<std::string> vIt = g2->vrtxs.cbegin(); vIt != g2->vrtxs.cend(); ++vIt)
  {
    newGr->insertToVrtxs(*vIt);
  }

  for (size_t i = 0; i < g1->gr.slots_; ++i)
  {
    for (auto eIt = g1->gr.ht_[i].begin(); eIt != g1->gr.ht_[i].end(); ++eIt)
    {
      std::pair<std::string, std::string> &key = (*eIt).key;
      List<size_t> &weights = (*eIt).value;

      for (LIter<size_t> wIt = weights.begin(); wIt != weights.end(); ++wIt)
      {
        bindWithArg(newGrName, key.first, key.second, *wIt, grs);
      }
    }
  }

  for (size_t i = 0; i < g2->gr.slots_; ++i)
  {
    for (auto eIt = g2->gr.ht_[i].begin(); eIt != g2->gr.ht_[i].end(); ++eIt)
    {
      std::pair<std::string, std::string> &key = (*eIt).key;
      List<size_t> &weights = (*eIt).value;

      for (LIter<size_t> wIt = weights.begin(); wIt != weights.end(); ++wIt)
      {
        bindWithArg(newGrName, key.first, key.second, *wIt, grs);
      }
    }
  }
}

inline void lavrentev::Graph::extract(std::istream &in, List<std::pair<std::string, Graph>> &grs)
{
  std::string newGrName, oldGrName;
  size_t amountVrtxs;
  List<std::string> vertexes;

  in >> newGrName >> oldGrName >> amountVrtxs;
  for (size_t i = 0; i < amountVrtxs; ++i)
  {
    std::string v;
    in >> v;
    vertexes.pushFront(v);
  }

  LIter<std::pair<std::string, Graph>> it = grs.begin();
  bool flag = false, flagNew = true;
  for(; it != grs.end(); ++it)
  {
    if ((*it).first == oldGrName)
    {
      LIter<std::string> needVrtxsIt = vertexes.begin();
      for(; needVrtxsIt != vertexes.end(); ++needVrtxsIt)
      {
        if (!(*it).second.hasVertex(*needVrtxsIt))
        {
          throw;
        }
      }
      flag = true;
      break;
    }
    if ((*it).first == newGrName)
    {
      flagNew = false;
    }
  }
  if (!flag || !flagNew)
  {
    throw;
  }

  createWithArg(newGrName, grs);

  Graph *newGr = nullptr;
  Graph *oldGr = nullptr;
  for (LIter<std::pair<std::string, Graph>> it = grs.begin(); it != grs.end(); ++it)
  {
    if ((*it).first == newGrName)
    {
      newGr = &(*it).second;
    }
    if ((*it).first == oldGrName)
    {
      oldGr = &(*it).second;
    }
  }

  LIter<std::string> needVrtxsIt = vertexes.begin();
  for (; needVrtxsIt != vertexes.end(); ++needVrtxsIt)
  {
    newGr->insertToVrtxs(*needVrtxsIt);
  }

  LIter<std::string> vrtxItFirst = vertexes.begin();
  LIter<std::string> vrtxItSecond = vertexes.begin();
  for (; vrtxItFirst != vertexes.end(); ++vrtxItFirst)
  {
    for(; vrtxItSecond != vertexes.end(); ++vrtxItSecond)
    {
      std::pair<std::string, std::string> key = {(*vrtxItFirst), (*vrtxItSecond)};
      if ((*oldGr).gr.has(key))
      {
        List<size_t> &w = (*oldGr).gr[key];
        LIter<size_t> wIt = w.begin();
        for(; wIt != w.end(); ++wIt)
        {
          bindWithArg(newGrName, *vrtxItFirst, *vrtxItSecond, *wIt, grs);
        }
      }
    }
    vrtxItSecond = vertexes.begin();
  }
}

#endif
