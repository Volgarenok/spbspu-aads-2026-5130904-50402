#include "serializer.hpp"

#include <fstream>
#include <istream>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>

#include "question.hpp"
#include "tag.hpp"

static void writeQuestionTags(std::ostream& out, const petrov::Question& question)
{
  out << question.tags.size() << '\n';
  for (std::set< std::string >::const_iterator it = question.tags.begin(); it != question.tags.end(); ++it) {
    out << *it << '\n';
  }
}

static bool readLine(std::istream& in, std::string& out)
{
  return static_cast< bool >(std::getline(in, out));
}

static bool readCount(std::istream& in, std::size_t& out)
{
  std::string line;
  if (!std::getline(in, line)) {
    return false;
  }
  try {
    out = static_cast< std::size_t >(std::stoul(line));
  } catch (const std::exception&) {
    return false;
  }
  return true;
}

bool petrov::exportDatabase(const std::string& path, const Database& database)
{
  std::ofstream file(path);
  if (!file) {
    return false;
  }
  file << "TAGS " << database.tags.size() << '\n';
  for (TagTree::const_iterator it = database.tags.begin(); it != database.tags.end(); ++it) {
    file << it->first << '\n';
  }
  file << "QUESTIONS " << database.questions.size() << '\n';
  for (QuestionTree::const_iterator it = database.questions.begin(); it != database.questions.end(); ++it) {
    const Question& question = it->second;
    file << typeName(question) << '\n';
    file << question.id << '\n';
    file << question.text << '\n';
    file << question.options.size() << '\n';
    for (std::size_t i = 0; i < question.options.size(); ++i) {
      file << question.options[i] << '\n';
    }
    if (question.type == QuestionType::matching) {
      for (std::size_t i = 0; i < question.rights.size(); ++i) {
        file << question.rights[i] << '\n';
      }
    } else {
      file << question.correct.size() << '\n';
      for (std::size_t i = 0; i < question.correct.size(); ++i) {
        file << question.correct[i] << '\n';
      }
    }
    writeQuestionTags(file, question);
    file << "END\n";
  }
  return static_cast< bool >(file);
}

bool petrov::importDatabase(const std::string& path, Database& database, std::string& error)
{
  std::ifstream file(path);
  if (!file) {
    error = "file " + path + " does not exist";
    return false;
  }
  Database loaded;
  std::string token;
  std::size_t tagCount = 0;
  if (!(file >> token) || token != "TAGS" || !(file >> tagCount)) {
    error = "invalid file format";
    return false;
  }
  file.ignore();
  for (std::size_t i = 0; i < tagCount; ++i) {
    std::string name;
    if (!readLine(file, name)) {
      error = "invalid tag data";
      return false;
    }
    Tag tag;
    tag.name = name;
    loaded.tags.push(name, tag);
  }
  std::size_t questionCount = 0;
  if (!(file >> token) || token != "QUESTIONS" || !(file >> questionCount)) {
    error = "invalid question section";
    return false;
  }
  file.ignore();
  for (std::size_t i = 0; i < questionCount; ++i) {
    std::string kind;
    std::string id;
    std::string text;
    if (!readLine(file, kind) || !readLine(file, id) || !readLine(file, text)) {
      error = "invalid question data";
      return false;
    }
    Question question;
    question.id = id;
    question.text = text;
    std::size_t optionCount = 0;
    if (!readCount(file, optionCount)) {
      error = "invalid options data";
      return false;
    }
    for (std::size_t k = 0; k < optionCount; ++k) {
      std::string option;
      if (!readLine(file, option)) {
        error = "invalid options data";
        return false;
      }
      question.options.push_back(option);
    }
    if (kind == "matching") {
      question.type = QuestionType::matching;
      for (std::size_t k = 0; k < optionCount; ++k) {
        std::string right;
        if (!readLine(file, right)) {
          error = "invalid pairs data";
          return false;
        }
        question.rights.push_back(right);
      }
    } else if (kind == "single" || kind == "multiple") {
      question.type = (kind == "single") ? QuestionType::single : QuestionType::multiple;
      std::size_t correctCount = 0;
      if (!readCount(file, correctCount)) {
        error = "invalid answers data";
        return false;
      }
      for (std::size_t k = 0; k < correctCount; ++k) {
        std::size_t value = 0;
        if (!readCount(file, value)) {
          error = "invalid answers data";
          return false;
        }
        question.correct.push_back(value);
      }
    } else {
      error = "unknown question type in file: " + kind;
      return false;
    }
    std::size_t questionTagCount = 0;
    if (!readCount(file, questionTagCount)) {
      error = "invalid question tag data";
      return false;
    }
    for (std::size_t k = 0; k < questionTagCount; ++k) {
      std::string tagName;
      if (!readLine(file, tagName)) {
        error = "invalid question tag data";
        return false;
      }
      question.tags.insert(tagName);
      if (!loaded.tags.has(tagName)) {
        Tag tag;
        tag.name = tagName;
        loaded.tags.push(tagName, tag);
      }
      loaded.tags.get(tagName).questionIds.insert(id);
    }
    std::string end;
    readLine(file, end);
    loaded.questions.push(id, question);
  }
  database.questions = loaded.questions;
  database.tags = loaded.tags;
  database.deletedQuestions = 0;
  database.deletedTags = 0;
  return true;
}

