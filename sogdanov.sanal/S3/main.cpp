#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include "Hashtable.hpp"
#include "commands.hpp"

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: missing filename parameter.\n";
    return 1;
  }

  try
  {
    sogdanov::load_file(argv[1]);
  }
  catch (...)
  {
    std::cerr << "Error: failed to load file.\n";
    return 1;
  }

  using cmd_t = void (*)(std::istream &, std::ostream &);
  sogdanov::HashTable<std::string, cmd_t> cmds(20);

  cmds.add("graphs", sogdanov::cmd_graphs);
  cmds.add("vertexes", sogdanov::cmd_vertexes);
  cmds.add("outbound", sogdanov::cmd_outbound);
  cmds.add("inbound", sogdanov::cmd_inbound);
  cmds.add("bind", sogdanov::cmd_bind);
  cmds.add("cut", sogdanov::cmd_cut);
  cmds.add("create", sogdanov::cmd_create);
  cmds.add("merge", sogdanov::cmd_merge);
  cmds.add("extract", sogdanov::cmd_extract);

  std::string line;
  while (std::getline(std::cin, line))
  {
    // Защита от Windows-переносов (\r\n) в WSL
    if (line.empty())
    {
      continue;
    }
    std::istringstream iss(line);
    std::string cmd;
    if (!(iss >> cmd))
    {
      continue;
    }
    iss.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    try
    {
      if (!cmds.has(cmd))
      {
        throw std::logic_error("Command not found");
      }
      cmds.get(cmd)(iss, std::cout);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}
