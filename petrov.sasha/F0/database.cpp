#include "database.hpp"

petrov::Database::Database():
  questions(),
  tags(),
  deletedQuestions(0),
  deletedTags(0)
{}

void petrov::detachQuestionFromTags(Database& database, Question& question)
{
  std::set< std::string >::const_iterator it = question.tags.begin();
  for (; it != question.tags.end(); ++it) {
    if (database.tags.has(*it)) {
      database.tags.get(*it).questionIds.erase(question.id);
    }
  }
  question.tags.clear();
}

