#include "cmds.hpp"
#include <string>
#include <utility>

void dirko::cmdPrint(std::istream &in, std::ostream &out, dirko::Datasets &datasets)
{
  std::string name;
  if (!(in >> name)) {
    throw std::runtime_error("Invalid input");
  }
  try {
    const dirko::Dataset &ds = datasets.get(name);
    if (ds.empty()) {
      out << "<EMPTY>" << '\n';
    } else {
      out << name;
      for (const std::pair< const int, std::string > &v : ds) {
        out << ' ' << v.first << ' ' << v.second;
      }
      out << '\n';
    }
  } catch (const std::out_of_range &) {
    throw std::runtime_error("Dataset not found");
  }
}
