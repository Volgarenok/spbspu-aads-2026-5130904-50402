#include "text_ops.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <list.hpp>
#include "list_utils.hpp"
#include "stack.hpp"

namespace samarin {

  namespace {
    LIter< Line > appendLines(Text& out, LIter< Line > tail, const Text& src)
    {
      for (LCIter< Line > line = src.cbegin(); line != src.cend(); ++line) {
        tail = out.insert_after(tail, *line);
      }
      return tail;
    }

    LIter< std::string > appendWords(Line& out, LIter< std::string > tail, const Line& src)
    {
      for (LCIter< std::string > word = src.cbegin(); word != src.cend(); ++word) {
        tail = out.insert_after(tail, *word);
      }
      return tail;
    }
  }

}

void samarin::replaceWord(Text& text, std::size_t line, std::size_t word, const std::string& value)
{
  listAt(listAt(text, line), word) = value;
}

void samarin::swapWords(Text& text, std::size_t line1, std::size_t word1, std::size_t line2, std::size_t word2)
{
  std::string& first = listAt(listAt(text, line1), word1);
  std::string& second = listAt(listAt(text, line2), word2);
  std::swap(first, second);
}

void samarin::insertLine(Text& text, std::size_t pos, const Line& line)
{
  text.insert_after(iterBefore(text, pos), line);
}

void samarin::removeLine(Text& text, std::size_t pos)
{
  text.erase_after(iterBefore(text, pos));
}

samarin::Text samarin::concat(const Text& first, const Text& second)
{
  Text out;
  LIter< Line > tail = appendLines(out, out.before_begin(), first);
  appendLines(out, tail, second);
  return out;
}

samarin::Text samarin::concatLines(const Text& first, const Text& second)
{
  Text out;
  LIter< Line > tail = out.before_begin();
  LCIter< Line > left = first.cbegin();
  LCIter< Line > right = second.cbegin();
  while (left != first.cend() || right != second.cend()) {
    Line merged;
    LIter< std::string > wordTail = merged.before_begin();
    if (left != first.cend()) {
      wordTail = appendWords(merged, wordTail, *left);
      ++left;
    }
    if (right != second.cend()) {
      wordTail = appendWords(merged, wordTail, *right);
      ++right;
    }
    tail = out.insert_after(tail, merged);
  }
  return out;
}

samarin::Text samarin::repeatVertical(const Text& text, std::size_t times)
{
  Text out;
  LIter< Line > tail = out.before_begin();
  for (std::size_t i = 0; i < times; ++i) {
    tail = appendLines(out, tail, text);
  }
  return out;
}

samarin::Text samarin::repeatHorizontal(const Text& text, std::size_t times)
{
  Text out;
  LIter< Line > tail = out.before_begin();
  for (LCIter< Line > line = text.cbegin(); line != text.cend(); ++line) {
    Line repeated;
    LIter< std::string > wordTail = repeated.before_begin();
    for (std::size_t i = 0; i < times; ++i) {
      wordTail = appendWords(repeated, wordTail, *line);
    }
    tail = out.insert_after(tail, repeated);
  }
  return out;
}

samarin::Text samarin::interleaveLines(const Text& first, const Text& second)
{
  Text out;
  LIter< Line > tail = out.before_begin();
  LCIter< Line > left = first.cbegin();
  LCIter< Line > right = second.cbegin();
  while (left != first.cend() || right != second.cend()) {
    if (left != first.cend()) {
      tail = out.insert_after(tail, *left);
      ++left;
    }
    if (right != second.cend()) {
      tail = out.insert_after(tail, *right);
      ++right;
    }
  }
  return out;
}

samarin::Text samarin::interleaveWords(const Text& first, const Text& second)
{
  Text out;
  LIter< Line > tail = out.before_begin();
  const Line empty;
  LCIter< Line > left = first.cbegin();
  LCIter< Line > right = second.cbegin();
  while (left != first.cend() || right != second.cend()) {
    const Line& lineLeft = (left != first.cend()) ? *left : empty;
    const Line& lineRight = (right != second.cend()) ? *right : empty;
    Line merged;
    LIter< std::string > wordTail = merged.before_begin();
    LCIter< std::string > wordLeft = lineLeft.cbegin();
    LCIter< std::string > wordRight = lineRight.cbegin();
    while (wordLeft != lineLeft.cend() || wordRight != lineRight.cend()) {
      if (wordLeft != lineLeft.cend()) {
        wordTail = merged.insert_after(wordTail, *wordLeft);
        ++wordLeft;
      }
      if (wordRight != lineRight.cend()) {
        wordTail = merged.insert_after(wordTail, *wordRight);
        ++wordRight;
      }
    }
    tail = out.insert_after(tail, merged);
    if (left != first.cend()) {
      ++left;
    }
    if (right != second.cend()) {
      ++right;
    }
  }
  return out;
}

samarin::Text samarin::reverseLines(const Text& text)
{
  Stack< Line > stack;
  for (LCIter< Line > line = text.cbegin(); line != text.cend(); ++line) {
    stack.push(*line);
  }
  Text out;
  LIter< Line > tail = out.before_begin();
  while (!stack.empty()) {
    tail = out.insert_after(tail, stack.drop());
  }
  return out;
}

samarin::Text samarin::reverseWords(const Text& text)
{
  Text out;
  LIter< Line > tail = out.before_begin();
  for (LCIter< Line > line = text.cbegin(); line != text.cend(); ++line) {
    Stack< std::string > stack;
    for (LCIter< std::string > word = line->cbegin(); word != line->cend(); ++word) {
      stack.push(*word);
    }
    Line reversed;
    LIter< std::string > wordTail = reversed.before_begin();
    while (!stack.empty()) {
      wordTail = reversed.insert_after(wordTail, stack.drop());
    }
    tail = out.insert_after(tail, reversed);
  }
  return out;
}

samarin::Text samarin::transpose(const Text& text)
{
  List< std::size_t > widths;
  LIter< std::size_t > widthTail = widths.before_begin();
  std::size_t maxWidth = 0;
  for (LCIter< Line > line = text.cbegin(); line != text.cend(); ++line) {
    const std::size_t width = listSize(*line);
    widthTail = widths.insert_after(widthTail, width);
    if (width > maxWidth) {
      maxWidth = width;
    }
  }
  Text out;
  LIter< Line > tail = out.before_begin();
  for (std::size_t col = 0; col < maxWidth; ++col) {
    Line row;
    LIter< std::string > wordTail = row.before_begin();
    LCIter< std::size_t > width = widths.cbegin();
    for (LCIter< Line > line = text.cbegin(); line != text.cend(); ++line, ++width) {
      if (col < *width) {
        wordTail = row.insert_after(wordTail, listAt(*line, col));
      }
    }
    tail = out.insert_after(tail, row);
  }
  return out;
}
