#ifndef TEXT_HPP
#define TEXT_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <list.hpp>

namespace samarin {

  using Line = List< std::string >;
  using Text = List< Line >;

  struct Position {
    std::size_t line;
    std::size_t word;

    bool operator<(const Position& other) const
    {
      return (line != other.line) ? (line < other.line) : (word < other.word);
    }

    bool operator==(const Position& other) const
    {
      return line == other.line && word == other.word;
    }
  };

  Text parseText(std::istream& in);
  void writeText(std::ostream& out, const Text& text);

}

#endif
