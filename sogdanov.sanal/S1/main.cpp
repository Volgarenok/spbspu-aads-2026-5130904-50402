#include <iostream>
#include <string>
#include <limits>
#include "list.hpp"
namespace sogdanov {
  template<class T>
  LIter<T> list_append(List<T>& list, LIter<T> last, T val) {
    if (list.empty()) {
      list.push_front(std::move(val));
      return list.begin();
    }
    return list.insert_after(last, std::move(val));
  }
  using List_pair = List<std::pair<std::string, List<size_t>>>;
  void input(std::istream& in, List_pair& list) {
    LIter<std::pair<std::string, List<size_t>>> last;
    std::string name;
    while (in >> name) {
      List<size_t> nums;
      LIter<size_t> nums_last;
      char ch;
      while (in.get(ch)) {
        if (ch == '\n') {
          break;
        }
        if (ch == ' ') {
          continue;
        }
        in.unget();
        size_t val;
        if (in >> val) {
          nums_last = list_append(nums, nums_last, val);
        }
      }
      last = list_append(list, last,std::pair<std::string , List<size_t>>{name, \
                                                                     std::move(nums)});
    }
  }
  void transpose(const List_pair& list, List<List<size_t>>& data) {
    size_t max_size = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
      if ((*it).second.size() > max_size) {
        max_size = (*it).second.size();
      }
    }
    LIter<List<size_t>> data_last;
    for (size_t i = 0; i < max_size; ++i) {
      List<size_t> row;
      LIter<size_t> row_last;
      for (auto it = list.begin(); it != list.end(); ++it) {
        auto elem = (*it).second.begin();
        for (size_t j = 0; j < i && elem != (*it).second.end(); ++j) {
          ++elem;
        }
        if (elem != (*it).second.end()) {
          row_last = list_append(row, row_last, *elem);
        }
      }
      data_last = list_append(data, data_last, std::move(row));
    }
  }
  List<size_t> calc_sum(const List<List<size_t>>& transp) {
    List<size_t> sums;
    LIter<size_t> sums_last;
    for (auto row = transp.begin(); row != transp.end(); ++row) {
      size_t sum = 0;
      for (auto it = (*row).begin(); it != (*row).end(); ++it) {
        if (sum > std::numeric_limits<size_t>::max() - *it) {
          throw std::overflow_error("overflow errror\n");
        }
        sum += *it;
      }
      sums_last = list_append(sums, sums_last, sum);
    }
    return sums;
  }
  void output(std::ostream& out, const List_pair& names, const List<List<size_t>>& transp) {
    bool is_name = true;
    for (auto it = names.begin(); it != names.end(); ++it) {
      if (!is_name) {
        out << ' ';
      }
      out << (*it).first;
      is_name = false;
    }
    out << '\n';
    for (auto row = transp.begin(); row != transp.end(); ++row) {
      bool f = true;
        for (auto it = (*row).begin(); it != (*row).end(); ++it) {
        if (!f) {
          out << ' ';
        }
        out << *it;
        f = false;
      }
      out << '\n';
    }
    List<size_t> sums = calc_sum(transp);
    if (sums.empty()) {
      out << 0 << '\n';
      return;
    }
    bool is_sum = true;
    for (auto it = sums.begin(); it != sums.end(); ++it) {
      if(!is_sum) {
        out << ' ';
      }
      out << *it;
      is_sum = false;
    }
    out << '\n';
  }
}
int main() {
  using namespace sogdanov;
  List_pair list;
  input(std::cin, list);
  if (list.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }
  List<List<size_t>> transp;
  transpose(list, transp);
  try {
    output(std::cout, list, transp);
  } catch (const std::overflow_error& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
