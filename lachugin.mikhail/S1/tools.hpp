#ifndef TOOLS_HPP
#define TOOLS_HPP
#include "iostream"
#include "list.hpp"
namespace lachugin
{
  using pair = std::pair< std::string, List< int > >;
  List < pair > getline(std::istream& in);
  List < List < int > > process(List< pair > l);
  void output(List< List< int > > lVal, List< pair > l);
}
#endif
