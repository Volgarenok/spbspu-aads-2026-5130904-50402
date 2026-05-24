#include <fstream>
#include <string>
#include "calculate.hpp"
#include "task.hpp"
#include "stack.hpp"
#include "queue.hpp"

int main(int argc, char **argv)
{
  namespace per = permyakov;

  if (argc > 2) {
    std::cerr << "Too much arguments\n";
    return 1;
  }
  std::ifstream file;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Failed when opening file\n";
      return 1;
    }
  }
  std::istream &is = (argc == 2) ? file : std::cin;
  per::Queue< per::Queue< std::string > > expressions = per::input(is);
  per::Stack< long long > result;
  try {
    while (!expressions.isEmpty()) {
      per::Queue< std::string > postfix = per::convertToPostfix(expressions.front());
      expressions.pop();
      result.push(per::calculateExpression(postfix));
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  if (!result.isEmpty()) {
    per::output(std::cout, result);
  }
  std::cout << '\n';
}
