#include "Commands.hpp"
#include <fstream>
#include <sstream>

namespace detail {

  bool isValidPosForm(const std::string& pos, const std::string& form)
  {
    if (pos == "noun") {
      return form == "singular" || form == "plural";
    } else if (pos == "verb") {
      return form == "infinitive" || form == "past" || form == "present" || form == "gerund";
    } else if (pos == "adj" || pos == "adv") {
      return form == "base" || form == "comparative" || form == "superlative";
    } else if (pos == "unknown") {
      return form == "unknown";
    }
    return false;
  }

  std::string cleanWord(const std::string& word)
  {
    std::string result;
    for (std::size_t i = 0; i < word.length(); ++i) {
      if (std::isalpha(word[i])) {
        result += static_cast< char >(std::tolower(word[i]));
      }
    }
    return result;
  }

}

void sogdanov::createDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  in >> name;
  if (dicts.find(name) != nullptr) {
    throw std::logic_error("Dictionary already exists");
  }
  dicts.insert(name, Dictionary(name));
  out << "<DICT: " << name << ", WORDS: 0>\n";
}

void sogdanov::showDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  in >> name;
  Dictionary& dict = dicts.at(name);
  out << "<DICT: " << dict.getName() << ", WORDS: " << dict.getWordsCount() << ">\n";

  Vector< HashTable< std::string, Vector< Translation > >::Entry > entries = dict.getEntries();
  for (std::size_t i = 0; i < entries.getSize(); ++i) {
    for (std::size_t j = 0; j < entries[i].value.getSize(); ++j) {
      const Translation& t = entries[i].value[j];
      out << entries[i].key << " (" << t.pos << ", " << t.form << "): " << t.russian << "\n";
    }
  }
}

void sogdanov::dropDict(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  in >> name;
  dicts.at(name);
  dicts.remove(name);
}

void sogdanov::addWord(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  std::string eng;
  std::string rus;
  std::string pos;
  std::string form;
  in >> name >> eng >> rus >> pos >> form;

  if (!detail::isValidPosForm(pos, form)) {
    throw std::logic_error("Invalid part of speech or form");
  }
  Dictionary& dict = dicts.at(name);
  dict.addWord(eng, rus, pos, form);
}

void sogdanov::removeWord(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  std::string eng;
  in >> name >> eng;
  Dictionary& dict = dicts.at(name);

  if (dict.translate(eng) == nullptr) {
    throw std::logic_error("Word not found");
  }
  dict.removeWord(eng);
}

void sogdanov::translateWord(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  std::string eng;
  in >> name >> eng;
  Dictionary& dict = dicts.at(name);

  const Vector< Translation >* trans_list = dict.translate(eng);
  if (trans_list == nullptr) {
    throw std::logic_error("Word missing");
  }
  out << eng << ":\n";
  for (std::size_t i = 0; i < trans_list->getSize(); ++i) {
    out << "  (" << (*trans_list)[i].pos << ", " << (*trans_list)[i].form << ") " << (*trans_list)[i].russian << "\n";
  }
}

void sogdanov::reverseTranslate(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  std::string rus;
  in >> name >> rus;
  Dictionary& dict = dicts.at(name);

  out << rus << ":\n";
  bool found = false;
  Vector< HashTable< std::string, Vector< Translation > >::Entry > entries = dict.getEntries();

  for (std::size_t i = 0; i < entries.getSize(); ++i) {
    for (std::size_t j = 0; j < entries[i].value.getSize(); ++j) {
      if (entries[i].value[j].russian == rus) {
        out << "  " << entries[i].key << " (" << entries[i].value[j].pos << ", "
            << entries[i].value[j].form << ")\n";
        found = true;
      }
    }
  }
  if (!found) {
    out << "  <NO MATCHES>\n";
  }
}

void sogdanov::findByPos(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  std::string pos;
  in >> name >> pos;
  Dictionary& dict = dicts.at(name);

  if (pos != "noun" && pos != "verb" && pos != "adj" && pos != "adv") {
    throw std::logic_error("Invalid pos");
  }

  out << "<PART OF SPEECH: " << pos << ">\n";
  Vector< HashTable< std::string, Vector< Translation > >::Entry > entries = dict.getEntries();

  for (std::size_t i = 0; i < entries.getSize(); ++i) {
    for (std::size_t j = 0; j < entries[i].value.getSize(); ++j) {
      if (entries[i].value[j].pos == pos) {
        out << entries[i].key << " (" << entries[i].value[j].form << "): "
            << entries[i].value[j].russian << "\n";
      }
    }
  }
}

void sogdanov::mergeDicts(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts)
{
  std::string new_name;
  std::string name1;
  std::string name2;
  in >> new_name >> name1 >> name2;

  if (dicts.find(new_name) != nullptr) {
    throw std::logic_error("New dictionary already exists");
  }
  Dictionary& dict1 = dicts.at(name1);
  Dictionary& dict2 = dicts.at(name2);

  Dictionary new_dict(new_name);
  Vector< HashTable< std::string, Vector< Translation > >::Entry > entries1 = dict1.getEntries();
  for (std::size_t i = 0; i < entries1.getSize(); ++i) {
    for (std::size_t j = 0; j < entries1[i].value.getSize(); ++j) {
      new_dict.addWord(entries1[i].key, entries1[i].value[j].russian,
                       entries1[i].value[j].pos, entries1[i].value[j].form);
    }
  }

  Vector< HashTable< std::string, Vector< Translation > >::Entry > entries2 = dict2.getEntries();
  for (std::size_t i = 0; i < entries2.getSize(); ++i) {
    for (std::size_t j = 0; j < entries2[i].value.getSize(); ++j) {
      new_dict.addWord(entries2[i].key, entries2[i].value[j].russian,
                       entries2[i].value[j].pos, entries2[i].value[j].form);
    }
  }
  dicts.insert(new_name, new_dict);
}

void sogdanov::unionDicts(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  mergeDicts(in, out, dicts);
}

void sogdanov::saveDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  std::string filename;
  in >> name >> filename;
  Dictionary& dict = dicts.at(name);

  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::logic_error("Cannot open file");
  }

  Vector< HashTable< std::string, Vector< Translation > >::Entry > entries = dict.getEntries();
  for (std::size_t i = 0; i < entries.getSize(); ++i) {
    for (std::size_t j = 0; j < entries[i].value.getSize(); ++j) {
      file << entries[i].key << " " << entries[i].value[j].russian << " "
           << entries[i].value[j].pos << " " << entries[i].value[j].form << "\n";
    }
  }
  out << "<SAVED: " << name << " -> " << filename << ">\n";
}

void sogdanov::loadDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  std::string filename;
  in >> name >> filename;

  if (dicts.find(name) != nullptr) {
    throw std::logic_error("Dictionary already exists");
  }

  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::logic_error("Cannot open file");
  }

  Dictionary new_dict(name);
  std::string eng;
  std::string rus;
  std::string pos;
  std::string form;
  while (file >> eng >> rus >> pos >> form) {
    new_dict.addWord(eng, rus, pos, form);
  }
  dicts.insert(name, new_dict);
  out << "<LOADED: " << filename << " -> " << name << ">\n";
}

void sogdanov::listDicts(std::istream&, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  Vector< std::string > keys = dicts.getKeys();
  out << "<DICTIONARIES: " << keys.getSize() << ">\n";
  for (std::size_t i = 0; i < keys.getSize(); ++i) {
    out << keys[i] << ": " << dicts.at(keys[i]).getWordsCount() << " words\n";
  }
}

void sogdanov::statsDict(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string name;
  in >> name;
  Dictionary& dict = dicts.at(name);

  std::size_t total_words = dict.getWordsCount();
  std::size_t total_translations = 0;
  std::size_t count_noun = 0;
  std::size_t count_verb = 0;
  std::size_t count_adj = 0;
  std::size_t count_adv = 0;

  Vector< HashTable< std::string, Vector< Translation > >::Entry > entries = dict.getEntries();
  for (std::size_t i = 0; i < entries.getSize(); ++i) {
    total_translations += entries[i].value.getSize();
    for (std::size_t j = 0; j < entries[i].value.getSize(); ++j) {
      if (entries[i].value[j].pos == "noun") ++count_noun;
      else if (entries[i].value[j].pos == "verb") ++count_verb;
      else if (entries[i].value[j].pos == "adj") ++count_adj;
      else if (entries[i].value[j].pos == "adv") ++count_adv;
    }
  }

  out << "<STATISTICS: " << name << ">\n";
  out << "Total English words: " << total_words << "\n";
  out << "Total translations: " << total_translations << "\n";
  out << "Parts of speech:\n";
  if (count_noun > 0) out << "  noun: " << count_noun << "\n";
  if (count_verb > 0) out << "  verb: " << count_verb << "\n";
  if (count_adj > 0) out << "  adj: " << count_adj << "\n";
  if (count_adv > 0) out << "  adv: " << count_adv << "\n";
}

void sogdanov::unknownWords(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string dict_name;
  std::string filename;
  std::string new_name;
  in >> dict_name >> filename >> new_name;

  if (dicts.find(new_name) != nullptr) {
    throw std::logic_error("New dictionary already exists");
  }
  Dictionary& dict = dicts.at(dict_name);

  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::logic_error("Cannot open file");
  }

  Dictionary result_dict(new_name);
  Vector< std::string > known;
  Vector< std::string > unknown;

  std::string raw_word;
  while (file >> raw_word) {
    std::string word = detail::cleanWord(raw_word);
    if (word.empty()) continue;

    if (dict.translate(word) != nullptr) {
      bool found_in_known = false;
      for (std::size_t i = 0; i < known.getSize(); ++i) {
        if (known[i] == word) found_in_known = true;
      }
      if (!found_in_known) known.pushBack(word);
    } else {
      bool found_in_unknown = false;
      for (std::size_t i = 0; i < unknown.getSize(); ++i) {
        if (unknown[i] == word) found_in_unknown = true;
      }
      if (!found_in_unknown) {
        unknown.pushBack(word);
        result_dict.addWord(word, "???", "unknown", "unknown");
      }
    }
  }

  dicts.insert(new_name, result_dict);
  out << "<TEXT ANALYSIS>\n";
  out << "Known words: " << known.getSize() << " (";
  for (std::size_t i = 0; i < known.getSize(); ++i) {
    out << known[i] << (i + 1 == known.getSize() ? "" : ", ");
  }
  out << ")\n";

  out << "Unknown words: " << unknown.getSize() << " (";
  for (std::size_t i = 0; i < unknown.getSize(); ++i) {
    out << unknown[i] << (i + 1 == unknown.getSize() ? "" : ", ");
  }
  out << ")\n";
}

void sogdanov::demistifyWords(std::istream& in, std::ostream& out, HashTable< std::string, Dictionary >& dicts)
{
  std::string unk_name;
  std::string ref_name;
  std::string new_name;
  in >> unk_name >> ref_name >> new_name;

  if (dicts.find(new_name) != nullptr) {
    throw std::logic_error("New dictionary already exists");
  }
  Dictionary& unk_dict = dicts.at(unk_name);
  Dictionary& ref_dict = dicts.at(ref_name);

  Dictionary result_dict(new_name);
  std::size_t found_count = 0;
  std::size_t total_count = unk_dict.getWordsCount();

  Vector< HashTable< std::string, Vector< Translation > >::Entry > entries = unk_dict.getEntries();
  for (std::size_t i = 0; i < entries.getSize(); ++i) {
    const std::string& word = entries[i].key;
    const Vector< Translation >* ref_trans = ref_dict.translate(word);
    if (ref_trans != nullptr) {
      ++found_count;
      for (std::size_t j = 0; j < ref_trans->getSize(); ++j) {
        result_dict.addWord(word, (*ref_trans)[j].russian, (*ref_trans)[j].pos, (*ref_trans)[j].form);
      }
    }
  }

  dicts.insert(new_name, result_dict);
  out << "<DEMISTIFIED: " << found_count << " of " << total_count << " words found>\n";
}

void sogdanov::complementDicts(std::istream& in, std::ostream&, HashTable< std::string, Dictionary >& dicts)
{
  std::string new_name;
  std::string name1;
  std::string name2;
  in >> new_name >> name1 >> name2;

  if (dicts.find(new_name) != nullptr) {
    throw std::logic_error("New dictionary already exists");
  }
  Dictionary& dict1 = dicts.at(name1);
  Dictionary& dict2 = dicts.at(name2);

  Dictionary result_dict(new_name);
  Vector< HashTable< std::string, Vector< Translation > >::Entry > entries1 = dict1.getEntries();
  for (std::size_t i = 0; i < entries1.getSize(); ++i) {
    if (dict2.translate(entries1[i].key) == nullptr) {
      for (std::size_t j = 0; j < entries1[i].value.getSize(); ++j) {
        result_dict.addWord(entries1[i].key, entries1[i].value[j].russian,
                            entries1[i].value[j].pos, entries1[i].value[j].form);
      }
    }
  }
  dicts.insert(new_name, result_dict);
}
