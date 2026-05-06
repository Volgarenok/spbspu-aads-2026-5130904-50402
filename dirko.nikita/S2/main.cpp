#include <fstream>
#include <iostream>
#include "actions.hpp"
#include "calc.hpp"
#include "queue.hpp"
#include "stack.hpp"

int main(int argc, char *argv[])
{
  if (argc > 2) {
    std::cerr << "too many args\n";
    return 1;
  }
  std::ifstream file;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "failed to open file\n";
      return 1;
    }
  }
  std::istream &is = (argc == 2) ? file : std::cin;
  dirko::Stack< long long > res;
  try {
    dirko::Queue< dirko::Queue< std::string > > expretions = dirko::input(is);
    while (!expretions.empty()) {
      dirko::Queue< std::string > post = dirko::convert(expretions.get());
      expretions.pop();
      res.push(dirko::calcExpr(post));
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  if (!res.empty()) {
    std::cout << res.get();
    res.pop();
  }
  while (!res.empty()) {
    std::cout << ' ' << res.get();
    res.pop();
  }
  std::cout << '\n';
}
