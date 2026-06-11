#include "commands.hpp"
#include "BSTree.hpp"

void shirokov::print(std::ostream& out, std::istream& in, shirokov::datasets_t& datasets)
{
  std::string name;
  in >> name;
  shirokov::ds_t ds = datasets.at(name);
  if (ds.empty())
  {
    out << "<EMPTY>";
    return;
  }
  out << name;
  for (auto it = ds.begin(); it != ds.end(); ++it)
  {
    out << ' ' << (*it).first << ' ' << (*it).second;
  }
}
