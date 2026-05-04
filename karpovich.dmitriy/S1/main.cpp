#include <iostream>
#include <stdexcept>
#include "process.hpp"

int main()
{
  namespace karp = karpovich;
  karp::List< std::pair< std::string, karp::List< size_t > > > sequences;
  karp::List< karp::List< size_t > > transposed;
  karp::input(std::cin, sequences);

  if (sequences.size() == 0) {
    std::cout << "0\n";
    return 0;
  }
  karp::transpose(sequences, transposed);
  try {
    karp::output(std::cout, sequences, transposed);
  } catch (std::overflow_error &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
