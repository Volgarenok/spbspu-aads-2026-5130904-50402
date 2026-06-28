#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <cstddef>
#include <iosfwd>
#include <set>
#include <string>
#include <vector>

namespace petrov
{
  enum class QuestionType
  {
    single,
    multiple,
    matching
  };

  struct Question
  {
    std::string id;
    std::string text;
    QuestionType type;
    std::vector< std::string > options;
    std::vector< std::string > rights;
    std::vector< std::size_t > correct;
    std::set< std::string > tags;

    Question():
      id(),
      text(),
      type(QuestionType::single),
      options(),
      rights(),
      correct(),
      tags()
    {}
  };

  std::string typeName(const Question& question);
  void renderBody(std::ostream& out, const Question& question);
  std::string answerString(const Question& question);
  bool matchesInBody(const Question& question, const std::string& needle);

  bool letterToIndex(const std::string& letter, std::size_t& index);
  char indexToLetter(std::size_t index);
}

#endif

