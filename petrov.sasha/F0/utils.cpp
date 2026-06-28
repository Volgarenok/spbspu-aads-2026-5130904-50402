#include "utils.hpp"

#include <cctype>
#include <string>
#include <vector>

std::string petrov::trim(const std::string& source)
{
  std::size_t begin = 0;
  std::size_t end = source.size();
  while (begin < end && std::isspace(static_cast< unsigned char >(source[begin]))) {
    ++begin;
  }
  while (end > begin && std::isspace(static_cast< unsigned char >(source[end - 1]))) {
    --end;
  }
  return source.substr(begin, end - begin);
}

std::string petrov::toLower(const std::string& source)
{
  std::string result;
  result.reserve(source.size());
  for (std::size_t i = 0; i < source.size(); ++i) {
    const char lowered = static_cast< char >(std::tolower(static_cast< unsigned char >(source[i])));
    result.push_back(lowered);
  }
  return result;
}

bool petrov::containsIgnoreCase(const std::string& haystack, const std::string& needle)
{
  if (needle.empty()) {
    return true;
  }
  return toLower(haystack).find(toLower(needle)) != std::string::npos;
}

std::vector< std::string > petrov::tokenize(const std::string& line)
{
  std::vector< std::string > tokens;
  std::string current;
  bool inQuotes = false;
  bool hasToken = false;
  for (std::size_t i = 0; i < line.size(); ++i) {
    const char symbol = line[i];
    if (inQuotes) {
      if (symbol == '"') {
        inQuotes = false;
      } else {
        current.push_back(symbol);
      }
    } else if (symbol == '"') {
      inQuotes = true;
      hasToken = true;
    } else if (std::isspace(static_cast< unsigned char >(symbol))) {
      if (hasToken) {
        tokens.push_back(current);
        current.clear();
        hasToken = false;
      }
    } else {
      current.push_back(symbol);
      hasToken = true;
    }
  }
  if (hasToken) {
    tokens.push_back(current);
  }
  return tokens;
}

std::vector< std::string > petrov::splitList(const std::string& source, char delimiter)
{
  std::vector< std::string > result;
  std::string current;
  for (std::size_t i = 0; i < source.size(); ++i) {
    if (source[i] == delimiter) {
      const std::string trimmed = trim(current);
      if (!trimmed.empty()) {
        result.push_back(trimmed);
      }
      current.clear();
    } else {
      current.push_back(source[i]);
    }
  }
  const std::string trimmed = trim(current);
  if (!trimmed.empty()) {
    result.push_back(trimmed);
  }
  return result;
}


