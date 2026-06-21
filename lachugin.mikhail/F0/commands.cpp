#include "commands.hpp"


namespace lachugin
{
  void cmdMake(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string name;
    in >> name;
    manager.createWordbook(name);
    out << "Wordbook \"" << name << "\" was created\n";
  }

  void cmdDelete(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string name;
    in >> name;
    manager.deleteWordbook(name);
    out << "Wordbook \"" << name << "\" was deleted\n";
  }

  void cmdAddWord(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    std::string eng;
    std::string rus;
    in >> wordbook >> eng >> rus;
    manager.getWordbook(wordbook).addWord(eng, rus);
    out << "Word " << eng << " was added in " << wordbook << "\n";
  }

  void cmdRemove( std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    std::string word;
    in >> wordbook >> word;
    manager.getWordbook(wordbook).removeWord(word);
    out << "Word " << word << " was removed from " << wordbook << ".\n";
  }

  void cmdAddAnother( std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    std::string eng;
    std::string rus;
    in >> wordbook >> eng >> rus;
    manager.getWordbook(wordbook).addAnotherTranslation(eng, rus);

    out<< "Added another translate of " << eng << " in " << wordbook << ".\n";
  }


  void cmdAddInterpretation(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    std::string word;
    in >> wordbook >> word;
    std::string interpretation;
    std::getline(in >> std::ws, interpretation);
    manager.getWordbook(wordbook).addInterpretation(word, interpretation);

    out << "Added interpretation of " << word<< " in " << wordbook << ".\n";
  }


  void cmdRemoveInterpretation(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    std::string word;
    in >> wordbook >> word;
    manager.getWordbook(wordbook).removeInterpretation(word);

    out << "Interpretation of " << word << " in " << wordbook << " was removed\n";
  }

  void cmdDivWord(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    std::string word;
    std::string pos;
    in >> wordbook >> word >> pos;
    manager.getWordbook(wordbook).assignPartOfSpeech(word, pos);

    out << "Assign " << word<< " to " << pos << "s.\n";
  }

  void cmdShow(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    in >> wordbook;
    manager.getWordbook(wordbook).show(out);
  }

  void cmdShowTranslate(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    std::string language;
    std::string word;
    in >> wordbook >> language >> word;
    manager.getWordbook(wordbook).showTranslate(out, language, word);
  }

  void cmdShowStartsWith(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    char letter;
    in >> wordbook >> letter;
    manager.getWordbook(wordbook).showStartsWith(out, letter);
  }

  void cmdShowPartsOfSpeech(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string wordbook;
    std::string part;

    in >> wordbook >> part;
    manager.getWordbook(wordbook).showPartsOfSpeech(out, part);
  }

  void cmdMerge(std::istream& in, std::ostream& out, WordbookManager& manager)
  {
    std::string result;
    std::string first;
    std::string second;
    in >> result >> first >> second;
    manager.merge(result, first, second);

    out << "Wordbooks " << first << " and " << second << " were merged into " << result << ".\n";
  }

}
