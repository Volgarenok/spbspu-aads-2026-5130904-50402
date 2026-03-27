#include <cctype>
#include <cstddef>
#include <iostream>
#include <limits>
#include "../common/iters.hpp"
#include "../common/list.hpp"
#include "../common/node.hpp"
#include "funcs.hpp"

strelnikov::List< std::pair< std::string, strelnikov::List< size_t > > > strelnikov::input(std::istream &in)
{
  List< std::pair< std::string, List< size_t > > > list;
  auto list_it = list.cbegin();

  std::string name;
  while (in >> name) {
    List< size_t > nums;
    auto nums_it = nums.cbegin();

    size_t num;
    while (in >> num) {
      if (!nums.empty()) {
        nums_it = nums.insert_after(nums_it, num);

      } else {
        nums.push_front(num);

        nums_it = nums.cbegin();
      }
    }

    in.clear();

    if (!list.empty()) {
      list_it = list.insert_after(list_it, {name, nums});

    } else {
      list.push_front({name, nums});

      list_it = list.cbegin();
    }
  }

  return list;
}

strelnikov::List< strelnikov::List< size_t > >
strelnikov::leave_nums(List< std::pair< std::string, List< size_t > > > &list)
{
  size_t max_len = 0;
  auto it = list.cbegin();
  while (it != list.cend()) {
    size_t curr_size = size((*it).second);
    max_len = max_len > curr_size ? max_len : curr_size;
    ++it;
  }

  if (!max_len) {
    return List< List< size_t > >();
  }

  List< List< size_t > > res;
  auto res_it = res.cbegin();

  for (size_t i = 0; i < max_len; ++i) {
    List< size_t > tmp;
    auto it_tmp = tmp.cbegin();
    it = list.cbegin();

    while (it != list.cend()) {
      if (i < size((*it).second)) {
        auto it_nums = (*it).second.cbegin();

        for (size_t j = 0; j < i; ++j) {
          ++it_nums;
        }
        if (!tmp.empty()) {
          it_tmp = tmp.insert_after(it_tmp, *it_nums);
        } else {
          tmp.push_front(*it_nums);
          it_tmp = tmp.cbegin();
        }
      }

      ++it;
    }

    if (!res.empty()) {
      res_it = res.insert_after(res_it, tmp);
    } else {
      res.push_front(tmp);
      res_it = res.cbegin();
    }
  }

  return res;
}

strelnikov::List< size_t > strelnikov::get_sum(List< List< size_t > > &list)
{
  List< size_t > sum;
  auto sum_it = sum.cbegin();

  auto it = list.cbegin();

  while (it != list.cend()) {
    size_t res = 0;
    auto it_inner = (*it).cbegin();

    while (it_inner != (*it).cend()) {
      if (std::numeric_limits< size_t >::max() - res < *it_inner) {
        throw std::overflow_error("overflow\n");
      }
      res += *it_inner;
      ++it_inner;
    }

    if (!sum.empty()) {
      sum_it = sum.insert_after(sum_it, res);
    } else {
      sum.push_front(res);
      sum_it = sum.cbegin();
    }

    ++it;
  }

  return sum;
}

std::ostream &strelnikov::printNames(std::ostream &out, const List< std::pair< std::string, List< size_t > > > &list)
{
  auto it = list.cbegin();

  if (it == list.cend()) {
    return out;
  }

  out << (*it).first;
  ++it;

  while (it != list.cend()) {
    out << ' ' << (*it).first;
    ++it;
  }
  return out;
}

std::ostream &strelnikov::printList(std::ostream &out, const List< List< size_t > > &list)
{
  bool flag = false;

  auto it = list.cbegin();

  if (it == list.cend()) {
    return out;
  }

  while (it != list.cend()) {
    if (flag) {
      out << '\n';
    }
    auto it_inner = (*it).cbegin();

    if (it_inner == (*it).cend()) {
      ++it;
      continue;
    }

    out << *it_inner;
    ++it_inner;

    while (it_inner != (*it).cend()) {
      out << ' ' << *it_inner;
      ++it_inner;
    }
    ++it;

    flag = true;
  }
  return out;
}

std::ostream &strelnikov::printSum(std::ostream &out, const List< size_t > &list)
{
  auto it = list.cbegin();

  if (it == list.cend()) {
    return out;
  }

  out << *it;
  ++it;

  while (it != list.cend()) {
    out << ' ' << *it;
    ++it;
  }
  return out;
}
