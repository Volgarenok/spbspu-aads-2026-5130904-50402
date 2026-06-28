#ifndef TAG_HPP
#define TAG_HPP

#include <set>
#include <string>

namespace petrov
{
  struct Tag
  {
    std::string name;
    std::set< std::string > questionIds;

    Tag();
  };
}

#endif

