#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <algorithm>
#include <iostream>
#include "CCHTable.hpp"
#include "../common/hasher.hpp"

namespace lavrentev
{
  void importFile(std::istream&, std::ostream&, CCHTable< size_t, Audio, Siphash, std::equal> &cht);
  void viewAll(std::istream&, std::ostream&);
  void convert(std::istream&, std::ostream&);
  void convertToWav(std::istream&, std::ostream&);
  void convertToMp3(std::istream&, std::ostream&);
  void pitch(std::istream&, std::ostream&);
  void cut(std::istream&, std::ostream&);
  void changeBitrate(std::istream&, std::ostream&);
  void normalize(std::istream&, std::ostream&);
  void changeVolume(std::istream&, std::ostream&);
  void exportFile(std::istream&, std::ostream&);

  void info(std::istream&, std::ostream&);
  void viewFormats(std::istream&, std::ostream&);
  void help(std::istream&, std::ostream&);
}

#endif