#include <fstream>
#include <stdexcept>
#include <bilist.hpp>
#include "BSTree.hpp"
#include "Parse.hpp"

void novikov::parse(std::istream& in, List< BSTree< size_t, std::string, std::less< size_t > > >& trees)
{
  std::string name;
  if (!(in >> name))
  {
    return;
  }

  BSTree< size_t, std::string, std::less< size_t > > tree;
  tree.setName(name);

  size_t key;
  std::string value;

  while (in)
  {
    if (in >> key >> value)
    {
      tree[key] = value;
    }
    else
    {
      if (in.eof())
      {
        trees.pushFront(tree);
        break;
      }

      in.clear();
      trees.pushFront(tree);
      tree = BSTree< size_t, std::string, std::less< size_t > >();

      if (in >> name)
      {
        tree.setName(name);
      }
    }
  }
}
