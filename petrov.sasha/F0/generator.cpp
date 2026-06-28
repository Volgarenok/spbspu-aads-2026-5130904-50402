#include "generator.hpp"
#include "utils.hpp"

#include <fstream>
#include <random>
#include <stdexcept>
#include <string>

bool petrov::parseGenFile(const std::string& path, GenParams& params, std::string& error)
{
  std::ifstream file(path);
  if (!file) {
    error = "cannot open input file " + path;
    return false;
  }
  std::string line;
  while (std::getline(file, line)) {
    const std::string trimmed = trim(line);
    if (trimmed.empty() || trimmed[0] == '#') {
      continue;
    }
    const std::size_t eq = trimmed.find('=');
    if (eq == std::string::npos) {
      continue;
    }
    const std::string key = trim(trimmed.substr(0, eq));
    const std::string value = trim(trimmed.substr(eq + 1));
    if (key == "title") {
      if (!value.empty()) {
        params.title = value;
      }
    } else if (key == "variants") {
      try {
        params.variants = std::stoi(value);
      } catch (const std::exception&) {
        error = "invalid variants value";
        return false;
      }
    } else if (key == "questions_per_variant") {
      try {
        params.questionsPerVariant = std::stoi(value);
      } catch (const std::exception&) {
        error = "invalid questions_per_variant value";
        return false;
      }
    } else if (key == "tags") {
      params.tags = splitList(value, ',');
    } else if (key == "exclude_ids") {
      const std::vector< std::string > ids = splitList(value, ',');
      for (std::size_t i = 0; i < ids.size(); ++i) {
        params.excludeIds.insert(ids[i]);
      }
    }
  }
  if (params.variants <= 0) {
    error = "variants must be positive";
    return false;
  }
  if (params.questionsPerVariant <= 0) {
    error = "questions_per_variant must be positive";
    return false;
  }
  return true;
}

void petrov::selectCandidates(const Database& database, const GenParams& params,
  std::vector< std::string >& candidates)
{
  for (QuestionTree::const_iterator it = database.questions.begin(); it != database.questions.end(); ++it) {
    const Question& question = it->second;
    if (params.excludeIds.count(question.id) != 0) {
      continue;
    }
    bool matched = params.tags.empty();
    for (std::size_t i = 0; i < params.tags.size() && !matched; ++i) {
      if (question.tags.count(params.tags[i]) != 0) {
        matched = true;
      }
    }
    if (matched) {
      candidates.push_back(question.id);
    }
  }
}

void petrov::buildVariants(const GenParams& params, const std::vector< std::string >& candidates,
  std::vector< std::vector< std::string > >& variants)
{
  const unsigned int seed = 12345u;
  std::mt19937 generator(seed);
  const std::size_t needed = static_cast< std::size_t >(params.questionsPerVariant);
  for (int v = 0; v < params.variants; ++v) {
    std::vector< std::string > pool(candidates);
    for (std::size_t i = pool.size(); i > 1; --i) {
      std::uniform_int_distribution< std::size_t > distribution(0, i - 1);
      const std::size_t j = distribution(generator);
      const std::string temp = pool[i - 1];
      pool[i - 1] = pool[j];
      pool[j] = temp;
    }
    std::vector< std::string > variant;
    for (std::size_t i = 0; i < needed; ++i) {
      variant.push_back(pool[i]);
    }
    variants.push_back(variant);
  }
}

std::string petrov::makeAnswersPath(const std::string& outputPath)
{
  const std::size_t dot = outputPath.find_last_of('.');
  if (dot == std::string::npos) {
    return outputPath + "_answers.md";
  }
  return outputPath.substr(0, dot) + "_answers" + outputPath.substr(dot);
}


