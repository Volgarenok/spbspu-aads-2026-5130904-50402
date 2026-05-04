#include <iostream>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Ошибка: не указано имя файла.\n";
    return 1;
  }

  const char * filename = argv[1];
  std::cout << "Имя файла: " << filename << std::endl;

  return 0;
}
