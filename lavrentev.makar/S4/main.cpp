#include <cstddef>
#include "BSTree.hpp"
#include <List.hpp>
#include <exception>
#include "readfile.cpp"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Input processing error" << "\n";
    return 1;
  }

  lavrentev::List<lavrentev::BSTree<size_t, std::string, std::less<size_t>>> bsts{};

  try
  {
    lavrentev::readfile(argv[1], bsts);
  } catch (const std::runtime_error &) {
    std::cerr << "Input processing error" << "\n";
    return 2;
  }

  std::cerr << "DEBUG: number of trees = ";
  size_t count = 0;
  for (auto it = bsts.begin(); it != bsts.end(); ++it) ++count;
  std::cerr << count << std::endl;
  for (auto it = bsts.begin(); it != bsts.end(); ++it) {
      std::cerr << "  tree name = '" << (*it).getName() << "'" << std::endl;
  }

  lavrentev::BSTree<std::string, cmd_t, std::less<std::string>> commands{};
  commands["print"] = lavrentev::print;
  commands["complement"] = lavrentev::complement;
  commands["intersect"] = lavrentev::intersect;
  commands["union"] = lavrentev::unionn;

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

      commands[cmd](std::cin, std::cout, bsts);
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND> " << e.what() << "\n";

      std::cin.clear();
      std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
      );
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
