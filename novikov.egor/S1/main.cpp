#include <iostream>
#include <string>
#include <utility>
#include "List.hpp"

int main()
{
  int num = 0;
  std::string name = "";
  novikov::List< std::pair< std::string, novikov::List< int > > > main_seq;

  while (std::cin >> name) {
    std::pair< std::string, novikov::List< int > > seq(name, {});

    while (std::cin >> num) {
      seq.second.push_back(num);
    }

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
    }

    main_seq.push_back(seq);
  }

  if (!main_seq.empty()) {
    auto main_head = main_seq.begin();
    auto m_h = main_head;
    bool is_first = true;

    do {
      std::cout << (is_first ? "" : " ") << m_h->first;
      is_first = false;
      ++m_h;
    } while (m_h != main_head);

    std::cout << "\n";

    novikov::List< novikov::IterState< int > > states;
    m_h = main_head;
    size_t counter = 0;

    do {
      if (!m_h->second.empty()) {
        states.push_back({m_h->second.begin(), m_h->second.begin()});
        ++counter;
      }
      ++m_h;
    } while (m_h != main_head);

    if (counter < 2) {
      std::cerr << "Can't sum.";
      return 1;
    }

    novikov::List< size_t > sums;
    size_t sum = 0;

    while (!states.empty()) {
      novikov::List< novikov::IterState< int > > next_state;
      auto head_state = states.begin();
      auto it_state = head_state;
      bool first = true;

      do {
        std::cout << (first ? "" : " ") << *it_state->current;
        first = false;

        sum += *it_state->current;

        novikov::LIter< int > tmp = it_state->current;
        ++tmp;

        if (tmp != it_state->start) {
          next_state.push_back({tmp, it_state->start});
        }

        ++it_state;
      } while (it_state != head_state);

      std::cout << "\n";

      states = next_state;
      sums.push_back(sum);
      sum = 0;
    }

    auto start_sum = sums.begin();
    auto sum_res = start_sum;
    bool first = true;

    do {
      std::cout << (first ? "" : " ") << *sum_res;
      first = false;
      ++sum_res;
    } while (start_sum != sum_res);
  } else {
    std::cout << 0;
  }
}
