#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <functional>
#include "CuckooHashTable/CuckooHashTable.hpp"
#include "CuckooHashTable/CuckooHashIter.hpp"
#include "CuckooHashTable/CuckooHashFunction.hpp"
#include "Note.hpp"

namespace afanasev
{
  using CmdHash = afanasev::Hasher< std::string >;

  using NoteSet = afanasev::CuckooHashTable< std::string, afanasev::Note, CmdHash,
    CmdHash, std::equal_to< std::string > >;

  using CmdFunc = void (*)(std::istream &, std::ostream &, NoteSet &);

  void deleteDepth(const std::string & title, NoteSet & ns, unsigned int depth);
  void collectAtDepth(const std::string & title, const NoteSet & ns,
    Vector< std::string > & result, unsigned int targetDepth, unsigned int currentDepth);

  void cmdCr(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdStr(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdDel(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdDelk(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdSee(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdLink(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdDeltagNote(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdSeetag(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdSeetagAnd(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdSeetagOr(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdTagRp(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdTagAddNew(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdDelTag(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdTag(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdTagDel(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdGetLiked(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdAddLinkTag(std::istream & in, std::ostream & out, NoteSet & ns);
  void cmdDelLinkTag(std::istream & in, std::ostream & out, NoteSet & ns);
}

void afanasev::collectAtDepth(const std::string & title, const afanasev::NoteSet & ns,
  afanasev::Vector< std::string > & result, unsigned int targetDepth, unsigned int currentDepth)
{
  if (!ns.has(title))
  {
    return;
  }

  if (currentDepth == targetDepth)
  {
    result.pushBack(title);
    return;
  }
  const afanasev::Note & note = ns.get(title);
  for (afanasev::LCIter< std::string > it = note.getChildren().begin();
    it != afanasev::LCIter< std::string >();
    ++it)
  {
    collectAtDepth(*it, ns, result, targetDepth, currentDepth + 1);
  }
}

void afanasev::deleteDepth(const std::string & title, NoteSet & ns, unsigned int depth)
{
  if (!ns.has(title))
  {
    return;
  }

  const Note & note = ns.get(title);
  LIter< std::string > end = LIter< std::string >();
  List< std::string > childrenCopy = note.getChildren();

  if (!depth)
  {
    ns.drop(title);
  }

  depth = (!depth) ? 0 : depth - 1;

  for (LIter< std::string > it = childrenCopy.begin(); it != end; ++it)
  {
    deleteDepth(*it, ns, depth);
  }
}


void afanasev::cmdCr(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title;
  in >> std::quoted(title);

  if (ns.has(title))
  {
    throw std::runtime_error("Note exists");
  }

  ns.add(title, Note());
  out << "created \"" << title << "\"\n";
}

void afanasev::cmdStr(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title, line;
  in >> std::quoted(title) >> std::quoted(line);

  if (!ns.has(title))
  {
    throw std::runtime_error("Note not found");
  }

  ns.get(title).addLine(line);
  out << "added line to \"" << title << "\"\n";
}

void afanasev::cmdDel(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title;
  in >> std::quoted(title);

  if (!ns.has(title))
  {
    throw std::runtime_error("Note not found");
  }

  ns.drop(title);
  out << "\"" << title << "\" deleted\n";
}

void afanasev::cmdDelk(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title;
  unsigned int depth = 0;

  in >> std::quoted(title) >> depth;

  if (!ns.has(title))
  {
    throw std::runtime_error("Note not found");
  }

  deleteDepth(title, ns, depth);
  out << "\"" << title << "\" subtree up to depth " << depth << " deleted\n";
}

void afanasev::cmdSee(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title;
  in >> std::quoted(title);

  if (!ns.has(title))
  {
    throw std::runtime_error("Note not found");
  }

  const Note & note = ns.get(title);

  out << "name: \"" << title << "\"\n";
  out << "tags:";

  Vector< std::string > tags = note.getTags();
  for (size_t i = 0; i < tags.getSize(); ++i)
  {
    out << " " << tags[i];
  }
  out << "\n";

  const Vector< std::string > & lines = note.getLines();
  for (size_t i = 0; i < lines.getSize(); ++i)
  {
    out << (i + 1) << ": " << lines[i] << "\n";
  }
}

void afanasev::cmdLink(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string parentTitle, childTitle;
  in >> std::quoted(parentTitle) >> std::quoted(childTitle);

  if (!ns.has(parentTitle))
  {
    throw std::runtime_error("Parent note not found");
  }
  if (!ns.has(childTitle))
  {
    throw std::runtime_error("Child note not found");
  }

  ns.get(parentTitle).addChild(childTitle);
  out << "\"" << parentTitle << "\" linked \"" << childTitle << "\"\n";
}

void afanasev::cmdDeltagNote(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string tag;
  in >> std::quoted(tag);

  Vector< std::string > toDelete;
  for (NoteSet::HCIter it = ns.cbegin(); it != ns.cend(); ++it)
  {
    const std::string & title = (*it).first;
    const Note & note = (*it).second;
    if (note.hasTag(tag))
    {
      toDelete.pushBack(title);
    }
  }

  size_t deletedCount = 0;
  for (size_t i = 0; i < toDelete.getSize(); ++i)
  {
    const std::string & title = toDelete[i];
    if (ns.has(title))
    {
      ns.drop(title);
      ++deletedCount;
    }
  }

  out << "\"" << tag << "\" deleted " << deletedCount << " notes\n";
}

void afanasev::cmdSeetag(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string tag;
  in >> std::quoted(tag);

  for (NoteSet::HCIter it = ns.cbegin(); it != ns.cend(); ++it)
  {
    const std::string & title = (*it).first;
    const Note & note = (*it).second;
    if (note.hasTag(tag))
    {
      out << "\"" << title << "\"\n";
    }
  }
}

void afanasev::cmdSeetagAnd(std::istream & in, std::ostream & out, NoteSet & ns)
{
  unsigned int count = 0;
  in >> count;

  if (!count)
  {
    return;
  }

  Vector< std::string > tags;
  for (unsigned int i = 0; i < count; ++i)
  {
    std::string tag;
    in >> std::quoted(tag);
    tags.pushBack(tag);
  }

  for (NoteSet::HCIter it = ns.cbegin(); it != ns.cend(); ++it)
  {
    const std::string & title = (*it).first;
    const Note & note = (*it).second;
    bool hasAll = true;
    for (size_t i = 0; i < tags.getSize(); ++i)
    {
      if (!note.hasTag(tags[i]))
      {
        hasAll = false;
        break;
      }
    }
    if (hasAll)
    {
      out << "\"" << title << "\"\n";
    }
  }
}

void afanasev::cmdSeetagOr(std::istream & in, std::ostream & out, NoteSet & ns)
{
  unsigned int count = 0;
  in >> count;

  if (!count)
  {
    return;
  }

  Vector< std::string > tags;
  for (unsigned int i = 0; i < count; ++i)
  {
    std::string tag;
    in >> std::quoted(tag);
    tags.pushBack(tag);
  }

  for (NoteSet::HCIter it = ns.cbegin(); it != ns.cend(); ++it)
  {
    const std::string & title = (*it).first;
    const Note & note = (*it).second;

    for (size_t i = 0; i < tags.getSize(); ++i)
    {
      if (note.hasTag(tags[i]))
      {
        out << "\"" << title << "\"\n";
        break;
      }
    }
  }
}

void afanasev::cmdTagRp(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string oldTag, newTag;
  in >> std::quoted(oldTag) >> std::quoted(newTag);

  size_t replacedCount = 0;

  for (NoteSet::HCIter it = ns.cbegin(); it != ns.cend(); ++it)
  {
    Note & note = ns.get((*it).first);
    if (note.hasTag(oldTag))
    {
      note.removeTag(oldTag);
      note.addTag(newTag);
      ++replacedCount;
    }
  }

  out << "\"" << oldTag << "\" replaced to \"" << newTag << "\" in " << replacedCount << " notes\n";
}

void afanasev::cmdTagAddNew(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string existingTag, newTag;
  in >> std::quoted(existingTag) >> std::quoted(newTag);

  size_t affectedCount = 0;

  for (NoteSet::HCIter it = ns.cbegin(); it != ns.cend(); ++it)
  {
    Note & note = ns.get((*it).first);
    if (note.hasTag(existingTag))
    {
      note.addTag(newTag);
      ++affectedCount;
    }
  }

  out << "added \"" << newTag << "\" to notes with \"" << existingTag << "\" (" << affectedCount << " notes)\n";
}

void afanasev::cmdDelTag(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string tag;
  in >> std::quoted(tag);

  for (NoteSet::HCIter it = ns.cbegin(); it != ns.cend(); ++it)
  {
    Note & note = ns.get((*it).first);
    if (note.hasTag(tag))
    {
      note.removeTag(tag);
    }
  }

  out << "\"" << tag << "\" deleted\n";
}

void afanasev::cmdTag(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title, tag;
  in >> std::quoted(title) >> std::quoted(tag);

  if (!ns.has(title))
  {
    throw std::runtime_error("Note not found");
  }

  ns.get(title).addTag(tag);
  out << "tag: " << tag << " added to \"" << title << "\"\n";
}

void afanasev::cmdTagDel(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title, tag;
  in >> std::quoted(title) >> std::quoted(tag);

  if (!ns.has(title))
  {
    throw std::runtime_error("Note not found");
  }

  ns.get(title).removeTag(tag);
  out << "tag removed from \"" << title << "\"\n";
}

void afanasev::cmdGetLiked(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title;
  unsigned int depth = 0;
  in >> std::quoted(title) >> depth;

  if (!ns.has(title))
  {
    throw std::runtime_error("Note not found");
  }

  afanasev::Vector< std::string > result;

  if (depth == 0)
  {
    out << "\"" << title << "\"\n";
    return;
  }

  collectAtDepth(title, ns, result, depth, 0);

  if (!result.getSize())
  {
    out << "\"There are no notes with this depth\"\n";
    return;
  }

  for (size_t i = 0; i < result.getSize(); ++i)
  {
    out << "\"" << result[i] << "\"\n";
  }
}

void afanasev::cmdAddLinkTag(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title;
  unsigned int depth = 0;
  std::string tag;
  in >> std::quoted(title) >> depth >> std::quoted(tag);

  if (!ns.has(title))
  {
    throw std::runtime_error("Note not found");
  }

  Vector< std::string > targets;
  if (depth == 0)
  {
    targets.pushBack(title);
  }
  else
  {
    collectAtDepth(title, ns, targets, depth, 0);
  }

  for (size_t i = 0; i < targets.getSize(); ++i)
  {
    ns.get(targets[i]).addTag(tag);
  }

  out << "added tag \"" << tag << "\" to " << targets.getSize() << " note(s) at depth " << depth << " from \"" << title << "\"\n";
}

void afanasev::cmdDelLinkTag(std::istream & in, std::ostream & out, NoteSet & ns)
{
  std::string title;
  unsigned int depth = 0;
  std::string tag;
  in >> std::quoted(title) >> depth >> std::quoted(tag);

  if (!ns.has(title))
    throw std::runtime_error("Note not found");

  Vector< std::string > targets;
  if (depth == 0)
  {
    targets.pushBack(title);
  }
  else
  {
    collectAtDepth(title, ns, targets, depth, 0);
  }

  for (size_t i = 0; i < targets.getSize(); ++i)
  {
    ns.get(targets[i]).removeTag(tag);
  }

  out << "removed tag \"" << tag << "\" from " << targets.getSize() << " note(s) at depth " << depth << " from \"" << title << "\"\n";
}

#endif
