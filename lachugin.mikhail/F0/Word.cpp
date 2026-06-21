#include "Word.hpp"

namespace lachugin
{
  Word::Word()
  {}

  Word::Word(const std::string& translation)
  {
    translations_.pushBack(translation);
  }

  void Word::addTranslation(const std::string& translation)
  {
    translations_.pushBack(translation);
  }

  bool Word::hasTranslation(const std::string& translation) const
  {
    for (auto it = translations_.begin(); it != translations_.end(); ++it)
    {
      if (*it == translation)
      {
        return true;
      }
    }
    return false;
  }

  List< std::string >& Word::getTranslations()
  {
    return translations_;
  }

  const List< std::string >& Word::getTranslations() const
  {
    return translations_;
  }

  void Word::setInterpretation(const std::string& interpretation)
  {
    interpretation_ = interpretation;
  }

  void Word::removeInterpretation()
  {
    interpretation_.clear();
  }

  bool Word::hasInterpretation() const
  {
    return !interpretation_.empty();
  }

  const std::string& Word::getInterpretation() const
  {
    return interpretation_;
  }

  void Word::setPartOfSpeech(const std::string& partOfSpeech)
  {
    partOfSpeech_ = partOfSpeech;
  }

  const std::string& Word::getPartOfSpeech() const
  {
    return partOfSpeech_;
  }





}
