#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Dictionary.hpp"
#include <limits>

using strHash_t = size_t(*)(const std::string&);
using stringEq_t = bool(*)(const std::string&, const std::string&);

size_t stringHasher(const std::string& s)
{
  boost::hash2::siphash_64 hash;
  hash.update(s.data(), s.size());
  return hash.result();
}

bool stringEq(const std::string& a, const std::string& b)
{
  return a == b;
}

void loadCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string filename, dictName;
  i >> filename >> dictName;
  manager.load(filename, dictName);
  o << "Load successful.\n";
}

void deleteCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string dictName;
  i >> dictName;
  manager.deleteDict(dictName);
  o << "Delete successful.\n";
}

void mergeCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string dict1, dict2, result;
  i >> dict1 >> dict2 >> result;
  manager.merge(dict1, dict2, result);
  o << "Merge successful.\n";
}

void findCmd(std::ostream&, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string dictName, part, word;
  i >> dictName >> part >> word;
  manager.find(dictName, part, word);
}

void saveCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string filename, dictName;
  i >> filename >> dictName;
  manager.save(filename, dictName);
  o << "Save successful.\n";
}

void addWordCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string dictName, word, part, translations;
  i >> dictName >> word >> part >> translations;
  manager.addWord(dictName, word, part, translations);
  o << "Word added successfully.\n";
}

void addTranslationCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string dictName, word, translation;
  i >> dictName >> word >> translation;
  manager.addTranslation(dictName, word, translation);
  o << "Translation added successfully.\n";
}

void deleteWordCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string dictName, word;
  i >> dictName >> word;
  manager.deleteWord(dictName, word);
  o << "Word deleted successfully.\n";
}

void deleteTranslationCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string dictName, word, translation;
  i >> dictName >> word >> translation;
  manager.deleteTranslation(dictName, word, translation);
  o << "Translation deleted successfully\n";
}

void changePartCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string dictName, word, newPart;
  i >> dictName >> word >> newPart;
  manager.changePart(dictName, word, newPart);
  o << "Part of speech changed successfully.\n";
}

void findTranslationCmd(std::ostream&, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string dictName, translation;
  i >> dictName >> translation;
  manager.findTranslation(dictName, translation);
}

void translateTextCmd(std::ostream& o, std::istream& i, vishnevskiy::DictionaryManager& manager)
{
  std::string filePath, dictName, resultFilename;
  i >> filePath >> dictName >> resultFilename;
  manager.translateText(filePath, dictName, resultFilename);
  o << "Saved to " << resultFilename << "\n";
}

int main(int argc, char* argv[])
{
  if (argc != 1)
  {
    return 1;
  }

  vishnevskiy::DictionaryManager manager(12, stringHasher, stringEq);

  using cmd_t = void(*)(std::ostream&, std::istream&, vishnevskiy::DictionaryManager&);
  vishnevskiy::HashTable<std::string, cmd_t, strHash_t, stringEq_t> cmds(12, stringHasher, stringEq);

  cmds.add("load", loadCmd);
  cmds.add("delete", deleteCmd);
  cmds.add("merge", mergeCmd);
  cmds.add("find", findCmd);
  cmds.add("save", saveCmd);
  cmds.add("add-word", addWordCmd);
  cmds.add("add-translation", addTranslationCmd);
  cmds.add("delete-word", deleteWordCmd);
  cmds.add("delete-translation", deleteTranslationCmd);
  cmds.add("change-ps", changePartCmd);
  cmds.add("find-translation", findTranslationCmd);
  cmds.add("translate-text", translateTextCmd);

  std::string cmd;
  std::cout << ">";
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cout, std::cin, manager);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
    std::cout << ">";
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input!\n";
    return 1;
  }
  return 0;
}
