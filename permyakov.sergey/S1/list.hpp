#ifdef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include "iterators.hpp"

namespace permyakov
{
  template < class T > class List
  {
    T val;
    List <T> * next;
    size_t size;
    public:
      LIter front();
      LCter front();
      LIter insert_after(LIter < T > pos, const T value);
      bool isEmpty();
      void push_front(const T value);
      void clear();
      void pop_front();
      void erase_after(LIter < T > pos);
  };
}

#endif