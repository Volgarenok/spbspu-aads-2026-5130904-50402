#include "text.hpp"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

samarin::Text samarin::parseText(std::istream& in)
{
  Text text;
  LIter< Line > lineTail = text.before_begin();
  std::string raw;
  while (std::getline(in, raw)) {
    if (!raw.empty() && raw.back() == '\r') {
      raw.pop_back();
    }
    Line words;
    LIter< std::string > wordTail = words.before_begin();
    std::istringstream stream(raw);
    std::string word;
    while (stream >> word) {
      wordTail = words.insert_after(wordTail, word);
    }
    lineTail = text.insert_after(lineTail, words);
  }
  return text;
}

void samarin::writeText(std::ostream& out, const Text& text)
{
  for (LCIter< Line > line = text.cbegin(); line != text.cend(); ++line) {
    bool first = true;
    for (LCIter< std::string > word = line->cbegin(); word != line->cend(); ++word) {
      if (!first) {
        out << " ";
      }
      out << *word;
      first = false;
    }
    out << "\n";
  }
}
