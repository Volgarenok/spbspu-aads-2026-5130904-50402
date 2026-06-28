#include "question.hpp"
#include "utils.hpp"

#include <cctype>
#include <ostream>
#include <string>

std::string petrov::typeName(const Question& question)
{
  if (question.type == QuestionType::single) {
    return "single";
  }
  if (question.type == QuestionType::multiple) {
    return "multiple";
  }
  return "matching";
}

bool petrov::letterToIndex(const std::string& letter, std::size_t& index)
{
  if (letter.size() != 1) {
    return false;
  }
  const char symbol = letter[0];
  if (symbol >= 'A' && symbol <= 'Z') {
    index = static_cast< std::size_t >(symbol - 'A');
    return true;
  }
  if (symbol >= 'a' && symbol <= 'z') {
    index = static_cast< std::size_t >(symbol - 'a');
    return true;
  }
  return false;
}

char petrov::indexToLetter(std::size_t index)
{
  return static_cast< char >('A' + index);
}

void petrov::renderBody(std::ostream& out, const Question& question)
{
  if (question.type == QuestionType::matching) {
    for (std::size_t i = 0; i < question.options.size(); ++i) {
      out << (i + 1) << ". " << question.options[i] << "\n";
    }
    out << "\n";
    for (std::size_t i = 0; i < question.rights.size(); ++i) {
      out << indexToLetter(i) << ") " << question.rights[i] << "\n";
    }
    return;
  }
  for (std::size_t i = 0; i < question.options.size(); ++i) {
    out << indexToLetter(i) << ") " << question.options[i] << "\n";
  }
}

std::string petrov::answerString(const Question& question)
{
  std::string result;
  if (question.type == QuestionType::matching) {
    for (std::size_t i = 0; i < question.options.size(); ++i) {
      if (i != 0) {
        result += ", ";
      }
      result += std::to_string(i + 1);
      result += "-";
      result += indexToLetter(i);
    }
    return result.empty() ? std::string("?") : result;
  }
  for (std::size_t i = 0; i < question.correct.size(); ++i) {
    if (i != 0) {
      result += " ";
    }
    result += indexToLetter(question.correct[i]);
  }
  return result.empty() ? std::string("?") : result;
}

bool petrov::matchesInBody(const Question& question, const std::string& needle)
{
  for (std::size_t i = 0; i < question.options.size(); ++i) {
    if (containsIgnoreCase(question.options[i], needle)) {
      return true;
    }
  }
  for (std::size_t i = 0; i < question.rights.size(); ++i) {
    if (containsIgnoreCase(question.rights[i], needle)) {
      return true;
    }
  }
  return false;
}


