#include <cstddef>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "hasher.hpp"
#include "CCHTable.hpp"
#include "commands.hpp"

int main()
{
  using cmd_t = void (*)(std::istream&, std::ostream&, lavrentev::CCHT& cht);
  lavrentev::CCHTable< std::string, cmd_t, Siphash< std::string >, std::equal_to< std::string > > commands;
  commands["import"] = lavrentev::importFile;
  commands["view_all"] = lavrentev::viewAll;
  commands["convert"] = lavrentev::convert;
  commands["convert_to_wav"] = lavrentev::convertToWav;
  commands["convert_to_mp3"] = lavrentev::convertToMp3;
  commands["pitch"] = lavrentev::pitch;
  commands["cut"] = lavrentev::cut;
  commands["change_bitrate"] = lavrentev::changeBitrate;
  commands["normalize"] = lavrentev::normalize;
  commands["change_volume"] = lavrentev::changeVolume;
  commands["export"] = lavrentev::exportFile;
  commands["info"] = lavrentev::info;
  commands["view_formats"] = lavrentev::viewFormats;
  commands["help"] = lavrentev::help;
  commands["1"] = lavrentev::importFile;
  commands["2"] = lavrentev::viewAll;
  commands["3"] = lavrentev::convert;
  commands["4"] = lavrentev::convertToWav;
  commands["5"] = lavrentev::convertToMp3;
  commands["6"] = lavrentev::pitch;
  commands["7"] = lavrentev::cut;
  commands["8"] = lavrentev::changeBitrate;
  commands["9"] = lavrentev::normalize;
  commands["10"] = lavrentev::changeVolume;
  commands["11"] = lavrentev::exportFile;
  commands["12"] = lavrentev::info;
  commands["13"] = lavrentev::viewFormats;
  commands["14"] = lavrentev::help;

  lavrentev::CCHT data;

  std::string line;
  std::string cmd;
  while (std::getline(std::cin, line))
  {
    std::istringstream args(line);
    if (!(args >> cmd))
    {
      continue;
    }
    try
    {
      if (!commands.has(cmd))
      {
        throw std::logic_error("Unknown command");
      }

      commands[cmd](args, std::cout, data);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>" << "\n";
    }
  }

  if (std::cin.bad())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
