#ifndef MATVEEV_DICTIONARY_OPERATIONS_HPP
#define MATVEEV_DICTIONARY_OPERATIONS_HPP

#include "binary-search-tree.hpp"

#include <string>

namespace matveev
{
using Dictionary = BSTree< int, std::string >;

inline void addIfMissing(Dictionary& dict, int key, const std::string& value)
{
  if (!dict.has(key))
  {
    dict.push(key, value);
  }
}

inline Dictionary complementDictionary(const Dictionary& lhs, const Dictionary& rhs)
{
  Dictionary result;

  for (Dictionary::ConstIterator it = lhs.cbegin(); it != lhs.cend(); ++it)
  {
    if (!rhs.has(it->first))
    {
      result.push(it->first, it->second);
    }
  }

  return result;
}

inline Dictionary intersectDictionary(const Dictionary& lhs, const Dictionary& rhs)
{
  Dictionary result;

  for (Dictionary::ConstIterator it = lhs.cbegin(); it != lhs.cend(); ++it)
  {
    if (rhs.has(it->first))
    {
      result.push(it->first, it->second);
    }
  }

  return result;
}

inline Dictionary unionDictionary(const Dictionary& lhs, const Dictionary& rhs)
{
  Dictionary result;

  for (Dictionary::ConstIterator it = lhs.cbegin(); it != lhs.cend(); ++it)
  {
    result.push(it->first, it->second);
  }

  for (Dictionary::ConstIterator it = rhs.cbegin(); it != rhs.cend(); ++it)
  {
    addIfMissing(result, it->first, it->second);
  }

  return result;
}
}

#endif
