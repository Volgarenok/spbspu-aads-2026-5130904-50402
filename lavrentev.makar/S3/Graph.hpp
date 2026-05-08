#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "HashTable.hpp"
#include "List.hpp"

namespace lavrentev{
  struct Graph {
    public:

    private:
      HashTable< std::pair<std::string, std::string>,
        List< size_t >,
        Siphash< std::string >,
        std::equal_to<std::pair<std::string, std::string>>
      > gr;
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

#endif
