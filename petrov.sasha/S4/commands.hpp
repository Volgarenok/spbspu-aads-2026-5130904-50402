#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include "bstree.hpp"

namespace petrov
{
  using InnerTree = BSTree< int, std::string >;
  using Storage = BSTree< std::string, InnerTree >;

  struct Command
  {
    const char* name;
    void (*execute)(Storage& storage, std::istream& in, std::ostream& out);
  };
  const Command* findCommand(const std::string& name);
  void printCommand(Storage& storage, std::istream& in, std::ostream& out);
  void complementCommand(Storage& storage, std::istream& in, std::ostream& out);
  void intersectCommand(Storage& storage, std::istream& in, std::ostream& out);
  void unionCommand(Storage& storage, std::istream& in, std::ostream& out);
}

#endif

