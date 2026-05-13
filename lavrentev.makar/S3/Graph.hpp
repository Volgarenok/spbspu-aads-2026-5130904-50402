#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "HashTable.hpp"
#include "../common/List.hpp"

namespace lavrentev{
  struct Graph;
}

using cmd_t = void (*)(
  lavrentev::List<std::pair<std::string, lavrentev::Graph>> &);

namespace lavrentev{
  struct Graph {
    friend void bindWithArg(std::string name,
      std::string v1,
      std::string v2,
      int weight,
      List<std::pair<std::string, Graph>> &grs
    );

    public:
      static void vertexes(List<std::pair<std::string, Graph>> &grs);
      static void outbound(List<std::pair<std::string, Graph>> &grs);
      static void inbound(List<std::pair<std::string, Graph>> &grs);
      static void bind(List<std::pair<std::string, Graph>> &grs);
      static void cut(List<std::pair<std::string, Graph>> &grs);
      static void create(List<std::pair<std::string, Graph>> &grs);
      static void merge(List<std::pair<std::string, Graph>> &grs);
      static void extract(List<std::pair<std::string, Graph>> &grs);
    private:
      HashTable< std::pair<std::string, std::string>,
        List< int >,
        Siphash< std::pair<std::string, std::string> >,
        std::equal_to<std::pair<std::string, std::string>>
      > gr;
      List<std::string> vrtxs;
      bool hasVertex(std::string v) const;
      void insertToVrtxs(std::string v);
  };

  void graphs(List<std::pair<std::string, Graph>> &grs);
  void createWithArg(std::string gr, List<std::pair<std::string, Graph>> &grs);
  void bindWithArg(std::string name,
    std::string v1,
    std::string v2,
    int weight,
    List<std::pair<std::string, Graph>> &grs
  );
}

inline void lavrentev::graphs(List<std::pair<std::string, Graph>> &grs)
{
  LCIter<std::pair<std::string, Graph>> it;
  for (it = grs.cbegin(); it != grs.cend(); ++it)
  {
    std::cout << (*it).first << "\n";
  }
}

inline void lavrentev::Graph::vertexes(List<std::pair<std::string, Graph>> &grs)
{
  std::string name;
  std::cin >> name;

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
  std::cerr << "<INVALID COMMAND>" << "\n";
}

inline void lavrentev::Graph::outbound(List<std::pair<std::string, Graph>> &grs)
{
  std::string name, v;
  std::cin >> name >> v;

  bool graphFound = false;
  bool vertexExists = false;

  LCIter<std::pair<std::string, Graph>> it;
  for (it = grs.cbegin(); it != grs.cend(); ++it)
  {
    if ((*it).first == name)
    {
      graphFound = true;

      HashCIter<std::pair<std::string, std::string>,
        List< int >,
        Siphash<std::pair<std::string, std::string>>,
        std::equal_to<std::pair<std::string, std::string>>>
      vrtxIt((*it).second.gr.ht_, (*it).second.gr.ht_ + (*it).second.gr.bucket_count(), (*it).second.gr.ht_[0].cbegin());

      HashCIter<std::pair<std::string, std::string>,
        List< int >,
        Siphash<std::pair<std::string, std::string>>,
        std::equal_to<std::pair<std::string, std::string>>>
      vrtxItEnd((*it).second.gr.ht_ + (*it).second.gr.bucket_count(), (*it).second.gr.ht_ + (*it).second.gr.bucket_count(), (*it).second.gr.ht_[0].cbegin());

      while(vrtxIt != vrtxItEnd)
      {
        if ((*vrtxIt).key.first == v)
        {
          vertexExists = true;

          std::cout << (*vrtxIt).key.second << " ";
          LCIter< int > weightIter = (*vrtxIt).value.cbegin();
          std::cout << (*weightIter);
          ++weightIter;
          while(weightIter != (*vrtxIt).value.cend())
          {
            std::cout << " " << (*weightIter);
            ++weightIter;
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
    std::cerr << "<INVALID COMMAND>" << "\n";
  }
}

inline void lavrentev::Graph::inbound(List<std::pair<std::string, Graph>> &grs)
{
  std::string name, v;
  std::cin >> name >> v;

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

      HashCIter<std::pair<std::string, std::string>,
        List< int >,
        Siphash<std::pair<std::string, std::string>>,
        std::equal_to<std::pair<std::string, std::string>>>
      vrtxIt((*it).second.gr.ht_,
             (*it).second.gr.ht_ + (*it).second.gr.bucket_count(),
             (*it).second.gr.ht_[0].cbegin());

      HashCIter<std::pair<std::string, std::string>,
        List< int >,
        Siphash<std::pair<std::string, std::string>>,
        std::equal_to<std::pair<std::string, std::string>>>
      vrtxItEnd((*it).second.gr.ht_ + (*it).second.gr.bucket_count(),
                (*it).second.gr.ht_ + (*it).second.gr.bucket_count(),
                (*it).second.gr.ht_[0].cbegin());

      while(vrtxIt != vrtxItEnd)
      {
        if ((*vrtxIt).key.second == v)
        {
          std::cout << (*vrtxIt).key.first << " ";
          LCIter< int > weightIter = (*vrtxIt).value.cbegin();
          if (weightIter != (*vrtxIt).value.cend())
          {
            std::cout << (*weightIter);
            ++weightIter;
            while(weightIter != (*vrtxIt).value.cend())
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
    std::cerr << "<INVALID COMMAND>" << "\n";
  }
}

inline bool lavrentev::Graph::hasVertex(std::string v) const
{
  LCIter<std::string> it;
  for(it = vrtxs.cbegin(); it != vrtxs.cend(); ++it)
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

inline void lavrentev::Graph::bind(List<std::pair<std::string, Graph>> &grs){
  std::string name, v1, v2;
  int weight;
  std::cin >> name >> v1 >> v2 >> weight;
  bindWithArg(name, v1, v2, weight, grs);
}

inline void lavrentev::bindWithArg(
  std::string name,
  std::string v1,
  std::string v2,
  int weight,
  List<std::pair<std::string, Graph>> &grs)
{
  LIter<std::pair<std::string, Graph>> it;
  for(it = grs.begin(); it != grs.end(); ++it)
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
      g.gr[{v1, v2}].pushFront(weight);
      return;
    }
  }
  std::cerr << "<INVALID COMMAND>" << "\n";
}

inline void lavrentev::Graph::cut(List<std::pair<std::string, Graph>> &grs)
{
  std::string name, v1, v2;
  int weight;
  std::cin >> name >> v1 >> v2 >> weight;

  LIter<std::pair<std::string, Graph>> it;
  for(it = grs.begin(); it != grs.end(); ++it)
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

      List< int > &weights = g.gr[keyToFind];
      LIter< int > wIt = weights.begin();

      for(; wIt != weights.end(); ++wIt)
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
  std::cerr << "<INVALID COMMAND>" << "\n";
}

inline void lavrentev::Graph::create(List<std::pair<std::string, Graph>> &grs)
{
  std::string name;
  std::cin >> name;
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
  for(; it != grs.end(); ++it)
  {
    if((*it).first == name)
    {
      std::cerr << "<INVALID COMMAND>" << "\n";
      return;
    } else if ((*it).first > name)
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

inline void lavrentev::Graph::merge(List<std::pair<std::string, Graph>>& grs)
{
  std::string newGrName, gr1Name, gr2Name;
  std::cin >> newGrName >> gr1Name >> gr2Name;

  Graph* g1 = nullptr;
  Graph* g2 = nullptr;

  for (auto it = grs.begin(); it != grs.end(); ++it)
  {
    if ((*it).first == newGrName)
    {
      std::cerr << "<INVALID COMMAND>\n";
      return;
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
    std::cerr << "<INVALID COMMAND>\n";
    return;
  }

  createWithArg(newGrName, grs);

  Graph* newGr = nullptr;
  for (auto it = grs.begin(); it != grs.end(); ++it)
  {
    if ((*it).first == newGrName)
    {
      newGr = &(*it).second;
      break;
    }
  }

  for (auto vIt = g1->vrtxs.cbegin(); vIt != g1->vrtxs.cend(); ++vIt)
  {
    newGr->insertToVrtxs(*vIt);
  }
  for (auto vIt = g2->vrtxs.cbegin(); vIt != g2->vrtxs.cend(); ++vIt)
  {
    newGr->insertToVrtxs(*vIt);
  }

  for (size_t i = 0; i < g1->gr.slots_; ++i)
  {
    for (auto eIt = g1->gr.ht_[i].begin(); eIt != g1->gr.ht_[i].end(); ++eIt)
    {
      auto& key = (*eIt).key;
      auto& weights = (*eIt).value;

      for (auto wIt = weights.cbegin(); wIt != weights.cend(); ++wIt)
      {
        bindWithArg(newGrName, key.first, key.second, *wIt, grs);
      }
    }
  }

  for (size_t i = 0; i < g2->gr.slots_; ++i)
  {
    for (auto eIt = g2->gr.ht_[i].begin(); eIt != g2->gr.ht_[i].end(); ++eIt)
    {
      auto& key = (*eIt).key;
      auto& weights = (*eIt).value;

      for (auto wIt = weights.cbegin(); wIt != weights.cend(); ++wIt)
      {
        bindWithArg(newGrName, key.first, key.second, *wIt, grs);
      }
    }
  }
}

inline void lavrentev::Graph::extract(
  List<std::pair<std::string, Graph>> &grs)
{
  std::string newGr, oldGr;
  size_t amountVrtxs;
  List<std::string> vertexes;

  std::cin >> newGr >> oldGr >> amountVrtxs;
  for(size_t i = 0; i < amountVrtxs; ++i)
  {
    std::string v;
    std::cin >> v;
    vertexes.pushFront(v);
  }
}

#endif
