#ifndef MATVEEV_DICTIONARY_COMMANDS_HPP
#define MATVEEV_DICTIONARY_COMMANDS_HPP

#include "dictionary-reader.hpp"

#include <iosfwd>
#include <string>

namespace matveev
{
constexpr const char* INVALID_COMMAND = "<INVALID COMMAND>";
constexpr const char* EMPTY_OUTPUT = "<EMPTY>";

bool executeCommand(std::ostream& out, DictionaryStorage& storage, const std::string& line);
void printDictionary(std::ostream& out, const std::string& name, const Dictionary& dictionary);
}

#endif
