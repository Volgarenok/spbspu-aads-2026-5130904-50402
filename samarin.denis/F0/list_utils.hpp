#ifndef LIST_UTILS_HPP
#define LIST_UTILS_HPP

#include <cstddef>
#include <stdexcept>
#include <list.hpp>

namespace samarin {

  template< class T >
  std::size_t listSize(const List< T >& list)
  {
    std::size_t count = 0;
    for (LCIter< T > it = list.cbegin(); it != list.cend(); ++it) {
      ++count;
    }
    return count;
  }

  template< class T >
  T& listAt(List< T >& list, std::size_t index)
  {
    LIter< T > it = list.begin();
    for (std::size_t i = 0; i < index; ++i) {
      if (it == list.end()) {
        throw std::out_of_range("index out of range");
      }
      ++it;
    }
    if (it == list.end()) {
      throw std::out_of_range("index out of range");
    }
    return *it;
  }

  template< class T >
  const T& listAt(const List< T >& list, std::size_t index)
  {
    LCIter< T > it = list.cbegin();
    for (std::size_t i = 0; i < index; ++i) {
      if (it == list.cend()) {
        throw std::out_of_range("index out of range");
      }
      ++it;
    }
    if (it == list.cend()) {
      throw std::out_of_range("index out of range");
    }
    return *it;
  }

  template< class T >
  LIter< T > iterBefore(List< T >& list, std::size_t index)
  {
    LIter< T > prev = list.before_begin();
    for (std::size_t i = 0; i < index; ++i) {
      LIter< T > next = prev;
      ++next;
      if (next == list.end()) {
        throw std::out_of_range("index out of range");
      }
      prev = next;
    }
    return prev;
  }

  template< class T >
  List< T > mergeSorted(const List< T >& left, const List< T >& right)
  {
    List< T > out;
    LIter< T > tail = out.before_begin();
    LCIter< T > a = left.cbegin();
    LCIter< T > b = right.cbegin();
    while (a != left.cend() && b != right.cend()) {
      if (*b < *a) {
        tail = out.insert_after(tail, *b);
        ++b;
      } else {
        tail = out.insert_after(tail, *a);
        ++a;
      }
    }
    while (a != left.cend()) {
      tail = out.insert_after(tail, *a);
      ++a;
    }
    while (b != right.cend()) {
      tail = out.insert_after(tail, *b);
      ++b;
    }
    return out;
  }

  template< class T >
  void listSort(List< T >& list)
  {
    const std::size_t count = listSize(list);
    if (count < 2) {
      return;
    }
    List< T > left;
    List< T > right;
    LIter< T > leftTail = left.before_begin();
    LIter< T > rightTail = right.before_begin();
    std::size_t index = 0;
    for (LCIter< T > it = list.cbegin(); it != list.cend(); ++it, ++index) {
      if (index < count / 2) {
        leftTail = left.insert_after(leftTail, *it);
      } else {
        rightTail = right.insert_after(rightTail, *it);
      }
    }
    listSort(left);
    listSort(right);
    list = mergeSorted(left, right);
  }

}

#endif
