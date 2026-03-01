#include "process.hpp"
#include <stdexcept>

int main()
{
  namespace karp = karpovich;
  karp::List< std::pair< std::string, karp::List< size_t > > > sequences;
  karp::List< karp::List< size_t > > transposed;
  karp::input(std::cin, sequences);

  if (sequences.size() == 0) {
    std::cout << (*sequences.begin()).first << "\n0\n";
    return 0;
  }

  karp::transpose(sequences, transposed);
  try {
    karp::output(std::cout, sequences, transposed);
  } catch (std::overflow_error &) {
    return 1;
  }
}
