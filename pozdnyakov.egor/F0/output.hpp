#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iosfwd>
#include <string>
#include "dictionary.hpp"

namespace pozdnyakov
{
  void printTranslation(std::ostream &out, const AvlDictionary &dict, const std::string &engWord);
  void printDictionary(std::ostream &out, const AvlDictionary &dict, const std::string &name);
  void printReverse(std::ostream &out, const AvlDictionary &dict, const std::string &rusWord);
  void printStats(std::ostream &out, const AvlDictionary &dict, const std::string &name);
}

#endif
