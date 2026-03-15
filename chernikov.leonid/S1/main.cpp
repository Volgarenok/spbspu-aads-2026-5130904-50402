#include <iostream>
#include "List_collection.hpp"
#include "actions.hpp"

int main()
{
  try
  {
    auto sequences = chernikov::parser(std::cin);
    if (sequences.empty())
    {
      std::cout << "0\n";
    }
    chernikov::print_names(std::cout, sequences);
    chernikov::print_transposed(std::cout, sequences);
    chernikov::print_sums(std::cout, sequences);
  } catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
