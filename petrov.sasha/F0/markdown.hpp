#ifndef MARKDOWN_HPP
#define MARKDOWN_HPP

#include <string>
#include <vector>

#include "database.hpp"

namespace petrov
{
  bool writeVariants(const std::string& path, const std::string& title,
    const std::vector< std::vector< std::string > >& variants, const Database& database);

  bool writeAnswers(const std::string& path, const std::string& title,
    const std::vector< std::vector< std::string > >& variants, const Database& database);
}

#endif

