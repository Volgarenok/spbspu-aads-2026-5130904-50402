#include "output.hpp"
#include <ostream>

namespace
{
  using Groups = pozdnyakov::List< pozdnyakov::detail::PosGroup >;

  void printWords(std::ostream &out, const pozdnyakov::List< std::string > &words)
  {
    bool isFirst = true;
    for (auto word = words.cbegin(); word != words.cend(); ++word) {
      if (!isFirst) {
        out << ", ";
      }
      out << *word;
      isFirst = false;
    }
  }

  std::size_t countGroups(const Groups &groups)
  {
    std::size_t total = 0;
    for (auto group = groups.cbegin(); group != groups.cend(); ++group) {
      ++total;
    }
    return total;
  }

  struct PrintWordLines
  {
    std::ostream &out_;

    void operator()(const std::string &engWord, const Groups &groups) const
    {
      for (auto group = groups.cbegin(); group != groups.cend(); ++group) {
        out_ << engWord << " [" << (*group).partOfSpeech_ << "]: ";
        printWords(out_, (*group).rusWords_);
        out_ << "\n";
      }
    }
  };

  struct CollectReverse
  {
    const std::string &rusWord_;
    std::string result_;
    bool isFirst_;

    void operator()(const std::string &engWord, const Groups &groups)
    {
      for (auto group = groups.cbegin(); group != groups.cend(); ++group) {
        for (auto word = (*group).rusWords_.cbegin(); word != (*group).rusWords_.cend(); ++word) {
          if (*word == rusWord_) {
            if (!isFirst_) {
              result_ += ", ";
            }
            result_ += engWord;
            isFirst_ = false;
            return;
          }
        }
      }
    }
  };
}

void pozdnyakov::printTranslation(std::ostream &out, const AvlDictionary &dict, const std::string &engWord)
{
  const List< detail::PosGroup > * const groups = dict.groupsOf(engWord);
  if (groups == nullptr) {
    out << "<NOT FOUND: word '" << engWord << "' is not in the dictionary>\n";
    return;
  }

  if (countGroups(*groups) == 1) {
    out << engWord << " [" << (*groups).front().partOfSpeech_ << "]: ";
    printWords(out, (*groups).front().rusWords_);
    out << "\n";
    return;
  }

  out << engWord << ":\n";
  for (auto group = groups->cbegin(); group != groups->cend(); ++group) {
    out << "[" << (*group).partOfSpeech_ << "]: ";
    printWords(out, (*group).rusWords_);
    out << "\n";
  }
}

void pozdnyakov::printDictionary(std::ostream &out, const AvlDictionary &dict, const std::string &name)
{
  if (dict.empty()) {
    out << "<DICT: " << name << ", WORDS: 0>\n";
    return;
  }
  out << "<DICTIONARY: " << name << ">\n";
  dict.traverse(PrintWordLines{ out });
  out << "<TOTAL WORDS: " << dict.size() << ">\n";
}

void pozdnyakov::printReverse(std::ostream &out, const AvlDictionary &dict, const std::string &rusWord)
{
  const CollectReverse collector = dict.traverse(CollectReverse{ rusWord, std::string(), true });
  out << "<REVERSE '" << rusWord << "'>: ";
  if (collector.isFirst_) {
    out << "<EMPTY>\n";
  } else {
    out << collector.result_ << "\n";
  }
}

void pozdnyakov::printStats(std::ostream &out, const AvlDictionary &dict, const std::string &name)
{
  out << "<STATS " << name << ": " << dict.size() << " words, " << dict.translationCount()
      << " translations>\n";
}
