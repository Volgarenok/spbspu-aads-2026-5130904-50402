#ifndef WORDBOOK_HPP
#define WORDBOOK_HPP

#include "Word.hpp"

namespace lachugin
{
  class Wordbook
  {
  public:
    using table_type = CuckooHashTable< std::string, Word, DefaultHash1 <std::string >,
      DefaultHash2< std::string >, std::equal_to< std::string > >;
    Wordbook();
    Wordbook(const std::string& name);
    const std::string& getName() const;

    void addWord(const std::string& eng, const std::string& rus);
    void removeWord(const std::string& eng);
    void addAnotherTranslation(const std::string& eng, const std::string& rus);
    void addInterpretation(const std::string& eng, const std::string& interpretation);
    void removeInterpretation(const std::string& eng);
    void assignPartOfSpeech(const std::string& eng, const std::string& pos);
    bool hasWord(const std::string& eng) const;
    Word& getWord(const std::string& eng);
    const Word& getWord(const std::string& eng) const;

    void addWordObject(const std::string& eng, const Word& word);
    void showStartsWith(std::ostream& out, char letter);
    void showPartsOfSpeech(std::ostream& out, const std::string& pos);
    void showTranslate(std::ostream& out, const std::string& lang, const std::string& word);
    void show(std::ostream& out);

    CuckooIter< std::string, Word, DefaultHash1< std::string >, DefaultHash2< std::string >,
      std::equal_to< std::string > > begin();
    CuckooIter< std::string, Word, DefaultHash1< std::string >, DefaultHash2< std::string >,
      std::equal_to< std::string > > end();
  private:

    void showTranslateEng(std::ostream& out, const std::string& word);
    void showTranslateRus(std::ostream& out,const std::string& word);
    std::string name_;
    table_type words_;
  };
}


#endif

