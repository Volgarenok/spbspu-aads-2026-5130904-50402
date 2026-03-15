#ifndef TOOLS_HPP
#define TOOLS_HPP
#include "iostream"
#include "list.hpp"
namespace lachugin
{
  using pair = std::pair< std::string, List< int >* >;
  List < pair > getline(std::istream& in);
}
#endif
