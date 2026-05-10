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

void dirko::cmdComplement(std::istream &in, std::ostream &, dirko::Datasets &datasets)
{
  std::string new_name, name1, name2;
  if (!(in >> new_name >> name1 >> name2)) {
    throw std::runtime_error("Invalid input");
  }
  const dirko::Dataset &ds1 = datasets.get(name1);
  const dirko::Dataset &ds2 = datasets.get(name2);
  dirko::Dataset new_ds;
  for (const std::pair< const int, std::string > &v : ds1) {
    try {
      ds2.get(v.first);
    } catch (const std::out_of_range &) {
      new_ds.push(v.first, v.second);
    }
  }
  datasets.push(new_name, std::move(new_ds));
}

void dirko::cmdIntersect(std::istream &in, std::ostream &, dirko::Datasets &datasets)
{
  std::string new_name, name1, name2;
  if (!(in >> new_name >> name1 >> name2)) {
    throw std::runtime_error("Invalid input");
  }
  const dirko::Dataset &ds1 = datasets.get(name1);
  const dirko::Dataset &ds2 = datasets.get(name2);
  dirko::Dataset new_ds;
  for (const std::pair< const int, std::string > &v : ds1) {
    try {
      ds2.get(v.first);
      new_ds.push(v.first, v.second);
    } catch (const std::out_of_range &) {
    }
  }
  datasets.push(new_name, std::move(new_ds));
}
