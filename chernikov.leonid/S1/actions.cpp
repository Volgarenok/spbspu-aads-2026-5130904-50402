#include "actions.hpp"
#include "LIter.hpp"
#include "LCIter.hpp"
#include <istream>
#include <string>
#include <cctype>
#include <limits>

namespace chernikov {

  List< Sequence > parser(std::istream &in)
  {
    List< Sequence > result;
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
        continue;
      size_t pos = 0;
      size_t len = line.length();
      while (pos < len && std::isspace(line[pos]))
        ++pos;
      if (pos >= len)
        continue;
      std::string name;
      while (pos < len && std::isalpha(line[pos]))
      {
        name += line[pos];
        ++pos;
      }
      if (name.empty())
        continue;
      List< size_t > numbers;
      while (pos < len)
      {
        while (pos < len && std::isspace(line[pos]))
          ++pos;
        if (pos >= len)
          break;
        if (!std::isdigit(line[pos]))
          break;
        size_t value = 0;
        while (pos < len && std::isdigit(line[pos]))
        {
          value = value * 10 + (line[pos] - '0');
          ++pos;
        }
        numbers.push_back(value);
      }
      if (result.empty())
      {
        result.add({name, numbers});
      } else
      {
        auto last = result.begin();
        auto prev = last;
        while (last != result.end())
        {
          prev = last;
          ++last;
        }
        result.insert_after(prev, {name, numbers});
      }
    }
    return result;
  }
  void print_names(std::ostream &out, const List< Sequence > &sequences)
  {
    if (sequences.empty())
    {
      return;
    }
    bool first_element = true;
    for (auto it = sequences.begin(); it != sequences.end(); ++it)
    {
      if (!first_element)
      {
        out << " ";
      }
      out << it->first;
      first_element = false;
    }
    out << "\n";
  }
  size_t max_sequence_length(const List< Sequence > &sequences)
  {
    size_t max_len = 0;
    for (auto it = sequences.begin(); it != sequences.end(); ++it)
    {
      size_t current_len = it->second.size();
      if (current_len > max_len)
      {
        max_len = current_len;
      }
    }
    return max_len;
  }
  void print_transposed(std::ostream &out, const List< Sequence > &sequences)
  {
    if (sequences.empty())
    {
      return;
    }
    size_t max_len = max_sequence_length(sequences);
    for (size_t i = 0; i < max_len; ++i)
    {
      bool first = true;
      for (auto seq_it = sequences.begin(); seq_it != sequences.end(); ++seq_it)
      {
        const List< size_t > &numbers = seq_it->second;
        if (i < numbers.size())
        {
          auto num_it = numbers.begin();
          for (size_t j = 0; j < i; ++j)
          {
            ++num_it;
          }

          if (!first)
          {
            out << " ";
          }
          out << *num_it;
          first = false;
        }
      }
      out << "\n";
    }
  }
  void print_sums(std::ostream &out, const List< Sequence > &sequences)
  {
    if (sequences.empty())
    {
      out << "0\n";
      return;
    }
    size_t max_len = max_sequence_length(sequences);
    if (max_len == 0)
    {
      out << "0\n";
      return;
    }
    bool overflow = false;
    for (size_t i = 0; i < max_len; ++i)
    {
      size_t sum = 0;
      bool has_numbers = false;
      for (auto seq_it = sequences.begin(); seq_it != sequences.end(); ++seq_it)
      {
        const List< size_t > &numbers = seq_it->second;
        if (i < numbers.size())
        {
          auto num_it = numbers.begin();
          for (size_t j = 0; j < i; ++j)
          {
            ++num_it;
          }
          if (sum > std::numeric_limits< size_t >::max() - *num_it)
          {
            overflow = true;
            break;
          }
          sum += *num_it;
          has_numbers = true;
        }
      }
      if (has_numbers && !overflow)
      {
        if (i > 0)
        {
          out << " ";
        }
        out << sum;
      }
    }
    out << "\n";
    if (overflow)
    {
      throw std::overflow_error("Sum overflow");
    }
  }
}
