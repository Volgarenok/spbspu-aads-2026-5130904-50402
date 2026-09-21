#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "HashTableImpl.hpp"
#include "ListImpl.hpp"

namespace vishnevskiy
{
  using strHash_t = size_t(*)(const std::string&);
  using stringEq_t = bool(*)(const std::string&, const std::string&);

  using dict_t = HashTable<std::string, List<std::string>, strHash_t, stringEq_t>;

  class DictionaryManager
  {
    private:
      HashTable<std::string, dict_t*, strHash_t, stringEq_t> dictionaries;
      strHash_t hashFunc;
      stringEq_t eqFunc;
      std::string makeKey(const std::string& word, const std::string& part);
      std::pair<std::string, std::string> splitKey(const std::string& key);
      bool isValid(const std::string& part);
      void copyList(List<std::string>& dest, const List<std::string>& src);
      void addToList(List<std::string>& list, const std::string& translation);
      void printList(const List<std::string>& list, std::ostream& out);
      bool isInList(const List<std::string>& list, const std::string& translation) const;
      void removeTranslation(List<std::string>& list, const std::string& translation);
      size_t getSize(const List<std::string>& list) const;

    public:
      DictionaryManager(size_t capacity, strHash_t hash, stringEq_t eq);
      ~DictionaryManager();

      void load(const std::string& filename, const std::string& dictName);
      void deleteDict(const std::string& dictName);
      void merge(const std::string& dict1, const std::string& dict2, const std::string& result);
      void find(const std::string& dictName, const std::string& part, const std::string& word);
      void save(const std::string& filename, const std::string& dictName);
      void addWord(const std::string& dictName, const std::string& word, const std::string& part, const std::string& translations);
      void addTranslation(const std::string& dictName, const std::string& word, const std::string& translation);
      void deleteWord(const std::string& dictName, const std::string& word);
      void deleteTranslation(const std::string& dictName, const std::string& word, const std::string& translation);
      void changePart(const std::string& dictName, const std::string& word, const std::string& newPart);
      void findTranslation(const std::string& dictName, const std::string& translation);
      void translateText(const std::string& filePath, const std::string& dictName, const std::string& resultFilename);
      bool exists(const std::string& dictName);
  };
}

#endif
