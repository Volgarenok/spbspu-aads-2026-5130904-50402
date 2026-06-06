#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "Dictionary.hpp"
#include "HashTable.hpp"

namespace sogdanov {

  void createDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void showDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void dropDict(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts);
  void addWord(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts);
  void removeWord(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts);
  void translateWord(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void reverseTranslate(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void findByPos(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void mergeDicts(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts);
  void saveDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void loadDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void listDicts(std::istream&, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void statsDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void unknownWords(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void demistifyWords(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void unionDicts(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts);
  void complementDicts(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts);

}

#endif
