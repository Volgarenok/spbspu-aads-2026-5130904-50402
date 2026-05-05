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
