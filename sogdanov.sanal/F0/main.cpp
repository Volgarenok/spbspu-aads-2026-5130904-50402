#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "Dictionary.hpp"
#include "Commands.hpp"
#include "HashTable.hpp"

int main()
{
  sogdanov::HashTable< std::string, sogdanov::Dictionary > db;

  using cmd_t = void(*)(std::istream&, std::ostream&, sogdanov::HashTable< std::string, sogdanov::Dictionary >&);
  sogdanov::HashTable< std::string, cmd_t > cmds;

  cmds.insert("create", sogdanov::createDict);
  cmds.insert("show", sogdanov::showDict);
  cmds.insert("drop", sogdanov::dropDict);
  cmds.insert("add-word", sogdanov::addWord);
  cmds.insert("remove-word", sogdanov::removeWord);
  cmds.insert("translate", sogdanov::translateWord);
  cmds.insert("reverse-translate", sogdanov::reverseTranslate);
  cmds.insert("find-by-pos", sogdanov::findByPos);
  cmds.insert("merge", sogdanov::mergeDicts);
  cmds.insert("save", sogdanov::saveDict);
  cmds.insert("load", sogdanov::loadDict);
  cmds.insert("list-dicts", sogdanov::listDicts);
  cmds.insert("stats", sogdanov::statsDict);
  cmds.insert("unknown", sogdanov::unknownWords);
  cmds.insert("demistify", sogdanov::demistifyWords);
  cmds.insert("union", sogdanov::unionDicts);
  cmds.insert("complement", sogdanov::complementDicts);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      std::streamsize toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error&) {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail()) {
        std::cin.clear();
      }
      std::streamsize toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }

  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }

}
