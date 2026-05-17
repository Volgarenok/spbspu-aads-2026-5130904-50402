#include "BSTree.hpp"
#include <List.hpp>
#include <fstream>
#include <stdexcept>

namespace lavrentev {
  void readfile(std::string name, List<BSTree<std::string, size_t, std::less<std::string>>> &grs);
}

inline void lavrentev::readfile(std::string name, List<BSTree<std::string, size_t, std::less<std::string>>> &grs)
{
  std::ifstream file(name);
  if (file.is_open())
  {
    
    file.close();
  }
  else
  {
    throw std::runtime_error("File open error");
  }
}
