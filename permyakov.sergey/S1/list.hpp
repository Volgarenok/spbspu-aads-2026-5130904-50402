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
    Node < T > * tail;
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
      bool isEmpty() const;
      LIter < T > begin();
      LIter < T > end();
      LCIter < T > beginC() const;
      LCIter < T > endC() const;
      LIter < T > insert_after(LIter < T > pos, const T value);
      void swap(List< T > & scndList);
      void erase_after(LIter < T > pos);
      void clear();
      void push_front(const T value);
      void pop_front();
  };

  template < class T > List < T >::List():
    nodes(new Node < T > {T(), nullptr}),
    tail(nodes),
    size_(0)
  {}

  template < class T > List < T >::List(const List < T > & scndList):
    nodes(new Node < T > {T(), nullptr}),
    tail(nodes),
    size_(0)
  {
    if(!scndList.isEmpty()) {
      push_front(scndList.front());
      LIter < T > iter = begin();
      for(LCIter < T > iIter = scndList.beginC(); size_ < scndList.size_; ++iter) {
        ++iIter;
        insert_after(iter, *iIter);
      }
    }
  }

  template < class T > List < T >::List(List< T > && scndList):
    nodes(scndList.nodes),
    tail(scndList.tail),
    size_(scndList.size_)
  {
    scndList.nodes = new Node < T > {T(), nullptr};
    scndList.tail = scndList.nodes;
    scndList.size_ = 0;
  }

  template < class T > List < T >::~List()
    {
      clear();
      delete nodes;
    }

  template < class T > List < T > & List < T >::operator=(const List < T > & scndList)
  {
    List < T > list(scndList);
    *this = std::move(list);
    return *this;
  }

  template < class T > List < T > & List < T >::operator=(List< T > && scndList)
  {
    clear();
    swap(scndList);
    return *this;
  }

  template < class T > T List < T >::front() const
  {
    if(isEmpty()) {
      throw std::logic_error("empty list");
    }
    return nodes -> val;
  }

  template < class T > size_t List < T >::size() const
  {
    return size_;
  }

  template < class T > bool List < T >::isEmpty() const
  {
    return !size_;
  }

  template < class T > LIter < T > List < T >::begin()
  {
    if(isEmpty()) {
      throw std::logic_error("empty list");
    }
    return LIter < T > (nodes);
  }

  template < class T > LIter < T > List < T >::end()
  {
    if(isEmpty()) {
      throw std::logic_error("empty list");
    }
    return LIter < T > (tail);
  }

  template < class T > LCIter < T > List < T >::beginC() const
  {
    if(isEmpty()) {
      throw std::logic_error("empty list");
    }
    return LCIter < T > (nodes);
  }

  template < class T > LCIter < T > List < T >::endC() const
  {
    if(isEmpty()) {
      throw std::logic_error("empty list");
    }
    return LCIter < T > (tail);
  }

  template < class T > LIter < T > List < T >::insert_after(LIter < T > pos, const T value)
  {
    if(!(pos.curr)) {
      throw std::logic_error("iterator doesnt point to value");
    }
    Node < T > * nextNode = pos.curr -> next;
    Node < T > * newNode = new Node < T > {value, nextNode};
    pos.curr -> next = newNode;
    if(!(newNode -> next)) {
      tail = newNode;
    }
    ++size_;
    return LIter < T > (newNode);
  }

  template < class T > void List < T >::swap(List< T > & scndList)
  {
    Node < T > * cpyNodes = nodes;
    Node < T > * cpyTail = tail;
    size_t cpySize = size_;
    nodes = scndList.nodes;
    tail = scndList.tail;
    size_ = scndList.size_;
    scndList.nodes = cpyNodes;
    scndList.tail = cpyTail;
    scndList.size_ = cpySize;
  }

  template < class T > void List < T >::erase_after(LIter < T > pos)
  {
    if(!(pos.curr)) {
      throw std::logic_error("iterator doesnt point to value");
    }
    Node < T > * ersNode = pos.curr -> next;
    pos.curr -> next = ersNode -> next;
    delete ersNode;
    size_--;
    if(isEmpty()) {
      nodes = new Node < T > {T(), nullptr};
    }
  }

  template < class T > void List < T >::clear()
  {
    while(size_ > 0) {
      pop_front();
    }
  }

  template < class T > void List < T >::push_front(const T value)
  {
    if(isEmpty()) {
      delete nodes;
    }
    nodes = new Node < T > {value, nodes};
    size_++;
    if(size_ == 1) {
      tail = nodes;
    }
  }

  template < class T > void List < T >::pop_front()
  {
    if(isEmpty()) {
      throw std::logic_error("empty list");
    }
    Node < T > * ersNode = nodes;
    nodes = nodes -> next;
    delete ersNode;
    size_--;
    if(isEmpty()) {
      nodes = new Node < T > {T(), nullptr};
    }
  }
}

#endif
