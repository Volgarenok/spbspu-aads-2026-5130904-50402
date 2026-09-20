#include "commands.hpp"
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>
#include "operations.hpp"
#include "output.hpp"

namespace
{
  const char *const INVALID_COMMAND = "<INVALID COMMAND>";

  std::string readToken(std::istream &in)
  {
    std::string token;
    if (!(in >> token)) {
      throw std::invalid_argument(INVALID_COMMAND);
    }
    return token;
  }

  std::size_t readCount(std::istream &in)
  {
    std::size_t count = 0;
    if (!(in >> count) || count == 0) {
      throw std::invalid_argument(INVALID_COMMAND);
    }
    return count;
  }

  pozdnyakov::AvlDictionary &requireDict(pozdnyakov::DictTable &dicts, const std::string &name)
  {
    pozdnyakov::AvlDictionary * const dict = dicts.find(name);
    if (dict == nullptr) {
      throw std::invalid_argument("<INVALID COMMAND: dictionary '" + name + "' does not exist>");
    }
    return *dict;
  }

  void requireFreeName(const pozdnyakov::DictTable &dicts, const std::string &name)
  {
    if (dicts.find(name) != nullptr) {
      throw std::invalid_argument("<INVALID COMMAND: dictionary '" + name + "' already exists>");
    }
  }

  void requireWord(const pozdnyakov::AvlDictionary &dict, const std::string &engWord)
  {
    if (!dict.contains(engWord)) {
      throw std::invalid_argument("<NOT FOUND: word '" + engWord + "' is not in the dictionary>");
    }
  }

  pozdnyakov::DictRefs readSources(pozdnyakov::DictTable &dicts, std::istream &in, std::size_t count)
  {
    pozdnyakov::DictRefs sources;
    for (std::size_t i = 0; i < count; ++i) {
      const std::string name = readToken(in);
      sources.pushBack(std::addressof(requireDict(dicts, name)));
    }
    return sources;
  }
}

void pozdnyakov::cmdMake(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string name = readToken(in);
  requireFreeName(dicts, name);
  dicts.add(name, AvlDictionary());
}

void pozdnyakov::cmdDrop(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string name = readToken(in);
  if (!dicts.drop(name)) {
    throw std::invalid_argument("<INVALID COMMAND: dictionary '" + name + "' does not exist>");
  }
}

void pozdnyakov::cmdAddWord(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string name = readToken(in);
  const std::string engWord = readToken(in);
  const std::string rusWord = readToken(in);
  const std::string partOfSpeech = readToken(in);
  requireDict(dicts, name).addWord(engWord, rusWord, partOfSpeech);
}

void pozdnyakov::cmdAddTrans(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string name = readToken(in);
  const std::string engWord = readToken(in);
  const std::string rusWord = readToken(in);

  const AvlDictionary::AddTransStatus status = requireDict(dicts, name).addTranslation(engWord, rusWord);
  if (status == AvlDictionary::AddTransStatus::NoWord) {
    throw std::invalid_argument("<NOT FOUND: word '" + engWord + "' is not in the dictionary>");
  }
  if (status == AvlDictionary::AddTransStatus::AmbiguousPos) {
    throw std::invalid_argument("<INVALID COMMAND: ambiguous part of speech for '" + engWord + "'>");
  }
}

void pozdnyakov::cmdDelWord(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string name = readToken(in);
  const std::string engWord = readToken(in);
  if (!requireDict(dicts, name).removeWord(engWord)) {
    throw std::invalid_argument("<NOT FOUND: word '" + engWord + "' is not in the dictionary>");
  }
}

void pozdnyakov::cmdDelTrans(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string name = readToken(in);
  const std::string engWord = readToken(in);
  const std::string rusWord = readToken(in);

  AvlDictionary &dict = requireDict(dicts, name);
  requireWord(dict, engWord);
  if (!dict.removeTranslation(engWord, rusWord)) {
    throw std::invalid_argument("<NOT FOUND: translation '" + rusWord + "' for word '" + engWord + "'>");
  }
}

void pozdnyakov::cmdTranslate(DictTable &dicts, std::istream &in, std::ostream &out)
{
  const std::string name = readToken(in);
  const std::string engWord = readToken(in);
  printTranslation(out, requireDict(dicts, name), engWord);
}

void pozdnyakov::cmdReverse(DictTable &dicts, std::istream &in, std::ostream &out)
{
  const std::string name = readToken(in);
  const std::string rusWord = readToken(in);
  printReverse(out, requireDict(dicts, name), rusWord);
}

void pozdnyakov::cmdShow(DictTable &dicts, std::istream &in, std::ostream &out)
{
  const std::string name = readToken(in);
  printDictionary(out, requireDict(dicts, name), name);
}

void pozdnyakov::cmdCount(DictTable &dicts, std::istream &in, std::ostream &out)
{
  const std::string name = readToken(in);
  printStats(out, requireDict(dicts, name), name);
}

void pozdnyakov::cmdFilter(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string newName = readToken(in);
  const std::string sourceName = readToken(in);
  const std::string partOfSpeech = readToken(in);

  const AvlDictionary &source = requireDict(dicts, sourceName);
  requireFreeName(dicts, newName);

  AvlDictionary result;
  filterDictionary(result, source, partOfSpeech);
  dicts.add(newName, std::move(result));
}

void pozdnyakov::cmdUnion(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string newName = readToken(in);
  const std::size_t count = readCount(in);
  const DictRefs sources = readSources(dicts, in, count);
  requireFreeName(dicts, newName);

  AvlDictionary result;
  mergeDictionaries(result, sources);
  dicts.add(newName, std::move(result));
}

void pozdnyakov::cmdIntersect(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string newName = readToken(in);
  const std::size_t count = readCount(in);
  const DictRefs sources = readSources(dicts, in, count);
  requireFreeName(dicts, newName);

  AvlDictionary result;
  intersectDictionaries(result, sources);
  dicts.add(newName, std::move(result));
}

void pozdnyakov::cmdDiff(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string newName = readToken(in);
  const std::size_t count = readCount(in);
  const DictRefs sources = readSources(dicts, in, count);
  requireFreeName(dicts, newName);

  AvlDictionary result;
  diffDictionaries(result, sources);
  dicts.add(newName, std::move(result));
}

void pozdnyakov::cmdMerge(DictTable &dicts, std::istream &in, std::ostream &)
{
  const std::string targetName = readToken(in);
  const std::size_t count = readCount(in);
  const DictRefs sources = readSources(dicts, in, count);

  AvlDictionary &target = requireDict(dicts, targetName);
  for (std::size_t i = 0; i < sources.size(); ++i) {
    if (sources[i] == std::addressof(target)) {
      throw std::invalid_argument("<INVALID COMMAND: '" + targetName + "' is both target and source>");
    }
  }
  mergeDictionaries(target, sources);
}

void pozdnyakov::initCommands(CmdTable &cmds)
{
  cmds.add("make", &cmdMake);
  cmds.add("drop", &cmdDrop);
  cmds.add("add-word", &cmdAddWord);
  cmds.add("add-trans", &cmdAddTrans);
  cmds.add("del-word", &cmdDelWord);
  cmds.add("del-trans", &cmdDelTrans);
  cmds.add("translate", &cmdTranslate);
  cmds.add("reverse", &cmdReverse);
  cmds.add("show", &cmdShow);
  cmds.add("count", &cmdCount);
  cmds.add("filter", &cmdFilter);
  cmds.add("union", &cmdUnion);
  cmds.add("intersect", &cmdIntersect);
  cmds.add("diff", &cmdDiff);
  cmds.add("merge", &cmdMerge);
}
