#include "WordbookManager.hpp"

namespace lachugin
{

  void WordbookManager::createWordbook(const std::string& name)
  {
    if (wordbooks_.has(name))
    {
      throw std::logic_error("Wordbook already exists");
    }
    wordbooks_.add(name, Wordbook(name));
  }

  void WordbookManager::deleteWordbook(const std::string& name)
  {
    if (!wordbooks_.has(name))
    {
      throw std::logic_error("Wordbook does not exist");
    }
    wordbooks_.drop(name);
  }

  bool WordbookManager::hasWordbook(const std::string& name) const
  {
    return wordbooks_.has(name);
  }

  Wordbook& WordbookManager::getWordbook(const std::string& name)
  {
    if (!wordbooks_.has(name))
    {
      throw std::logic_error("Wordbook does not exist");
    }
    return wordbooks_.get(name);
  }

  const Wordbook& WordbookManager::getWordbook(const std::string& name) const
  {
    if (!wordbooks_.has(name))
    {
      throw std::logic_error("Wordbook does not exist");
    }
    return wordbooks_.get(name);
  }

  void WordbookManager::merge(const std::string& frst, const std::string& scnd, const std::string& newName)
  {

    if (!hasWordbook(frst))
    {
      throw std::logic_error("Wordbook does not exist");
    }

    if (!hasWordbook(scnd))
    {
      throw std::logic_error("Wordbook does not exist");
    }
    createWordbook(newName);


    Wordbook& result = getWordbook(newName);
    Wordbook& wb1 = getWordbook(frst);
    Wordbook& wb2 = getWordbook(scnd);
    for (iter_type it = wb1.begin(); it != wb1.end(); ++it)
    {
      result.addWordObject(it->first, it->second);
    }

    for (iter_type it = wb2.begin(); it != wb2.end(); ++it)
    {
      if (!result.hasWord(it->first))
      {
        result.addWordObject(
          it->first,
          it->second
        );
      }

    }
  }

}

