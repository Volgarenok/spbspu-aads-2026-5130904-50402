#include "process.hpp"

void karpovich::input(std::istream& in, list_pair_t& list)
{
  std::string name;
  while (in >> name) {
    List< size_t > numbers;
    size_t num = 0;
    while (in.peek() != '\n' && !in.eof()) {
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

void karpovich::transpose(const list_pair_t& list, list_pair_t& data)
{
  size_t max_len = 0;
  for (auto it = list.begin(); it != list.end(); it++) {

  }
}

void karpovich::output(std::ostream& out)
{

}
