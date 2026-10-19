#ifndef MANAGER_HPP
#define MANAGER_HPP
#include <iosfwd>
#include <fstream>
#include <sstream>
#include <string>
#include "hashrobin.hpp"

using evn_t = permyakov::RobinTable< std::string, float >;
using spc_t = permyakov::RobinTable< std::string, evn_t >;

namespace permyakov
{
  class Manager
  {
    const std::string filename;
    spc_t spaces;
    std::ostream & output;
    void parseLine(std::string line);
    public:
      Manager(const std::string file, spc_t empTble, std::ostream & out):
        filename(file),
        spaces(empTble),
        output(out)
      {}

      std::ostream & getOutput();
      spc_t & getSpaces();
      void loadFile();
      void saveToFile();
  };
}

#endif