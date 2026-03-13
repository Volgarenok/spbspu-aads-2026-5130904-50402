#ifndef LISTPROCESSING_HPP
#define LISTPROCESSING_HPP
#include "list.hpp"
namespace karpenkov {

  using pair_list = List<std::pair<std::string, List<size_t>>>;

  inline void readSequences(std::istream& input, pair_list& list) {
    std::string name;
    while(input >> name) {
      List<size_t> seq;
      size_t num;
      char nextChar = input.peek();
      while(nextChar != '\n' && !input.eof()) {
        input >> num;
        seq.push_back(num);
        nextChar = input.peek();
      }
      list.push_back({name, seq});
    }
  }

  inline void printNames(const pair_list& list, std::ostream& output) {
    for(auto it = list.cbegin(); it != list.cend(); ++it)
      output << (*it).first << " ";
    output << '\n';
  }

  inline void printSequences(const pair_list& list, std::ostream& output) {
    List<LCIter<size_t>> its;
    List<LCIter<size_t>> endsList;
    List<size_t> sums;

    for(auto it = list.cbegin(); it != list.cend(); ++it){
      its.push_back((*it).second.cbegin());
      endsList.push_back((*it).second.cend());
    }

    bool hasElement = true;
    while(hasElement) {
      hasElement = false;
      size_t sum = 0;
      auto it_it = its.begin();
      auto end_it = endsList.begin();
      while(it_it != its.end()) {
        if(*it_it != *end_it){
          hasElement = true;
          output << **it_it << " ";
          sum += **it_it;
          ++(*it_it);
        }
        ++it_it;
        ++end_it;
      }
      if(hasElement){
        sums.push_back(sum);
        output << '\n';
      }
    }

    for(auto it = sums.begin(); it != sums.end(); ++it)
      output << *it << " ";
    output << '\n';
  }

}
#endif
