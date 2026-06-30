#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "bstree.hpp"

using namespace chernikov;

using Dictionary = BSTree< int, std::string >;
using DictMap = BSTree< std::string, Dictionary >;

std::string trim(const std::string &s)
{
  size_t start = 0;
  while (start < s.length() && s[start] == ' ')
    ++start;
  size_t end = s.length();
  while (end > start && s[end - 1] == ' ')
    --end;
  return s.substr(start, end - start);
}

struct SplitResult
{
  std::string parts[100];
  size_t count;
};

SplitResult split(const std::string &line)
{
  SplitResult result;
  result.count = 0;

  std::string current;
  for (size_t i = 0; i < line.length(); ++i)
  {
    if (line[i] == ' ')
    {
      if (!current.empty() && result.count < 100)
      {
        result.parts[result.count++] = current;
        current.clear();
      }
    } else
    {
      current += line[i];
    }
  }
  if (!current.empty() && result.count < 100)
  {
    result.parts[result.count++] = current;
  }

  return result;
}

int string_to_int(const std::string &s)
{
  int result = 0;
  int sign = 1;
  size_t i = 0;

  if (s.length() > 0 && s[0] == '-')
  {
    sign = -1;
    i = 1;
  }

  for (; i < s.length(); ++i)
  {
    result = result * 10 + (s[i] - '0');
  }

  return result * sign;
}

void cmd_print(DictMap &dicts, const std::string &name)
{
  if (!dicts.has(name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Dictionary &dict = dicts.get(name);

  if (dict.empty())
  {
    std::cout << "<EMPTY>\n";
    return;
  }

  std::cout << name;
  for (auto it = dict.begin(); it != dict.end(); ++it)
  {
    std::pair< const int &, std::string & > p = *it;
    std::cout << " " << p.first << " " << p.second;
  }
  std::cout << "\n";
}

void cmd_complement(DictMap &dicts, const std::string &new_name, const std::string &name1, const std::string &name2)
{
  if (!dicts.has(name1) || !dicts.has(name2) || dicts.has(new_name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Dictionary result;
  Dictionary &d1 = dicts.get(name1);
  Dictionary &d2 = dicts.get(name2);

  for (auto it = d1.begin(); it != d1.end(); ++it)
  {
    std::pair< const int &, std::string & > p = *it;
    if (!d2.has(p.first))
    {
      result.push(p.first, p.second);
    }
  }

  dicts.push(new_name, result);
}

void cmd_intersect(DictMap &dicts, const std::string &new_name, const std::string &name1, const std::string &name2)
{
  if (!dicts.has(name1) || !dicts.has(name2) || dicts.has(new_name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Dictionary result;
  Dictionary &d1 = dicts.get(name1);
  Dictionary &d2 = dicts.get(name2);

  for (auto it = d1.begin(); it != d1.end(); ++it)
  {
    std::pair< const int &, std::string & > p = *it;
    if (d2.has(p.first))
    {
      result.push(p.first, p.second);
    }
  }

  dicts.push(new_name, result);
}

void cmd_union(DictMap &dicts, const std::string &new_name, const std::string &name1, const std::string &name2)
{
  if (!dicts.has(name1) || !dicts.has(name2) || dicts.has(new_name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Dictionary result;
  Dictionary &d1 = dicts.get(name1);
  Dictionary &d2 = dicts.get(name2);

  for (auto it = d1.begin(); it != d1.end(); ++it)
  {
    std::pair< const int &, std::string & > p = *it;
    result.push(p.first, p.second);
  }

  for (auto it = d2.begin(); it != d2.end(); ++it)
  {
    std::pair< const int &, std::string & > p = *it;
    if (!result.has(p.first))
    {
      result.push(p.first, p.second);
    }
  }

  dicts.push(new_name, result);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: No input file specified\n";
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Error: Cannot open file '" << argv[1] << "'\n";
    return 1;
  }

  DictMap dicts;

  std::string line;
  while (std::getline(file, line))
  {
    line = trim(line);
    if (line.empty())
      continue;

    SplitResult tokens = split(line);
    if (tokens.count < 3)
      continue;

    std::string dict_name = tokens.parts[0];
    Dictionary dict;

    for (size_t i = 1; i + 1 < tokens.count; i += 2)
    {
      int key = string_to_int(tokens.parts[i]);
      std::string value = tokens.parts[i + 1];
      dict.push(key, value);
    }

    dicts.push(dict_name, dict);
  }

  file.close();

  while (std::getline(std::cin, line))
  {
    line = trim(line);
    if (line.empty())
      continue;

    SplitResult tokens = split(line);
    if (tokens.count == 0)
      continue;

    std::string cmd = tokens.parts[0];

    if (cmd == "print" && tokens.count >= 2)
    {
      cmd_print(dicts, tokens.parts[1]);
    } else if (cmd == "complement" && tokens.count >= 4)
    {
      cmd_complement(dicts, tokens.parts[1], tokens.parts[2], tokens.parts[3]);
    } else if (cmd == "intersect" && tokens.count >= 4)
    {
      cmd_intersect(dicts, tokens.parts[1], tokens.parts[2], tokens.parts[3]);
    } else if (cmd == "union" && tokens.count >= 4)
    {
      cmd_union(dicts, tokens.parts[1], tokens.parts[2], tokens.parts[3]);
    } else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
