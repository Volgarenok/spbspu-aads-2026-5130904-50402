#include <fstream>
#include <iostream>
int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Incorrect usage";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cant open file";
    return 1;
  }
  file.close();
}
