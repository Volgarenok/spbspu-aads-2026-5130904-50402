#include "commands.hpp"

#include <cstddef>
#include <fstream>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list.hpp>
#include "cuckoo_table.hpp"
#include "hashers.hpp"
#include "list_utils.hpp"
#include "stack.hpp"
#include "text.hpp"
#include "text_index.hpp"
#include "text_ops.hpp"

namespace samarin {

  using CommandHandler = void (*)(std::istream&, std::ostream&, Documents&);
  using CommandTable = CuckooTable< std::string, CommandHandler, StringHash >;

  namespace {
    TextIndex& requireDoc(Documents& docs, const std::string& id)
    {
      if (!docs.has(id)) {
        throw std::logic_error("no such document");
      }
      return docs.at(id);
    }

    void requireAbsent(const Documents& docs, const std::string& id)
    {
      if (docs.has(id)) {
        throw std::logic_error("document exists");
      }
    }

    void storeText(Documents& docs, const std::string& id, const Text& text)
    {
      docs[id].build(text);
    }

    Text restoreDoc(Documents& docs, const std::string& id)
    {
      return requireDoc(docs, id).restore();
    }

    void requireStream(const std::istream& in)
    {
      if (!in) {
        throw std::logic_error("missing arguments");
      }
    }

    std::size_t toIndex(std::size_t oneBased)
    {
      if (oneBased == 0) {
        throw std::logic_error("index must be positive");
      }
      return oneBased - 1;
    }

    Line readLine(std::istream& in)
    {
      Line line;
      LIter< std::string > tail = line.before_begin();
      std::string word;
      while (in >> word) {
        tail = line.insert_after(tail, word);
      }
      return line;
    }

    void printKeys(std::ostream& out, const Documents& docs)
    {
      List< std::string > names;
      LIter< std::string > tail = names.before_begin();
      for (Documents::const_iterator it = docs.cbegin(); it != docs.cend(); ++it) {
        tail = names.insert_after(tail, it->first);
      }
      listSort(names);
      if (names.empty()) {
        out << "<EMPTY>\n";
        return;
      }
      for (LCIter< std::string > it = names.cbegin(); it != names.cend(); ++it) {
        out << *it << "\n";
      }
    }

    void cmdLoad(std::istream& in, std::ostream&, Documents& docs)
    {
      std::string file;
      std::string id;
      in >> file >> id;
      requireStream(in);
      requireAbsent(docs, id);
      std::ifstream source(file.c_str());
      if (!source.is_open()) {
        throw std::logic_error("cannot open file");
      }
      storeText(docs, id, parseText(source));
    }

    void cmdSave(std::istream& in, std::ostream&, Documents& docs)
    {
      std::string id;
      std::string file;
      in >> id >> file;
      requireStream(in);
      const Text text = restoreDoc(docs, id);
      std::ofstream sink(file.c_str());
      if (!sink.is_open()) {
        throw std::logic_error("cannot open file");
      }
      writeText(sink, text);
    }

    void cmdList(std::istream&, std::ostream& out, Documents& docs)
    {
      printKeys(out, docs);
    }

    void cmdDrop(std::istream& in, std::ostream&, Documents& docs)
    {
      std::string id;
      in >> id;
      docs.drop(id);
    }

    void cmdShow(std::istream& in, std::ostream& out, Documents& docs)
    {
      std::string id;
      in >> id;
      writeText(out, restoreDoc(docs, id));
    }

    void cmdHelp(std::istream&, std::ostream& out, Documents&)
    {
      out << "load save list drop show help exit\n";
      out << "replace swap insert-line remove-line\n";
      out << "concat concat-lines repeat-vertical repeat-horizontal\n";
      out << "interleave-lines interleave-words reverse-lines reverse-words transpose\n";
      out << "find find-first find-last find-around find-count dump-index\n";
    }

    using UnaryOp = Text (*)(const Text&);
    using BinaryOp = Text (*)(const Text&, const Text&);
    using RepeatOp = Text (*)(const Text&, std::size_t);

    void runUnary(std::istream& in, Documents& docs, UnaryOp op)
    {
      std::string id;
      std::string newId;
      in >> id >> newId;
      requireStream(in);
      requireAbsent(docs, newId);
      storeText(docs, newId, op(restoreDoc(docs, id)));
    }

    void runBinary(std::istream& in, Documents& docs, BinaryOp op)
    {
      std::string first;
      std::string second;
      std::string newId;
      in >> first >> second >> newId;
      requireStream(in);
      requireAbsent(docs, newId);
      const Text left = restoreDoc(docs, first);
      const Text right = restoreDoc(docs, second);
      storeText(docs, newId, op(left, right));
    }

    void runRepeat(std::istream& in, Documents& docs, RepeatOp op)
    {
      std::string id;
      std::size_t times = 0;
      std::string newId;
      in >> id >> times >> newId;
      requireStream(in);
      requireAbsent(docs, newId);
      storeText(docs, newId, op(restoreDoc(docs, id), times));
    }

    void cmdConcat(std::istream& in, std::ostream&, Documents& docs)
    {
      runBinary(in, docs, &concat);
    }

    void cmdConcatLines(std::istream& in, std::ostream&, Documents& docs)
    {
      runBinary(in, docs, &concatLines);
    }

    void cmdInterleaveLines(std::istream& in, std::ostream&, Documents& docs)
    {
      runBinary(in, docs, &interleaveLines);
    }

    void cmdInterleaveWords(std::istream& in, std::ostream&, Documents& docs)
    {
      runBinary(in, docs, &interleaveWords);
    }

    void cmdRepeatVertical(std::istream& in, std::ostream&, Documents& docs)
    {
      runRepeat(in, docs, &repeatVertical);
    }

    void cmdRepeatHorizontal(std::istream& in, std::ostream&, Documents& docs)
    {
      runRepeat(in, docs, &repeatHorizontal);
    }

    void cmdReverseLines(std::istream& in, std::ostream&, Documents& docs)
    {
      runUnary(in, docs, &reverseLines);
    }

    void cmdReverseWords(std::istream& in, std::ostream&, Documents& docs)
    {
      runUnary(in, docs, &reverseWords);
    }

    void cmdTranspose(std::istream& in, std::ostream&, Documents& docs)
    {
      runUnary(in, docs, &transpose);
    }

    template< class Mutator >
    void editDoc(Documents& docs, const std::string& id, Mutator mutate)
    {
      TextIndex& index = requireDoc(docs, id);
      Text text = index.restore();
      mutate(text);
      index.build(text);
    }

    void cmdReplace(std::istream& in, std::ostream&, Documents& docs)
    {
      std::string id;
      std::size_t line = 0;
      std::size_t word = 0;
      std::string value;
      in >> id >> line >> word >> value;
      requireStream(in);
      editDoc(docs, id, [&](Text& text)
      {
        replaceWord(text, toIndex(line), toIndex(word), value);
      });
    }

    void cmdSwap(std::istream& in, std::ostream&, Documents& docs)
    {
      std::string id;
      std::size_t line1 = 0;
      std::size_t word1 = 0;
      std::size_t line2 = 0;
      std::size_t word2 = 0;
      in >> id >> line1 >> word1 >> line2 >> word2;
      requireStream(in);
      editDoc(docs, id, [&](Text& text)
      {
        swapWords(text, toIndex(line1), toIndex(word1), toIndex(line2), toIndex(word2));
      });
    }

    void cmdInsertLine(std::istream& in, std::ostream&, Documents& docs)
    {
      std::string id;
      std::size_t pos = 0;
      in >> id >> pos;
      requireStream(in);
      const Line line = readLine(in);
      editDoc(docs, id, [&](Text& text)
      {
        insertLine(text, toIndex(pos), line);
      });
    }

    void cmdRemoveLine(std::istream& in, std::ostream&, Documents& docs)
    {
      std::string id;
      std::size_t pos = 0;
      in >> id >> pos;
      requireStream(in);
      editDoc(docs, id, [&](Text& text)
      {
        removeLine(text, toIndex(pos));
      });
    }

    enum class Edge {
      both,
      left,
      right
    };

    struct findopts_t {
      std::size_t limit;
      bool fromEnd;
      Edge edge;
      std::size_t context;
    };

    bool startsWith(const std::string& text, const std::string& prefix)
    {
      return text.size() >= prefix.size() && text.compare(0, prefix.size(), prefix) == 0;
    }

    std::size_t parseSize(const std::string& text)
    {
      std::istringstream stream(text);
      std::size_t value = 0;
      if (!(stream >> value) || !stream.eof()) {
        throw std::logic_error("bad number");
      }
      return value;
    }

    bool matchFlag(const std::string& flag, const std::string& prefix, std::string& value)
    {
      if (!startsWith(flag, prefix)) {
        return false;
      }
      value = flag.substr(prefix.size());
      return true;
    }

    void parseFlag(const std::string& flag, findopts_t& options)
    {
      std::string value;
      if (matchFlag(flag, "--limit=", value)) {
        options.limit = parseSize(value);
      } else if (matchFlag(flag, "--context=", value)) {
        options.context = parseSize(value);
      } else if (matchFlag(flag, "--from=", value)) {
        if (value == "start") {
          options.fromEnd = false;
        } else if (value == "end") {
          options.fromEnd = true;
        } else {
          throw std::logic_error("bad from");
        }
      } else if (matchFlag(flag, "--edge=", value)) {
        if (value == "left") {
          options.edge = Edge::left;
        } else if (value == "right") {
          options.edge = Edge::right;
        } else {
          throw std::logic_error("bad edge");
        }
      } else {
        throw std::logic_error("unknown flag");
      }
    }

    List< Position > collectSequence(const PostingList& source, bool fromEnd)
    {
      List< Position > ordered = source;
      listSort(ordered);
      if (!fromEnd) {
        return ordered;
      }
      List< Position > reversed;
      Stack< Position > stack;
      for (LCIter< Position > it = ordered.cbegin(); it != ordered.cend(); ++it) {
        stack.push(*it);
      }
      LIter< Position > tail = reversed.before_begin();
      while (!stack.empty()) {
        tail = reversed.insert_after(tail, stack.drop());
      }
      return reversed;
    }

    void printContext(std::ostream& out, const Line& line, const Position& pos, const findopts_t& options)
    {
      std::size_t lo = pos.word;
      std::size_t hi = pos.word;
      if (options.edge != Edge::right) {
        lo = (pos.word > options.context) ? (pos.word - options.context) : 0;
      }
      if (options.edge != Edge::left) {
        hi = pos.word + options.context;
      }
      out << (pos.line + 1) << " " << (pos.word + 1) << ":";
      std::size_t col = 0;
      for (LCIter< std::string > word = line.cbegin(); word != line.cend() && col <= hi; ++word, ++col) {
        if (col < lo) {
          continue;
        }
        out << " ";
        if (col == pos.word) {
          out << "[" << *word << "]";
        } else {
          out << *word;
        }
      }
      out << "\n";
    }

    findopts_t readFindOptions(std::istream& in)
    {
      findopts_t options = { std::numeric_limits< std::size_t >::max(), false, Edge::both, 0 };
      std::string flag;
      while (in >> flag) {
        parseFlag(flag, options);
      }
      return options;
    }

    void printMatches(std::ostream& out, const TextIndex& index, const std::string& word, const findopts_t& options)
    {
      const List< Position > sequence = collectSequence(index.positions(word), options.fromEnd);
      const Text text = index.restore();
      std::size_t shown = 0;
      for (LCIter< Position > it = sequence.cbegin(); it != sequence.cend() && shown < options.limit; ++it) {
        printContext(out, listAt(text, it->line), *it, options);
        ++shown;
      }
    }

    void cmdFind(std::istream& in, std::ostream& out, Documents& docs)
    {
      std::string id;
      std::string word;
      in >> id >> word;
      requireStream(in);
      const findopts_t options = readFindOptions(in);
      const TextIndex& index = requireDoc(docs, id);
      if (!index.has(word)) {
        out << "<EMPTY>\n";
        return;
      }
      printMatches(out, index, word, options);
    }

    void emitScoped(std::ostream& out, const TextIndex& index, const std::string& word, const findopts_t& options)
    {
      if (!index.has(word)) {
        out << "<EMPTY>\n";
        return;
      }
      printMatches(out, index, word, options);
    }

    void cmdFindFirst(std::istream& in, std::ostream& out, Documents& docs)
    {
      std::string id;
      std::string word;
      std::size_t count = 0;
      in >> id >> word >> count;
      requireStream(in);
      const findopts_t options = { count, false, Edge::both, 0 };
      emitScoped(out, requireDoc(docs, id), word, options);
    }

    void cmdFindLast(std::istream& in, std::ostream& out, Documents& docs)
    {
      std::string id;
      std::string word;
      std::size_t count = 0;
      in >> id >> word >> count;
      requireStream(in);
      const findopts_t options = { count, true, Edge::both, 0 };
      emitScoped(out, requireDoc(docs, id), word, options);
    }

    void cmdFindAround(std::istream& in, std::ostream& out, Documents& docs)
    {
      std::string id;
      std::string word;
      std::size_t context = 0;
      in >> id >> word >> context;
      requireStream(in);
      const findopts_t options = { std::numeric_limits< std::size_t >::max(), false, Edge::both, context };
      emitScoped(out, requireDoc(docs, id), word, options);
    }

    void cmdFindCount(std::istream& in, std::ostream& out, Documents& docs)
    {
      std::string id;
      std::string word;
      in >> id >> word;
      requireStream(in);
      const TextIndex& index = requireDoc(docs, id);
      const std::size_t total = index.has(word) ? listSize(index.positions(word)) : 0;
      out << total << "\n";
    }

    void cmdDumpIndex(std::istream& in, std::ostream& out, Documents& docs)
    {
      std::string id;
      in >> id;
      const TextIndex& index = requireDoc(docs, id);
      List< std::string > words;
      LIter< std::string > wordTail = words.before_begin();
      for (PostingsTable::const_iterator it = index.postings().cbegin(); it != index.postings().cend(); ++it) {
        wordTail = words.insert_after(wordTail, it->first);
      }
      listSort(words);
      if (words.empty()) {
        out << "<EMPTY>\n";
        return;
      }
      for (LCIter< std::string > word = words.cbegin(); word != words.cend(); ++word) {
        out << *word << ":";
        List< Position > positions = index.positions(*word);
        listSort(positions);
        for (LCIter< Position > p = positions.cbegin(); p != positions.cend(); ++p) {
          out << " (" << (p->line + 1) << "," << (p->word + 1) << ")";
        }
        out << "\n";
      }
    }

    CommandTable makeCommands()
    {
      CommandTable commands;
      commands.add("load", &cmdLoad);
      commands.add("save", &cmdSave);
      commands.add("list", &cmdList);
      commands.add("drop", &cmdDrop);
      commands.add("show", &cmdShow);
      commands.add("help", &cmdHelp);
      commands.add("replace", &cmdReplace);
      commands.add("swap", &cmdSwap);
      commands.add("insert-line", &cmdInsertLine);
      commands.add("remove-line", &cmdRemoveLine);
      commands.add("concat", &cmdConcat);
      commands.add("concat-lines", &cmdConcatLines);
      commands.add("repeat-vertical", &cmdRepeatVertical);
      commands.add("repeat-horizontal", &cmdRepeatHorizontal);
      commands.add("interleave-lines", &cmdInterleaveLines);
      commands.add("interleave-words", &cmdInterleaveWords);
      commands.add("reverse-lines", &cmdReverseLines);
      commands.add("reverse-words", &cmdReverseWords);
      commands.add("transpose", &cmdTranspose);
      commands.add("find", &cmdFind);
      commands.add("find-first", &cmdFindFirst);
      commands.add("find-last", &cmdFindLast);
      commands.add("find-around", &cmdFindAround);
      commands.add("find-count", &cmdFindCount);
      commands.add("dump-index", &cmdDumpIndex);
      return commands;
    }
  }

}

void samarin::processCommands(std::istream& in, std::ostream& out, Documents& docs)
{
  const CommandTable commands = makeCommands();
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream stream(line);
    std::string name;
    if (!(stream >> name)) {
      continue;
    }
    if (name == "exit") {
      break;
    }
    try {
      const CommandTable::const_iterator handler = commands.find(name);
      if (handler == commands.cend()) {
        throw std::logic_error("unknown command");
      }
      handler->second(stream, out, docs);
    } catch (const std::exception&) {
      out << "<INVALID COMMAND>\n";
    }
  }
}
