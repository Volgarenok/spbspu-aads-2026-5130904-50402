#include <cstddef>
#include <limits>
#include <string>
#include <iostream>
#include <hasher.hpp>
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

  lavrentev::CCHT data;

  std::string cmd;
  while (true)
  {
    if (!(std::cin >> cmd))
    {
      break;
    }
    try
    {
      if (!commands.has(cmd))
      {
        throw std::logic_error("Unknown command");
      }

      commands[cmd](std::cin, std::cout, data);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>" << "\n";

      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
