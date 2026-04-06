#include <iostream>
#include <istream>
#include <fstream>
int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "Bad arguments";
    return 1;
  }
  std::istream *input = &std::cin;
  std::ifstream fin(argv[1]);
  if (!fin.is_open())
  {
    std::cerr << "Error opening file\n";
    return 1;
  }
  input = &fin;
}
