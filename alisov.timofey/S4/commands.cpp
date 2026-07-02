#include "commands.hpp"
#include "BSTree.hpp"

void alisov::print(std::ostream &out, std::istream &in, alisov::datasets_t &datasets)
{
  std::string name;
  in >> name;
  alisov::ds_t ds = datasets.at(name);
  if (ds.empty()) {
    out << "<EMPTY>" << '\n';
    return;
  }
  out << name;
  for (auto it = ds.begin(); it != ds.end(); ++it) {
    out << ' ' << (*it).first << ' ' << (*it).second;
  }
  out << '\n';
}

void alisov::complement(std::ostream &, std::istream &in, alisov::datasets_t &datasets)
{
  std::string newName, name1, name2;
  ds_t newDataset;
  in >> newName >> name1 >> name2;
  ds_t src1 = datasets.at(name1), src2 = datasets.at(name2);
  for (auto it = src1.begin(); it != src1.end(); ++it) {
    if (!src2.contains((*it).first)) {
      newDataset.insert((*it).first, (*it).second);
    }
  }
  datasets[newName] = newDataset;
}

void alisov::intersect(std::ostream &, std::istream &in, alisov::datasets_t &datasets)
{
  std::string newName, name1, name2;
  ds_t newDataset;
  in >> newName >> name1 >> name2;
  ds_t src1 = datasets.at(name1), src2 = datasets.at(name2);
  for (auto it = src1.begin(); it != src1.end(); ++it) {
    if (src2.contains((*it).first)) {
      newDataset.insert((*it).first, (*it).second);
    }
  }
  datasets[newName] = newDataset;
}

void alisov::union_(std::ostream &, std::istream &in, alisov::datasets_t &datasets)
{
  std::string newName, name1, name2;
  ds_t newDataset;
  in >> newName >> name1 >> name2;
  ds_t src1 = datasets.at(name1), src2 = datasets.at(name2);
  for (auto it = src1.begin(); it != src1.end(); ++it) {
    newDataset.insert((*it).first, (*it).second);
  }
  for (auto it = src2.begin(); it != src2.end(); ++it) {
    if (!src1.contains((*it).first)) {
      newDataset.insert((*it).first, (*it).second);
    }
  }
  datasets[newName] = newDataset;
}
