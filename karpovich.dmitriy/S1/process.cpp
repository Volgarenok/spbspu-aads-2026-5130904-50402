#include "process.hpp"

void karpovich::input(std::istream& in, List< std::pair< std::string, List< size_t > > >& list) 
{
  std::string name;
  while (in >> name) {
    List< size_t > numbers;
    size_t num = 0;
    while (in.peek() != '\n' && in.peek() != in.eof()) {
      if (in >> num) {
        numbers.push_back(num);
      } else {
        break;
      }
    }    
    if (in.peek() == '\n') {
      in.get();
    }    
    list.push_back(std::make_pair(name, numbers));
  }
}
