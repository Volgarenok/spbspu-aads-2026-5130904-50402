#include "commands.hpp"
#include <limits>
#include <string>

namespace pozdnyakov
{

  bool isInteger(const std::string &str)
  {
    if (str.empty() || str == "-") {
      return false;
    }
    try {
      std::size_t pos = 0;
      std::stoi(str, &pos);
      return pos == str.length();
    } catch (const std::invalid_argument &) {
      return false;
    } catch (const std::out_of_range &) {
      return false;
    }
  }

  void cmdPrint(std::istream &in, std::ostream &out, Datasets &datasets)
  {
    std::string name;
    in >> name;
    try {
      const Dataset &dict = datasets.get(name);
      if (dict.empty()) {
        out << "<EMPTY>\n";
      } else {
        out << name;
        for (auto it = dict.begin(); it != dict.end(); ++it) {
          out << " " << (*it).first << " " << (*it).second;
        }
        out << "\n";
      }
    } catch (const std::out_of_range &) {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmdIntersect(std::istream &in, std::ostream &out, Datasets &datasets)
  {
    std::string resultName;
    std::string dict1;
    std::string dict2;
    in >> resultName >> dict1 >> dict2;
    try {
      const Dataset intersected = intersect(datasets.get(dict1), datasets.get(dict2));
      datasets.push(resultName, intersected);
    } catch (const std::out_of_range &) {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmdComplement(std::istream &in, std::ostream &out, Datasets &datasets)
  {
    std::string resultName;
    std::string dict1;
    std::string dict2;
    in >> resultName >> dict1 >> dict2;
    try {
      const Dataset comp = complement(datasets.get(dict1), datasets.get(dict2));
      datasets.push(resultName, comp);
    } catch (const std::out_of_range &) {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmdUnion(std::istream &in, std::ostream &out, Datasets &datasets)
  {
    std::string resultName;
    std::string dict1;
    std::string dict2;
    in >> resultName >> dict1 >> dict2;
    try {
      const Dataset uni = union_(datasets.get(dict1), datasets.get(dict2));
      datasets.push(resultName, uni);
    } catch (const std::out_of_range &) {
      out << "<INVALID COMMAND>\n";
    }
  }

}
