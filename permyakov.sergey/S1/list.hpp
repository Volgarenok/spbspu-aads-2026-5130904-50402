#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <utility>
#include "iterators.hpp"

namespace permyakov
{
  template < class T > class List
  {
    Node < T > * nodes;
    size_t size_;
    public:
      List();
      List(const List< T > & scndList);
      List(List< T > && scndList);
      ~List();
      List & operator=(const List< T > &);
      List & operator=(List < T > &&);
      T front() const;
      size_t size() const;
      LIter < T > begin();
      LIter < T > end();
      LCIter < T > beginC() const;
      LCIter < T > endC() const;
      LIter < T > insert_after(LIter < T > pos, const T value);
      void erase(LIter < T > pos);
      void clear();
      void push_front(const T value);
      void pop_front();
  };

  template < class T > List < T >::List():
    nodes(new Node < T > {T(), nullptr}),
    size_(0)
  {}

  template < class T > List < T >::List(const List < T > & scndList):
    nodes(new Node < T > {scndList.front(), nullptr}),
    size_(scndList.size())
  {
    LCIter < T > iIter = scndList.beginC();
    LCIter < T > endIter = scndList.endC();
    LIter < T > fIter = begin();
    while(iIter != endIter) {
      insert_after(fIter, *(++iIter));
      ++fIter;
    }
  }

  template < class T > List < T >::List(List< T > && scndList):
    nodes(scndList.nodes),
    size_(scndList.size())
  {
    scndList.nodes = new Node < T > {T(), nullptr};
    scndList.size_ = 0;
  }

  template < class T > List < T > & List < T >::operator=(const List < T > & scndList)
  {
    List < T > list(scndList);
    *this = std::move(list);
  }

  template < class T > List < T > & List < T >::operator=(List< T > && scndList)
  {
    clear();
    delete[] nodes;
    nodes = scndList.nodes;
    size = scndList.size();
    scndList.nodes = new Node < T > {T(), nullptr};
    scndList.size_ = 0;
  }

  template < class T > List < T >::~List()
  {
    clear();
    delete nodes;
  }
}

#endif