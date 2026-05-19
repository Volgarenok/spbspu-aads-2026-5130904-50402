#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include "math_op.hpp"
#include "stack.hpp"
#include "queue.hpp"
#include "process.hpp"

int main(int argc, char **argv)
{
  namespace karp = karpovich;
  if (argc > 2) {
    std::cerr << "bad num of arguments" << '\n';
    return 1;
  }
  std::ifstream file;
  std::istream *input = &std::cin;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "bad file" << '\n';
      return 1;
    }
    input = &file;
  }
  try {
    karp::Queue< karp::Queue< std::string > > queue;
    karp::inputQueue(*input, queue);
    karp::Stack< long long > result;
    while (!queue.empty()) {
      karp::Queue< std::string > infix = queue.front();
      queue.pop();
      karp::Queue< std::string > postfix = karp::convertToPostfix(infix);
      long long res = karpovich::calculatePostfix(postfix);
      result.push(res);
    }
    karpovich::output(std::cout, result);
  } catch (const std::exception &e) {
    std::cout << e.what() << '\n';
    return 1;
  }
}
