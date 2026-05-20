#include <List.hpp>
#include <fstream>
#include <stdexcept>
#include "BSTree.hpp"

namespace lavrentev
{
  void readfile(std::string name, List< BSTree< size_t, std::string, std::less< size_t > > >& bsts);
}

inline void lavrentev::readfile(std::string filename, List< BSTree< size_t, std::string, std::less< size_t > > >& bsts)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("File open error");
  }

  std::string name;
  if (!(file >> name))
  {
    return;
  }

  BSTree< size_t, std::string, std::less< size_t > > bst;
  bst.setName(name);

  size_t key;
  std::string value;

  while (file)
  {
    if (file >> key >> value)
    {
      bst[key] = value;
    }
    else
    {
      if (file.eof())
      {
        bsts.pushFront(bst);
        break;
      }

      file.clear();
      bsts.pushFront(bst);
      bst = BSTree< size_t, std::string, std::less< size_t > >();

      if (file >> name)
      {
        bst.setName(name);
      }
    }
  }
  file.close();
}
