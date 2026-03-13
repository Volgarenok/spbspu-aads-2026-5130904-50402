#include <iostream>
#include "listProcessing.hpp"
#include "list.hpp"

void karpenkov::readSequences(std::istream& input, pair_list& list){
  std::string name;
  while (input >> name){
    List<size_t> seq;
    size_t num;
    char nextChar = input.peek();
    while(nextChar != '\n'){
      input >> num;
      seq.push_back(num);
      nextChar = input.peek();
    }
    list.push_back({name, seq});
  }
}
void printNames(const pair_list& list, std::ostream& output){
  for (auto it = list.cbegin(); it != list.cend(); ++it){
    output << (*it).first << " ";
  }
  output << '\n';
}
