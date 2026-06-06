#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <string>
#include "HashTable.hpp"
#include "../common/Vec.hpp"

namespace sogdanov {

  struct Translation {
    std::string russian;
    std::string pos;
    std::string form;
  };

  class Dictionary {
  public:
    Dictionary();
    Dictionary(const std::string& name);

    const std::string& getName() const;
    std::size_t getWordsCount() const;

    void addWord(const std::string& eng, const std::string& rus,
                 const std::string& pos, const std::string& form);
    void removeWord(const std::string& eng);

    const sogdanov::Vector< sogdanov::Translation >* translate(const std::string& eng) const;
    sogdanov::Vector< HashTable< std::string, sogdanov::Vector< sogdanov::Translation > >::Entry > getEntries() const;

  private:
    std::string name_;
    HashTable< std::string, sogdanov::Vector< sogdanov::Translation > > data_;
  };

}

#endif
