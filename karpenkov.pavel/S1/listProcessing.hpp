#ifndef LISTPROCESSING_HPP
#define LISTPROCESSING_HPP
#include "list.hpp"
#include <limits>
namespace karpenkov
{

  using pair_list = List< std::pair< std::string, List< size_t > > >;

  inline void readSequences(std::istream &input, pair_list &list)
  {
    std::string name;
    while (input >> name) {
      List< size_t > seq;
      size_t num;
      char nextChar = input.peek();
      while (nextChar != '\n' && !input.eof()) {
        input >> num;
        seq.push_back(num);
        nextChar = input.peek();
      }
      list.push_back({name, seq});
    }
  }

  inline void printNames(const pair_list &list, std::ostream &output)
  {
    bool first = true;
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
      if (!first) {
        output << " ";
      }
      output << (*it).first;
      first = false;
    }
    output << '\n';
  }

  inline void printSequences(const pair_list &list, std::ostream &output)
  {
    List< LCIter< size_t > > its;
    List< LCIter< size_t > > endsList;
    List< size_t > sums;

    for (auto it = list.cbegin(); it != list.cend(); ++it) {
      its.push_back((*it).second.cbegin());
      endsList.push_back((*it).second.cend());
    }

    bool hasElement = true;
    while (hasElement) {
      hasElement = false;
      size_t sum = 0;
      auto it_it = its.begin();
      auto end_it = endsList.begin();
      bool first = true;
      while (it_it != its.end()) {
        if (*it_it != *end_it) {
          hasElement = true;
          if (!first) {
            output << " ";
          }
          output << **it_it;
          first = false;
          if (**it_it > std::numeric_limits< size_t >::max() - sum) {
            output << '\n';
            throw std::overflow_error("Number is too big");
          }
          sum += **it_it;
          ++(*it_it);
        }
        ++it_it;
        ++end_it;
      }
      if (hasElement) {
        sums.push_back(sum);
        output << '\n';
      }
    }
    if (sums.begin() == sums.end()) {
      output << 0 << '\n';
      return;
    }
    for (auto it = sums.begin(); it != sums.end(); ++it) {
      if (it != sums.begin()) {
        output << " ";
      }
      output << *it;
    }
    output << '\n';
  }
}
#endif
