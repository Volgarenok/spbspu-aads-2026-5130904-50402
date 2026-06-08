#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <hasher.hpp>
#include "Audio.hpp"
#include "CCHTable.hpp"

namespace lavrentev
{
  using CCHT = CCHTable< size_t, Audio, Siphash<size_t>, std::equal_to<size_t>>;

  void importFile(std::istream&, std::ostream&, CCHT &cht);
  void viewAll(std::istream&, std::ostream&, CCHT &cht);
  void convert(std::istream&, std::ostream&, CCHT &cht);
  void convertToWav(std::istream&, std::ostream&, CCHT &cht); //TODO
  void convertToMp3(std::istream&, std::ostream&, CCHT &cht); //TODO
  void pitch(std::istream&, std::ostream&, CCHT &cht); //TODO
  void cut(std::istream&, std::ostream&, CCHT &cht);
  void changeBitrate(std::istream&, std::ostream&, CCHT &cht);
  void normalize(std::istream&, std::ostream&, CCHT &cht);
  void changeVolume(std::istream&, std::ostream&, CCHT &cht);
  void exportFile(std::istream&, std::ostream&, CCHT &cht);
  void info(std::istream&, std::ostream&, CCHT &cht);

  void viewFormats(std::istream&, std::ostream&);
  void help(std::istream&, std::ostream&);

  std::string extractName(const std::string& filePath);
  std::string extractFormat(const std::string& filePath);
}

#endif