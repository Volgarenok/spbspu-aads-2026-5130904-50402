#include "operations.hpp"

namespace
{
  using Groups = pozdnyakov::List< pozdnyakov::detail::PosGroup >;

  struct CopyMatchingPos
  {
    pozdnyakov::AvlDictionary &target_;
    const std::string &partOfSpeech_;

    void operator()(const std::string &engWord, const Groups &groups) const
    {
      for (auto group = groups.cbegin(); group != groups.cend(); ++group) {
        if ((*group).partOfSpeech_ != partOfSpeech_) {
          continue;
        }
        for (auto word = (*group).rusWords_.cbegin(); word != (*group).rusWords_.cend(); ++word) {
          target_.addWord(engWord, *word, partOfSpeech_);
        }
      }
    }
  };

  struct CopyAll
  {
    pozdnyakov::AvlDictionary &target_;

    void operator()(const std::string &engWord, const Groups &groups) const
    {
      for (auto group = groups.cbegin(); group != groups.cend(); ++group) {
        for (auto word = (*group).rusWords_.cbegin(); word != (*group).rusWords_.cend(); ++word) {
          target_.addWord(engWord, *word, (*group).partOfSpeech_);
        }
      }
    }
  };

  struct CopyCommon
  {
    pozdnyakov::AvlDictionary &target_;
    const pozdnyakov::DictRefs &sources_;

    bool presentInAll(const std::string &engWord) const
    {
      for (std::size_t i = 1; i < sources_.size(); ++i) {
        if (!sources_[i]->contains(engWord)) {
          return false;
        }
      }
      return true;
    }

    bool translatedInAll(const std::string &engWord, const std::string &rusWord,
      const std::string &partOfSpeech) const
    {
      for (std::size_t i = 1; i < sources_.size(); ++i) {
        if (!sources_[i]->containsTranslation(engWord, rusWord, partOfSpeech)) {
          return false;
        }
      }
      return true;
    }

    void operator()(const std::string &engWord, const Groups &groups) const
    {
      if (!presentInAll(engWord)) {
        return;
      }
      for (auto group = groups.cbegin(); group != groups.cend(); ++group) {
        const std::string &partOfSpeech = (*group).partOfSpeech_;
        for (auto word = (*group).rusWords_.cbegin(); word != (*group).rusWords_.cend(); ++word) {
          if (translatedInAll(engWord, *word, partOfSpeech)) {
            target_.addWord(engWord, *word, partOfSpeech);
          }
        }
      }
    }
  };

  struct CopyUnique
  {
    pozdnyakov::AvlDictionary &target_;
    const pozdnyakov::DictRefs &sources_;

    void operator()(const std::string &engWord, const Groups &groups) const
    {
      for (std::size_t i = 1; i < sources_.size(); ++i) {
        if (sources_[i]->contains(engWord)) {
          return;
        }
      }
      const CopyAll copier{ target_ };
      copier(engWord, groups);
    }
  };
}

void pozdnyakov::filterDictionary(AvlDictionary &target, const AvlDictionary &source,
  const std::string &partOfSpeech)
{
  source.traverse(CopyMatchingPos{ target, partOfSpeech });
}

void pozdnyakov::mergeDictionaries(AvlDictionary &target, const DictRefs &sources)
{
  for (std::size_t i = 0; i < sources.size(); ++i) {
    sources[i]->traverse(CopyAll{ target });
  }
}

void pozdnyakov::intersectDictionaries(AvlDictionary &target, const DictRefs &sources)
{
  if (sources.empty()) {
    return;
  }
  sources[0]->traverse(CopyCommon{ target, sources });
}

void pozdnyakov::diffDictionaries(AvlDictionary &target, const DictRefs &sources)
{
  if (sources.empty()) {
    return;
  }
  sources[0]->traverse(CopyUnique{ target, sources });
}
