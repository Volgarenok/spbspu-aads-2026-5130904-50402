#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include "iterators.hpp"

namespace permyakov
{
  template < class T > class List
  {
    Node < T > * list;
    size_t size_;
    public:
      List();
      List(const List< T > & scndList);
      List(List< T > && scndList);
      ~List();
      T front();
      size_t size();
      LIter < T > begin();
      LIter < T > end();
      LCIter < T > beginC();
      LCIter < T > endC();
      LIter < T > insert_after(LIter < T > pos, const T value);
      void erase(LIter < T > pos);
      void clear();
      void push_front(const T value);
      void pop_front();
  };
}

#endif