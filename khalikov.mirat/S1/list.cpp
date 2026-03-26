#ifndef LIST_CPP
#define LIST_CPP
#include "list.hpp"

namespace khalikov
{

  template< class T >
  void List< T >::clear()
  {
    while(h)
    {
      Node< T > * temp = h->next;
      delete h;
      h = temp;
    }
  }

  template< class T >
  List< T >::List()
  {
    h = nullptr;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T > & List< T >::operator=(const List< T > & other)
  {
    if (this != &other)
    {
      List< T > temp(other);
      Node< T > * swapper = h;
      h = temp.h;
      temp.h = swapper;
    }
    return *this;
  }

  template< class T >
  List< T >::List(const List< T > & other)
  {
    if(!other.h)
    {
      h = nullptr;
      return;
    }

    h = new Node< T > {other.h->val, nullptr};
    Node< T > * curr = h;
    Node< T > * curr_other = other.h->next;

    while(curr_other)
    {
      curr->next = new Node< T > {curr_other->val, nullptr};
      curr = curr->next;
      curr_other = curr_other->next;
    }
  }

  template< class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T >(h);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const
  {
    return LCIter< T >(h);
  }

  template< class T >
  bool List< T >::empty() const
  {
    return h == nullptr;
  }

  template< class T >
  void List< T >::erase_after(LIter< T > pos)
  {
    Node< T > * erase = pos.curr->next;
    pos.curr->next = erase->next;
    delete erase;
  }

  template< class T >
  void List< T >::insert_after(LIter< T > pos, const T & value)
  {
    Node< T > * new_element = new Node< T > {value, pos.curr->next};
    pos.curr->next = new_element;
  }

  template< class T >
  void List< T >::merge(List< T > & other)
  {
    Node< T > * res_h = nullptr;
    if (h->val > other.h->val)
    {
      res_h = other.h;
      other.h = other.h->next;
    }
    else
    {
      res_h = h;
      h = h->next;
    }
    Node< T > * res_tail = res_h;
    res_tail->next = nullptr;
    while (h && other.h)
    {
      if (h->val > other.h->val)
      {
        res_tail->next = other.h;
        other.h = other.h->next;
      }
      else
      {
        res_tail->next = h;
        h = h->next;
      }
      res_tail = res_tail->next;
      res_tail->next = nullptr;
    }
    if (h)
    {
      res_tail->next = h;
    }
    if (other.h)
    {
      res_tail->next = other.h;
    }
    other.h = nullptr;
    h = res_h;
  }

  template< class T >
  void List< T >::pop_front()
  {
    Node< T > * temp = h;
    h = h->next;
    delete temp;
  }

  template< class T >
  void List< T >::push_front(const T & value)
  {
    Node< T > * temp = new Node< T > {value, h};
    h = temp;
  }

  template< class T >
  void List< T >::erase(LIter< T > pos)
  {
    Node< T > * prev = h;
    while (prev->next != pos.curr)
    {
      prev = prev->next;
    }
    prev->next = pos.curr->next;
    delete pos.curr;
  }

  template< class T >
  void List< T >::remove(const T & value)
  {
    auto it = begin();
    while (it.curr != nullptr)
    {
      auto next = it;
      next++;
      if (it.curr->val == value)
      {
        erase(it);
      }
      it = next;
    }
  }

  template< class T >
  void List< T >::sort()
  {
    auto i = begin();
    auto j = ++i;
    for (; i.curr != nullptr; ++i)
    {
      for (; j.curr != nullptr; ++j)
      {
        if (i.curr->val;)
}

#endif
