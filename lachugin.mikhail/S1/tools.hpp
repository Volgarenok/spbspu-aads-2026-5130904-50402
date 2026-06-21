#ifndef TOOLS_HPP
#define TOOLS_HPP
#include "iostream"
#include "../common/list.hpp"
namespace lachugin
{
  using pair = std::pair< std::string, List< size_t > >;
  List < pair > getline(std::istream& in);
  List < List < size_t> > process(List< pair > l);
  void output(List< List< size_t > >& lVal, List< pair >& l);
  List< size_t > listSums(List< List< size_t > > lVal);
}
#endif
