#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include <string>
#include "../common/vector.hpp"
#include "dictionary.hpp"

namespace pozdnyakov
{
  using DictRefs = Vector< const AvlDictionary * >;

  void filterDictionary(AvlDictionary &target, const AvlDictionary &source, const std::string &partOfSpeech);
  void mergeDictionaries(AvlDictionary &target, const DictRefs &sources);
  void intersectDictionaries(AvlDictionary &target, const DictRefs &sources);
  void diffDictionaries(AvlDictionary &target, const DictRefs &sources);
}

#endif
