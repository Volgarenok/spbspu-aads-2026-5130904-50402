#ifndef COMANDS_HPP
#define COMANDS_HPP
#include <iostream>
#include "BSTree.hpp"

using Dataset = saldaev::BSTree< long long, std::string >;
using Datasets = saldaev::BSTree< std::string, Dataset >;

namespace saldaev
{
  void handlePrint(std::istream &in, std::ostream &out, Datasets &datasets)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::runtime_error("Invalid input");
    }

    const Dataset dat = datasets.get(name);
    if (dat.empty()) {
      out << "<EMPTY>" << '\n';
    } else {
      out << name;
      for (const std::pair< long long, std::string > &i : dat) {
        out << ' ' << i.first << ' ' << i.second;
      }
      out << '\n';
    }
  }

  void handleComplement(std::istream &in, std::ostream &, Datasets &datasets)
  {
    std::string name1, name2, new_name;
    if (!(in >> new_name >> name1 >> name2)) {
      throw std::runtime_error("Invalid input");
    }

    const Dataset dat1 = datasets.get(name1);
    const Dataset dat2 = datasets.get(name2);
    Dataset new_dat;
    for (const std::pair< long long, std::string > &i : dat1) {
      try {
        dat2.get(i.first);
      } catch (const std::out_of_range &) {
        new_dat.push(i.first, i.second);
      }
    }
    datasets.push(new_name, std::move(new_dat));
  }

  void handleIntersect(std::istream &in, std::ostream &, Datasets &datasets)
  {
    std::string name1, name2, new_name;
    if (!(in >> new_name >> name1 >> name2)) {
      throw std::runtime_error("Invalid input");
    }

    const Dataset dat1 = datasets.get(name1);
    const Dataset dat2 = datasets.get(name2);
    Dataset new_dat;
    for (const std::pair< long long, std::string > &i : dat1) {
      try {
        dat2.get(i.first);
        new_dat.push(i.first, i.second);
      } catch (const std::out_of_range &) {
      }
    }
    datasets.push(new_name, std::move(new_dat));
  }

  void handleUnion(std::istream &in, std::ostream &, Datasets &datasets)
  {
    std::string name1, name2, new_name;
    if (!(in >> new_name >> name1 >> name2)) {
      throw std::runtime_error("Invalid input");
    }

    const Dataset dat1 = datasets.get(name1);
    const Dataset dat2 = datasets.get(name2);
    Dataset new_dat(dat1);
    for (const std::pair< long long, std::string > &i : dat2) {
      try {
        dat1.get(i.first);
      } catch (...) {
        new_dat.push(i.first, i.second);
      }
    }
    datasets.push(new_name, std::move(new_dat));
  }
}

#endif
