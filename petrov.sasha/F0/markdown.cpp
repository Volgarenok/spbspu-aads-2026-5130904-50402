#include "markdown.hpp"

#include <fstream>
#include <string>

#include "question.hpp"

bool petrov::writeVariants(const std::string& path, const std::string& title,
  const std::vector< std::vector< std::string > >& variants, const Database& database)
{
  std::ofstream file(path);
  if (!file) {
    return false;
  }
  file << "# " << title << "\n\n";
  for (std::size_t v = 0; v < variants.size(); ++v) {
    file << "## Вариант " << (v + 1) << "\n\n";
    for (std::size_t i = 0; i < variants[v].size(); ++i) {
      const Question& question = database.questions.get(variants[v][i]);
      file << (i + 1) << ". " << question.text << "\n";
      renderBody(file, question);
      file << "\n";
    }
  }
  return static_cast< bool >(file);
}

bool petrov::writeAnswers(const std::string& path, const std::string& title,
  const std::vector< std::vector< std::string > >& variants, const Database& database)
{
  std::ofstream file(path);
  if (!file) {
    return false;
  }
  file << "# " << title << " — Ответы\n\n";
  for (std::size_t v = 0; v < variants.size(); ++v) {
    file << "## Вариант " << (v + 1) << "\n\n";
    for (std::size_t i = 0; i < variants[v].size(); ++i) {
      const Question& question = database.questions.get(variants[v][i]);
      file << (i + 1) << ". " << answerString(question) << "\n";
    }
    file << "\n";
  }
  return static_cast< bool >(file);
}

