#ifndef MATVEEV_DICTIONARY_READER_HPP
#define MATVEEV_DICTIONARY_READER_HPP

#include "dictionary-operations.hpp"

#include <iosfwd>
#include <string>

namespace matveev
{
using DictionaryStorage = BSTree< std::string, Dictionary >;

DictionaryStorage readDictionaries(std::istream& input);
Dictionary readDictionaryLine(const std::string& line, std::string& name);
bool parseInt(const std::string& text, int& value);
}

#endif
