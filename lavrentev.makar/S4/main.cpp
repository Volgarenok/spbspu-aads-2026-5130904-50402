#include <cstddef>
#include "BSTree.hpp"
#include <List.hpp>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Input processing error" << "\n";
    return 1;
  }

  lavrentev::BSTree<std::string, cmd_t, std::less<std::string>> commands{};
  lavrentev::List<lavrentev::BSTree<std::string, size_t, std::less<std::string>>> bst{};

  commands["print"] = lavrentev::print;
  commands["complement"] = lavrentev::complement;
  commands["intersect"] = lavrentev::intersect;
  commands["union"] = lavrentev::unionn;
}
