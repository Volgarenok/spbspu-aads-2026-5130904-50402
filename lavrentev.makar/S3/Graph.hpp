#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "HashTable.hpp"
#include "List.hpp"
#include <stdexcept>

namespace lavrentev{
  struct Graph {
    public:
      void vertexes(List<std::pair<std::string, Graph>> grs, std::string name);
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
    }
  }
  throw std::invalid_argument("<INVALID COMMAND>");
}



#endif
