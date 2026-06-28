#include "commands.hpp"

#include "utils.hpp"
#include "generator.hpp"
#include "markdown.hpp"
#include "serializer.hpp"

#include <cstddef>
#include <istream>
#include <ostream>
#include <set>
#include <string>
#include <vector>

namespace petrov
{
  const Command COMMANDS[] = {
    { "addq", addQuestion },
    { "delq", delQuestion },
    { "editq", editQuestion },
    { "listq", listQuestions },
    { "findq", findQuestions },
    { "addtag", addTag },
    { "deltag", delTag },
    { "listtag", listTags },
    { "tagq", tagQuestion },
    { "untagq", untagQuestion },
    { "tagsq", questionTags },
    { "gen", generate },
    { "template", printTemplate },
    { "exportdb", exportDb },
    { "importdb", importDb },
    { "statsq", statsQuestions },
    { "statstag", statsTags }
  };
  const std::size_t COMMAND_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);
}

const petrov::Command* petrov::findCommand(const std::string& name)
{
  for (std::size_t i = 0; i < COMMAND_COUNT; ++i) {
    if (name == COMMANDS[i].name) {
      return std::addressof(COMMANDS[i]);
    }
  }
  return nullptr;
}

static void reportInvalid(std::ostream& out, const std::string& reason)
{
  out << "<INVALID COMMAND> " << reason << '\n';
}

static std::size_t findMarker(const petrov::Tokens& tokens, const std::string& marker,
  std::size_t from)
{
  for (std::size_t i = from; i < tokens.size(); ++i) {
    if (tokens[i] == marker) {
      return i;
    }
  }
  return tokens.size();
}

void petrov::addQuestion(Database& database, const Tokens& tokens, std::istream&, std::ostream& out)
{
  if (tokens.size() < 3) {
    reportInvalid(out, "not enough arguments");
    return;
  }
  const std::string& id = tokens[1];
  const std::string& type = tokens[2];
  if (database.questions.has(id)) {
    reportInvalid(out, "question id=" + id + " already exists");
    return;
  }
  if (type == "single") {
    if (tokens.size() < 4) {
      reportInvalid(out, "not enough arguments");
      return;
    }
    const std::size_t correctPos = findMarker(tokens, "correct", 4);
    if (correctPos == tokens.size()) {
      reportInvalid(out, "missing correct marker");
      return;
    }
    if (correctPos == 4) {
      reportInvalid(out, "no answer options");
      return;
    }
    if (correctPos + 2 != tokens.size()) {
      reportInvalid(out, "single requires exactly one correct answer");
      return;
    }
    Question question;
    question.id = id;
    question.text = tokens[3];
    question.type = QuestionType::single;
    for (std::size_t i = 4; i < correctPos; ++i) {
      question.options.push_back(tokens[i]);
    }
    std::size_t index = 0;
    if (!letterToIndex(tokens[correctPos + 1], index) || index >= question.options.size()) {
      reportInvalid(out, "invalid correct answer");
      return;
    }
    question.correct.push_back(index);
    database.questions.push(id, question);
    out << "<Добавлен вопрос id=" << id << ", тип single>\n";
    return;
  }
  if (type == "multiple") {
    if (tokens.size() < 4) {
      reportInvalid(out, "not enough arguments");
      return;
    }
    const std::size_t correctPos = findMarker(tokens, "correct", 4);
    if (correctPos == tokens.size()) {
      reportInvalid(out, "missing correct marker");
      return;
    }
    if (correctPos == 4) {
      reportInvalid(out, "no answer options");
      return;
    }
    if (correctPos + 1 >= tokens.size()) {
      reportInvalid(out, "no correct answers");
      return;
    }
    Question question;
    question.id = id;
    question.text = tokens[3];
    question.type = QuestionType::multiple;
    for (std::size_t i = 4; i < correctPos; ++i) {
      question.options.push_back(tokens[i]);
    }
    std::set< std::size_t > unique;
    for (std::size_t i = correctPos + 1; i < tokens.size(); ++i) {
      std::size_t index = 0;
      if (!letterToIndex(tokens[i], index) || index >= question.options.size()) {
        reportInvalid(out, "invalid correct answer");
        return;
      }
      unique.insert(index);
    }
    std::set< std::size_t >::const_iterator it = unique.begin();
    for (; it != unique.end(); ++it) {
      question.correct.push_back(*it);
    }
    database.questions.push(id, question);
    out << "<Добавлен вопрос id=" << id << ", тип multiple>\n";
    return;
  }
  if (type == "matching") {
    if (tokens.size() < 6) {
      reportInvalid(out, "not enough arguments");
      return;
    }
    const std::size_t count = tokens.size() - 4;
    if (count % 2 != 0) {
      reportInvalid(out, "odd number of pair elements");
      return;
    }
    Question question;
    question.id = id;
    question.text = tokens[3];
    question.type = QuestionType::matching;
    for (std::size_t i = 4; i < tokens.size(); i += 2) {
      question.options.push_back(tokens[i]);
      question.rights.push_back(tokens[i + 1]);
    }
    database.questions.push(id, question);
    out << "<Добавлен вопрос id=" << id << ", тип matching>\n";
    return;
  }
  reportInvalid(out, "unknown type " + type);
}

void petrov::delQuestion(Database& database, const Tokens& tokens, std::istream& in,
  std::ostream& out)
{
  if (tokens.size() != 2) {
    reportInvalid(out, "not enough arguments");
    return;
  }
  const std::string& id = tokens[1];
  if (!database.questions.has(id)) {
    reportInvalid(out, "Вопрос с id=" + id + " не найден");
    return;
  }
  Question& question = database.questions.get(id);
  if (!question.tags.empty()) {
    out << "<Вопрос id=" << id << " содержит теги. ";
    out << "Подтвердить удаление? (Y/N)>\n";
    std::string answer;
    if (!std::getline(in, answer)) {
      return;
    }
    answer = trim(answer);
    if (answer != "Y" && answer != "y") {
      out << "<Удаление отменено>\n";
      return;
    }
  }
  detachQuestionFromTags(database, question);
  database.questions.drop(id);
  database.deletedQuestions = database.deletedQuestions + 1;
  out << "<Удалён вопрос id=" << id << ">\n";
}

void petrov::editQuestion(Database& database, const Tokens& tokens, std::istream&,
  std::ostream& out)
{
  if (tokens.size() < 3) {
    reportInvalid(out, "not enough arguments");
    return;
  }
  const std::string& id = tokens[1];
  if (!database.questions.has(id)) {
    reportInvalid(out, "Вопрос с id=" + id + " не найден");
    return;
  }
  Question& question = database.questions.get(id);
  const std::size_t textPos = findMarker(tokens, "text", 2);
  const std::size_t correctPos = findMarker(tokens, "correct", 2);
  if (textPos == tokens.size() && correctPos == tokens.size()) {
    reportInvalid(out, "no parameters to edit");
    return;
  }
  if (textPos != tokens.size()) {
    const bool hasCorrect = (correctPos != tokens.size() && correctPos > textPos);
    const std::size_t end = hasCorrect ? correctPos : tokens.size();
    if (textPos + 1 >= end) {
      reportInvalid(out, "empty text");
      return;
    }
    std::string newText;
    for (std::size_t i = textPos + 1; i < end; ++i) {
      if (i > textPos + 1) {
        newText += " ";
      }
      newText += tokens[i];
    }
    question.text = newText;
  }
  if (correctPos != tokens.size()) {
    if (question.type == QuestionType::matching) {
      reportInvalid(out, "matching answers are not editable");
      return;
    }
    if (question.type == QuestionType::single && correctPos + 2 != tokens.size()) {
      reportInvalid(out, "single requires exactly one correct answer");
      return;
    }
    if (correctPos + 1 >= tokens.size()) {
      reportInvalid(out, "no correct answers");
      return;
    }
    std::set< std::size_t > unique;
    for (std::size_t i = correctPos + 1; i < tokens.size(); ++i) {
      std::size_t index = 0;
      if (!letterToIndex(tokens[i], index) || index >= question.options.size()) {
        reportInvalid(out, "invalid correct answer");
        return;
      }
      unique.insert(index);
    }
    question.correct.clear();
    std::set< std::size_t >::const_iterator it = unique.begin();
    for (; it != unique.end(); ++it) {
      question.correct.push_back(*it);
    }
  }
  out << "<Изменён вопрос id=" << id << ">\n";
}

void petrov::listQuestions(Database& database, const Tokens&, std::istream&, std::ostream& out)
{
  QuestionTree::const_iterator it = database.questions.begin();
  for (; it != database.questions.end(); ++it) {
    const Question& question = it->second;
    out << question.id << " [" << typeName(question) << "] " << question.text << '\n';
  }
}

void petrov::findQuestions(Database& database, const Tokens& tokens, std::istream&,
  std::ostream& out)
{
  if (tokens.size() < 2) {
    reportInvalid(out, "no substring");
    return;
  }
  std::string needle = tokens[1];
  for (std::size_t i = 2; i < tokens.size(); ++i) {
    needle += " " + tokens[i];
  }
  out << "Результаты поиска \"" << needle << "\":\n";
  QuestionTree::const_iterator it = database.questions.begin();
  for (; it != database.questions.end(); ++it) {
    const Question& question = it->second;
    if (containsIgnoreCase(question.text, needle) || matchesInBody(question, needle)) {
      out << question.id << " " << question.text << '\n';
    }
  }
}

void petrov::addTag(Database& database, const Tokens& tokens, std::istream&, std::ostream& out)
{
  if (tokens.size() != 2) {
    reportInvalid(out, "not enough arguments");
    return;
  }
  const std::string& name = tokens[1];
  if (database.tags.has(name)) {
    reportInvalid(out, "tag \"" + name + "\" already exists");
    return;
  }
  Tag tag;
  tag.name = name;
  database.tags.push(name, tag);
  out << "<Тег \"" << name << "\" создан>\n";
}

void petrov::delTag(Database& database, const Tokens& tokens, std::istream&, std::ostream& out)
{
  if (tokens.size() != 2) {
    reportInvalid(out, "not enough arguments");
    return;
  }
  const std::string& name = tokens[1];
  if (!database.tags.has(name)) {
    reportInvalid(out, "tag \"" + name + "\" does not exist");
    return;
  }
  Tag& tag = database.tags.get(name);
  std::set< std::string >::const_iterator it = tag.questionIds.begin();
  for (; it != tag.questionIds.end(); ++it) {
    if (database.questions.has(*it)) {
      database.questions.get(*it).tags.erase(name);
    }
  }
  database.tags.drop(name);
  database.deletedTags = database.deletedTags + 1;
  out << "<Тег \"" << name << "\" удалён>\n";
}

void petrov::listTags(Database& database, const Tokens& tokens, std::istream&, std::ostream& out)
{
  if (tokens.size() != 2) {
    reportInvalid(out, "specify all or <tag>");
    return;
  }
  if (tokens[1] == "all") {
    out << "Список тегов:\n";
    TagTree::const_iterator it = database.tags.begin();
    for (; it != database.tags.end(); ++it) {
      out << "- " << it->first << " (" << it->second.questionIds.size() << " вопросов)\n";
    }
    return;
  }
  const std::string& name = tokens[1];
  if (!database.tags.has(name)) {
    reportInvalid(out, "tag \"" + name + "\" does not exist");
    return;
  }
  const Tag& tag = database.tags.get(name);
  out << "Тег \"" << name << "\":\n";
  std::set< std::string >::const_iterator it = tag.questionIds.begin();
  for (; it != tag.questionIds.end(); ++it) {
    if (database.questions.has(*it)) {
      const Question& question = database.questions.get(*it);
      out << question.id << " " << question.text << '\n';
    }
  }
}

void petrov::tagQuestion(Database& database, const Tokens& tokens, std::istream&, std::ostream& out)
{
  if (tokens.size() < 3) {
    reportInvalid(out, "not enough arguments");
    return;
  }
  const std::string& id = tokens[1];
  if (!database.questions.has(id)) {
    reportInvalid(out, "Вопрос с id=" + id + " не найден");
    return;
  }
  for (std::size_t i = 2; i < tokens.size(); ++i) {
    if (!database.tags.has(tokens[i])) {
      reportInvalid(out, "tag \"" + tokens[i] + "\" does not exist");
      return;
    }
  }
  Question& question = database.questions.get(id);
  std::size_t added = 0;
  for (std::size_t i = 2; i < tokens.size(); ++i) {
    if (question.tags.insert(tokens[i]).second) {
      database.tags.get(tokens[i]).questionIds.insert(id);
      added = added + 1;
    }
  }
  out << "<Вопрос id=" << id << " обновлён. ";
  out << "Добавлено тегов: " << added << ">\n";
}

void petrov::untagQuestion(Database& database, const Tokens& tokens, std::istream&,
  std::ostream& out)
{
  if (tokens.size() < 3) {
    reportInvalid(out, "not enough arguments");
    return;
  }
  const std::string& id = tokens[1];
  if (!database.questions.has(id)) {
    reportInvalid(out, "Вопрос с id=" + id + " не найден");
    return;
  }
  for (std::size_t i = 2; i < tokens.size(); ++i) {
    if (!database.tags.has(tokens[i])) {
      reportInvalid(out, "tag \"" + tokens[i] + "\" does not exist");
      return;
    }
  }
  Question& question = database.questions.get(id);
  std::size_t removed = 0;
  for (std::size_t i = 2; i < tokens.size(); ++i) {
    if (question.tags.erase(tokens[i]) > 0) {
      database.tags.get(tokens[i]).questionIds.erase(id);
      removed = removed + 1;
    }
  }
  out << "<У вопроса id=" << id << " удалено тегов: " << removed << ">\n";
}

void petrov::questionTags(Database& database, const Tokens& tokens, std::istream&,
  std::ostream& out)
{
  if (tokens.size() != 2) {
    reportInvalid(out, "not enough arguments");
    return;
  }
  const std::string& id = tokens[1];
  if (!database.questions.has(id)) {
    reportInvalid(out, "Вопрос с id=" + id + " не найден");
    return;
  }
  const Question& question = database.questions.get(id);
  out << "Теги вопроса " << id << ":";
  std::set< std::string >::const_iterator it = question.tags.begin();
  for (; it != question.tags.end(); ++it) {
    out << " " << *it;
  }
  out << '\n';
}

void petrov::printTemplate(Database&, const Tokens&, std::istream&, std::ostream& out)
{
  out << "# Шаблон входного файла\n";
  out << "title=\n";
  out << "variants=\n";
  out << "questions_per_variant=\n";
  out << "tags=\n";
  out << "exclude_ids=\n";
}

void petrov::exportDb(Database& database, const Tokens& tokens, std::istream&, std::ostream& out)
{
  if (tokens.size() != 2) {
    reportInvalid(out, "specify file name");
    return;
  }
  if (!exportDatabase(tokens[1], database)) {
    reportInvalid(out, "cannot write file " + tokens[1]);
    return;
  }
  out << "<База экспортирована в " << tokens[1] << ">\n";
}

void petrov::importDb(Database& database, const Tokens& tokens, std::istream&, std::ostream& out)
{
  if (tokens.size() != 2) {
    reportInvalid(out, "specify file name");
    return;
  }
  std::string error;
  if (!importDatabase(tokens[1], database, error)) {
    reportInvalid(out, error);
    return;
  }
  out << "<База импортирована из " << tokens[1] << ">\n";
}

void petrov::statsQuestions(Database& database, const Tokens&, std::istream&, std::ostream& out)
{
  std::size_t single = 0;
  std::size_t multiple = 0;
  std::size_t matching = 0;
  QuestionTree::const_iterator it = database.questions.begin();
  for (; it != database.questions.end(); ++it) {
    if (it->second.type == QuestionType::single) {
      single = single + 1;
    } else if (it->second.type == QuestionType::multiple) {
      multiple = multiple + 1;
    } else {
      matching = matching + 1;
    }
  }
  out << "Статистика вопросов:\n";
  out << "Всего вопросов: " << database.questions.size() << '\n';
  out << "single: " << single << '\n';
  out << "multiple: " << multiple << '\n';
  out << "matching: " << matching << '\n';
  out << "Удалённых вопросов: " << database.deletedQuestions << '\n';
}

void petrov::statsTags(Database& database, const Tokens&, std::istream&, std::ostream& out)
{
  out << "Статистика тегов:\n";
  out << "Всего тегов: " << database.tags.size() << '\n';
  TagTree::const_iterator it = database.tags.begin();
  for (; it != database.tags.end(); ++it) {
    out << "- " << it->first << ": " << it->second.questionIds.size() << '\n';
  }
  out << "Удалённых тегов: " << database.deletedTags << '\n';
}

void petrov::generate(Database& database, const Tokens& tokens, std::istream&, std::ostream& out)
{
  if (tokens.size() < 3) {
    reportInvalid(out, "specify input and output files");
    return;
  }
  const std::string& inPath = tokens[1];
  const std::string& outPath = tokens[2];
  const bool withAnswers = (tokens.size() >= 4 && tokens[3] == "answers");
  GenParams params;
  std::string error;
  if (!parseGenFile(inPath, params, error)) {
    reportInvalid(out, error);
    return;
  }
  std::vector< std::string > candidates;
  selectCandidates(database, params, candidates);
  if (candidates.size() < static_cast< std::size_t >(params.questionsPerVariant)) {
    reportInvalid(out, "not enough available questions");
    return;
  }
  std::vector< std::vector< std::string > > variants;
  buildVariants(params, candidates, variants);
  if (!writeVariants(outPath, params.title, variants, database)) {
    reportInvalid(out, "cannot write file " + outPath);
    return;
  }
  if (withAnswers) {
    const std::string answersPath = makeAnswersPath(outPath);
    if (!writeAnswers(answersPath, params.title, variants, database)) {
      reportInvalid(out, "cannot write answers file");
      return;
    }
  }
  out << "<Сгенерировано " << params.variants << " варианта по ";
  out << params.questionsPerVariant << " вопросов>\n";
}


