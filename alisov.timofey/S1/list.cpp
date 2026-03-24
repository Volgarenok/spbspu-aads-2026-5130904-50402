#include "list.hpp"

template class alisov::BiList< int >;

template < class T > bool alisov::BiList< T >::empty() const noexcept
{
  return !head;
}

template < class T > alisov::BiList< T >::~BiList() noexcept
{
  clear();
}

template < class T > void alisov::BiList< T >::clear() noexcept
{
  while (head)
  {
    Node *next = head->next;
    delete head;
    head = next;
  }
}