#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <iosfwd>
#include <string>
#include "../common/hashTable.hpp"
#include "dictionary.hpp"

namespace pozdnyakov
{

  using DictTable = HashTable< std::string, AvlDictionary, std::hash< std::string > >;
  using CommandFunc = void (*)(DictTable &, std::istream &, std::ostream &);
  using CmdTable = HashTable< std::string, CommandFunc, std::hash< std::string > >;

  void cmdMake(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdDrop(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdAddWord(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdAddTrans(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdDelWord(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdDelTrans(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdTranslate(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdReverse(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdShow(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdCount(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdFilter(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdUnion(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdIntersect(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdDiff(DictTable &dicts, std::istream &in, std::ostream &out);
  void cmdMerge(DictTable &dicts, std::istream &in, std::ostream &out);

  void initCommands(CmdTable &cmds);

}

#endif
