#include "cmd.hpp"

#include <iostream>
#include <string>

namespace sogdanov {

  void cmdPrint(std::istream &in, std::ostream &out, Datasets &datasets)
  {
    std::string name;
    in >> name;

    Datasets::iterator it = datasets.find(name);
    if (it == datasets.end()) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Dataset &tree = (*it).second;
    if (tree.empty()) {
      out << "<EMPTY>\n";
      return;
    }

    out << name;
    for (Dataset::iterator val_it = tree.begin();
         val_it != tree.end(); ++val_it) {
      out << " " << (*val_it).first << " " << (*val_it).second;
    }
    out << "\n";
  }

  void cmdComplement(std::istream &in, std::ostream &out, Datasets &datasets)
  {
    std::string new_name;
    std::string d1_name;
    std::string d2_name;
    in >> new_name >> d1_name >> d2_name;

    Datasets::iterator it1 = datasets.find(d1_name);
    Datasets::iterator it2 = datasets.find(d2_name);

    if (it1 == datasets.end() || it2 == datasets.end()) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Dataset new_tree;
    for (Dataset::iterator val_it = (*it1).second.begin();
         val_it != (*it1).second.end(); ++val_it) {
      if (it2 == datasets.end() || (*it2).second.find((*val_it).first) == (*it2).second.end()) {
        new_tree.insert((*val_it).first, (*val_it).second);
      }
    }

    datasets.insert(new_name, new_tree);
  }

  void cmdIntersect(std::istream &in, std::ostream &out, Datasets &datasets)
  {
    std::string new_name;
    std::string d1_name;
    std::string d2_name;
    in >> new_name >> d1_name >> d2_name;

    Datasets::iterator it1 = datasets.find(d1_name);
    Datasets::iterator it2 = datasets.find(d2_name);

    if (it1 == datasets.end() || it2 == datasets.end()) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Dataset new_tree;
    for (Dataset::iterator val_it = (*it1).second.begin();
         val_it != (*it1).second.end(); ++val_it) {
      if ((*it2).second.find((*val_it).first) != (*it2).second.end()) {
        new_tree.insert((*val_it).first, (*val_it).second);
      }
    }

    datasets.insert(new_name, new_tree);
  }

  void cmdUnion(std::istream &in, std::ostream &out, Datasets &datasets)
  {
    std::string new_name;
    std::string d1_name;
    std::string d2_name;
    in >> new_name >> d1_name >> d2_name;

    Datasets::iterator it1 = datasets.find(d1_name);
    Datasets::iterator it2 = datasets.find(d2_name);

    if (it1 == datasets.end() || it2 == datasets.end()) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Dataset new_tree;
    for (Dataset::iterator val_it = (*it1).second.begin();
         val_it != (*it1).second.end(); ++val_it) {
      new_tree.insert((*val_it).first, (*val_it).second);
    }

    if (it2 != datasets.end()) {
      for (Dataset::iterator val_it = (*it2).second.begin();
           val_it != (*it2).second.end(); ++val_it) {
        if (new_tree.find((*val_it).first) == new_tree.end()) {
          new_tree.insert((*val_it).first, (*val_it).second);
        }
      }
    }

    datasets.insert(new_name, new_tree);
  }
}
