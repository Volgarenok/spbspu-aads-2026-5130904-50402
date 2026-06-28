#include "text_index.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <list.hpp>
#include "cuckoo_table.hpp"
#include "list_utils.hpp"

namespace samarin {

  namespace {
    struct PositionHash {
      std::size_t operator()(const Position& pos) const
      {
        const std::size_t spread = 1000003ull;
        return detail::cuckooMix(pos.line * spread + pos.word);
      }
    };

    using ForwardTable = CuckooTable< Position, std::string, PositionHash >;
  }

}

void samarin::TextIndex::build(const Text& text)
{
  PostingsTable freshPostings;
  List< std::size_t > freshLengths;
  LIter< std::size_t > lenTail = freshLengths.before_begin();
  std::size_t lineNo = 0;
  for (LCIter< Line > line = text.cbegin(); line != text.cend(); ++line, ++lineNo) {
    std::size_t col = 0;
    for (LCIter< std::string > word = line->cbegin(); word != line->cend(); ++word, ++col) {
      const Position pos = { lineNo, col };
      freshPostings[*word].push_front(pos);
    }
    lenTail = freshLengths.insert_after(lenTail, col);
  }
  postings_ = std::move(freshPostings);
  lineLengths_ = std::move(freshLengths);
}

std::size_t samarin::TextIndex::wordCount() const
{
  std::size_t total = 0;
  for (LCIter< std::size_t > len = lineLengths_.cbegin(); len != lineLengths_.cend(); ++len) {
    total += *len;
  }
  return total;
}

samarin::Text samarin::TextIndex::restore() const
{
  ForwardTable forward(wordCount() * 2 + 1);
  for (PostingsTable::const_iterator entry = postings_.cbegin(); entry != postings_.cend(); ++entry) {
    for (LCIter< Position > pos = entry->second.cbegin(); pos != entry->second.cend(); ++pos) {
      forward.add(*pos, entry->first);
    }
  }
  Text text;
  LIter< Line > lineTail = text.before_begin();
  std::size_t lineNo = 0;
  for (LCIter< std::size_t > len = lineLengths_.cbegin(); len != lineLengths_.cend(); ++len, ++lineNo) {
    Line words;
    LIter< std::string > wordTail = words.before_begin();
    for (std::size_t col = 0; col < *len; ++col) {
      const Position pos = { lineNo, col };
      wordTail = words.insert_after(wordTail, forward.at(pos));
    }
    lineTail = text.insert_after(lineTail, words);
  }
  return text;
}

bool samarin::TextIndex::has(const std::string& word) const
{
  return postings_.has(word);
}

const samarin::PostingList& samarin::TextIndex::positions(const std::string& word) const
{
  return postings_.at(word);
}

std::size_t samarin::TextIndex::lineCount() const
{
  return listSize(lineLengths_);
}

const samarin::PostingsTable& samarin::TextIndex::postings() const
{
  return postings_;
}
