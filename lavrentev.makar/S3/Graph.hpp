#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "HashTable.hpp"
#include "List.hpp"
#include <stdexcept>

namespace lavrentev{
  struct Graph {
    public:
      void vertexes(List<std::pair<std::string, Graph>> grs, std::string name);
      void outbound(List<std::pair<std::string, Graph>> grs, std::string grh, std::string v);
    private:
      HashTable< std::pair<std::string, std::string>,
        List< size_t >,
        Siphash< std::string >,
        std::equal_to<std::pair<std::string, std::string>>
      > gr;
      List<std::string> vrtxs;
  };

  void graphs(List<std::pair<std::string, Graph>> grs);
}

inline void lavrentev::graphs(List<std::pair<std::string, Graph>> grs)
{
  LCIter<std::pair<std::string, Graph>> it;
  for (it = grs.cbegin(); it != grs.cend(); ++it)
  {
    std::cout << (*it).first << "\n";
  }
}

inline void lavrentev::Graph::vertexes(List<std::pair<std::string, Graph>> grs, std::string name)
{
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
  throw std::invalid_argument("<INVALID COMMAND>");
}

inline void lavrentev::Graph::outbound(
  List<std::pair<std::string, Graph>> grs,
  std::string name,
  std::string v)
{
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
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

#endif
