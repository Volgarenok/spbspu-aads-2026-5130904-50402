#include <cstddef>
#include "BSTree.hpp"
#include <List.hpp>
#include "readfile.cpp"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Input processing error" << "\n";
    return 1;
  }

  lavrentev::List<lavrentev::BSTree<std::string, size_t, std::less<std::string>>> bst{};

  try
  {
    lavrentev::readfile(argv[1], bst);
  } catch (const std::runtime_error &) {
    std::cerr << "Input processing error" << "\n";
    return 2;
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

      commands[cmd](std::cin, bst);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";

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
