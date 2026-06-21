#ifndef WORD_HPP
#define WORD_HPP
#include "CuckooHashTable.hpp"
#include <stdexcept>
#include "../common/list.hpp"
#include <string>

namespace lachugin
{
  class Word
  {
  public:
    Word();
    Word(const std::string& translation);

    void addTranslation(const std::string& translation);
    bool hasTranslation(const std::string& translation) const;
    List< std::string >& getTranslations();
    const List< std::string >& getTranslations() const;
    void setInterpretation(const std::string& interpretation);
    void removeInterpretation();
    bool hasInterpretation() const;
    const std::string& getInterpretation() const;
    void setPartOfSpeech(const std::string& partOfSpeech);
    const std::string& getPartOfSpeech() const;
  private:
    List< std::string > translations_;
    std::string interpretation_;
    std::string partOfSpeech_;
  };

}

#endif

