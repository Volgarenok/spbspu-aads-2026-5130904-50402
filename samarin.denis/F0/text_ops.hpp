#ifndef TEXT_OPS_HPP
#define TEXT_OPS_HPP

#include <cstddef>
#include <string>
#include "text.hpp"

namespace samarin {

  void replaceWord(Text& text, std::size_t line, std::size_t word, const std::string& value);
  void swapWords(Text& text, std::size_t line1, std::size_t word1, std::size_t line2, std::size_t word2);
  void insertLine(Text& text, std::size_t pos, const Line& line);
  void removeLine(Text& text, std::size_t pos);

  Text concat(const Text& first, const Text& second);
  Text concatLines(const Text& first, const Text& second);
  Text repeatVertical(const Text& text, std::size_t times);
  Text repeatHorizontal(const Text& text, std::size_t times);
  Text interleaveLines(const Text& first, const Text& second);
  Text interleaveWords(const Text& first, const Text& second);
  Text reverseLines(const Text& text);
  Text reverseWords(const Text& text);
  Text transpose(const Text& text);

}

#endif
