#ifndef WORDBOOKMANAGER_HPP
#define WORDBOOKMANAGER_HPP

#include "CuckooHashTable.hpp"
#include "Wordbook.hpp"
namespace lachugin
{
  class WordbookManager
  {
  public:
    using iter_type = CuckooIter< std::string, Word, DefaultHash1< std::string >, DefaultHash2< std::string >,
      std::equal_to< std::string > >;
    void createWordbook(const std::string& name);
    void deleteWordbook(const std::string& name);
    bool hasWordbook(const std::string& name) const;
    Wordbook& getWordbook(const std::string& name);

    const Wordbook& getWordbook(const std::string& name) const;
    void merge(const std::string& frst,const std::string& scn, const std::string& newName);
  private:
    CuckooHashTable< std::string, Wordbook, DefaultHash1< std::string >, DefaultHash2< std::string >,
      std::equal_to< std::string > > wordbooks_;

  };


}

#endif
