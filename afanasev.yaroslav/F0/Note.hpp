#ifndef NOTE_HPP
#define NOTE_HPP

#include "CuckooHashTable/CuckooHashTable.hpp"
#include "CuckooHashTable/CuckooHashIter.hpp"
#include "CuckooHashTable/CuckooHashFunction.hpp"
#include <top-it-vector.hpp>
#include <list.hpp>

namespace afanasev
{
  class Note
  {
  public:
    using TagSet = CuckooHashTable< std::string, bool, Hasher< std::string >,
      Hasher< std::string >, std::equal_to< std::string > >;

    void addLine(const std::string & line);
    const List< std::string > & getChildren() const;
    const Vector< std::string > & getLines() const;
    Vector< std::string > getTags() const;
    void addChild(const std::string & child);
    void addTag(const std::string & tag);
    bool hasTag(const std::string & tag) const;
    void removeTag(const std::string & tag);

  private:
    Vector< std::string > lines_;
    TagSet tags_{16};
    List< std::string > children_;
  };
}

void afanasev::Note::
removeTag(const std::string & tag)
{
  tags_.drop(tag);
}

bool afanasev::Note::
hasTag(const std::string & tag) const
{
  return tags_.has(tag);
}

void afanasev::Note::
addTag(const std::string & tag)
{
  tags_.add(tag, true);
}

void afanasev::Note::
addChild(const std::string & child)
{
  children_.pushFront(child);
}

afanasev::Vector< std::string > afanasev::Note::
getTags() const
{
  Vector< std::string > result;
  for (TagSet::HCIter it = tags_.cbegin(); it != tags_.cend(); ++it)
  {
    result.pushBack((*it).first);
  }
  return result;
}

const afanasev::Vector< std::string > & afanasev::Note::
getLines() const
{
  return lines_;
}

const afanasev::List< std::string > & afanasev::Note::
getChildren() const
{
  return children_;
}

void afanasev::Note::
addLine(const std::string & line)
{
  lines_.pushBack(line);
}

#endif
