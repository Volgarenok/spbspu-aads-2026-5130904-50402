#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include <vector>

#include "database.hpp"

namespace petrov
{
  using Tokens = std::vector< std::string >;

  struct Command
  {
    const char* name;
    void (*execute)(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  };

  const Command* findCommand(const std::string& name);

  void addQuestion(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void delQuestion(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void editQuestion(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void listQuestions(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void findQuestions(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);

  void addTag(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void delTag(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void listTags(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void tagQuestion(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void untagQuestion(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void questionTags(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);

  void generate(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void printTemplate(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);

  void exportDb(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void importDb(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);

  void statsQuestions(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
  void statsTags(Database& database, const Tokens& tokens, std::istream& in, std::ostream& out);
}

#endif
