#include "dictionary-reader.hpp"

#include <istream>
#include <limits>
#include <stdexcept>

namespace matveev
{
bool parseInt(const std::string& text, int& value)
{
  if (text.empty())
  {
    return false;
  }

  size_t index = 0;
  bool negative = false;

  if (text[index] == '-')
  {
    negative = true;
    ++index;
  }

  if (index == text.size())
  {
    return false;
  }

  long long result = 0;

  for (; index < text.size(); ++index)
  {
    if (text[index] < '0' || text[index] > '9')
    {
      return false;
    }

    result = result * 10 + static_cast< long long >(text[index] - '0');

    if (!negative && result > std::numeric_limits< int >::max())
    {
      return false;
    }

    if (negative && -result < std::numeric_limits< int >::min())
    {
      return false;
    }
  }

  value = static_cast< int >(negative ? -result : result);
  return true;
}

Dictionary readDictionaryLine(const std::string& line, std::string& name)
{
  Dictionary dictionary;
  size_t pos = 0;

  while (pos < line.size() && line[pos] == ' ')
  {
    ++pos;
  }

  while (pos < line.size() && line[pos] != ' ')
  {
    name += line[pos];
    ++pos;
  }

  if (name.empty())
  {
    throw std::logic_error("invalid dictionary");
  }

  while (pos < line.size())
  {
    while (pos < line.size() && line[pos] == ' ')
    {
      ++pos;
    }

    if (pos == line.size())
    {
      break;
    }

    std::string key_text;

    while (pos < line.size() && line[pos] != ' ')
    {
      key_text += line[pos];
      ++pos;
    }

    while (pos < line.size() && line[pos] == ' ')
    {
      ++pos;
    }

    if (pos == line.size())
    {
      throw std::logic_error("invalid dictionary");
    }

    std::string value;

    while (pos < line.size() && line[pos] != ' ')
    {
      value += line[pos];
      ++pos;
    }

    int key = 0;

    if (!parseInt(key_text, key))
    {
      throw std::logic_error("invalid key");
    }

    dictionary.push(key, value);
  }

  return dictionary;
}

DictionaryStorage readDictionaries(std::istream& input)
{
  DictionaryStorage storage;
  std::string line;

  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::string name;
    Dictionary dictionary = readDictionaryLine(line, name);
    storage.push(name, dictionary);
  }

  return storage;
}
}
