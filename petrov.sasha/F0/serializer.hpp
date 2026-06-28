#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <string>

#include "database.hpp"

namespace petrov
{
  bool exportDatabase(const std::string& path, const Database& database);
  bool importDatabase(const std::string& path, Database& database, std::string& error);
}

#endif
