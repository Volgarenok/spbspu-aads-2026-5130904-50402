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
      List();
      List(const List< T > &scndList);
      List(List< T > &&scndList);
      ~List();
      T front();
      LIter begin();
      LCter beginC();
      LIter end();
      LCter endC();
      LIter insert_after(LIter < T > pos, const T value);
      void erase(LIter < T > pos);
      void clear();
      void push_front(const T value);
      void pop_front();
  };
}

#endif