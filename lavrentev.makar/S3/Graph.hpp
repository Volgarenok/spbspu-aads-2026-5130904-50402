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
        List< size_t >,
        Siphash< std::string >,
        std::equal_to<std::pair<std::string, std::string>>
      > gr;
      List<std::string> vrtxs;
      bool hasVertex(std::string v) const;
  };

  void graphs(List<std::pair<std::string, Graph>> &grs);
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
        List< size_t >,
        Siphash< std::string >,
        std::equal_to<std::pair<std::string, std::string>>>
      vrtxIt((*it).second.gr.ht_, (*it).second.gr.ht_ + (*it).second.gr.bucket_count(), (*it).second.gr.ht_[0].cbegin());

      HashCIter<std::pair<std::string, std::string>,
        List< size_t >,
        Siphash< std::string >,
        std::equal_to<std::pair<std::string, std::string>>>
      vrtxItEnd((*it).second.gr.ht_ + (*it).second.gr.bucket_count(), (*it).second.gr.ht_ + (*it).second.gr.bucket_count(), (*it).second.gr.ht_[0].cbegin());

      while(vrtxIt != vrtxItEnd)
      {
        if ((*vrtxIt).key.first == v)
        {
          vertexExists = true;

          std::cout << (*vrtxIt).key.second << " ";
          LCIter<size_t> weightIter = (*vrtxIt).value.cbegin();
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
        List< size_t >,
        Siphash< std::string >,
        std::equal_to<std::pair<std::string, std::string>>>
      vrtxIt((*it).second.gr.ht_,
             (*it).second.gr.ht_ + (*it).second.gr.bucket_count(),
             (*it).second.gr.ht_[0].cbegin());

      HashCIter<std::pair<std::string, std::string>,
        List< size_t >,
        Siphash< std::string >,
        std::equal_to<std::pair<std::string, std::string>>>
      vrtxItEnd((*it).second.gr.ht_ + (*it).second.gr.bucket_count(),
                (*it).second.gr.ht_ + (*it).second.gr.bucket_count(),
                (*it).second.gr.ht_[0].cbegin());

      while(vrtxIt != vrtxItEnd)
      {
        if ((*vrtxIt).key.second == v)
        {
          std::cout << (*vrtxIt).key.first << " ";
          LCIter<size_t> weightIter = (*vrtxIt).value.cbegin();
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

inline void lavrentev::Graph::bind(List<std::pair<std::string, Graph>> &grs){
  std::string name, vrtx1, vrtx2;
  int weight;
  std::cin >> name >> vrtx1 >> vrtx2 >> weight;
}

inline void lavrentev::Graph::cut(
  List<std::pair<std::string, Graph>>&)
{
}

inline void lavrentev::Graph::create(
  List<std::pair<std::string, Graph>>&)
{
}

inline void lavrentev::Graph::merge(
  List<std::pair<std::string, Graph>>&)
{
}

inline void lavrentev::Graph::extract(
  List<std::pair<std::string, Graph>>&)
{
}

#endif
