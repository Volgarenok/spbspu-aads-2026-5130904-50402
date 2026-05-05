#include "commands.hpp"
#include <stdexcept>
#include <utility>

void karpovich::cmdPrint(std::istream &in, std::ostream &out, karpovich::Datasets &datasets)
{
  std::string name;
  if (!(in >> name)) {
    throw std::runtime_error("Invalid input");
  }
  try {
    const karpovich::Dataset &ds = datasets.at(name);
    if (ds.empty()) {
      out << "<EMPTY>" << '\n';
    } else {
      out << name;
      for (auto it = ds.cbegin(); it != ds.cend(); ++it) {
        auto pair = *it;
        out << ' ' << pair.first << ' ' << pair.second;
      }
      out << '\n';
    }
  } catch (const std::out_of_range &) {
    throw std::runtime_error("Dataset not found");
  }
}

void karpovich::cmdComplement(std::istream &in, std::ostream &, karpovich::Datasets &datasets)
{
  std::string new_name, name1, name2;
  if (!(in >> new_name >> name1 >> name2)) {
    throw std::runtime_error("Invalid input");
  }
  try {
    datasets.at(new_name);
    throw std::runtime_error("Dataset exists");
  } catch (const std::out_of_range &) {
  }
  const karpovich::Dataset &ds1 = datasets.at(name1);
  const karpovich::Dataset &ds2 = datasets.at(name2);
  karpovich::Dataset new_ds;
  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it) {
    auto pair = *it;
    try {
      ds2.at(pair.first);
    } catch (const std::out_of_range &) {
      new_ds.push(pair.first, pair.second);
    }
  }
  datasets.push(new_name, std::move(new_ds));
}

void karpovich::cmdIntersect(std::istream &in, std::ostream &, karpovich::Datasets &datasets)
{
  std::string new_name, name1, name2;
  if (!(in >> new_name >> name1 >> name2)) {
    throw std::runtime_error("Invalid input");
  }
  try {
    datasets.at(new_name);
    throw std::runtime_error("Dataset exists");
  } catch (const std::out_of_range &) {
  }
  const karpovich::Dataset &ds1 = datasets.at(name1);
  const karpovich::Dataset &ds2 = datasets.at(name2);
  karpovich::Dataset new_ds;
  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it) {
    auto pair = *it;
    try {
      ds2.at(pair.first);
      new_ds.push(pair.first, pair.second);
    } catch (const std::out_of_range &) {
    }
  }
  datasets.push(new_name, std::move(new_ds));
}

void karpovich::cmdUnion(std::istream &in, std::ostream &, karpovich::Datasets &datasets)
{
  std::string new_name, name1, name2;
  if (!(in >> new_name >> name1 >> name2)) {
    throw std::runtime_error("Invalid input");
  }
  try {
    datasets.at(new_name);
    throw std::runtime_error("Dataset exists");
  } catch (const std::out_of_range &) {
  }
  const karpovich::Dataset &ds1 = datasets.at(name1);
  const karpovich::Dataset &ds2 = datasets.at(name2);
  karpovich::Dataset new_ds;
  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it) {
    auto pair = *it;
    new_ds.push(pair.first, pair.second);
  }
  for (auto it = ds2.cbegin(); it != ds2.cend(); ++it) {
    auto pair = *it;
    try {
      new_ds.at(pair.first);
    } catch (const std::out_of_range &) {
      new_ds.push(pair.first, pair.second);
    }
  }
  datasets.push(new_name, std::move(new_ds));
}
