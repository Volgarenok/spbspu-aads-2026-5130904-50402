#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <utility>
#include <stdexcept>
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

  template < class T > T List < T >::front() const
  {
    if(size_ == 0) {
      throw std::logic_error("empty list");
    }
    return nodes -> val;
  }

  template < class T > size_t List < T >::size() const
  {
    return size_;
  }

  template < class T > LIter < T > List < T >::begin()
  {
    if(size_ == 0) {
      throw std::logic_error("empty list");
    }
    return LIter < T > (nodes);
  }
  
  template < class T > LIter < T > List < T >::end()
  {
    if(size_ == 0) {
      throw std::logic_error("empty list");
    }
    LIter < T > iter = begin();
    while(iter.curr -> next) {
      ++iter;
    }
    return iter;
  }

  template < class T > LCIter < T > List < T >::beginC() const
  {
    if(size_ == 0) {
      throw std::logic_error("empty list");
    }
    return LCIter < T > (nodes);
  }

  template < class T > LCIter < T > List < T >::endC() const
  {
    if(size_ == 0) {
      throw std::logic_error("empty list");
    }
    LCIter < T > iter = beginC();
    while(iter.curr -> next) {
      ++iter;
    }
    return iter;
  }

  template < class T > LIter < T > List < T >::insert_after(LIter < T > pos, const T value)
  {
    Node < T > * nextNode = pos.curr -> next;
    Node < T > * newNode = new Node < T > {value, nextNode};
    pos.curr -> next = newNode;
    return LIter < T > (newNode);
  }

  template < class T > void List < T >::erase(LIter < T > pos)
  {
    Node < T > * ersNode = pos.curr;
    ++pos;
    delete[] ersNode; 
    size_--;
  }

  template < class T > void List < T >::clear()
  {
    LIter < T > iter = begin();
    for(;size_ > 0; --size_) {
      erase(iter);
    }
    nodes = new Node < T > {T(), nullptr};
  }

  template < class T > void List < T >::push_front(const T value)
  {
    nodes = new Node < T > {value, nodes};
    size_++;
  }

  template < class T > void List < T >::pop_front()
  {
    Node < T > * ersNode = nodes;
    nodes = nodes -> next;
    delete[] ersNode; 
    size_--;
  }
}

#endif