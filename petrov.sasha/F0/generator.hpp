#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <set>
#include <string>
#include <vector>

#include "database.hpp"

namespace petrov
{
  struct GenParams
  {
    std::string title;
    int variants;
    int questionsPerVariant;
    std::vector< std::string > tags;
    std::set< std::string > excludeIds;

    GenParams():
      title("Самостоятельная работа"),
      variants(0),
      questionsPerVariant(0),
      tags(),
      excludeIds()
    {}
  };

  bool parseGenFile(const std::string& path, GenParams& params, std::string& error);
  void selectCandidates(const Database& database, const GenParams& params, std::vector< std::string >& candidates);
  void buildVariants(const GenParams& params, const std::vector< std::string >& candidates,
    std::vector< std::vector< std::string > >& variants);
  std::string makeAnswersPath(const std::string& outputPath);
}

#endif
