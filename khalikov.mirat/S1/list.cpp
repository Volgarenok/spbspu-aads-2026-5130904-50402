#ifndef LIST_CPP
#define LIST_CPP
#include "node.hpp"
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
}

#endif
