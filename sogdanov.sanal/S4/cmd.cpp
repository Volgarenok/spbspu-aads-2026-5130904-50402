#include "cmd.hpp"

namespace sogdanov
{

  void cmdPrint(std::istream &in, std::ostream &out, BSTree<std::string, BSTree<int, std::string>> &datasets)
  {
    std::string name;
    in >> name;

    BSTIterator<std::string, BSTree<int, std::string>> it = datasets.find(name);
    if (it == datasets.end())
    {
      out << "<EMPTY>\n";
      return;
    }

    BSTree<int, std::string> &tree = (*it).second;
    if (tree.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    out << name;
    for (BSTIterator<int, std::string> val_it = tree.begin(); val_it != tree.end(); ++val_it)
    {
      out << " " << (*val_it).first << " " << (*val_it).second;
    }
    out << "\n";
  }

  void cmdComplement(std::istream &in, std::ostream &out, BSTree<std::string, BSTree<int, std::string>> &datasets)
  {
    std::string new_name;
    std::string d1_name;
    std::string d2_name;
    in >> new_name >> d1_name >> d2_name;

    BSTIterator<std::string, BSTree<int, std::string>> it1 = datasets.find(d1_name);
    BSTIterator<std::string, BSTree<int, std::string>> it2 = datasets.find(d2_name);

    BSTree<int, std::string> new_tree;

    if (it1 != datasets.end())
    {
      for (BSTIterator<int, std::string> val_it = (*it1).second.begin(); val_it != (*it1).second.end(); ++val_it)
      {
        if (it2 == datasets.end() || (*it2).second.find((*val_it).first) == (*it2).second.end())
        {
          new_tree.push((*val_it).first, (*val_it).second);
        }
      }
    }

    datasets.push(new_name, new_tree);
  }

  void cmdIntersect(std::istream &in, std::ostream &out, BSTree<std::string, BSTree<int, std::string>> &datasets)
  {
    std::string new_name;
    std::string d1_name;
    std::string d2_name;
    in >> new_name >> d1_name >> d2_name;

    BSTIterator<std::string, BSTree<int, std::string>> it1 = datasets.find(d1_name);
    BSTIterator<std::string, BSTree<int, std::string>> it2 = datasets.find(d2_name);

    BSTree<int, std::string> new_tree;

    if (it1 != datasets.end() && it2 != datasets.end())
    {
      for (BSTIterator<int, std::string> val_it = (*it1).second.begin(); val_it != (*it1).second.end(); ++val_it)
      {
        if ((*it2).second.find((*val_it).first) != (*it2).second.end())
        {
          new_tree.push((*val_it).first, (*val_it).second);
        }
      }
    }

    datasets.push(new_name, new_tree);
  }

  void cmdUnion(std::istream &in, std::ostream &out, BSTree<std::string, BSTree<int, std::string>> &datasets)
  {
    std::string new_name;
    std::string d1_name;
    std::string d2_name;
    in >> new_name >> d1_name >> d2_name;

    BSTIterator<std::string, BSTree<int, std::string>> it1 = datasets.find(d1_name);
    BSTIterator<std::string, BSTree<int, std::string>> it2 = datasets.find(d2_name);

    BSTree<int, std::string> new_tree;

    if (it1 != datasets.end())
    {
      for (BSTIterator<int, std::string> val_it = (*it1).second.begin(); val_it != (*it1).second.end(); ++val_it)
      {
        new_tree.push((*val_it).first, (*val_it).second);
      }
    }

    if (it2 != datasets.end())
    {
      for (BSTIterator<int, std::string> val_it = (*it2).second.begin(); val_it != (*it2).second.end(); ++val_it)
      {
        if (new_tree.find((*val_it).first) == new_tree.end())
        {
          new_tree.push((*val_it).first, (*val_it).second);
        }
      }
    }

    datasets.push(new_name, new_tree);
  }

}
