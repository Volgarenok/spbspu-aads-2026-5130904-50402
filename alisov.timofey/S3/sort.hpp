#ifndef SORT_HPP
#define SORT_HPP
#include "vector.hpp"

namespace alisov
{
  template < class T >
  void sortVector(Vector< T > &vec)
  {
    if (vec.empty()) {
      return;
    }
    for (size_t i = 0; i < vec.size() - 1; ++i) {
      for (size_t j = 0; j < vec.size() - i - 1; ++j) {
        if (vec[j] > vec[j + 1]) {
          std::swap(vec[j], vec[j + 1]);
        }
      }
    }
  }
}
#endif
