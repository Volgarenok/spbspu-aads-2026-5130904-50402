#include "dictionary-commands.hpp"

#include <exception>
#include <ostream>
#include <stdexcept>

namespace
{
bool getToken(const std::string& line, size_t& pos, std::string& token)
{
  token.clear();

  while (pos < line.size() && line[pos] == ' ')
  {
    ++pos;
  }

  if (pos == line.size())
  {
    return false;
  }

  while (pos < line.size() && line[pos] != ' ')
  {
    token += line[pos];
    ++pos;
  }

  return true;
}

bool hasExtraTokens(const std::string& line, size_t pos)
{
  while (pos < line.size())
  {
    if (line[pos] != ' ')
    {
      return true;
    }

    ++pos;
  }

  return false;
}

bool readOneArg(const std::string& line, size_t& pos, std::string& first)
{
  if (!getToken(line, pos, first))
  {
    return false;
  }

  return !hasExtraTokens(line, pos);
}

bool readThreeArgs(
  const std::string& line,
  size_t& pos,
  std::string& first,
  std::string& second,
  std::string& third
)
{
  if (!getToken(line, pos, first))
  {
    return false;
  }

  if (!getToken(line, pos, second))
  {
    return false;
  }

  if (!getToken(line, pos, third))
  {
    return false;
  }

  return !hasExtraTokens(line, pos);
}

void replaceDictionary(
  matveev::DictionaryStorage& storage,
  const std::string& name,
  const matveev::Dictionary& dictionary
)
{
  matveev::DictionaryStorage tmp(storage);

  if (tmp.has(name))
  {
    tmp.drop(name);
  }

  tmp.push(name, dictionary);
  storage.swap(tmp);
}
}

namespace matveev
{
void printDictionary(std::ostream& out, const std::string& name, const Dictionary& dictionary)
{
  if (dictionary.empty())
  {
    out << EMPTY_OUTPUT << '\n';
    return;
  }

  out << name;

  for (Dictionary::ConstIterator it = dictionary.cbegin(); it != dictionary.cend(); ++it)
  {
    out << ' ' << it->first << ' ' << it->second;
  }

  out << '\n';
}

bool executeCommand(std::ostream& out, DictionaryStorage& storage, const std::string& line)
{
  size_t pos = 0;
  std::string command;

  if (!getToken(line, pos, command))
  {
    return true;
  }

  if (command == "print")
  {
    std::string name;

    if (!readOneArg(line, pos, name))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    try
    {
      printDictionary(out, name, storage.at(name));
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == "complement")
  {
    std::string new_name;
    std::string lhs_name;
    std::string rhs_name;

    if (!readThreeArgs(line, pos, new_name, lhs_name, rhs_name))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    try
    {
      Dictionary result = complementDictionary(storage.at(lhs_name), storage.at(rhs_name));
      replaceDictionary(storage, new_name, result);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == "intersect")
  {
    std::string new_name;
    std::string lhs_name;
    std::string rhs_name;

    if (!readThreeArgs(line, pos, new_name, lhs_name, rhs_name))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    try
    {
      Dictionary result = intersectDictionary(storage.at(lhs_name), storage.at(rhs_name));
      replaceDictionary(storage, new_name, result);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  if (command == "union")
  {
    std::string new_name;
    std::string lhs_name;
    std::string rhs_name;

    if (!readThreeArgs(line, pos, new_name, lhs_name, rhs_name))
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    try
    {
      Dictionary result = unionDictionary(storage.at(lhs_name), storage.at(rhs_name));
      replaceDictionary(storage, new_name, result);
    }
    catch (const std::exception&)
    {
      out << INVALID_COMMAND << '\n';
      return false;
    }

    return true;
  }

  out << INVALID_COMMAND << '\n';
  return false;
}
}
