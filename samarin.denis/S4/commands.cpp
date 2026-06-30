#include "commands.hpp"

#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "dataset.hpp"
#include "tree.hpp"

namespace samarin {

  using CommandHandler = void (*)(std::istream &, std::ostream &, DatasetCollection &);
  using CommandTable = BSTree< std::string, CommandHandler >;

  static Dataset & requireDataset(DatasetCollection & datasets, const std::string & name)
  {
    DatasetCollection::iterator it = datasets.find(name);
    if (it == datasets.end()) {
      throw std::logic_error("no such dataset");
    }
    return it->second;
  }

  static void cmdPrint(std::istream & in, std::ostream & out, DatasetCollection & datasets)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("missing name");
    }
    const Dataset & dataset = requireDataset(datasets, name);
    if (dataset.empty()) {
      out << "<EMPTY>\n";
      return;
    }
    out << name;
    for (Dataset::const_iterator it = dataset.cbegin(); it != dataset.cend(); ++it) {
      out << " " << it->first << " " << it->second;
    }
    out << "\n";
  }

  enum class SetOp {
    complement,
    intersect,
    unite
  };

  static Dataset combine(const Dataset & left, const Dataset & right, SetOp op)
  {
    Dataset result;
    if (op == SetOp::unite) {
      for (Dataset::const_iterator it = left.cbegin(); it != left.cend(); ++it) {
        result.push(it->first, it->second);
      }
      for (Dataset::const_iterator it = right.cbegin(); it != right.cend(); ++it) {
        if (!left.contains(it->first)) {
          result.push(it->first, it->second);
        }
      }
      return result;
    }
    const bool wantShared = op == SetOp::intersect;
    for (Dataset::const_iterator it = left.cbegin(); it != left.cend(); ++it) {
      if (right.contains(it->first) == wantShared) {
        result.push(it->first, it->second);
      }
    }
    return result;
  }

  static void runSetOp(std::istream & in, DatasetCollection & datasets, SetOp op)
  {
    std::string target;
    std::string first;
    std::string second;
    if (!(in >> target >> first >> second)) {
      throw std::logic_error("missing operands");
    }
    const Dataset & left = requireDataset(datasets, first);
    const Dataset & right = requireDataset(datasets, second);
    Dataset result = combine(left, right, op);
    datasets.push(target, result);
  }

  static void cmdComplement(std::istream & in, std::ostream &, DatasetCollection & datasets)
  {
    runSetOp(in, datasets, SetOp::complement);
  }

  static void cmdIntersect(std::istream & in, std::ostream &, DatasetCollection & datasets)
  {
    runSetOp(in, datasets, SetOp::intersect);
  }

  static void cmdUnion(std::istream & in, std::ostream &, DatasetCollection & datasets)
  {
    runSetOp(in, datasets, SetOp::unite);
  }

  static CommandTable makeCommands()
  {
    CommandTable commands;
    commands.push("print", &cmdPrint);
    commands.push("complement", &cmdComplement);
    commands.push("intersect", &cmdIntersect);
    commands.push("union", &cmdUnion);
    return commands;
  }

}

void samarin::processCommands(std::istream & in, std::ostream & out, DatasetCollection & datasets)
{
  const CommandTable commands = makeCommands();
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream stream(line);
    std::string name;
    if (!(stream >> name)) {
      continue;
    }
    try {
      CommandTable::const_iterator handler = commands.find(name);
      if (handler == commands.cend()) {
        throw std::logic_error("unknown command");
      }
      handler->second(stream, out, datasets);
    } catch (const std::exception &) {
      out << "<INVALID COMMAND>\n";
    }
  }
}
