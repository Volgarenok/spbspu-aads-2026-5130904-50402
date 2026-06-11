#include "commands.hpp"
#include "BSTree.hpp"

void shirokov::print(std::ostream& out, std::istream& in, shirokov::datasets_t& datasets)
{
  std::string name;
  in >> name;
  shirokov::ds_t ds = datasets.at(name);
  if (ds.empty())
  {
    out << "<EMPTY>" << '\n';
    return;
  }
  out << name;
  for (auto it = ds.begin(); it != ds.end(); ++it)
  {
    out << ' ' << (*it).first << ' ' << (*it).second;
  }
  out << '\n';
}

void shirokov::complement(std::ostream&, std::istream& in, shirokov::datasets_t& datasets)
{
  std::string newName, name1, name2;
  ds_t newDataset;
  in >> newName >> name1 >> name2;
  if (datasets.contains(newName))
  {
    throw std::logic_error("A dataset with that name already exists");
  }
  ds_t src1 = datasets.at(name1), src2 = datasets.at(name2);
  for (auto it = src1.begin(); it != src1.end(); ++it)
  {
    if (!src2.contains((*it).first))
    {
      newDataset.insert((*it).first, (*it).second);
    }
  }
  datasets.insert(newName, newDataset);
}
