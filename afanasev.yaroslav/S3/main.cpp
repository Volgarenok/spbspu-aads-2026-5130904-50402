#include <iostream>
#include "HashTable.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "I need filename\n";
    return 1;
  }

  const char * filename = argv[1];
  std::cout << "Имя файла: " << filename << '\n';

  return 0;
}
