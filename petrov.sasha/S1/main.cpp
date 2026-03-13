#include "list.hpp"

#include <iostream>
#include <string>
#include <utility>

int main()
{}

bool petrov::readSequences(std::istream& input, SequenceList& sequences) {
  while (true) {
    std::string name = '';
    if (!(input >> name)) {
      if (input.eof()) {
        return true;
      }
      return false;
    }
    NumberList numbers;
    while(true) {
      const int next_symbol = input.peek();
      if (next_symbol == '\n') {
        input.get();
        break;
      }
      if (next_symbol == std::char_traits< char >::eof()) {
        break;
      }
      std::size_t number = 0;
      if (!(input >> number)) {
        return false;
      }
      numbers.pushBack(number);
    }
    sequences.pushBack(std::make_pair(name, std::move(numbers)));

    if (input.eof()) {
      break;
    }
  }
  return true;
}