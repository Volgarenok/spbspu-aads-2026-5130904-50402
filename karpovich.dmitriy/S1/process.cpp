#include "process.hpp"
#include <cstddef>

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

void karpovich::transpose(const list_pair_t& list, List< List< size_t > >& data)
{
  size_t max_len = 0;
  for (auto it = list.begin(); it != list.end(); it++) {
    if (it->second.size() > max_len) {
      max_len = it->second.size();
    }
  }
  for (size_t i = 0; i < max_len; i++) {
    List< size_t > row_numbers;
    for (auto seq_it = list.begin(); seq_it != list.end(); seq_it++) {
      if (i < seq_it->second.size()) {
        auto num_it = seq_it->second.begin();
        for (size_t j = 0; j < i; ++j) {
          ++num_it;
        }
        row_numbers.push_back(*num_it);
      }
    }
    if (row_numbers.size() > 0) {
      data.push_back(row_numbers);
    }
  }
}

void karpovich::output(std::ostream& out)
{

}
